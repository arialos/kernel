TARGET=i686-elf
PROJDIRS := arch drivers fonts kernel lib
DESTDIR = build
INCLUDES = -Ilib/include -Iarch/i686 -Iarch/i686/boot -Iarch/i686/drivers -Idrivers -Ifonts

CC = $(HOME)/opt/bin/$(TARGET)

VERSIONFILE = kernel/version.h
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
CFLAGS := -ffreestanding -O2 -nostdlib -std=gnu99 $(INCLUDES) -g 
LDFLAGS := -ffreestanding -O2 -nostdlib -lgcc

.PHONY: all clean run iso
.SUFFIXES: .o .c .s

all: arial.elf

arial.elf: version.h $(OBJFILES) linker.ld
	@$(CC)-gcc -T linker.ld -o $@ $(LDFLAGS) $(OBJFILES)
	@echo Linking $@
    

version.h: 
	echo "#define KERNEL_VERSION \"0.1.0.`date +%Y%m%d%H%M%S`\"" > $(VERSIONFILE)
	echo "#define KERNEL_CODENAME \"Palo Alto\"" >> $(VERSIONFILE)
	echo "#define KERNEL_BUILD_DATE \"`date +%Y-%m-%d`\"" >> $(VERSIONFILE)
	echo "#define KERNEL_BUILD_TIME \"`date +%H:%M:%S`\"" >> $(VERSIONFILE)
	echo "#define KERNEL_BUILD_NUMBER \"`date +%Y%m%d%H%M%S`\"" >> $(VERSIONFILE)



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
	# @./limine/limine-s2deploy arial.iso

clean:
	@rm -f  *.o **/*.o **/**/*.o **/**/**/*.o
	@rm -f  *.d **/*.d **/**/*.d **/**/**/*.d
	@rm -f $(VERSIONFILE)
	@rm -f arial.*
	@rm -f *.img
	@rm -r build

run: clean iso
	qemu-system-i386 -cdrom arial.iso -vga std


-include $(DEPFILES)
