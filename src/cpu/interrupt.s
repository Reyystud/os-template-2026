global main_interrupt_empty_handler
global main_interrupt_handler_0x21
extern main_interrupt_handler

; Default empty handler untuk entri IDT yang belum dikonfigurasi
main_interrupt_empty_handler:
    push dword 0                ; Push dummy error code
    push dword 0                ; Push dummy interrupt number
    jmp isr_common_stub

; Macro untuk ISR tanpa Error Code
%macro ISR_NOERRCODE 1
global main_interrupt_handler_%1
main_interrupt_handler_%1:
    push dword 0                ; Push dummy error code
    push dword %1               ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Buat handler untuk ISR 0x21 (Keyboard Interrupt / IRQ1)
ISR_NOERRCODE 0x21

; Stub umum untuk menyimpan register dan memanggil C handler
isr_common_stub:
    pusha                       ; Push EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    call main_interrupt_handler
    popa                        ; Pop general registers
    add esp, 8                  ; Clean up pushed error code dan int number
    iret                        ; Interrupt Return