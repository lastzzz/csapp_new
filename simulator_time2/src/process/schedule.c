#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "header/cpu.h"
#include "header/memory.h"
#include "header/interrupt.h"
#include "header/process.h"

pcb_t *get_current_pcb()
{
    kstack_t *ks = (kstack_t *)get_kstack_RSP();
    pcb_t *current_pcb = ks->threadinfo.pcb;
    return current_pcb;
}

static void store_context(pcb_t *proc)
{
    context_t ctx = {
        .regs = cpu_reg,
        .flags = cpu_flags
    };
    // Be especially careful that RSP is stored as context
    // And RIP is not restored as context!!!
    memcpy(&(proc->context), &ctx, sizeof(context_t));
}

static void restore_context(pcb_t *proc)
{
    // restore cpu registers from user frame
    memcpy(&cpu_reg, &(proc->context.regs), sizeof(cpu_reg_t));
    memcpy(&cpu_flags, &(proc->context.flags), sizeof(cpu_flags_t));
}

void os_schedule()
{
    // The magic is: RIP is not updated at all
    // only kstack & page table will do the switch

    pcb_t *pcb_old = get_current_pcb();

    // pcb_new should be selected by the scheduling algorithm
    pcb_t *pcb_new = pcb_old->next;
    printf("    \033[31;1mOS schedule [%ld] -> [%ld]\033[0m\n", pcb_old->pid, pcb_new->pid);

    // context switch

    // store the context of the old process
    store_context(pcb_old);

    // restore the context of the new process
    restore_context(pcb_new);

    // update TR -> TSS
    tr_global_tss.ESP0 = get_kstack_RSP() + KERNEL_STACK_SIZE;

    // update CR3 -> page table in MMU
    // will cause the refreshing of MMU TLB cache
    cpu_controls.cr3 = (uint64_t)(pcb_new->mm.pgd);
}