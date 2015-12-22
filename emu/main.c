#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define VROWS     32 // number of rows in video memory
#define VCOLS     64 // number of columns in video memory
#define BYTES   4096 // bytes of memory
#define START 0x0200 // program counter (pc) starts here
#define VSIZE     16 // number of general purpose register
#define SSIZE     16 // number of nests in the subroutine stack
typedef uint64_t u64;
typedef uint16_t u16;
typedef uint8_t  u8;
/* MEMORY LIST */
static u16 pc = START; // program counter, starts at address START
static u16 I;          // general purpose pointer
static u16 s[SSIZE];   // subroutine address stack
static u16 op; // opcode
static u8  dt; // delay timer
static u8  st; // sound timer
static u8  sp; // stack pointer
static u8  mem[BYTES];// memory
static u8  v[VSIZE];  // general purpose registers
static u8  vmem[VROWS][VCOLS]; // video memory
/* OPCODE LIST */
static void _0000(void) { puts("executed _0000"); } // no operation ... a unique operation I added for the sake of opcode map initialization
static void _00E0(void) { puts("executed _00E0"); } // for(u16 i=0; i<VROWS; i++) for(u16 j=0; j<VCOLS; j++) vmem[i][j]=0x00; } // clears screen   */
static void _00EE(void) { puts("executed _00EE"); } // pc=s[--sp]; } // returns from subroutine by decrementing stack pointer and assigning pc */
static void _1NNN(void) { puts("executed _1NNN"); } // u16 nnn=op&0x0FFF; pc=nnn; } // jumps to memory location nnn; equivalent to C's goto */
static void _2NNN(void) { puts("executed _2NNN"); } // u16 nnn=op&0x0FFF; s[sp++]=pc; pc=nnn; } // saves pc and executes subroutine at nnn */
static void _3XNN(void) { puts("executed _3XNN"); } // u16 x=(op&0x0F00)>>8, nn=op&0x00FF; if(v[x]==nn) pc+=0x0002; } // compare and skip? */
static void _4XNN(void) { puts("executed _4XNN"); } // u16 x=(op&0x0F00)>>8, nn=op&0x00FF; if(v[x]!=nn) pc+=0x0002; } // compare and stay? */
static void _5XY0(void) { puts("executed _5XY0"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]==v[y]) pc+=0x0002; } // compare and skip? */
static void _6XNN(void) { puts("executed _6XNN"); } // u16 nn=op&0x00FF, x=(op&0x0F00)>>8; v[x]=nn; } // assign nn to a register */
static void _7XNN(void) { puts("executed _7XNN"); } // u16 nn=op&0x00FF, x=(op&0x0F00)>>8; v[x]+=nn; } // add nn to a register */
static void _8XY0(void) { puts("executed _8XY0"); } // u16 x=(op&0x0F00)>>8, y=op&0x00F0; v[x]=v[y]; } // assign a register to that of another */
static void _8XY1(void) { puts("executed _8XY1"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]|=v[y]; } //  or and assign registers */
static void _8XY2(void) { puts("executed _8XY2"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]&=v[y]; } // and and assign registers */
static void _8XY3(void) { puts("executed _8XY3"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]^=v[y]; } // xor and assign registers */
static void _8XY4(void) { puts("executed _8XY4"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]+v[y]>0xFF?0x01:0x00; v[x]+=v[y]; } */
static void _8XY5(void) { puts("executed _8XY5"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]-v[y]<0x00?0x01:0x00; v[x]-=v[y]; } */
static void _8XY6(void) { puts("executed _8XY6"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x01; v[x]=v[y]>>1; } // shift right */
static void _8XY7(void) { puts("executed _8XY7"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]-v[x]<0x00?0x00:0x01; v[x]=v[y]-v[x]; } */
static void _8XYE(void) { puts("executed _8XYE"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x80; v[x]=v[y]<<1; } // shift left */
static void _9XY0(void) { puts("executed _9XY0"); } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]!=v[y]) pc+=0x0002; } // compare and skip? */
static void _ANNN(void) { puts("executed _ANNN"); } // u16 nnn=op&0x0FFF; I=nnn; } // assign address nnn to general purpose pointer I */
static void _BNNN(void) { puts("executed _BNNN"); } // u16 nnn=op&0x0FFF; pc=nnn+v[0x0]; } // assign the additive value of nnn and v[0x0] to pc */
static void _CXNN(void) { puts("executed _CXNN"); } // u16 x=(op&0x0F00)>>8, nn=op&0x00FF; v[x]=nn&(rand()%256); } // randomize nn, assign v[x] */
static void _DXYN(void) { puts("executed _DXYN"); }
//    u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4, n=(op&0x000F);
//    for(u16 i=0; i<n; i++) // draw a sprite to the screen
//    for(u16 j=0; j<8; j++) { // starting at row=v[y] and col=v[x]
//        u8 bit = (mem[I+i]>>(7-j))&0x01; // pull one bit from u8 byte
//        v[0xF] = (vmem[v[y]][v[x]]&&bit)?0x01:0x00; // v[0xF] xor flag check
//        vmem[v[y]][v[x]] ^= bit; // xor bits to vmem
//    }
//}
static void _EXA1(void) { puts("executed _EXA1"); }
static void _EX9E(void) { puts("executed _EX9E"); }
static void _FX07(void) { puts("executed _FX07"); } //u16 x=(op&0x0F00)>>8; v[x]=dt; } // store delay timer
static void _FX0A(void) { puts("executed _FX0A"); } //}
static void _FX15(void) { puts("executed _FX15"); } //u16 x=(op&0x0F00)>>8; dt=v[x]; } // assign delay timer
static void _FX18(void) { puts("executed _FX18"); } //u16 x=(op&0x0F00)>>8; st=v[x]; } // assign sound timer
static void _FX1E(void) { puts("executed _FX1E"); } //u16 x=(op&0x0F00)>>8; I+=v[x]; } // add to register to general purpose pointer
static void _FX29(void) { puts("executed _FX29"); } //u16 x=(op&0x0F00)>>8; I=5*v[x]; } // point to character for display display
static void _FX33(void) { puts("executed _FX33"); } //u16 x=(op&0x0F00)>>8; mem[I]=(v[x]/100)%10, mem[I+1]=(v[x]/10)%10, mem[I+2]=v[x]%10; } // BCD
static void _FX55(void) { puts("executed _FX55"); } //u16 x=(op&0x0F00)>>8, i; for(i=0; i<x; i++) mem[I+i]=v[i]; I+=i; } // save all general purpose registers
static void _FX65(void) { puts("executed _FX65"); } //u16 x=(op&0x0F00)>>8, i; for(i=0; i<x; i++) v[i]=mem[I+i]; I+=i; } // get all general purpose registers
/* MAPPING LIST */
static void _0___(void);
static void _8___(void);
static void _E___(void);
static void _F___(void);
static void (*opsa[ 16])(void) = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _0000 };
static void (*opsb[ 16])(void) = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
static void (*opsc[ 16])(void) = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
static void (*opsd[128])(void) = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
/* This opcode table looks-     */ _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
/* up which instruction to      */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
/* execute based on the         */ _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* chip8's method of opcode     */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* branching - all functions    */ _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* must be void(*a)(void)       */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                              */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
static void (*list[ 16])(void) = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
static void exec(void (*opcode)(void)) { opcode(); }
static void _0___(void) { exec(opsa[op&0x000F]); }
static void _8___(void) { exec(opsb[op&0x000F]); }
static void _E___(void) { exec(opsc[op&0x000F]); }
static void _F___(void) { exec(opsd[op&0x00FF]); }

