TARGET=i686-elf
PROJDIRS = src
DESTDIR = build

CC = $(HOME)/opt/cross/bin/$(TARGET)
GRUB = $(HOME)/de

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

.PHONY: all clean run
.SUFFIXES: .o .c .s

all: arial.kernel
	grub-file --is-x86-multiboot arial.kernel

arial.kernel: $(OBJFILES) src/linker.ld
	$(CC)-gcc -T src/linker.ld -o $@ $(LDFLAGS) $(OBJFILES)

.c.o:
	$(CC)-gcc -MD -c $< -o $@ $(CFLAGS)
 
.s.o:
	$(CC)-as -MD -c $< -o $@

run: 
	qemu-system-i386 -kernel arial.kernel

clean:
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	rm -f arial.*
	rm -r build


-include $(DEPFILES)
