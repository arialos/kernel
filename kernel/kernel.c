#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "vbe.h"
#include "tty.h"
#include "font.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

void main(multiboot_info_t *mbi, unsigned long magic)
{
    /* Initialize terminal interface */
    // initTerminal();

    if (!initVBE(mbi))
        initTty();

    printf("[ MULTIBOOT ] Checking for Magic Header... ");
    (magic == MULTIBOOT_BOOTLOADER_MAGIC) ? printf("%1Success!%3 0x%x\n", MULTIBOOT_BOOTLOADER_MAGIC) : printf("%2Failed!%3\n");

    printf("[ MULTIBOOT ] Checking for Memory Size... ");
    (mbi->mem_lower != 0) ? printf("%1Success!%3 0x%x\n", mbi->mem_lower) : printf("%2Failed!%3\n");

    printf("[ MULTIBOOT ] Checking for Boot Loader Name... ");
    (mbi->boot_loader_name != 0) ? printf("%1Success!%3 \"%s\"\n", mbi->boot_loader_name) : printf("%2Failed!%3\n");

    printf("[ MULTIBOOT ] Checking for Kernel Command Line... ");
    (mbi->cmdline != 0) ? printf("%1Success!%3\n") : printf("%2Failed!%3\n");

    printf("[ MULTIBOOT ] Checking for Module Count... ");
    (mbi->mods_count != 0) ? printf("%1Success!%3\n") : printf("%2Failed!%3\n");

    printf("[ VIDEO ] Checking Graphics Mode... ");
    (CHECK_FLAG(mbi->flags, 12)) ? printf("VBE Graphics Mode!\n") : printf("VGA Text Mode!\n");

    printf("[ VIDEO ] Checking Framebuffer... ");
    (CHECK_FLAG(mbi->flags, 6)) ? printf("Framebuffer Found!\n") : printf("%2No Framebuffer Found%3\n");

    printf("[ VIDEO ] Checking Framebuffer Address... ");
    (mbi->framebuffer_addr != 0) ? printf("%1Success!%3 0x%x\n", mbi->framebuffer_addr) : printf("%2Failed!%3\n");

    printf("[ VIDEO ] Checking Framebuffer Pitch... ");
    (mbi->framebuffer_pitch != 0) ? printf("%1Success!%3 %d\n", mbi->framebuffer_pitch) : printf("%2Failed!%3\n");

    printf("[ VIDEO ] Checking Framebuffer Width... ");
    (mbi->framebuffer_width != 0) ? printf("%dpx\n", mbi->framebuffer_width) : printf("%2Failed!%3\n");

    printf("[ VIDEO ] Checking Framebuffer Height... ");
    (mbi->framebuffer_height != 0) ? printf("%dpx\n", mbi->framebuffer_height) : printf("%2Failed!%3\n");

    printf("\nWelcome to the Arial Kernel!\n");

    vbeDrawRect(((mbi->framebuffer_width / 2) - 50 + 8), ((mbi->framebuffer_height / 2) - 50 + 8), 100, 100, vbeColor(0xFF, 0xFF, 0xFF));
    vbeDrawRect(((mbi->framebuffer_width / 2) - 47 + 8), ((mbi->framebuffer_height / 2) - 47 + 8), 94, 94, vbeColor(0, 0, 0));

    vbeDrawRect(((mbi->framebuffer_width / 2) - 50 - 8), ((mbi->framebuffer_height / 2) - 50 - 8), 100, 100, vbeColor(0xFF, 0xFF, 0xFF));

    cursorY = ((mbi->framebuffer_height / 2) - 60);
    cursorX = ((mbi->framebuffer_width / 2) - (strlen("arial.os") * fontWidth / 2));
    printf("arial.os");
    // /* Newline support is left as an exercise. */
    // printf("\n\7Welcome to the Arial Kernel\n");
}