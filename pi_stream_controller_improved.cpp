#include "pi_stream_controller.h"
#include <assert.h>
#include <string.h>

namespace PiStream {

PiStreamController::PiStreamController(Responder * parentResponder) :
  StackViewController(parentResponder, &m_scrollableTextView, Pane::None),
  m_scrollableTextView(&m_textView),
  m_expressionView(),
  m_lastPollTime(0)
{
  m_textView.setFont(KDFont::SmallFont);
}

void PiStreamController::viewWillAppear() {
  m_buffer[0] = 0;
  m_textView.setText("Waiting for Pi data...");
  m_lastPollTime = Ion::Timing::millis();
  
  // Console is initialized by the system automatically
  
  // Clear stack and push initial views
  popAll();
  push(&m_scrollableTextView);
}

bool PiStreamController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Back) {
    return false; // Let parent handle back button
  }
  
  // Non-blocking check for new data every 50ms
  pollUART();
  return true;
}

void PiStreamController::pollUART() {
  KDCoordinate currentTime = Ion::Timing::millis();
  if (currentTime - m_lastPollTime < 50) return;
  m_lastPollTime = currentTime;

  // Since Ion::Console::readChar() is blocking, we need a different approach
  // One option is to use a separate thread or implement a timeout mechanism
  // For now, we'll use a simple approach with error handling
  
  // Alternative approach: Check if transmission is done and try reading
  if (Ion::Console::transmissionDone()) {
    try {
      // This is still blocking, but at least we know there might be data
      char c = Ion::Console::readChar();
      appendToBuffer(c);
      processBuffer();
    } catch (...) {
      // No data available or read error
    }
  }
}

void PiStreamController::appendToBuffer(char c) {
  int len = strlen(m_buffer);
  if (len < sizeof(m_buffer) - 1) {
    m_buffer[len] = c;
    m_buffer[len + 1] = 0;
  } else {
    // Buffer full, shift left to make room
    memmove(m_buffer, m_buffer + 1, sizeof(m_buffer) - 1);
    m_buffer[sizeof(m_buffer) - 1] = 0;
    int newLen = strlen(m_buffer);
    m_buffer[newLen] = c;
    m_buffer[newLen + 1] = 0;
  }
}

void PiStreamController::processBuffer() {
  // Look for LaTeX delimiters (e.g., $$latex$$ or \(latex\))
  char * start = strstr(m_buffer, "$$");
  if (!start) start = strstr(m_buffer, "\\(");
  
  if (start) {
    char * end = nullptr;
    int delimiterLength = 2;
    
    if (start == strstr(m_buffer, "$$")) {
      end = strstr(start + 2, "$$");
    } else {
      end = strstr(start + 2, "\\)");
    }
    
    if (end) {
      // Extract LaTeX expression
      *end = 0;  // Null-terminate LaTeX string
      char * latexExpr = start + 2;  // Skip opening delimiter
      
      try {
        // Parse and render the mathematical expression
        Poincare::Expression expr = Poincare::Expression::Parse(latexExpr, nullptr);
        if (!expr.isUninitialized()) {
          // Create layout for the expression
          Poincare::Layout layout = expr.createLayout(
            Poincare::Preferences::PrintFloatMode::Decimal, 
            Poincare::Preferences::ComplexFormat::Real
          );
          
          // Set the layout in the expression view
          m_expressionView.setLayout(layout);
          
          // Show the expression view
          popAll();
          push(&m_expressionView);
          
          // Log what we rendered
          appendText("Rendered math expression");
        } else {
          // Invalid LaTeX: treat as regular text
          appendText(latexExpr);
        }
      } catch (...) {
        // Error parsing: treat as text
        appendText(latexExpr);
      }
      
      // Remove processed part from buffer
      memmove(m_buffer, end + delimiterLength, strlen(end + delimiterLength) + 1);
      return;
    }
  }
  
  // No complete LaTeX expression found, check for newline to process text
  char * nl = strchr(m_buffer, '\n');
  if (nl) {
    *nl = 0;  // Null-terminate the line
    appendText(m_buffer);  // Display the text line
    // Remove processed line from buffer
    memmove(m_buffer, nl + 1, strlen(nl + 1) + 1);
  }
}

void PiStreamController::appendText(const char * text) {
  // For a proper implementation, we'd need to manage a text buffer
  // For now, just update the text view with the new content
  
  static char displayBuffer[2048] = {0};
  static bool bufferInitialized = false;
  
  if (!bufferInitialized) {
    strcpy(displayBuffer, "Pi Stream Display:\n");
    bufferInitialized = true;
  }
  
  // Append new text with newline
  size_t currentLen = strlen(displayBuffer);
  size_t textLen = strlen(text);
  
  if (currentLen + textLen + 2 < sizeof(displayBuffer)) {
    strcat(displayBuffer, text);
    strcat(displayBuffer, "\n");
    
    // Update the text view
    m_textView.setText(displayBuffer);
    
    // Go back to scrollable text view if we're showing expression
    popAll();
    push(&m_scrollableTextView);
    
    // Scroll to bottom to show latest content
    m_scrollableTextView.scrollToBottom();
  }
}

}
