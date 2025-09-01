#include <eadk.h>

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi Stream";
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

void draw_header() {
    // Clear screen
    eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_white);
    
    // Draw blue header bar
    eadk_rect_t header = {0, 0, EADK_SCREEN_WIDTH, 30};
    eadk_display_push_rect_uniform(header, eadk_color_blue);
    
    // Draw title
    eadk_display_draw_string("Pi Stream Display", (eadk_point_t){5, 8}, true, eadk_color_white, eadk_color_blue);
}

void draw_status_line(const char* status, int line) {
    int y = 40 + (line * 20);
    eadk_display_draw_string(status, (eadk_point_t){10, y}, false, eadk_color_black, eadk_color_white);
}

void clear_content_area() {
    eadk_rect_t content = {0, 35, EADK_SCREEN_WIDTH, EADK_SCREEN_HEIGHT - 35};
    eadk_display_push_rect_uniform(content, eadk_color_white);
}

void show_instructions() {
    draw_status_line("UART Connection: 115200 baud", 0);
    draw_status_line("Connect Pi to UART pins", 1);
    draw_status_line("", 2);
    draw_status_line("Controls:", 3);
    draw_status_line("OK = Clear display", 4);
    draw_status_line("Back = Exit app", 5);
    draw_status_line("", 6);
    draw_status_line("Waiting for Pi data...", 7);
}

// Simple number to string conversion
void int_to_string(int num, char* str) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int i = 0;
    int temp = num;
    
    // Handle negative numbers
    if (num < 0) {
        str[i++] = '-';
        num = -num;
    }
    
    // Count digits
    int digits = 0;
    temp = num;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }
    
    // Fill string backwards
    str[i + digits] = '\0';
    while (num > 0) {
        str[i + digits - 1] = '0' + (num % 10);
        num /= 10;
        digits--;
    }
}

void simulate_pi_data(int counter) {
    char buffer[50];
    char num_str[20];
    
    if (counter % 200 == 0) {  // Every ~4 seconds
        clear_content_area();
        
        if (counter < 400) {
            draw_status_line("Pi Connected!", 0);
            draw_status_line("Receiving data stream...", 1);
            draw_status_line("", 2);
            
            // Build "Data packet #N" string manually
            buffer[0] = 'D'; buffer[1] = 'a'; buffer[2] = 't'; buffer[3] = 'a';
            buffer[4] = ' '; buffer[5] = 'p'; buffer[6] = 'a'; buffer[7] = 'c';
            buffer[8] = 'k'; buffer[9] = 'e'; buffer[10] = 't'; buffer[11] = ' ';
            buffer[12] = '#';
            int_to_string(counter / 200, num_str);
            int j = 13;
            for (int k = 0; num_str[k] != '\0'; k++) {
                buffer[j++] = num_str[k];
            }
            buffer[j] = '\0';
            
            draw_status_line(buffer, 3);
            draw_status_line("Temperature: 25.3°C", 4);
            draw_status_line("CPU Usage: 42%", 5);
        } else if (counter < 800) {
            draw_status_line("Math from Pi:", 0);
            draw_status_line("E = mc²", 1);
            draw_status_line("π ≈ 3.14159", 2);
            draw_status_line("∫ sin(x)dx = -cos(x)", 3);
            draw_status_line("Calculation complete", 4);
        } else {
            // Reset counter display
            draw_status_line("Stream continues...", 0);
            draw_status_line("Real-time monitoring", 1);
            draw_status_line("Status: Connected", 2);
            draw_status_line("Press OK to clear", 3);
        }
    }
}

int main(int argc, char * argv[]) {
    // Show initial screen with external data info
    draw_header();
    
    // Show external data if available
    if (eadk_external_data && eadk_external_data[0] != '\0') {
        draw_status_line("External data loaded:", 0);
        draw_status_line(eadk_external_data, 1);
        eadk_timing_msleep(2000);  // Show for 2 seconds
    }
    
    show_instructions();
    
    int counter = 0;
    
    // Main loop
    while (1) {  // Use 1 instead of true for C99 compatibility
        eadk_keyboard_state_t keyboard = eadk_keyboard_scan();
        
        // Handle back button (exit)
        if (eadk_keyboard_key_down(keyboard, eadk_key_back)) {
            break;
        }
        
        // Handle OK button (clear)
        if (eadk_keyboard_key_down(keyboard, eadk_key_ok)) {
            clear_content_area();
            show_instructions();
            counter = 0;
        }
        
        // Simulate receiving data from Pi
        simulate_pi_data(counter);
        
        counter++;
        eadk_timing_msleep(20);  // 50 FPS update rate
    }
    
    return 0;
}
