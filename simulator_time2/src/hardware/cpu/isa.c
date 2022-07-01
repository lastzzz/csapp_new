#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/cpu.h"
#include "../../header/memory.h"
#include "../../header/common.h"
#include "../../header/instruction.h"

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




static uint64_t decode_operand(od_t *od);

static uint64_t decode_operand(od_t *od){

    
    if (od->type == IMM){
        return *((uint64_t *)&od->imm);//返回imm的值
    }
    else if (od->type == REG){
        return (uint64_t) od->reg1;// 返回指向reg的地址
    }
    else {
        uint64_t vaddr = 0;
        if (od->type == MEM_IMM){
            vaddr = od->imm;
        }
        else if (od->type == MEM_REG1){
            vaddr = (*((uint64_t *)od->reg1));
        }
        else if (od->type == MEM_IMM_REG1){
            vaddr = od->imm + (*((uint64_t *)od->reg1));
        }
        else if (od->type == MEM_REG1_REG2){
            vaddr = (*((uint64_t *)od->reg1)) + (*((uint64_t *)od->reg2));
        }
        else if (od->type == MEM_IMM_REG1_REG2){
            vaddr = (*((uint64_t *)od->reg1)) + (*((uint64_t *)od->reg2)) + od->imm;
        }
        else if (od->type == MEM_REG2_SCAL){
            vaddr = (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if (od->type == MEM_IMM_REG2_SCAL){
            vaddr = od->imm + (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if (od->type == MEM_REG1_REG2_SCAL){
            vaddr = (*((uint64_t *)od->reg1)) + (*((uint64_t *)od->reg2)) * od->scal;
        }
        else if (od->type == MEM_IMM_REG1_REG2_SCAL){
            vaddr = od->imm + (*((uint64_t *)od->reg1)) + (*((uint64_t *)od->reg2)) * od->scal;
        }
        return vaddr;

    }

}


// lookup table
static const char *reg_name_list[72] ={
    "%rax", "%eax", "%ax", "%ah", "%al",
    "%rbx", "%ebx", "%bx", "%bh", "%bl",
    "%rcx", "%ecx", "%cx", "%ch", "%cl",
    "%rdx", "%edx", "%dx", "%dh", "%dl",
    "%rsi", "%esi", "%si", "%sih", "%sil",
    "%rdi", "%edi", "%di", "%dih", "%dil",
    "%rbp", "%ebp", "%bp", "%bph", "%bpl",
    "%rsp", "%esp", "%sp", "%sph", "%spl",
    "%r8", "%r8d", "%r8w", "%r8b",
    "%r9", "%r9d", "%r9w", "%r9b",
    "%r10", "%r10d", "%r10w", "%r10b",
    "%r11", "%r11d", "%r11w", "%r11b",
    "%r12", "%r12d", "%r12w", "%r12b",
    "%r13", "%r13d", "%r13w", "%r13b",
    "%r14", "%r14d", "%r14w", "%r14b",
    "%r15", "%r15d", "%r15w", "%r15b",
};

static uint64_t reflect_register(const char *str){


    // lookup table


    // reg_t *reg = &(cr->reg);

    //-----------------------寄存器地址为64位
    uint64_t reg_addr[72] = {
        (uint64_t)&(cpu_reg.rax),(uint64_t)&(cpu_reg.eax),(uint64_t)&(cpu_reg.ax),(uint64_t)&(cpu_reg.ah),(uint64_t)&(cpu_reg.al),
        (uint64_t)&(cpu_reg.rbx),(uint64_t)&(cpu_reg.ebx),(uint64_t)&(cpu_reg.bx),(uint64_t)&(cpu_reg.bh),(uint64_t)&(cpu_reg.bl),
        (uint64_t)&(cpu_reg.rcx),(uint64_t)&(cpu_reg.ecx),(uint64_t)&(cpu_reg.cx),(uint64_t)&(cpu_reg.ch),(uint64_t)&(cpu_reg.cl),
        (uint64_t)&(cpu_reg.rdx),(uint64_t)&(cpu_reg.edx),(uint64_t)&(cpu_reg.dx),(uint64_t)&(cpu_reg.dh),(uint64_t)&(cpu_reg.dl),
        (uint64_t)&(cpu_reg.rsi),(uint64_t)&(cpu_reg.esi),(uint64_t)&(cpu_reg.si),(uint64_t)&(cpu_reg.sih),(uint64_t)&(cpu_reg.sil),
        (uint64_t)&(cpu_reg.rdi),(uint64_t)&(cpu_reg.edi),(uint64_t)&(cpu_reg.di),(uint64_t)&(cpu_reg.dih),(uint64_t)&(cpu_reg.dil),
        (uint64_t)&(cpu_reg.rbp),(uint64_t)&(cpu_reg.ebp),(uint64_t)&(cpu_reg.bp),(uint64_t)&(cpu_reg.bph),(uint64_t)&(cpu_reg.bpl),
        (uint64_t)&(cpu_reg.rsp),(uint64_t)&(cpu_reg.esp),(uint64_t)&(cpu_reg.sp),(uint64_t)&(cpu_reg.sph),(uint64_t)&(cpu_reg.spl),
        (uint64_t)&(cpu_reg.r8),(uint64_t)&(cpu_reg.r8d),(uint64_t)&(cpu_reg.r8w),(uint64_t)&(cpu_reg.r8b),
        (uint64_t)&(cpu_reg.r9),(uint64_t)&(cpu_reg.r9d),(uint64_t)&(cpu_reg.r9w),(uint64_t)&(cpu_reg.r9b),
        (uint64_t)&(cpu_reg.r10),(uint64_t)&(cpu_reg.r10d),(uint64_t)&(cpu_reg.r10w),(uint64_t)&(cpu_reg.r10b),
        (uint64_t)&(cpu_reg.r11),(uint64_t)&(cpu_reg.r11d),(uint64_t)&(cpu_reg.r11w),(uint64_t)&(cpu_reg.r11b),
        (uint64_t)&(cpu_reg.r12),(uint64_t)&(cpu_reg.r12d),(uint64_t)&(cpu_reg.r12w),(uint64_t)&(cpu_reg.r12b),
        (uint64_t)&(cpu_reg.r13),(uint64_t)&(cpu_reg.r13d),(uint64_t)&(cpu_reg.r13w),(uint64_t)&(cpu_reg.r13b),
        (uint64_t)&(cpu_reg.r14),(uint64_t)&(cpu_reg.r14d),(uint64_t)&(cpu_reg.r14w),(uint64_t)&(cpu_reg.r14b),
        (uint64_t)&(cpu_reg.r15),(uint64_t)&(cpu_reg.r15d),(uint64_t)&(cpu_reg.r15w),(uint64_t)&(cpu_reg.r15b),
    };
    for (int i = 0; i < 72; ++i){
        if (strcmp(str, reg_name_list[i]) == 0){

            // now we know that i is the index inside reg_name_list
            return reg_addr[i];

        }
    }

    printf("parse register %s error\n", str);
    exit(0);
}





static void parse_instruction(const char *str, inst_t *inst);
static void parse_operand(const char *str, od_t *od);

// 将汇编指令转换为inst_t类型的指令
static void parse_instruction(const char *str, inst_t *inst){

    char op_str[64] = {"\0"};
    int op_len = 0;
    char src_str[64] = {"\0"};
    int src_len = 0;
    char dst_str[64] = {"\0"};
    int dst_len = 0;

    char c;
    int count_parentheses = 0;// 括号数量
    int state = 0;            // 状态转移

    for (int i = 0; i < strlen(str); ++i){
        c = str[i];
        if (c == '(' || c == ')'){
            count_parentheses++;
        }

        if (state == 0 && c != ' '){
            state = 1;
            // 不加continue，因为要将读取的字符c加入到op_str中
        }
        else if (state == 1 && c == ' '){
            state = 2;
            continue;
        }
        else if (state == 2 && c != ' '){
            state = 3;
            // 不加continue，因为要将读取的字符c加入到src_str中
        }
        else if (state == 3 && c == ',' && (count_parentheses == 0 || count_parentheses == 2)){
            state = 4;
            continue;
        }
        else if (state == 4 && c != ' ' && c != ','){
            state = 5;
            // 不加continue，因为要将读取的字符c加入到dst_str中
        }
        else if (state == 5 && c == ' '){
            state = 6;
            continue;
        }

        if (state == 1){
            op_str[op_len] = c;
            op_len++;
            continue;
        }
        else if (state == 3){
            src_str[src_len] = c;
            src_len++;
            continue;
        }
        else if (state == 5){
            dst_str[dst_len] = c;
            dst_len++;
            continue;
        }
    }

    // op_str, src_str, dst_str
    // strlen(str)
    parse_operand(src_str, &(inst->src));
    parse_operand(dst_str, &(inst->dst));

    if (strcmp(op_str, "mov") == 0 || strcmp(op_str, "movq") == 0){
        inst->op = INST_MOV;
    }
    else if (strcmp(op_str, "push") ==0){
        inst->op = INST_PUSH;
    }
    else if (strcmp(op_str, "pop") == 0){
        inst->op = INST_POP;
    }
    else if (strcmp(op_str, "leaveq") == 0){
        inst->op = INST_LEAVE;
    }
    else if (strcmp(op_str, "callq") == 0){
        inst->op = INST_CALL;
    }
    else if (strcmp(op_str, "retq") == 0){
        inst->op = INST_RET;
    }
    else if (strcmp(op_str, "add") == 0){
        inst->op = INST_ADD;
    }
    else if (strcmp(op_str, "sub") == 0){
        inst->op = INST_SUB;
    }
    else if (strcmp(op_str, "cmpq") == 0){
        inst->op = INST_CMP;
    }
    else if (strcmp(op_str, "jne") == 0){
        inst->op = INST_JNE;
    }
    else if (strcmp(op_str, "jmp") == 0){
        inst->op = INST_JMP;
    }

    debug_printf(DEBUG_PARSEINST, "[%s (%d)] [%s (%d)] [%s (%d)]\n", op_str, inst->op,
    src_str, inst->src.type, dst_str, inst->dst.type);

}

// 将字符串解析为openrand的类型
static void parse_operand(const char *str, od_t *od){

    //src: assembly code string e.g mov $rsp, $rbp
    //dst: pointer to the address to store the parsed openrand
    //cr:  active core processor
    //初始化
    od->type = EMPTY;
    od->imm = 0;
    od->reg1 = 0;
    od->reg2 = 0;
    
    int str_len = strlen(str);
    if (str_len == 0){
        // empty openrand string
        return;
    }
    
    if (str[0] == '$'){

        // immediate number
        od->type = IMM;
        // try to parse immediate number
        od->imm = string2uint_range(str, 1, -1);
        return;
    }
    else if (str[0] == '%'){

        //reigsters
        od->type = REG;
        od->reg1 = reflect_register(str);
        return;
    }
    else{

        //memory access
        char imm[64] = {'\0'};
        int imm_len = 0;
        char reg1[64] = {'\0'};
        int reg1_len = 0;
        char reg2[64] = {'\0'};
        int reg2_len = 0;
        char scal[64] = {'\0'};
        int scal_len = 0;

        int ca = 0;// 数（）
        int cb = 0;// 数，

        for (int i = 0; i < str_len; i++){

            char c = str[i];
            if (c == '(' || c == ')'){
                ca++;
                continue;
            }
            else if (c == ','){
                cb++;
                continue;
            }
            else {
                // parse imm(reg1,reg2,scal)
                if (ca == 0){
                    //  xxx             x为正在处理的字符，？为处理过的字符
                    imm[imm_len] = c;
                    imm_len++;
                    continue;
                }
                else if (ca == 1){
                    
                    if (cb == 0){
                        // ???(xxx
                        // (xxx
                        reg1[reg1_len] = c;
                        reg1_len++;
                        continue;
                    }
                    else if (cb == 1){
                        // (???,xxx
                        //  ???(???,xxx
                        // (,xxx
                        // ???(,xxx
                        reg2[reg2_len] = c;
                        reg2_len++;
                        continue;
                    }
                    else if (cb == 2){
                        // (???,???,xxx
                        scal[scal_len] = c;
                        scal_len++;
                        continue;;
                    }


                }
            }


        }



        // imm, reg1, reg2, scal
        if (imm_len > 0){
            od->imm = string2uint(imm);
            if (ca == 0){
                od->type = MEM_IMM;
                return;
            }
        }

        if (scal_len > 0){
            od->scal = string2uint(scal);
            if (od->scal != 1 && od->scal != 2 && od->scal != 4 && od->scal != 8){
                printf("%s is not a legal scaler\n", scal);
                exit(0);
            }
        }

        if (reg1_len > 0){
            od->reg1 = reflect_register(reg1);
        }

        if (reg2_len > 0){
            od->reg2 = reflect_register(reg2);
        }

        // set openrand type
        if (cb == 0){
            if (imm_len > 0){
                od->type = MEM_IMM_REG1;
                return;
            }
            else {
                od->type = MEM_REG1;
                return;
            }
        }
        else if (cb == 1){
            if (imm_len > 0){
                od->type = MEM_IMM_REG1_REG2;
                return;
            }
            else {
                od->type = MEM_REG1_REG2;
                return;
            }
        }
        else if (cb == 2){
            if (reg1_len > 0){
                // reg1 exists
                if (imm_len > 0){
                    od->type = MEM_IMM_REG1_REG2_SCAL;
                    return;
                }
                else {
                    od->type = MEM_REG1_REG2_SCAL;
                    return;
                }
            }
            else {
                // no reg1
                if (imm_len > 0){
                    od->type = MEM_IMM_REG2_SCAL;
                    return;
                }
                else {
                    od->type = MEM_REG2_SCAL;
                    return;
                }
            }   
        }


    }


}


static void mov_handler                 (od_t *src_od, od_t *dst_od);
static void push_handler                (od_t *src_od, od_t *dst_od);
static void pop_handler                 (od_t *src_od, od_t *dst_od);
static void leave_handler               (od_t *src_od, od_t *dst_od);
static void call_handler                (od_t *src_od, od_t *dst_od);
static void ret_handler                 (od_t *src_od, od_t *dst_od);
static void add_handler                 (od_t *src_od, od_t *dst_od);
static void sub_handler                 (od_t *src_od, od_t *dst_od);
static void cmp_handler                 (od_t *src_od, od_t *dst_od);
static void jne_handler                 (od_t *src_od, od_t *dst_od);
static void jmp_handler                 (od_t *src_od, od_t *dst_od);

typedef void (*handler_t)(od_t *, od_t *);

static handler_t handler_table[NUM_INSTRTYPE] = {
    &mov_handler,               //0
    &push_handler,              //1
    &pop_handler,               //2
    &leave_handler,             //3
    &call_handler,              //4
    &ret_handler,               //5
    &add_handler,               //6
    &sub_handler,               //7
    &cmp_handler,               //8
    &jne_handler,               //9
    &jmp_handler,               //10
};


//reset the condition flags
//inline  to reduce cost
// static inline void reset_cflags(core_t *cr){

//     cr->flags.__flag_value = 0;
// }

// update the rip pointer to the next instruction sequentially
static inline void next_rip(){

    // we are handling the fixed-length of assembly string here
    // but their size can be variable as true x86 instructions
    // that's because the openrands' sizes follow the specific encoding rule
    // the risc=v is a fixed length ISA
    
    cpu_pc.rip += sizeof(char) * MAX_INSTRUCTION_CHAR;
}

// instruction handlers

static void mov_handler(od_t *src_od, od_t *dst_od){
    
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);

    if (src_od->type == REG && dst_od->type == REG){
        
        //src: register
        //dst: register
        *(uint64_t *)dst = *(uint64_t *)src;
        next_rip();
        cpu_flags.__flag_value = 0;
        // cr->flags.__flag_value = 0;
        return;
    }
    else if (src_od->type == REG && dst_od->type >= MEM_IMM){
        
        //src: register
        //dst: virtual address
        cpu_write64bits_dram(va2pa(dst), *(uint64_t *)src);
        next_rip();
        cpu_flags.__flag_value = 0;
        return;
    }
    else if (src_od->type >= MEM_IMM && dst_od->type == REG){

        // src: virtual address
        // dst: register
        *(uint64_t *)dst = cpu_read64bits_dram(va2pa(src));
        next_rip();
        cpu_flags.__flag_value = 0;
        return;
    }
    else if (src_od->type == IMM && dst_od->type == REG){

        // src: immediate number (uint64_t bit map)
        // dst: register
        *(uint64_t *)dst = src;
        next_rip();
        cpu_flags.__flag_value = 0;
        return;
    }
}

static void push_handler(od_t *src_od, od_t *dst_od){
    
    uint64_t src = decode_operand(src_od);

    if (src_od->type == REG){
        // src: register
        // dst: empty
        cpu_reg.rsp = cpu_reg.rsp - 8;
        cpu_write64bits_dram(va2pa(cpu_reg.rsp), *(uint64_t *)src);
        next_rip();
        cpu_flags.__flag_value = 0;
        return;
    }
}

static void pop_handler(od_t *src_od, od_t *dst_od){

    uint64_t src = decode_operand(src_od);

    if (src_od->type == REG){
        //src: register
        //dst: empty
        uint64_t old_val = cpu_read64bits_dram(va2pa(cpu_reg.rsp));
        cpu_reg.rsp += 8;
        *(uint64_t *)src = old_val;
        next_rip();
        cpu_flags.__flag_value = 0;
        return;
    }
}

static void leave_handler(od_t *src_od, od_t *dst_od){
    

    // 等价于 movq %rbp, %rsp  和 popq %rbp
    
    //movq %rbp, %rsp
    cpu_reg.rsp = cpu_reg.rbp;
    // (cr->reg).rsp = (cr->reg).rbp;


    // popq %rbp
    //src: register
    //dst: empty
    uint64_t old_val = cpu_read64bits_dram(va2pa(cpu_reg.rsp));
    cpu_reg.rsp += 8;
    cpu_reg.rbp = old_val;
    next_rip();
    cpu_flags.__flag_value = 0;

}

static void call_handler(od_t *src_od, od_t *dst_od){

    uint64_t src = decode_operand(src_od);

    //src: immediate number: virtual address of target
    //dst: empty
    //push the return value
    cpu_reg.rsp -= 8;
    // 将下一条指令写入栈中
    cpu_write64bits_dram(va2pa(cpu_reg.rsp), cpu_pc.rip + sizeof(char) * MAX_INSTRUCTION_CHAR);
    // jump to target functio address
    
    cpu_pc.rip = src;
    cpu_flags.__flag_value = 0;
}

static void ret_handler(od_t *src_od, od_t *dst_od){

    //src:  empty
    //dstL  empty
    //pop rsp
    uint64_t ret_addr = cpu_read64bits_dram(va2pa(cpu_reg.rsp));
    cpu_reg.rsp += 8;
    cpu_pc.rip = ret_addr;
    cpu_flags.__flag_value = 0;
}

static void add_handler(od_t *src_od, od_t *dst_od){
    
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);

    if (src_od->type == REG && dst_od->type == REG){

        // src: register(value: int64_t bit map)
        // dst: register(value: int64_t bit map)

        uint64_t val = *(uint64_t *)dst + *(uint64_t *)src;

        int val_sign = ((val >> 63) & 0x1);
        int src_sign = ((*(uint64_t *)src >> 63) & 0x1);
        int dst_sign = ((*(uint64_t *)dst >> 63) & 0x1);

        //set condition flags
        cpu_flags.CF = (val < *(uint64_t*)src);   //unsigned  a + b < a 无符号溢出，也就是进位
        cpu_flags.ZF = (val == 0);
        cpu_flags.SF = val_sign;
        cpu_flags.OF = (!(src_sign ^ dst_sign) && (val_sign ^ src_sign)); //signed    正数+负数不会发生溢出，只有正数+正数，负数+负数可能溢出



        //update registers
        *(uint64_t *)dst = val;
        // signed and unsigned values follow the same addition
        // e.g.
        // 5 = 0000000000000101, 3 = 0000000000000011, -3 = 1111111111111101,
        // 5 + (-3) = 0000000000000010
        next_rip();
        return;
    }
}

static void sub_handler(od_t *src_od, od_t *dst_od){

    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);

    if (src_od->type == IMM && dst_od->type == REG){

        // src: imm
        // dst: register(value: int64_t bit map)
        // dst = dst - src
        uint64_t val = *(uint64_t *)dst + (~src + 1);

        int val_sign = ((val >> 63) & 0x1);
        int src_sign = ((src >> 63) & 0x1);
        int dst_sign = ((*(uint64_t *)dst >> 63) & 0x1);

        //set condition flags
        cpu_flags.CF = (val > *(uint64_t *)dst);   //unsigned  减掉后的值比原来的值大  无符号溢出，也就是进位
        cpu_flags.ZF = (val == 0);
        cpu_flags.SF = val_sign;
        cpu_flags.OF = (src_sign == 1 && dst_sign == 0 && val_sign == 1) || (src_sign == 0 && dst_sign == 1 && val_sign == 0); //signed    case1： 正数减去一个负数等于一个负数   case2： 一个负数减去一个正数得到一个正数

        //update registers
        *(uint64_t *)dst = val;
        // signed and unsigned values follow the same addition
        // e.g.
        // 5 = 0000000000000101, 3 = 0000000000000011, -3 = 1111111111111101,
        // 5 + (-3) = 0000000000000010
        
        next_rip();
        return;
    }
}

static void cmp_handler(od_t *src_od, od_t *dst_od){
    uint64_t src = decode_operand(src_od);
    uint64_t dst = decode_operand(dst_od);

    if (src_od->type == IMM && dst_od->type >= MEM_IMM){

        // src: imm
        // dst: virtual address
        // dst = dst - src
        uint64_t dst_val = cpu_read64bits_dram(va2pa(dst));
        uint64_t val = dst_val + (~src + 1);

        int val_sign = ((val >> 63) & 0x1);
        int src_sign = ((src >> 63) & 0x1);
        int dst_sign = ((dst_val >> 63) & 0x1);

        //set condition flags
        cpu_flags.CF = (val > dst_val);   //unsigned  减掉后的值比原来的值大  无符号溢出，也就是进位
        cpu_flags.ZF = (val == 0);
        cpu_flags.SF = val_sign;
        cpu_flags.OF = (src_sign == 1 && dst_sign == 0 && val_sign == 1) || (src_sign == 0 && dst_sign == 1 && val_sign == 0); //signed    case1： 正数减去一个负数等于一个负数   case2： 一个负数减去一个正数得到一个正数

        // signed and unsigned values follow the same addition
        // e.g.
        // 5 = 0000000000000101, 3 = 0000000000000011, -3 = 1111111111111101,
        // 5 + (-3) = 0000000000000010
        
        next_rip();
        return;
    }
}

static void jne_handler(od_t *src_od, od_t *dst_od){
    
    uint64_t src = decode_operand(src_od);
    // 跳转到下一条指令的虚拟地址
    if (src_od->type == MEM_IMM){
        if (cpu_flags.ZF != 1){
            // last instruction value != 0
            cpu_pc.rip = src;
        }
        else {
            // last instruction value == 0
            next_rip();
        }
        cpu_flags.__flag_value = 0;
    }
}

static void jmp_handler(od_t *src_od, od_t *dst_od){
    
    uint64_t src = decode_operand(src_od);
    
    //src: immediate number of the target jumping address
    //dst: empty
    cpu_pc.rip = src;
    cpu_flags.__flag_value = 0;
}



void instruction_cycle(){

    // 不能用这种方式来读
    // const char *inst_str = (const char*)cr->rip;

    //正确读的方式
    char inst_str[MAX_INSTRUCTION_CHAR + 10];
    cpu_readinst_dram(va2pa(cpu_pc.rip), inst_str);

    debug_printf(DEBUG_INSTRUCTIONCYCLE, "%lx       %s\n", cpu_pc.rip, inst_str);

    inst_t inst;

    parse_instruction(inst_str, &inst);

    handler_t handler = handler_table[inst.op];

    handler(&(inst.src), &(inst.dst));

}

void print_register(){
    if ((DEBUG_VERBOSE_SET & DEBUG_REGISTERS) == 0X0){
        return;
    }
    // reg_t reg = cr->reg;
    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n", cpu_reg.rax, cpu_reg.rbx, cpu_reg.rcx, cpu_reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n", cpu_reg.rsi, cpu_reg.rdi, cpu_reg.rbp, cpu_reg.rsp);
    printf("rip = %16lx\n", cpu_pc.rip);
    printf("CF = %u\tZF = %u\tSF = %u\tOF = %u\n", cpu_flags.SF, cpu_flags.ZF, cpu_flags.SF, cpu_flags.OF);

}


