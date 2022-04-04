#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libio.h"

#include "multiboot.h"
#include "version.h"
#include "keyboard.h"
#include "gfx.h"
#include "tty.h"
#include "font.h"

#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

void main(multiboot_info_t *mbi, unsigned long magic)
{
    // Initialize terminal interface
    if (!initGFX(mbi))
        initTty();


    printf("Welcome to the Arial Kernel!\n");
    printf("Version: %s\n", KERNEL_VERSION);
    printf("Codename: %s\n", KERNEL_CODENAME);
    printf("Build date: %s\n", KERNEL_BUILD_DATE);
    printf("Build time: %s\n\n", KERNEL_BUILD_TIME);
    
    gfxDrawRect(((mbi->framebuffer_width / 2) - 50 + 8), ((mbi->framebuffer_height / 2) - 50 + 8), 100, 100, gfxColor(0xFF, 0xFF, 0xFF));
    gfxDrawRect(((mbi->framebuffer_width / 2) - 47 + 8), ((mbi->framebuffer_height / 2) - 47 + 8), 94, 94, gfxColor(0, 0, 0));

    gfxDrawRect(((mbi->framebuffer_width / 2) - 50 - 8), ((mbi->framebuffer_height / 2) - 50 - 8), 100, 100, gfxColor(0xFF, 0xFF, 0xFF));

    printf("[ MULTIBOOT ] Checking for Magic Header... ");
    (magic == MULTIBOOT_BOOTLOADER_MAGIC) ? printf("Success! 0x%x\n", MULTIBOOT_BOOTLOADER_MAGIC) : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Memory Size... ");
    (mbi->mem_lower != 0) ? printf("Success! 0x%x\n", mbi->mem_lower) : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Boot Loader Name... ");
    (mbi->boot_loader_name != 0) ? printf("Success! \"%s\"\n", mbi->boot_loader_name) : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Kernel Command Line... ");
    (mbi->cmdline != 0) ? printf("Success!\n") : printf("Failed!\n");

    printf("[ MULTIBOOT ] Checking for Module Count... ");
    (mbi->mods_count != 0) ? printf("Success!\n") : printf("Failed!\n");

    printf("\n");

    printf("[ VIDEO ] Checking Graphics Mode... ");
    (CHECK_FLAG(mbi->flags, 12)) ? printf("VBE Graphics Mode!\n") : printf("VGA Text Mode!\n");

    printf("[ VIDEO ] Checking Framebuffer... ");
    (CHECK_FLAG(mbi->flags, 6)) ? printf("Framebuffer Found!\n") : printf("No Framebuffer Found\n");

    printf("[ VIDEO ] Checking Framebuffer Address... ");
    (mbi->framebuffer_addr != 0) ? printf("Success! 0x%x\n", mbi->framebuffer_addr) : printf("Failed!\n");

    printf("[ VIDEO ] Checking Framebuffer Pitch... ");
    (mbi->framebuffer_pitch != 0) ? printf("Success! %d\n", mbi->framebuffer_pitch) : printf("Failed!\n");

    printf("[ VIDEO ] Checking Framebuffer Width... ");
    (mbi->framebuffer_width != 0) ? printf("%dpx\n", mbi->framebuffer_width) : printf("Failed!\n");

    printf("[ VIDEO ] Checking Framebuffer Height... ");
    (mbi->framebuffer_height != 0) ? printf("%dpx\n", mbi->framebuffer_height) : printf("Failed!\n");

    printf("\n");
    // /* Newline support is left as an exercise. */
    // printf("\n\7Welcome to the Arial Kernel\n");

}