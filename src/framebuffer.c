#include "header/cpu/framebuffer.h"

// Port I/O untuk mengendalikan kursor VGA
#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA    0x3D5

// Fungsi inline assembly untuk I/O Out
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t pos = r * FRAMEBUFFER_WIDTH + c;

    outb(CURSOR_PORT_COMMAND, 0x0F);
    outb(CURSOR_PORT_DATA,    (uint8_t) (pos & 0xFF));
    outb(CURSOR_PORT_COMMAND, 0x0E);
    outb(CURSOR_PORT_DATA,    (uint8_t) ((pos >> 8) & 0xFF));
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint16_t *attrib_ptr = FRAMEBUFFER_MEMORY_TOKEN + (row * FRAMEBUFFER_WIDTH + col);
    uint8_t attribute_byte = (bg << 4) | (fg & 0x0F);
    *attrib_ptr = (attribute_byte << 8) | c;
}

void framebuffer_clear(void) {
    for (uint8_t r = 0; r < FRAMEBUFFER_HEIGHT; r++) {
        for (uint8_t c = 0; c < FRAMEBUFFER_WIDTH; c++) {
            framebuffer_write(r, c, ' ', COLOR_WHITE, COLOR_BLACK);
        }
    }
    framebuffer_set_cursor(0, 0);
}