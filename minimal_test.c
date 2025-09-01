#include <stdint.h>

// Required EADK metadata sections
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;
const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Test";

// Minimal 55x56 icon in NWI format
const unsigned char eadk_app_icon[] __attribute__((section(".rodata.eadk_app_icon"))) = {
    0x4E, 0x57, 0x49,  // "NWI" magic
    0x37, 0x00,        // Width: 55
    0x38, 0x00,        // Height: 56
    // Minimal icon data (7 bytes header + minimal pixel data)
    0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x1F, 0x00
};

// Minimal main function
int main(void) {
    // Just loop and do nothing - minimal viable EADK app
    volatile int i = 0;
    while (i < 1000000) {
        i++;
    }
    return 0;
}
