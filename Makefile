TARGET=i686-elf
PROJDIRS := arch drivers fonts kernel lib
DESTDIR = build
INCLUDES = -Ilib/include -Iarch/i686 -Iarch/i686/mem  -Iarch/i686/boot -Iarch/i686/cpu -Iarch/i686/graphics -Iarch/i686/ps2 -Iarch/i686/system -Idrivers -Ifonts -Ikernel

CC = $(HOME)/opt/bin/$(TARGET)

VERSIONFILE = kernel/version.h
CFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
ASMFILES := $(shell find $(PROJDIRS) -type f -name "*.s")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
DEPFILES := $(patsubst %.c,%.d,$(CFILES))

SRCFILES := $(CFILES) $(ASMFILES)

OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))

WARNINGS = -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes

OPTFLAGS = -fexpensive-optimizations -ftree-loop-vectorize -finline-functions -fomit-frame-pointer -ftree-vectorize -O1 -mtune=generic
CFLAGS = -std=gnu99 -ffreestanding -fno-stack-protector -nostdlib $(WARNINGS) -m32 $(INCLUDES) -g
LDFLAGS = -ffreestanding -O2 -nostdlib -lgcc
ASFLAGS = -march=i686 --32

.PHONY: all clean run iso
.SUFFIXES: .o .c .s

all: arial.elf

arial.elf: version.h $(OBJFILES) linker.ld
	@$(CC)-gcc -T linker.ld -o $@ $(LDFLAGS) $(OBJFILES)
	@echo Linking $@
    

version.h: 
	@echo "#define KERNEL_VERSION \"0.1.3.`date +%Y%m%d%H%M%S`\"" > $(VERSIONFILE)
	@echo "#define KERNEL_CODENAME \"Palo Alto\"" >> $(VERSIONFILE)
	@echo "#define KERNEL_BUILD_DATE \"`date +%Y-%m-%d`\"" >> $(VERSIONFILE)
	@echo "#define KERNEL_BUILD_TIME \"`date +%H:%M:%S`\"" >> $(VERSIONFILE)
	@echo "#define KERNEL_BUILD_NUMBER \"`date +%Y%m%d%H%M%S`\"" >> $(VERSIONFILE)

.c.o:
	@echo Compiling $<
	@$(CC)-gcc $(CFLAGS) $(OPTFLAGS) -c $< -o $@

.s.o:
	@echo Assembling $<
	@$(CC)-as $(ASFLAGS) -c $< -o $@

iso: arial.elf
	@mkdir build
	@cp arial.elf limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin build
	@xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label build -o arial.iso
	# @./limine/limine-s2deploy arial.iso



clean:
	@echo "Cleaning build directory..."
	@rm -f $(OBJ) $(DEPFILES) arial.* *.img
	@rm -f $(VERSIONFILE)
	@rm -rf $(DESTDIR)

run: clean iso
	qemu-system-i386 -cdrom arial.iso -vga std

-include $(DEPFILES)
