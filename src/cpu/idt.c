#include "header/cpu/idt.h"
#include "header/cpu/portio.h"
extern void main_interrupt_empty_handler(void);

struct InterruptGate interrupt_descriptor_table[IDT_MAX_ENTRY] = {0};

// Cukup deklarasikan struct global tanpa langsung mengisinya
struct IDTR _idt_idtr;

/**
 * Remap 8259 PIC (Master & Slave) agar IRQ 0-15 masuk ke vektor 0x20-0x2F
 */
static void pic_remap(void) {
    // Save masks
    uint8_t a1 = inb(0x21);
    uint8_t a2 = inb(0xA1);

    // ICW1: Initialize PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Vector Offsets
    outb(0x21, 0x20); // Master PIC vector offset (0x20)
    outb(0xA1, 0x28); // Slave PIC vector offset (0x28)

    // ICW3: Cascading
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: Environment info (8086 mode)
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Restore saved masks
    outb(0x21, a1);
    outb(0xA1, a2);
}

void idt_set_interrupt_handler(uint8_t int_number, void *handler_pt, uint8_t gdt_seg, uint8_t attr) {
    uint32_t base = (uint32_t)(uintptr_t) handler_pt;
    interrupt_descriptor_table[int_number].offset_low     = base & 0xFFFF;
    interrupt_descriptor_table[int_number].segment        = gdt_seg;
    interrupt_descriptor_table[int_number]._reserved      = 0;
    interrupt_descriptor_table[int_number].type_attribute = attr;
    interrupt_descriptor_table[int_number].offset_high    = (base >> 16) & 0xFFFF;
}

void initialize_idt(void) {
    _idt_idtr.limit = sizeof(interrupt_descriptor_table) - 1;
    _idt_idtr.base  = (uint32_t)(uintptr_t) &interrupt_descriptor_table;

    // 1. Set semua entri IDT ke default empty handler (Kernel Code Segment = 0x08)
    for (int i = 0; i < IDT_MAX_ENTRY; i++) {
        idt_set_interrupt_handler(
            i, 
            (void*) main_interrupt_empty_handler, 
            0x08, 
            INTERRUPT_GATE_R0
        );
    }

    // 2. Remap PIC
    pic_remap();

    // 3. Load IDTR
    __asm__ volatile ("lidt %0" : : "m"(_idt_idtr));

    // 4. Baru aman aktifkan Interrupt Flag (STI)
    __asm__ volatile ("sti");
}

void main_interrupt_handler(struct InterruptFrame frame) {
    // Send EOI (End of Interrupt) jika berasal dari PIC (IRQ 0x20-0x2F)
    if (frame.int_number >= 0x20 && frame.int_number <= 0x2F) {
        if (frame.int_number >= 0x28) {
            outb(0xA0, 0x20); // Slave EOI
        }
        outb(0x20, 0x20);     // Master EOI
    }
}