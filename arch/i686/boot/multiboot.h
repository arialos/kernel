
#ifndef MULTIBOOT_H
#define MULTIBOOT_H 1

#include "stddef.h"

/* How many bytes from the start of the file we search for the header. */
#define MULTIBOOT_SEARCH       8192
#define MULTIBOOT_HEADER_ALIGN 4

/* The magic field should contain this. */
#define MULTIBOOT_HEADER_MAGIC 0x1BADB002

/* This should be in %eax. */
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

/* Alignment of multiboot modules. */
#define MULTIBOOT_MOD_ALIGN 0x00001000

/* Alignment of the multiboot info structure. */
#define MULTIBOOT_INFO_ALIGN 0x00000004

/* Flags set in the ’flags’ member of the multiboot header. */

/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN 0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO 0x00000002

/* Must pass video information to OS. */
#define MULTIBOOT_VIDEO_MODE 0x00000004

/* This flag indicates the use of the address fields in the header. */
#define MULTIBOOT_AOUT_KLUDGE 0x00010000

/* Flags to be set in the ’flags’ member of the multiboot info structure. */

/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_MEMORY 0x00000001
/* is there a boot device set? */
#define MULTIBOOT_INFO_BOOTDEV 0x00000002
/* is the command-line defined? */
#define MULTIBOOT_INFO_CMDLINE 0x00000004
/* are there modules to do something with? */
#define MULTIBOOT_INFO_MODS 0x00000008

/* These next two are mutually exclusive */

/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_AOUT_SYMS 0x00000010
/* is there an ELF section header table? */
#define MULTIBOOT_INFO_ELF_SHDR 0X00000020

/* is there a full memory map? */
#define MULTIBOOT_INFO_MEM_MAP 0x00000040

/* Is there drive info? */
#define MULTIBOOT_INFO_DRIVE_INFO 0x00000080

/* Is there a config table? */
#define MULTIBOOT_INFO_CONFIG_TABLE 0x00000100

/* Is there a boot loader name? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME 0x00000200

/* Is there a APM table? */
#define MULTIBOOT_INFO_APM_TABLE 0x00000400

/* Is there video information? */
#define MULTIBOOT_INFO_VBE_INFO         0x00000800
#define MULTIBOOT_INFO_FRAMEBUFFER_INFO 0x00001000

#ifndef ASM_FILE

typedef struct MultibootHeader {
    /* Must be MULTIBOOT_MAGIC - see above. */
    uint32_t magic;

    /* Feature flags. */
    uint32_t flags;

    /* The above fields plus this one must equal 0 mod 2^32. */
    uint32_t checksum;

    /* These are only valid if MULTIBOOT_AOUT_KLUDGE is set. */
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;

    /* These are only valid if MULTIBOOT_VIDEO_MODE is set. */
    uint32_t mode_type;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
} MultibootHeader;

/* The symbol table for a.out. */
typedef struct MultibootAoutSymbolTable {
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved;
} MultibootAoutSymbolTable;

/* The section header table for ELF. */
typedef struct MultibootElfSectionHeaderTable {
    uint32_t num;
    uint32_t size;
    uint32_t addr;
    uint32_t shndx;
} MultibootElfSectionHeaderTable;

typedef struct VbeInfo {
    uint16_t attributes;
    uint8_t winA, winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA, segmentB;
    uint32_t realFctPtr;
    uint16_t pitch;
    uint16_t Xres, Yres;
    uint8_t Wchar, Ychar, planes, bpp, banks;
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;
    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t rsv_mask, rsv_position;
    uint8_t directcolor_attributes;
    uint32_t physbase;
    uint32_t reserved1;
    uint16_t reserved2;
} __attribute__((packed)) VbeInfo;

typedef struct MultibootInfo {
    /* Multiboot info version number */
    uint32_t flags;

    /* Available memory from BIOS */
    uint32_t mem_lower;
    uint32_t mem_upper;

    /* "root" partition */
    uint32_t boot_device;

    /* Kernel command line */
    uint32_t cmdline;

    /* Boot-Module list */
    uint32_t mods_count;
    uint32_t mods_addr;

    union {
        MultibootAoutSymbolTable aout_sym;
        MultibootElfSectionHeaderTable elf_sec;
    } u;

    /* Memory Mapping buffer */
    uint32_t mmap_length;
    uint32_t mmap_addr;

    /* Drive Info buffer */
    uint32_t drives_length;
    uint32_t drives_addr;

    /* ROM configuration table */
    uint32_t config_table;

    /* Boot Loader Name */
    uint32_t boot_loader_name;

    /* APM table */
    uint32_t apm_table;

    /* Video */
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED  0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB      1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2

    uint8_t framebuffer_type;
    union {
        struct {
            uint32_t framebuffer_palette_addr;
            uint16_t framebuffer_palette_num_colors;
        };
        struct {
            uint8_t framebuffer_red_field_position;
            uint8_t framebuffer_red_mask_size;
            uint8_t framebuffer_green_field_position;
            uint8_t framebuffer_green_mask_size;
            uint8_t framebuffer_blue_field_position;
            uint8_t framebuffer_blue_mask_size;
        };
    };
} MultibootInfo;

typedef struct MultibootColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} MultibootColor;

typedef struct MultibootMmapEntry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;

#define MULTIBOOT_MEMORY_AVAILABLE        1
#define MULTIBOOT_MEMORY_RESERVED         2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS              4
#define MULTIBOOT_MEMORY_BADRAM           5

    uint32_t type;
} __attribute__((packed)) MultibootMmapEntry;

typedef struct MultibootModList {
    /* the memory used goes from bytes ’mod_start’ to ’mod_end-1’ inclusive */
    uint32_t mod_start;
    uint32_t mod_end;

    /* Module command line */
    uint32_t cmdline;

    /* padding to take it to 16 bytes (must be zero) */
    uint32_t pad;
} MultibootModList;

/* APM BIOS info. */
typedef struct MultibootApmInfo {
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg_16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_len;
    uint16_t cseg_16_len;
    uint16_t dseg_len;
} MultibootApmInfo;

#endif /* ! ASM_FILE */

#endif /* ! MULTIBOOT_HEADER */