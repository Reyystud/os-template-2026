#include <stdint.h>
#include <stdbool.h>
#include "header/cpu/gdt.h"
#include "header/cpu/idt.h"
#include "header/driver/framebuffer.h"
#include "header/kernel-entrypoint.h"

void kernel_setup(void) {
    uint32_t a;
    uint32_t volatile b = 0x0000BABE;
    __asm__("mov $0xCAFE0000, %0" : "=r"(a));

    // 1. Load GDT
    load_gdt(&_gdt_gdtr);

    // 2. Initialize IDT & Remap PIC
    initialize_idt();

    // 3. Clear Screen & Print Message
    framebuffer_clear();
    char *msg = "OS2025 - IDT & Interrupts Initialized!";
    for (int i = 0; msg[i] != '\0'; i++) {
        framebuffer_write(0, i, msg[i], COLOR_LIGHT_GREEN, COLOR_BLACK);
    }
    framebuffer_set_cursor(0, 38);

    while (true) {
        b += 1; 
        (void)b;
    }
}