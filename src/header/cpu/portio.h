#ifndef _PORTIO_H
#define _PORTIO_H

#include <stdint.h>

/**
 * Menulis 1 byte data ke I/O port tertentu.
 */
void outb(uint16_t port, uint8_t data);

/**
 * Membaca 1 byte data dari I/O port tertentu.
 */
uint8_t inb(uint16_t port);

#endif