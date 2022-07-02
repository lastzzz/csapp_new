// include guards to prevnet double declaration of any identifiers
// such as types ,enums and static variables
#ifndef MEMORY_GUARD
#define MEMORY_GUARD

#include <stdint.h>
#include <header/cpu.h>


/*==============================================*/
/*          physical memory on dram chips       */
/* =============================================*/


// physical memory space is decided by the physical address
// in this simulator, there are 4 + 6 + 6 = 16 bit physical adderss
// then the physical space is (1 << 16) = 65536
// total 16 physical memory


#define PHYSICAL_MEMORY_SPACE (65536)
#define MAX_NUM_PHYSICAL_PAGE (16)    // 1 + MAX_INDEX_PHYSICAL_PAGE

#define PAGE_TABLE_ENTRY_NUM    (512)
#define PAGE_SIZE    (4096)

// physical memory
// 16 physical memory pages
// used only for user process
uint8_t pm[PHYSICAL_MEMORY_SPACE];




// page table entry struct

// 8 bytes = 64 bits
typedef union{
    uint64_t pte_value;

    struct{
        uint64_t present            : 1;
        uint64_t readonly           : 1;
        uint64_t usermode           : 1;
        uint64_t writethough        : 1;
        uint64_t cachedisabled      : 1;
        uint64_t reference          : 1;
        uint64_t unused6            : 1;
        uint64_t smallpage          : 1;
        uint64_t global             : 1;
        uint64_t unused9_11         : 3;
        /*
        uint64_t paddr              : 40;
        uint64_t unused52_62        : 10;
        for malloc, a virtual address on heap is 48 bits
        for real world, a physical page number is 40 bits
        */
        uint64_t paddr              : 50;   // virtual address (48 bits) on simulator's heap
        uint64_t xdisabled          : 1;
    };

    struct{
        uint64_t _present           : 1;
        uint64_t saddr            : 63;   // swap space address
    };
} pte123_t; // PGD, PUD, PMD



// 8 bytes = 64 bits
typedef union{
    uint64_t pte_value;

    struct{
        uint64_t present            : 1;    // present = 1
        uint64_t readonly           : 1;
        uint64_t usermode           : 1;
        uint64_t writethough        : 1;
        uint64_t cachedisabled      : 1;
        uint64_t reference          : 1;
        uint64_t dirty              : 1;    // dirty bit - 1: dirty; 0: clean
        uint64_t zero7              : 1;
        uint64_t global             : 1;
        uint64_t unused9_11         : 3;
        uint64_t ppn                : 40;
        uint64_t unused52_62        : 10;
        uint64_t xdisabled          : 1;
    };

    struct{
        uint64_t _present           : 1;    // present = 0
        uint64_t saddr            : 63;   // swap space address
    };
} pte4_t;   // PT


// physical page descriptor
// typedef struct{
    
//     int allocated;
//     int dirty;
//     int time; // LRU cache


//     // real world： mapping to anon_vma or address_space
//     // we simply the simulator here
//     // T0DO: if multiple processes are using this page? E.g shared Library
//     pte4_t *pte4; // the reversed mapping: from PPN to page table entry
//     uint64_t daddr;   // binding the revesed mapping with mapping to disk
// }pd_t;

// for each pagable (swappable) physical page
// create one reversed mapping
// pd_t page_map[MAX_NUM_PHYSICAL_PAGE];


/*======================================*/
/*      memory R/W                      */
/*======================================*/

// used by instructions: read or write uint64_t to DRAM
uint64_t cpu_read64bits_dram(uint64_t paddr);
void cpu_write64bits_dram(uint64_t paddr, uint64_t data);
void cpu_readinst_dram(uint64_t paddr, char *buf);
void cpu_writeinst_dram(uint64_t paddr, const char *str);


void bus_read_cacheline(uint64_t paddr, uint8_t *block);
void bus_write_cacheline(uint64_t paddr, uint8_t *block);



#endif