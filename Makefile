# Compiler & linker
ASM           = nasm
LIN           = ld
CC            = gcc

# Directory
SOURCE_FOLDER = src
OUTPUT_FOLDER = bin
ISO_NAME      = OS2025

# Flags
WARNING_CFLAG = -Wall -Wextra -Werror
DEBUG_CFLAG   = -fshort-wchar -g
STRIP_CFLAG   = -nostdlib -fno-stack-protector -nostartfiles -nodefaultlibs -ffreestanding
CFLAGS        = $(DEBUG_CFLAG) $(WARNING_CFLAG) $(STRIP_CFLAG) -m32 -c -I$(SOURCE_FOLDER)
AFLAGS        = -f elf32 -g -F dwarf
LFLAGS        = -T $(SOURCE_FOLDER)/linker.ld -melf_i386


run: all
	@qemu-system-i386 -s -S -cdrom $(OUTPUT_FOLDER)/$(ISO_NAME).iso
all: build
build: iso
clean:
	rm -rf *.o *.iso $(OUTPUT_FOLDER)/kernel



kernel:
	@$(ASM) $(AFLAGS) src/kernel-entrypoint.s -o bin/kernel-entrypoint.o
	@$(ASM) $(AFLAGS) src/cpu/interrupt.s -o bin/interrupt.o
	@$(CC) $(CFLAGS) src/kernel.c -o bin/kernel.o
	@$(CC) $(CFLAGS) src/cpu/gdt.c -o bin/gdt.o
	@$(CC) $(CFLAGS) src/cpu/portio.c -o bin/portio.o
	@$(CC) $(CFLAGS) src/cpu/idt.c -o bin/idt.o
	@$(CC) $(CFLAGS) src/driver/framebuffer.c -o bin/framebuffer.o
	@echo Linking object files and generate elf32...
	@$(LIN) $(LFLAGS) bin/kernel-entrypoint.o bin/interrupt.o bin/kernel.o bin/gdt.o bin/portio.o bin/idt.o bin/framebuffer.o -o $(OUTPUT_FOLDER)/kernel
	@rm -f bin/*.o

iso: kernel
	@mkdir -p $(OUTPUT_FOLDER)/iso/boot/grub
	@cp $(OUTPUT_FOLDER)/kernel $(OUTPUT_FOLDER)/iso/boot/
	@cp other/grub1 $(OUTPUT_FOLDER)/iso/boot/grub/
	@cp $(SOURCE_FOLDER)/menu.lst $(OUTPUT_FOLDER)/iso/boot/grub/
	@genisoimage -R \
		-b boot/grub/grub1 \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-A os \
		-input-charset utf8 \
		-quiet \
		-o $(OUTPUT_FOLDER)/OS2025.iso \
		$(OUTPUT_FOLDER)/iso
	@rm -rf $(OUTPUT_FOLDER)/iso/
