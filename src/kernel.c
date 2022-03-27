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

    if (CHECK_FLAG (mbi->flags, 12))
    {
      multiboot_uint32_t color;
      unsigned i;
      void *fb = (void *) (unsigned long) mbi->framebuffer_addr;

      switch (mbi->framebuffer_type)
        {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
          {
            unsigned best_distance, distance;
            struct multiboot_color *palette;
            
            palette = (struct multiboot_color *) mbi->framebuffer_palette_addr;

            color = 0;
            best_distance = 4*256*256;
            
            for (i = 0; i < mbi->framebuffer_palette_num_colors; i++)
              {
                distance = (0xff - palette[i].blue) * (0xff - palette[i].blue)
                  + palette[i].red * palette[i].red
                  + palette[i].green * palette[i].green;
                if (distance < best_distance)
                  {
                    color = i;
                    best_distance = distance;
                  }
              }
          }
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
          color = ((1 << mbi->framebuffer_blue_mask_size) - 1) 
            << mbi->framebuffer_blue_field_position;
          break;

        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
          color = '\\' | 0x0100;
          break;

        default:
          color = 0xffffffff;
          break;
        }
      for (i = 0; i < mbi->framebuffer_width
             && i < mbi->framebuffer_height; i++)
        {
          switch (mbi->framebuffer_bpp)
            {
            case 8:
              {
                multiboot_uint8_t *pixel = fb + mbi->framebuffer_pitch * i + i;
                *pixel = color;
              }
              break;
            case 15:
            case 16:
              {
                multiboot_uint16_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 2 * i;
                *pixel = color;
              }
              break;
            case 24:
              {
                multiboot_uint32_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 3 * i;
                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
              }
              break;

            case 32:
              {
                multiboot_uint32_t *pixel
                  = fb + mbi->framebuffer_pitch * i + 4 * i;
                *pixel = color;
              }
              break;
            }
        }
    }
    
}