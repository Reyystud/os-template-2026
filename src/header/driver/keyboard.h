#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60

/**
 * Peta Scancode PS/2 Set 1 ke Karakter ASCII (Make code / Key Press)
 */
extern const char keyboard_scancode_1_to_ascii[256];

/**
 * Mengaktifkan Keyboard Interrupt Handler pada IDT
 */
void keyboard_state_activate(void);

/**
 * Fungsi C handler yang dipanggil saat terjadi interrupt keyboard (IRQ1 / 0x21)
 */
void keyboard_isr(void);

#endif