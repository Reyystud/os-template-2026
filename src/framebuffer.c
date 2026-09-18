#include "header/driver/framebuffer.h"
#include "header/cpu/portio.h"

void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t pos = r * FRAMEBUFFER_WIDTH + c;

    outb(CURSOR_PORT_CMD, 0x0F);
    outb(CURSOR_PORT_DATA,    (uint8_t) (pos & 0xFF));
    outb(CURSOR_PORT_CMD, 0x0E);
    outb(CURSOR_PORT_DATA,    (uint8_t) ((pos >> 8) & 0xFF));
}

void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint8_t *fb = (uint8_t*) FRAMEBUFFER_MEMORY_TOKEN + 2 * (row * FRAMEBUFFER_WIDTH + col);
    fb[0] = c;
    fb[1] = (bg << 4) | (fg & 0x0F);
}

void framebuffer_clear(void) {
    for (uint8_t r = 0; r < FRAMEBUFFER_HEIGHT; r++) {
        for (uint8_t c = 0; c < FRAMEBUFFER_WIDTH; c++) {
            framebuffer_write(r, c, ' ', COLOR_WHITE, COLOR_BLACK);
        }
    }
    framebuffer_set_cursor(0, 0);
}