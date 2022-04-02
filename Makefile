TARGET=i686-elf
PROJDIRS := arch drivers fonts kernel lib
DESTDIR = build
INCLUDES = -Ilib/include -Iarch/i686 -Iarch/i686/boot -Iarch/i686/drivers -Idrivers -Ifonts

CC = $(HOME)/opt/bin/$(TARGET)

CFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.s")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
DEPFILES := $(patsubst %.c,%.d,$(CFILES))

SRCFILES := $(CFILES) $(ASMFILES)

OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
CFLAGS := -ffreestanding -O2 -std=gnu99 $(INCLUDES) $(WARNINGS)
LDFLAGS := -ffreestanding -O2 -nostdlib -lgcc

.PHONY: all clean run iso
.SUFFIXES: .o .c .s

all: arial.elf

arial.elf: $(OBJFILES) linker.ld
	@$(CC)-gcc -T linker.ld -o $@ $(LDFLAGS) $(OBJFILES)
	@echo Linking $@

.c.o:
	@$(CC)-gcc -MD -c $< -o $@ $(CFLAGS)
	@echo Compiling $<

.s.o:
	@$(CC)-as -MD -c $< -o $@
	@echo Assembling $<

iso: arial.elf
	@mkdir build
	@cp arial.elf limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin build
	@xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label build -o arial.iso
	@./limine/limine-s2deploy arial.iso

run: iso
	qemu-system-i386 -cdrom arial.iso -vga std

clean:
	@rm -f  *.o **/*.o **/**/*.o **/**/**/*.o
	@rm -f  *.d **/*.d **/**/*.d **/**/**/*.d
	@rm -f arial.*
	@rm -f *.img
	@rm -r build


-include $(DEPFILES)
