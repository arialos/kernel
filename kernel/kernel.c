#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "fpu.h"
#include "gdt.h"
#include "gfx.h"
#include "idt.h"
#include "irq.h"
#include "keyboard.h"
#include "libio.h"
#include "libmath.h"
#include "libstring.h"
#include "mouse.h"
#include "multiboot.h"
#include "paging.h"
#include "pit.h"
#include "process.h"
#include "serial.h"
#include "smbios.h"
#include "version.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

void main(MultibootInfo* mbi, unsigned long magic) {
    enableFpu();
    initSerial();

    initHeap(mbi);

    // Initialize terminal interface
    initGraphics(mbi);

    printf("Welcome to the Arial Kernel!\n");
    printf("Version: %s\n", KERNEL_VERSION);
    printf("Codename: %s\n", KERNEL_CODENAME);
    printf("Build date: %s\n", KERNEL_BUILD_DATE);
    printf("Build time: %s\n\n", KERNEL_BUILD_TIME);

    printf("[ MULTIBOOT ] Checking for Magic Header... ");
    (magic == MULTIBOOT_BOOTLOADER_MAGIC) ? printf("Success! 0x%x\n", MULTIBOOT_BOOTLOADER_MAGIC)
                                          : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Memory Size... ");
    (mbi->mem_lower != 0) ? printf("Success! 0x%x\n", mbi->mem_lower) : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Boot Loader Name... ");
    (mbi->boot_loader_name != 0) ? printf("Success! \"%s\"\n", mbi->boot_loader_name) : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Kernel Command Line... ");
    (mbi->cmdline != 0) ? printf("Success!\n") : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Module Count... ");
    (mbi->mods_count != 0) ? printf("Success!\n") : printf("Failed!\n");

    printf("\n");

    if (initSMBios()) printf("[ SMBIOS ] SMBIOS is supported!\n");

    printf("\n");

    printf("[ GDT ] Loading GDT... Success!\n");
    gdtInit();

    printf("[ IDT ] Loading IDT... ");
    idtInit();
    printf("Success!\n");

    printf("[ ISR ] Loading ISR... ");
    isrInit();
    printf("Success!\n");

    printf("[ IRQ ] Loading IRQs... ");
    irqInit();
    printf("Success!\n");

    printf("\n");

    printf("[ MEMORY ] Memory Upper: 0x%x - %d\n", mbi->mem_upper, mbi->mem_upper);
    printf("[ MEMORY ] Memory Lower: 0x%x - %d\n", mbi->mem_lower, mbi->mem_lower);
    printf("[ MEMORY ] Memory Map address: 0x%x\n", mbi->mmap_addr);
    printf("[ MEMORY ] Enabling Paging... \n");
    initPaging();

    printf("\n");

    printf("[ ASM ] Setting interrupt flag... ");
    asm volatile("sti");
    printf("Success!\n");

    printf("\n");

    printf("[ VIDEO ] Checking Graphics Mode... ");
    (CHECK_FLAG(mbi->flags, 12)) ? printf("VBE Graphics Mode!\n") : printf("VGA Text Mode!\n");

    printf("[ VIDEO ] Checking Framebuffer... ");
    (CHECK_FLAG(mbi->flags, 6)) ? printf("Framebuffer Found!\n") : printf("No Framebuffer Found\n");

    printf("[ VIDEO ] Checking Framebuffer Address... ");
    (mbi->framebuffer_addr != 0) ? printf("Success! 0x%x\n", mbi->framebuffer_addr) : printf("Failed!\n");

    printf("[ VIDEO ] Checking Framebuffer Resolution... ");
    (mbi->framebuffer_width || mbi->framebuffer_height != 0)
        ? printf("%dx%d\n", mbi->framebuffer_width, mbi->framebuffer_height)
        : printf("Failed!\n");

    printf("\n");

    printf("[ KEYBOARD ] Initializing keyboard... \n");
    initKeyboard();

    printf("\n");

    printf("[ MOUSE ] Initializing mouse...\n");
    initMouse();

    printf("\n");

    printf("[ PIT ] Initializing timer...\n");
    initTimer(1000);

    printf("[ PROCESSES ] Initializing the process manager... \n");
    // initProcessManager();

    // createProcess(task_1, kernel_process.regs.eflags, (uint32_t*)kernel_process.regs.cr3);

    // yield();

    printf("Hello World!\n");

    gfxBuffer shell_buffer;

    shell_buffer.width  = 800;
    shell_buffer.height = 600;
    shell_buffer.buffer = kmalloc(shell_buffer.width * shell_buffer.height * fbBPP);

    // Draw a window inside the shell buffer
    gfxDrawRect(0, 0, shell_buffer.width, shell_buffer.height, shell_buffer, gfxColorHex(0x000000));
    gfxDrawRect(1, 1, framebuffer.width - 2, framebuffer.height - 2, shell_buffer, gfxColorHex(0xededed));

    gfxDrawRect(1, 0, framebuffer.width - 2, 20, shell_buffer, gfxColorHex(0xd8d8e3));
    gfxDrawRect(1, 0, framebuffer.width - 2, 1, shell_buffer, gfxColorHex(0x000000));

    gfxDrawRect(1, 21, framebuffer.width - 2, 1, shell_buffer, gfxColorHex(0x000000));

    gfxDrawString(5, 5, "Arial Shell", shell_buffer, gfxColorHex(0x000000));

    for (;;) {
        gfxClearBuffer(framebuffer, gfxColorHex(0x4d52e3));

        gfxDrawRect(1, 22, framebuffer.width - 2, framebuffer.height - 22, shell_buffer,
                    gfxColorHex(0xededed));
        gfxDrawString(5, 26, pressed_keys, shell_buffer, gfxColorHex(0x000000));

        gfxDrawBuffer(CTP(framebuffer.width, shell_buffer.width),
                      CTP(framebuffer.height, shell_buffer.height), shell_buffer, framebuffer);

        gfxDrawLegacyBitmap(mouseX, mouseY, systemCursor, framebuffer, gfxColorHex(0xffffff));
        gfxSwapBuffers();
    }
}