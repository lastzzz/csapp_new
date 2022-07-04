/* BCST - Introduction to Computer Systems
 * Author:      yangminz@outlook.com
 * Github:      https://github.com/yangminz/bcst_csapp
 * Bilibili:    https://space.bilibili.com/4564101
 * Zhihu:       https://www.zhihu.com/people/zhao-yang-min
 * This project (code repository and videos) is exclusively owned by yangminz 
 * and shall not be used for commercial and profitting purpose 
 * without yangminz's permission.
 */

// Dynamic Random Access Memory
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "header/cpu.h"
#include "header/memory.h"
#include "header/common.h"
#include "header/address.h"

void set_pagemap_swapaddr(uint64_t ppn, uint64_t swap_address);

// each swap file is swap page
// each line of this swap page is one uint64
#define SWAP_PAGE_FILE_LINES (512)
#define SWAP_ADDRESS_MIN (100)

// disk address counter
static char *SWAP_FILE_DIRECTORY = "./files/swap";
static uint64_t internal_swap_addr = SWAP_ADDRESS_MIN;

uint64_t allocate_swappage(uint64_t ppn)
{
    uint64_t saddr = internal_swap_addr++;
    
    char filename[128];
    sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIRECTORY, saddr);
    FILE *fw = fopen(filename, "w");
    assert(fw != NULL);

    // write zero page for anoymous page
    // But there is no transaction actually
    fclose(fw);
    uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
    memset(&pm[ppn_ppo], 0, PAGE_SIZE);
    
    // Now the page is like swapped in from swap space. So:
    // saddr is stored on page_map
    // ppn is sotred in page table entry (level 4)
    // we need to tell page_map the new saddr
    set_pagemap_swapaddr(ppn, saddr);

    return saddr;
}

int swap_in(uint64_t saddr, uint64_t ppn)
{
    assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
    
    FILE *fr = NULL;
    char filename[128];

    if (saddr == 0)
    {
        // saddr == 0 indicates that this page is not backed by file
        // nor backed by swap space. It should be a newly created 
        // anoymous page. Allocate one swap address for it.
        allocate_swappage(ppn);
        return 0;
    }

    assert(saddr >= SWAP_ADDRESS_MIN);
    sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIRECTORY, saddr);
    fr = fopen(filename, "r");
    assert(fr != NULL);

    uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
    char buf[64] = {'0'};
    for (int i = 0; i < SWAP_PAGE_FILE_LINES; ++ i)
    {
        char *str = fgets(buf, 64, fr);
        *((uint64_t *)(&pm[ppn_ppo + i * 8])) = string2uint(str);
    }
    fclose(fr);
    return 1;
}

int swap_out(uint64_t saddr, uint64_t ppn)
{
    assert(0 <= ppn && ppn < MAX_NUM_PHYSICAL_PAGE);
    assert(saddr >= SWAP_ADDRESS_MIN);

    FILE *fw = NULL;
    char filename[128];
    sprintf(filename, "%s/page-%ld.page.txt", SWAP_FILE_DIRECTORY, saddr);
    fw = fopen(filename, "w");
    assert(fw != NULL);

    uint64_t ppn_ppo = ppn << PHYSICAL_PAGE_OFFSET_LENGTH;
    for (int i = 0; i < SWAP_PAGE_FILE_LINES; ++ i)
    {
        fprintf(fw, "0x%16lx\n", *((uint64_t *)(&pm[ppn_ppo + i * 8])));
    }
    fclose(fw);
    return 0;
}