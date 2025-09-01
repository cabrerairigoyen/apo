#include <stdint.h>
#include <stdbool.h>

// Required EADK metadata sections
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Pi Stream";

// Simple 55x56 icon (NWI format)
const unsigned char eadk_app_icon[] __attribute__((section(".rodata.eadk_app_icon"))) = {
    // NWI header for 55x56 image
    0x4E, 0x57, 0x49,  // "NWI" magic
    0x37, 0x00,        // Width: 55 (little endian)
    0x38, 0x00,        // Height: 56 (little endian)
    // Simple pixel data for a blue icon (55*56*2 = 6160 bytes for RGB565)
    // Blue pattern for demo
    0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00,  // Blue pixels
    0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00,
    0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00,
    0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00,
    // Fill remaining bytes (simplified for demo)
    [32 ... 6166] = 0x1F  // Blue color pattern
};

// Simple demonstration main function
int main(int argc, char * argv[]) {
    // Minimal EADK app that just exists
    // In a real implementation, this would use EADK API calls
    // for display, keyboard input, etc.
    
    // For now, just loop briefly and exit
    // This demonstrates the EADK structure without complex API calls
    volatile int counter = 0;
    while (counter < 1000000) {
        counter++;
    }
    
    return 0;
}
