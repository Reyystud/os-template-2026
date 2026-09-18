#include <stdint.h>
#include <stdbool.h>
#include "header/cpu/gdt.h"
#include "header/cpu/idt.h"
#include "header/driver/framebuffer.h"
#include "header/driver/keyboard.h"
#include "header/kernel-entrypoint.h"

void kernel_setup(void) {
    uint32_t a;
    uint32_t volatile b = 0x0000BABE;
    __asm__("mov $0xCAFE0000, %0" : "=r"(a));

    load_gdt(&_gdt_gdtr);
    initialize_idt();

    framebuffer_clear();
    char *msg = "OS2025 - Type anything on keyboard:";
    for (int i = 0; msg[i] != '\0'; i++) {
        framebuffer_write(0, i, msg[i], COLOR_LIGHT_CYAN, COLOR_BLACK);
    }
    framebuffer_set_cursor(1, 0);

    // Aktifkan Keyboard Driver
    keyboard_state_activate();

    while (true) {
        b += 1; 
        (void)b;
    }
}