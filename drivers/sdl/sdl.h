#pragma ONCE
#include <stdint.h>

enum graphics_mode_t {
    TEXTMODE_40x30,
    TEXTMODE_80x30,
    TEXTMODE_160x100,

    CGA_160x200x16,
    CGA_320x200x4,
    CGA_640x200x2,

    TGA_320x200x16,
    EGA_320x200x16x4,
    // planar EGA
    VGA_320x200x256,
    VGA_320x200x256x4,
    // planar VGA
};

void graphics_init();

void graphics_set_mode(enum graphics_mode_t mode);

void graphics_set_buffer(uint8_t* buffer, uint16_t width, uint16_t height);

void graphics_set_offset(int x, int y);

void graphics_set_palette(uint8_t i, uint32_t color);

void graphics_set_textbuffer(uint8_t* buffer);

inline void graphics_set_bgcolor(uint32_t color888) {
    // dummy
}

inline void graphics_set_flashmode(bool flash_line, bool flash_frame) {
    // dummy
}

void draw_text(char* string, uint32_t x, uint32_t y, uint8_t color, uint8_t bgcolor);

void draw_window(char* title, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

void clrScr(uint8_t color);