static long load(const char* str)
{
    u8 ch[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
    };
    FILE* fp;
    if((fp = fopen(str, "rb")) == NULL) return -1;
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);
    u8 buf[BYTES];
    fread(buf, 1, sz, fp);
    fclose(fp);
    for(u16 i=0; i<80; i++) mem[i] = ch[i];
    for(u16 i=0; i<sz; i++) mem[i+START] = buf[i];
    return sz;
}

static void debug(void)
{
    static u64 cyc; // cycles since startup
    puts("resulting memory state:");
    printf("%04X, %04X, %04X, %02X, %02X, %02X, %llu\n", op, pc, I, dt, st, sp, cyc++);
    for(int i=0; i<VSIZE; i++) printf("%04X\t%04X\n", v[i], s[i]); // VSIZE is SSIZE so we can do this
    printf("Push any key to continue...\n");
    getchar();
};

static void cycle(void)
{
    op = (mem[pc]<<8)+(mem[pc+1]&0x00FF); // get opcode
    pc += 0x0002;       // point pc to next opcode for the next cycle
    if(dt>0) dt--;      // decrement delay timer if greater than 0
    if(st>0) st--;      // decrement sound timer if greater than 0
    exec(list[op>>12]); // execute one opcode
#define DEBUG
#   ifdef DEBUG
    debug();
#   endif
#undef DEBUG
}

int main(const int argc, const char* argv[])
{
    if(argc!=2) return 1; // no game specified or more than one game specified
    srand(time(NULL));    // seed rand() for opcode _CXNN
    if(load(argv[1]) == -1) return 2; // game does not exist
    for(;;) cycle();      // cycle forever, break with <ctrl+c>
}
