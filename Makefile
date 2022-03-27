TARGET=i686-elf
PROJDIRS = src
DESTDIR = build

CC = $(HOME)/opt/cross/bin/$(TARGET)

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
CFLAGS := -ffreestanding -O2 -std=gnu99 $(WARNINGS)
LDFLAGS := -ffreestanding -O2 -nostdlib -lgcc

.PHONY: all clean run iso
.SUFFIXES: .o .c .s

all: arial.kernel

arial.kernel: $(OBJFILES) src/linker.ld
	$(CC)-gcc -T src/linker.ld -o $@ $(LDFLAGS) $(OBJFILES)

.c.o:
	$(CC)-gcc -MD -c $< -o $@ $(CFLAGS)
 
.s.o:
	$(CC)-as -MD -c $< -o $@

iso: arial.kernel
	grub-file --is-x86-multiboot arial.kernel
	mkdir -p build
	mkdir -p build/boot
	cp arial.kernel build/boot/arial.kernel
	mkdir -p build/boot/grub
	cp grub.cfg build/boot/grub/grub.cfg
	mkdir -p build/boot/grub/i386-pc
	cp -r $(HOME)/opt/lib/grub/i386-pc/* build/boot/grub/i386-pc
	rm build/boot/grub/i386-pc/*.img
	grub-mkimage -O i386-pc -d $(HOME)/opt/lib/grub/i386-pc -o core.img -c grub.cfg --prefix=/boot/grub iso9660 biosdisk
	cat $(HOME)/opt/lib/grub/i386-pc/cdboot.img core.img > build/boot/grub/i386-pc/eltorito.img
	cat $(HOME)/opt/lib/grub/i386-pc/boot.img core.img > embedded.img
	xorriso -report_about HINT -as mkisofs -graft-points -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-info-table --embedded-boot embedded.img --protective-msdos-label -o arial.iso -r build --sort-weight 0 / --sort-weight 1 /boot

run: iso
	qemu-system-i386 -cdrom arial.iso

clean:
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	rm -f arial.*
	rm -f *.img
	rm -r build


-include $(DEPFILES)
