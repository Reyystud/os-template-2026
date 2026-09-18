#include "header/driver/keyboard.h"
#include "header/cpu/portio.h"
#include "header/cpu/idt.h"
#include "header/driver/framebuffer.h"

// Pemetaan Scancode PS/2 (Set 1) ke ASCII untuk Key Down (0x00 - 0x7F)
const char keyboard_scancode_1_to_ascii[256] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
     0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
     0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,
   '*',   0, ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static uint8_t cursor_row = 1;
static uint8_t cursor_col = 0;

extern void main_interrupt_handler_0x21(void);

void keyboard_state_activate(void) {
    // Daftarkan handler khusus ISR 0x21 (IRQ1) di IDT
    idt_set_interrupt_handler(
        0x21,
        (void*) main_interrupt_handler_0x21,
        0x08,
        INTERRUPT_GATE_R0
    );
}

void keyboard_isr(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Cek apakah scancode adalah "Make Code" (Key Press), yaitu bit-7 = 0 (scancode < 0x80)
    if (!(scancode & 0x80)) {
        char c = keyboard_scancode_1_to_ascii[scancode];
        if (c != 0) {
            if (c == '\n') {
                cursor_row++;
                cursor_col = 0;
            } else if (c == '\b') {
                if (cursor_col > 0) {
                    cursor_col--;
                    framebuffer_write(cursor_row, cursor_col, ' ', COLOR_WHITE, COLOR_BLACK);
                }
            } else {
                framebuffer_write(cursor_row, cursor_col, c, COLOR_LIGHT_GREEN, COLOR_BLACK);
                cursor_col++;
                if (cursor_col >= FRAMEBUFFER_WIDTH) {
                    cursor_col = 0;
                    cursor_row++;
                }
            }
            framebuffer_set_cursor(cursor_row, cursor_col);
        }
    }

    // EOI (End of Interrupt) dikirim otomatis di main_interrupt_handler
}