void print_stack(){
    if ((DEBUG_VERBOSE_SET & DEBUG_PRINTSTACK) == 0X0){
        return;
    }
    
    int n = 10;

    uint64_t *high = (uint64_t*)&pm[va2pa(cpu_reg.rsp)];
    high = &high[n];

    // 打印虚拟地址
    uint64_t rsp_start = cpu_reg.rsp + n * 8;

    for (int i = 0; i < 2 * n; ++i){
        
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%016lx : %16lx", rsp_start, (uint64_t) *ptr);

        if (i == n){
            printf(" <== rsp");
        }

        rsp_start -= 8;

        printf("\n");
    }

}


void TestParse_instruciation(){
    // ACTIVE_CORE = 0X0;
    // core_t *ac = (core_t *)&cores[ACTIVE_CORE];

    char assembly[15][MAX_INSTRUCTION_CHAR] = {
        "push  %rbp",                   //0
        "mov   %rsp, %rbp",             //1
        "mov   %rdi, -0x18(%rbp)",      //2
        "mov   %rsi, -0x20(%rbp)",      //3
        "mov   -0x18(%rbp), %rdx",      //4
        "mov   -0x20(%rbp), %rax",      //5
        "add   %rdx, %rax",             //6
        "mov   %rax, -0x8(%rbp)",       //7
        "mov   -0x8(%rbp), %rax",       //8
        "pop   %rbp",                   //9
        "retq",                         //10
        "mov   %rdx, %rsi",             //11
        "mov   %rax, %rdi",             //12
        "callq 0",                      //13
        "mov %rax, -0x8(%rbp)",         //14
    };

    inst_t inst;

    for (int i = 0; i < 15; i++){
        parse_instruction(assembly[i], &inst);
    }

}


void TestParse_operand(){
    
    // ACTIVE_CORE = 0X0;
    // core_t *ac = (core_t *)&cores[ACTIVE_CORE];


    const char *strs[11] = {
        "$0x1234",
        "%rax",
        "0xabcd",
        "(%rsp)",
        "0xabcd(%rsp)",
        "(%rsp,%rbx)",
        "0xabcd(%rsp,%rbx)",
        "(,%rbx,8)",
        "0xabcd(,%rbx,8)",
        "(%rsp,%rbx,8)",
        "0xabcd(%rsp,%rbx,8)",
    }; 

    printf("rax %p\n", &(cpu_reg.rax));
    printf("rsp %p\n", &(cpu_reg.rsp));
    printf("rbx %p\n", &(cpu_reg.rbx));


    for (int i = 0; i < 11; i++){
        od_t od;
        parse_operand(strs[i], &od);

        printf("\n %s\n", strs[i]);
        printf("od enum type: %d\n", od.type);
        printf("od imm: %lx\n", od.imm);
        printf("od reg1: %lx\n", od.reg1);
        printf("od reg2: %lx\n", od.reg2);
        printf("od sacl: %lx\n", od.scal);

        printf("\n");
    }
}



