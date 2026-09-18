#include <stdint.h>
#include <stdbool.h>
#include "header/cpu/gdt.h"
#include "header/kernel-entrypoint.h"
#include "header/cpu/framebuffer.h"

void kernel_setup(void) {
    uint32_t a;
    uint32_t volatile b = 0x0000BABE;
    __asm__("mov $0xCAFE0000, %0" : "=r"(a));

    load_gdt(&_gdt_gdtr);

    // Inisialisasi & Bersihkan layar VGA
    framebuffer_clear();

    // Cetak string percobaannya
    char *msg = "Halo lur, ada sukabolOS disini!";
    for (int i = 0; msg[i] != '\0'; i++) {
        framebuffer_write(0, i, msg[i], COLOR_LIGHT_GREEN, COLOR_BLACK);
    }

    // Set posisi kursor persis di akhir string
    framebuffer_set_cursor(0, 31);

    while (true) {
        b += 1; 
        (void)b;
    }
}