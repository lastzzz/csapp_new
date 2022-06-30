/* BCST - Introduction to Computer Systems
 * Author:      yangminz@outlook.com
 * Github:      https://github.com/yangminz/bcst_csapp
 * Bilibili:    https://space.bilibili.com/4564101
 * Zhihu:       https://www.zhihu.com/people/zhao-yang-min
 * This project (code repository and videos) is exclusively owned by yangminz 
 * and shall not be used for commercial and profitting purpose 
 * without yangminz's permission.
 */

// include guards to prevent double declaration of any identifiers 
// such as types, enums and static variables
#ifndef DEBUG_GUARD
#define DEBUG_GUARD

#include <stdint.h>

#define DEBUG_PRINTCACHESET         (0x8)
#define DEBUG_CACHEDETAILS          (0x10)
#define DEBUG_MMU                   (0x20)
#define DEBUG_LOADER                (0x80)
#define DEBUG_PARSEINST             (0x100)

#define DEBUG_VERBOSE_SET           (0x241)

// do page walk
#define DEBUG_ENABLE_PAGE_WALK      (0)

// type converter
// uint32 to its equivalent float with rounding
uint32_t uint2float(uint32_t u);

// convert string dec or hex to the integer bitmap
typedef enum
{
    STRING2UINT_LEADING_SPACE,
    STRING2UINT_FIRST_ZERO,
    STRING2UINT_POSITIVE_DEC,
    STRING2UINT_POSITIVE_HEX,
    STRING2UINT_NEGATIVE,
    STRING2UINT_NEGATIVE_FIRST_ZERO,
    STRING2UINT_NEGATIVE_DEC,
    STRING2UINT_NEGATIVE_HEX,
    STRING2UINT_ENDING_SPACE,
    STRING2UINT_FAILED,
} string2uint_state_t;
string2uint_state_t string2uint_next(string2uint_state_t state, char c, uint64_t *bmap);
uint64_t string2uint(const char *str);
uint64_t string2uint_range(const char *str, int start, int end);

// commonly shared variables
#define MAX_INSTRUCTION_CHAR (64)

/*======================================*/
/*      wrap of the memory              */
/*======================================*/
void *tag_malloc(uint64_t size, char *tagstr);
int tag_free(void *ptr);
void tag_sweep(char *tagstr);

#endif


// // include guards to prevent double declaration of any identifiers 
// // such as types, enums and static variables

// #ifndef DEBUG_GUARD
// #define DEBUG_GUARD

// #include <stdint.h>

// #define DEBUG_INSTRUCTIONCYCLE 0x1
// #define DEBUG_REGISTERS        0x2
// #define DEBUG_PRINTSTACK       0x4
// #define DEBUG_PRINTCACHESET    0x8
// #define DEBUG_CACHEDETAILS     0x10
// #define DEBUG_MMU              0x20
// #define DEBUG_LINKER           0x40
// #define DEBUG_LOADER           0x80
// #define DEBUG_PARSEINST        0x100

// #define DEBUG_VERBOSE_SET      0x41


// // do page walk
// #define DEBUG_ENABLE_PAGE_WALE 0


// // use sram cache for memory access
// #define DEBUG_ENABLE_SRAM_CACHE 0

// // printf wrapper
// uint64_t debug_printf(uint64_t open_set, const char *format, ...);


// // type converter
// // uint32 to its equivalent float with rounding
// uint32_t uint2float(uint32_t u);

// // convert string dec or hex to the integer bitmap
// uint64_t string2uint(const char *str);

// uint64_t string2uint_range(const char *str, int start, int end);






// #endif
