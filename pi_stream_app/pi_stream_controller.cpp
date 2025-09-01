#include "pi_stream_controller.h"
#include <assert.h>
#include <string.h>

namespace PiStream {

PiStreamController::PiStreamController(Responder * parentResponder) :
  StackViewController(parentResponder, &m_scrollableTextView, Pane::None),
  m_scrollableTextView(&m_textView),
  m_lastPollTime(0)
{
  m_textView.setFont(KDFont::SmallFont);
}

void PiStreamController::viewWillAppear() {
  m_buffer[0] = 0;
  m_textView.setText(m_buffer);
  m_lastPollTime = Ion::Timing::millis();
  
  // Note: Console is initialized by the system, no manual init needed
  
  // Clear stack and push initial views
  popAll();
  push(&m_scrollableTextView);
}

bool PiStreamController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Back) {
    return false; // Let parent handle back button
  }
  pollUART();
  return true;
}

void PiStreamController::pollUART() {
  KDCoordinate currentTime = Ion::Timing::millis();
  if (currentTime - m_lastPollTime < 50) return;
  m_lastPollTime = currentTime;

  // Check if data is available and read characters
  // Note: Ion::Console doesn't have an 'available' function, so we'd need to implement non-blocking read
  // For now, this is a simplified approach - you may need to implement buffering
  try {
    char c = Ion::Console::readChar(); // This is blocking
    appendToBuffer(c);
    processBuffer();
  } catch (...) {
    // Handle any read errors
  }
}

void PiStreamController::appendToBuffer(char c) {
  int len = strlen(m_buffer);
  if (len < sizeof(m_buffer) - 1) {
    m_buffer[len] = c;
    m_buffer[len + 1] = 0;
  } else {
    memmove(m_buffer, m_buffer + 1, sizeof(m_buffer) - 1);
    m_buffer[sizeof(m_buffer) - 1] = 0;
  }
}

void PiStreamController::processBuffer() {
  // Look for LaTeX delimiters (e.g., $$latex$$ or \(latex\))
  char * start = strstr(m_buffer, "$$");
  if (!start) start = strstr(m_buffer, "\\(");
  if (start) {
    char * end = strstr(start + 2, "$$");
    if (!end) end = strstr(start + 2, "\\)");
    if (end) {
      *end = 0;  // Null-terminate LaTeX string
      Poincare::Expression expr = Poincare::Expression::Parse(start + 2, nullptr);
      if (!expr.isUninitialized()) {
        // Render math using proper ExpressionView
        Poincare::Layout layout = expr.createLayout(Poincare::Preferences::PrintFloatMode::Decimal, Poincare::Preferences::ComplexFormat::Real);
        m_expressionView.setLayout(layout);
        push(&m_expressionView);
        return; // Don't append as text
      } else {
        // Invalid: Treat as text
        appendText(start);
      }
      // Shift buffer past processed part
      memmove(m_buffer, end + 2, strlen(end + 2) + 1);
      return;
    }
  }
  // No LaTeX: Append as text if newline found
  char * nl = strchr(m_buffer, '\n');
  if (nl) {
    *nl = 0;
    appendText(m_buffer);
    memmove(m_buffer, nl + 1, strlen(nl + 1) + 1);
  }
}

void PiStreamController::appendText(const char * text) {
  // Note: TextView text handling might need different approach
  // This is a simplified version - actual implementation may need buffer management
  m_textView.setText(text);
  m_scrollableTextView.scrollToBottom();
}

}
