#include <extapp_api.h>
#include <string.h>

// Buffer for received data
static char data_buffer[1024] = {0};
static int buffer_pos = 0;
static uint64_t last_poll_time = 0;

// Colors
#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x001F
#define COLOR_GREEN 0x07E0

void draw_header() {
    // Clear screen
    extapp_pushRectUniform(0, 0, LCD_WIDTH, LCD_HEIGHT, COLOR_WHITE);
    
    // Draw header
    extapp_pushRectUniform(0, 0, LCD_WIDTH, 30, COLOR_BLUE);
    extapp_drawTextLarge("Pi Stream Display", 10, 8, COLOR_WHITE, COLOR_BLUE, false);
}

void draw_status(const char* status) {
    extapp_pushRectUniform(10, 40, LCD_WIDTH-20, 20, COLOR_WHITE);
    extapp_drawTextSmall(status, 10, 45, COLOR_BLACK, COLOR_WHITE, false);
}

void draw_data() {
    // Clear data area
    extapp_pushRectUniform(10, 70, LCD_WIDTH-20, LCD_HEIGHT-80, COLOR_WHITE);
    
    // Draw buffer content
    if (strlen(data_buffer) > 0) {
        // Simple text display - split into lines if too long
        int y_pos = 75;
        int line_length = 35; // Characters per line
        int len = strlen(data_buffer);
        
        for (int i = 0; i < len && y_pos < LCD_HEIGHT - 20; i += line_length) {
            char line[36] = {0};
            int copy_len = (len - i > line_length) ? line_length : len - i;
            strncpy(line, data_buffer + i, copy_len);
            line[copy_len] = '\0';
            
            extapp_drawTextSmall(line, 10, y_pos, COLOR_BLACK, COLOR_WHITE, false);
            y_pos += 15;
        }
    } else {
        extapp_drawTextSmall("Waiting for data from Pi...", 10, 75, COLOR_BLACK, COLOR_WHITE, false);
        extapp_drawTextSmall("Connect UART cable", 10, 95, COLOR_BLACK, COLOR_WHITE, false);
        extapp_drawTextSmall("Baud: 115200", 10, 115, COLOR_BLACK, COLOR_WHITE, false);
    }
}

void poll_uart() {
    uint64_t current_time = extapp_millis();
    if (current_time - last_poll_time < 100) return; // Poll every 100ms
    last_poll_time = current_time;
    
    // Simulate receiving data (in real implementation, read from UART)
    // For now, just show a demo message
    static int demo_counter = 0;
    if (demo_counter == 0 && strlen(data_buffer) == 0) {
        strcpy(data_buffer, "Pi Stream Ready!\nUART: 115200 baud\nSend data from Pi\n$$E=mc^2$$");
        demo_counter = 1;
    }
}

void handle_input() {
    uint64_t scan = extapp_scanKeyboard();
    
    if (scan & SCANCODE_Back) {
        // Exit app
        return;
    }
    
    if (scan & SCANCODE_OK) {
        // Clear buffer
        memset(data_buffer, 0, sizeof(data_buffer));
        buffer_pos = 0;
    }
}

void extapp_main() {
    draw_header();
    draw_status("Initializing...");
    draw_data();
    
    last_poll_time = extapp_millis();
    
    while (1) {
        poll_uart();
        handle_input();
        
        uint64_t scan = extapp_scanKeyboard();
        if (scan & SCANCODE_Back) {
            break; // Exit app
        }
        
        // Update display
        draw_status("Connected - Press OK to clear, Back to exit");
        draw_data();
        
        extapp_msleep(50);
    }
}
