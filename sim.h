/* 
 * operation codes for pseudo machine
 */
#define OP_ALU      1   /* alu arithmetic-logic-op */
#define OP_DEC      2   /* dec  region, offset */
#define OP_INC      3   /* inc  region, offset */
#define OP_LOAD     4   /* load  region, offset */
#define OP_STORE    5   /* store  region, offset */
#define OP_POP      6   /* pop  region, offset */
#define OP_JUMPZ    7   /* jumpz label */
#define OP_JUMP     8   /* jump  label */
#define OP_CALL     9   /* call routine-address */
#define OP_ENTRY    10   /* entry local-frame-size */
#define OP_RETURN   11   /* return */

/* 
 * region modifiershs
 */
#define MOD_GLOBAL  1   /* global region */
#define MOD_PARAM   2   /* parameter region */
#define MOD_LOCAL   3   /* local region */
#define MOD_IMMED   4   /* load only: Constant */

/* 
 * OP_ALU modifiers
 */
#define ALU_ADD  1   /* addition */
#define ALU_SUB  2   /* subtraction */
#define ALU_MUL  3   /* multiplication */
#define ALU_DIV  4   /* division */
#define ALU_MOD  5   /* remainder */
#define ALU_LT   6   /* compare as: < */
#define ALU_GT   7   /*             > */
#define ALU_LE   8   /*             <= */
#define ALU_GE   9   /*             >= */
#define ALU_EQ   10   /*            == */
#define ALU_NE   11   /*            != */
#define ALU_AND  12   /* bit-wise and */
#define ALU_OR   13   /* bit-wise or */
#define ALU_XOR  14   /* bit-wise xor */

struct prog {
    short p_op;     /* operation code */
    short p_mod;   /* modifier */
    int p_val;      /* offset or other value */
};

/* 
 * OP_ALU modifiers
 */
#define L_PROG  200 /* max. program size */
#define L_DATA  100 /* max. area for stack, etc. */
#define DIM(x)  (sizeof x / sizeof x[0])    /* extent */