#include "sim.h"

extern void bug();
extern void fatal();

/* 
 * data and program memory
 */
static int data[L_DATA];
extern struct prog prog[];

/* 
 * registers
 */
static struct prog *inst;   /*  -> current instruction */
#define G   0   /* global segment */
static int P;   /*current parameter segment */
static int L;   /* current local segment */
static int T;   /* top of stack */

/* 
 * shorthand notations
 */
#define TOP data[T-1]       /* right ofoperand: top of stack */
#define NEXT data[T-2]      /* left operand: below TOP */
#define PUSH data[T++]      /* new cell to come onto stack */
#define POP     --T         /* -> discarded cell from stack */
#define MEMORY  data[address()] /* effectively addressed cell */
#define RESULT  data[G]     /* result value of function */

static int address()    /* effective current data address */
{
    register int ad;
    switch (inst->p_mod) {
        case MOD_GLOBAL:
            ad = G;
            break;
        case MOD_PARAM:
            ad = P;
            break;
        case MOD_LOCAL:
            ad = L;
            break;
        default:
            bug("invalud p_mod");
    }
    ad += inst->p_val;
    if (ad < 0 || ad >= T)
    bug("invalud effective address");
    return ad;
}

simulate(pc_limit, global, pc)
{
    /* initialize */
    printf("\nExecution begins...\n\n");

    for (;;)
    {
        /* fetch */
        if (pc < 0 || pc >= pc_limit)
            bug("pc not in program area");
        inst = &prog[pc++];

        /* decode operation and dispatch */
        switch(inst->p_op)
        {
        default:
            print("%d:\thalt\n", inst-prog);
            return;

        /* other instructions */
        case OP_ALU:
            if (T <= L+1)
                bug("simultor stack underflow");
            switch(inst->p_mod){
                default:
                    bug("illegal ALU instruction");
                case ALU_ADD:
                    NEXT += TOP;
                    break;
                case ALU_SUB:                       
                    NEXT -= TOP;
                    break;
                case ALU_MUL:   
                    NEXT *= TOP;
                    break;
                case ALU_DIV:   
                    NEXT /= TOP;
                    break;
                case ALU_MOD:  
                    NEXT %= TOP;
                    break;
                case ALU_LT:
                    NEXT = NEXT < TOP;
                    break;
                case ALU_GT:   
                    NEXT = NEXT > TOP;
                    break;
                case ALU_LE:   
                    NEXT = NEXT <= TOP;
                    break;
                case ALU_GE:   
                    NEXT = NEXT >= TOP;
                    break;
                case ALU_EQ:  
                    NEXT = NEXT == TOP;
                    break;
                case ALU_NE:  
                    NEXT = NEXT != TOP;
                    break;
                case ALU_AND: 
                    NEXT &= TOP;
                    break;
                case ALU_OR:   
                    NEXT |= TOP;
                    break;
                case ALU_XOR: 
                    NEXT ^= TOP;
                    break;
            }
            POP;
            break;
        case OP_LOAD:
            if (T >= DIM(data))
                fatal ("Too much data.");
            if (inst->p_mod == MOD_IMMED)
                PUSH = inst->p_val;
            else
                PUSH = MEMORY;
            break;
        case OP_STORE:
            if (T <= L)
                bug("simulator stack underflow");
                printf("%d:\tstore\t%d, %d\tto %d\n", inst-prog, inst->p_mod,
                inst->p_val, MEMORY = TOP);
            break;
        case OP_INC:
            if (T >= DIM(data))
                fatal("Too much data.");
            printf("%d:\tinc\t%d, %d\tto %d\n", inst-prog, inst->p_mod,
            inst->p_val, PUSH = ++MEMORY);
            break;
        case OP_DEC: 
            if (T >= DIM(data))
                fatal("Too much data.");
            printf("%d:\tdec\t%d, %d\tto %d\n", inst-prog, inst->p_mod,
            inst->p_val, PUSH = --MEMORY);
            break;
        case OP_POP:
            if (T <= L)
                bug("simulator stack underflow");
            POP;
            break;
        case OP_JUMP:
            printf("%d:\tjump\t%d, %d\tto %d\n", inst-prog, inst->p_val);
            pc = inst->p_val;
            break;
        case OP_JUMPZ:
            if (T <= L)
                bug("simulator stack underflow");
            if (data[POP] == 0)
            {
                printf("%d:\tjump\t%d, %d\tto %d\n", inst-prog, inst->p_val);
                pc = inst->p_val;
            }
            break;
        case OP_CALL:
            printf("%d:\tcall\t%d\n", inst-prog, inst->p_val);
            PUSH = pc;
            pc = inst->p_val;
            PUSH = P;
            P = T - 2 - inst->p_mod;
            break;
        case OP_ENTRY:
            PUSH = L;
            L = T;
            T += inst->p_val;
            if (T >= DIM(data))
                fatal("Too much data.");
            break;
        case OP_RETURN:
            if (T < L)
                bug ("simulator stack underflow");
            T = L;
            L = data[POP];
            P = data[POP];
            pc = data[POP];
            printf("%d:\treturn\t%d to %d\n", inst-prog, RESULT, pc);
            break;
        }
    }
}
