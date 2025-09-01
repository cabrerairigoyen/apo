#include <eadk.h>
#include <string.h>

// Required EADK metadata sections
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi Stream";

// Simple 55x56 icon (NWI format) - This is a placeholder, needs proper NWI encoding
const unsigned char eadk_app_icon[] __attribute__((section(".rodata.eadk_app_icon"))) = {
    // NWI header for 55x56 image
    0x4E, 0x57, 0x49,  // "NWI" magic
    0x37, 0x00,        // Width: 55
    0x38, 0x00,        // Height: 56
    // Placeholder pixel data (55*56*2 = 6160 bytes for RGB565)
    // This creates a simple blue icon
    [7 ... 6166] = 0x1F  // Blue color in RGB565
};

// App state
static char data_buffer[1024] = "Pi Stream Display\nWaiting for UART data...\nPress Back to exit";
static bool running = true;

// Colors (RGB565)
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_BLUE    0x001F
#define COLOR_GREEN   0x07E0
#define COLOR_RED     0xF800

void draw_screen() {
    // Clear screen
    eadk_display_push_rect_uniform(eadk_screen_rect, COLOR_WHITE);
    
    // Draw header bar
    eadk_rect_t header = {0, 0, eadk_screen_rect.width, 25};
    eadk_display_push_rect_uniform(header, COLOR_BLUE);
    
    // Draw title
    eadk_point_t title_point = {5, 5};
    eadk_display_draw_string("Pi Stream Display", title_point, true, COLOR_WHITE, COLOR_BLUE);
    
    // Draw status
    eadk_point_t status_point = {5, 35};
    eadk_display_draw_string("UART: 115200 baud | Back=Exit | OK=Clear", status_point, false, COLOR_BLACK, COLOR_WHITE);
    
    // Draw data buffer
    eadk_point_t data_point = {5, 55};
    
    // Split text into lines for display
    char *line_start = data_buffer;
    char *newline;
    int y_offset = 0;
    
    while ((newline = strchr(line_start, '\n')) != NULL && y_offset < 150) {
        // Temporarily null-terminate the line
        *newline = '\0';
        
        eadk_point_t line_point = {5, 55 + y_offset};
        eadk_display_draw_string(line_start, line_point, false, COLOR_BLACK, COLOR_WHITE);
        
        // Restore newline and move to next line
        *newline = '\n';
        line_start = newline + 1;
        y_offset += 15;
    }
    
    // Draw remaining text if any
    if (*line_start && y_offset < 150) {
        eadk_point_t line_point = {5, 55 + y_offset};
        eadk_display_draw_string(line_start, line_point, false, COLOR_BLACK, COLOR_WHITE);
    }
}

void handle_events() {
    eadk_event_t event = eadk_event_get(50);  // 50ms timeout
    
    switch (event) {
        case eadk_event_back:
            running = false;
            break;
            
        case eadk_event_ok:
            // Clear buffer
            strcpy(data_buffer, "Buffer cleared\nWaiting for new data...");
            break;
            
        default:
            // Simulate receiving UART data (for demonstration)
            static int demo_counter = 0;
            demo_counter++;
            
            if (demo_counter == 100) {  // Every ~5 seconds
                strcat(data_buffer, "\nDemo: Pi connected");
                demo_counter = 0;
                
                // Keep buffer manageable
                if (strlen(data_buffer) > 800) {
                    strcpy(data_buffer, "Pi Stream Display\nBuffer reset...");
                }
            }
            break;
    }
}

// EADK main entry point
int main(int argc, char * argv[]) {
    // Initialize
    eadk_display_push_rect_uniform(eadk_screen_rect, COLOR_WHITE);
    
    // Main loop
    while (running) {
        draw_screen();
        handle_events();
    }
    
    return 0;
}
