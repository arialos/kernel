#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "multiboot.h"

#include "include/vga.h"    
#include "include/tty.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void main(multiboot_info_t *mbi, unsigned long magic)
{
    /* Initialize terminal interface */
    initTerminal();



    printF("[ MULTIBOOT ] Checking for Magic Header... ");
    (magic != MULTIBOOT_BOOTLOADER_MAGIC) ? printF("\4Failed!\n") : printF("\2Success!\n");
    
    printF("[ MULTIBOOT ] Checking for Memory Size... ");
    (mbi->mem_lower != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ MULTIBOOT ] Checking for Boot Loader Name... ");
    (mbi->boot_loader_name != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ MULTIBOOT ] Checking for Kernel Command Line... ");
    (mbi->cmdline != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ MULTIBOOT ] Checking for Module Count... ");
    (mbi->mods_count != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ VIDEO ] Checking Graphics Mode... ");
    (CHECK_FLAG (mbi->flags, 12)) ? printF("\2VGA Graphics Mode!\n") : printF("\4VGA Text Mode!\n");

    printF("[ VIDEO ] Checking Framebuffer... ");
    (CHECK_FLAG (mbi->flags, 6)) ? printF("\2Framebuffer Found!\n") : printF("\4No Framebuffer Found!\n");

    printF("[ VIDEO ] Checking Framebuffer Address... ");
    (mbi->framebuffer_addr != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ VIDEO ] Checking Framebuffer Pitch... ");
    (mbi->framebuffer_pitch != 0) ? printF("\2Success!\n") : printF("\4Failed!\n");

    printF("[ VIDEO ] Checking Framebuffer Width... ");
    (mbi->framebuffer_width != 0) ? printF(mbi->framebuffer_width) : printF("\4Failed!");
    printF("\n");

    printF("[ VIDEO ] Checking Framebuffer Height... ");
    (mbi->framebuffer_height != 0) ? printF(mbi->framebuffer_height) : printF("\4Failed!");
    printF("\n");


    /* Newline support is left as an exercise. */
    printF("\n\7Welcome to the Arial Kernel\n");

    fillrect((uint16_t *)0xB8000, 0, 320, 200, 100,100);
}