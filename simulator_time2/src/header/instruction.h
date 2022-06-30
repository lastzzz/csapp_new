#ifndef INSTRUCTION_GUARD
#define INSTRUCTION_GUARD

#include <stdint.h>

/*======================================*/
/*      instruction set architecture    */
/*======================================*/

typedef enum OPERAND_TYPE
{
    OD_EMPTY,                  // 0
    OD_IMM,                    // 1
    OD_REG,                    // 2
    OD_MEM,                    // 3
} od_type_t;

typedef struct OPERAND_STRUCT
{
    od_type_t   type;   // OD_IMM, OD_REG, OD_MEM
    uint64_t    value;  // the value
} od_t;

// handler table storing the handlers to different instruction types
typedef void (*op_t)(od_t *, od_t *);

// local variables are allocated in stack in run-time
// we don't consider local STATIC variables
// ref: Computer Systems: A Programmer's Perspective 3rd
// Chapter 7 Linking: 7.5 Symbols and Symbol Tables
typedef struct INST_STRUCT
{
    op_t    op;         // enum of operators. e.g. mov, call, etc.
    od_t    src;        // operand src of instruction
    od_t    dst;        // operand dst of instruction
} inst_t;
// sizeof(inst_t) = 0x40

#define MAX_NUM_INSTRUCTION_CYCLE 100

#endif




// #ifndef INSTRUCTION_GUARD
// #define INSTRUCTION_GUARD

// #include <stdint.h>

// /*=========================================*/
// /*       instruction set architecture      */
// /*=========================================*/


// // data structures

// typedef enum INST_OPERATOR{
    
//     INST_MOV,       //0
//     INST_PUSH,      //1
//     INST_POP,       //2
//     INST_LEAVE,     //3
//     INST_CALL,      //4
//     INST_RET,       //5
//     INST_ADD,       //6
//     INST_SUB,       //7
//     INST_CMP,       //8
//     INST_JNE,       //9
//     INST_JMP,       //10
// }op_t;


// typedef enum OPERAND_TYPE{
//     EMPTY,                      //0
//     IMM,                        //1
//     REG,                        //2
//     MEM_IMM,                    //3
//     MEM_REG1,                   //4
//     MEM_IMM_REG1,               //5
//     MEM_REG1_REG2,              //6
//     MEM_IMM_REG1_REG2,          //7
//     MEM_REG2_SCAL,              //8
//     MEM_IMM_REG2_SCAL,          //9
//     MEM_REG1_REG2_SCAL,         //10
//     MEM_IMM_REG1_REG2_SCAL,     //11
// }od_type_t;

// typedef struct OPENRAND_STRUCT{

//     od_type_t type;     // IMM, REG, MEM
//     uint64_t  imm;      //immediate number
//     uint64_t  scal;     //scale number to register 2
//     uint64_t  reg1;     //main register
//     uint64_t  reg2;     //register
// }od_t;



// typedef struct INST_STRUCT{

//         op_t    op;
//         od_t    src;
//         od_t    dst;
// }inst_t;

// #define MAX_INSTRUCTION_CHAR 64






// #endif