#include "paging.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "heap.h"
#include "isr.h"
#include "libio.h"
#include "libmath.h"
#include "libstring.h"

uint32_t *current_directory;

static void pageFault(Registers *regs) {
    (void)regs;

    volatile uint32_t fault_addr;
    asm("mov %%cr2, %0" : "=r"(fault_addr));

    printf("[ PANIC ] Page fault at 0x%x\n", fault_addr);
    printf("[ PANIC ] EAX: 0x%x EBX: 0x%x\n", regs->eax, regs->ebx);
    printf("[ PANIC ] ESP: 0x%x EBP: 0x%x\n", regs->esp, regs->ebp);
    printf("[ PANIC ] ESI: 0x%x EDI: 0x%x\n", regs->esi, regs->edi);
    printf("[ PANIC ] EIP: 0x%x\n", regs->eip);

    panic("Page fault");
}

void pagingSwitchDirectory(uint32_t *dir) {
    current_directory = dir;
    extern void enablePaging(uint32_t);
    enablePaging((uint32_t)current_directory);
}

static uint32_t *identityMapTable(uint32_t start, int flags) {
    /* make a page table */
    uint32_t *table = kmallocA(0x1000);
    /* identity map 4MB */
    for (uint32_t i = start; i < start + 1024; ++i) {
        table[i - start] = (i * PAGE_SIZE) | flags;
    }
    return table;
}

int initPaging(void) {
    uint32_t *kernel_directory = kmallocA(0x1000);
    memset(kernel_directory, 0, 0x1000);
    /* blank the kernel directory */
    for (int i = 0; i < 1024; ++i) {
        kernel_directory[i] = PAGE_RW;
    }

    /* identity map all 4GB (and allocate all page tables) */
    for (int i = 0; i < 1024; ++i)
        kernel_directory[i] =
            (uint32_t)identityMapTable(i * 1024, PAGE_PRESENT | PAGE_RW) | PAGE_PRESENT | PAGE_RW;

    irqInstallHandler(14, pageFault);

    pagingSwitchDirectory(kernel_directory);

    return 0;
}
