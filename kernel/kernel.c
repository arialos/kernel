#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "multiboot.h"

#include "vbe.h" 
#include "vga.h"   
#include "tty.h"

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

void main(multiboot_info_t *mbi, unsigned long magic)
{
    /* Initialize terminal interface */
    // initTerminal();

    if(!initVBE(mbi)) initTerminal;

    vbePutString("[ MULTIBOOT ] Checking for Magic Header... ");
    (magic != MULTIBOOT_BOOTLOADER_MAGIC) ? vbePutString("Failed!\n") : vbePutString("Success!\n");
    
    vbePutString("[ MULTIBOOT ] Checking for Memory Size... ");
    (mbi->mem_lower != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ MULTIBOOT ] Checking for Boot Loader Name... ");
    (mbi->boot_loader_name != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ MULTIBOOT ] Checking for Kernel Command Line... ");
    (mbi->cmdline != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ MULTIBOOT ] Checking for Module Count... ");
    (mbi->mods_count != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ VIDEO ] Checking Graphics Mode... ");
    (CHECK_FLAG (mbi->flags, 12)) ? vbePutString("VBE Graphics Mode!\n") : vbePutString("VGA Text Mode!\n");

    vbePutString("[ VIDEO ] Checking Framebuffer... ");
    (CHECK_FLAG (mbi->flags, 6)) ? vbePutString("Framebuffer Found!\n") : vbePutString("No Framebuffer Found!\n");

    vbePutString("[ VIDEO ] Checking Framebuffer Address... ");
    (mbi->framebuffer_addr != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ VIDEO ] Checking Framebuffer Pitch... ");
    (mbi->framebuffer_pitch != 0) ? vbePutString("Success!\n") : vbePutString("Failed!\n");

    vbePutString("[ VIDEO ] Checking Framebuffer Width... ");
    (mbi->framebuffer_width != 0) ? vbePutString(mbi->framebuffer_width) : vbePutString("Failed!");
    vbePutString("\n");

    vbePutString("[ VIDEO ] Checking Framebuffer Height... ");
    (mbi->framebuffer_height != 0) ? vbePutString(mbi->framebuffer_height) : vbePutString("Failed!");
    vbePutString("\n");


    vbePutString("\nWelcome to the Arial Kernel!");

    vbeDrawRect(((mbi->framebuffer_width / 2) - 50 +8), ((mbi->framebuffer_height / 2) - 50 +8), 100, 100, vbeColor(0, 0, 0));
    vbeDrawRect(((mbi->framebuffer_width / 2) - 47 +8), ((mbi->framebuffer_height / 2) - 47 +8), 94, 94, vbeColor(0xFF, 0xFF, 0xFF));
    
    vbeDrawRect(((mbi->framebuffer_width / 2) - 50 -8), ((mbi->framebuffer_height / 2) - 50 -8), 100, 100, vbeColor(0, 0, 0));

    // /* Newline support is left as an exercise. */
    // vbePutString("\n\7Welcome to the Arial Kernel\n");
}