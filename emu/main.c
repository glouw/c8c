#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define VROWS     32
#define BYTES   4096
#define START 0x0200
#define VSIZE     16
#define SSIZE     16
#define BFONT     80 // Bytes of Font

typedef uint64_t u64;

u64 cyc;         // Cycles since boot
u64 vmem[VROWS]; // Video memory

typedef uint16_t u16;

u16 pc = START; // Program counter
u16 I;          // General purpose pointer
u16 s[SSIZE];   // Stack
u16 op;         // Opcode

typedef uint8_t u8;

u8 dt;         // Delay timer
u8 st;         // Sound timer
u8 sp;         // Stack pointer
u8 v[VSIZE];   // General purpose registers
u8 mem[BYTES]; // General purpose memory

/* Opcodes */
void _0000(void) { }
void _00E0(void) { /*  CLS */ puts("CLS");  for(u16 i = 0; i < VROWS; i++) while(vmem[i] >>= 1); }
void _00EE(void) { /*  RET */ puts("RET");  pc = s[--sp]; }
void _00EF(void) { /*  END */ puts("END");  exit(0); }
void _1NNN(void) { /*   JP */ puts("JP");   u16 nnn = op & 0x0FFF; pc = nnn; }
void _2NNN(void) { /* CALL */ puts("CALL"); u16 nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
void _3XNN(void) { /*   SE */ puts("SE");   u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
void _4XNN(void) { /*  SNE */ puts("SNE");  u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
void _5XY0(void) { /*   SE */ puts("SE");   u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
void _6XNN(void) { /*   LD */ puts("LD");   u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] = nn; }
void _7XNN(void) { /*  ADD */ puts("ADD");  u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
void _8XY0(void) { /*   LD */ puts("LD");   u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] = v[y]; }
void _8XY1(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]|=v[y]; } //  or and assign registers */
void _8XY2(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]&=v[y]; } // and and assign registers */
void _8XY3(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]^=v[y]; } // xor and assign registers */
void _8XY4(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]+v[y]>0xFF?0x01:0x00; v[x]+=v[y]; } */
void _8XY5(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]-v[y]<0x00?0x01:0x00; v[x]-=v[y]; } */
void _8XY6(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x01; v[x]=v[y]>>1; } // shift right */
void _8XY7(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]-v[x]<0x00?0x00:0x01; v[x]=v[y]-v[x]; } */
void _8XYE(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x80; v[x]=v[y]<<1; } // shift left */
void _9XY0(void) { /*  SNE */ puts("SNE");  u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
void _ANNN(void) { /*   LD */ puts("LD");   u16 nnn = op & 0x0FFF; I = nnn; }
void _BNNN(void) { } // u16 nnn=op&0x0FFF; pc=nnn+v[0x0]; } // assign the additive value of nnn and v[0x0] to pc */
void _CXNN(void) { } // u16 x=(op&0x0F00)>>8, nn=op&0x00FF; v[x]=nn&(rand()%256); } // randomize nn, assign v[x] */
void _DXYN(void) {
//    u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4, n=(op&0x000F);
//    for(u16 i=0; i<n; i++) // draw a fprite to the screen
//    for(u16 j=0; j<8; j++) { // starting at row=v[y] and col=v[x]
//        u8 bit = (mem[I+i]>>(7-j))&0x01; // pull one bit from u8 byte
//        v[0xF] = (vmem[v[y]][v[x]]&&bit)?0x01:0x00; // v[0xF] xor flag check
//        vmem[v[y]][v[x]] ^= bit; // xor bits to vmem
//    }
}
void _EXA1(void) { }
void _EX9E(void) { }
void _FX07(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; v[x] = dt; }
void _FX0A(void) { }
void _FX15(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; dt = v[x]; }
void _FX18(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; st = v[x]; }
void _FX1E(void) { } //u16 x=(op&0x0F00)>>8; I+=v[x]; } // add to register to general purpose pointer
void _FX29(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; I = 5 * v[x]; } 
void _FX33(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8;
    #define len(array) (signed)(sizeof(array) / sizeof(*array))
    int i, lookup[] = { 100, 10, 1 };
    for(i = 0; i < len(lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
void _FX55(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
void _FX65(void) { /*   LD */ puts("LD"); u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

/* Opcode map */
void _0___(void);
void _8___(void);
void _E___(void);
void _F___(void);
void (*opsa[])(void) = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _00EF };
void (*opsb[])(void) = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
void (*opsc[])(void) = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
void (*opsd[])(void) = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
/*                    */ _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
/*                    */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
/*         ...zZzZz   */ _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*    ()()            */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*    (-.-)           */ _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*    () ()           */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                    */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
void (*list[])(void) = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
void exec(void (*opcode)(void)) { opcode(); }
void _0___(void) { exec(opsa[op & 0x000F]); }
void _8___(void) { exec(opsb[op & 0x000F]); }
void _E___(void) { exec(opsc[op & 0x000F]); }
void _F___(void) { exec(opsd[op & 0x00FF]); }

void load(char* str)
{
    u8 ch[BFONT] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0x0000: 0 */ 
        0x20, 0x60, 0x20, 0x20, 0x70, /* 0x0005: 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 0x000A: 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 0x000F: 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 0x0014: 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 0x0019: 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 0x001E: 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 0x0023: 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 0x0028: 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 0x002D: 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* 0x0032: A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* 0x0037: B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* 0x003C: C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* 0x0041: D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* 0x0046: E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* 0x004B: F */
    };
    FILE* game = fopen(str, "rb");
    fseek(game, 0, SEEK_END);
    long sz = ftell(game);
    rewind(game);
    u8 buf[BYTES];
    fread(buf, 1, sz, game);
    fclose(game);
    for(u16 i = 0; i < BFONT; i++)
        mem[i] = ch[i];
    for(u16 i = 0; i < sz; i++)
        mem[i + START] = buf[i];
}

void pollute(void), print(u64 line), debug(void);

void cycle(void)
{
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    if(dt > 0) dt--;
    if(st > 0) st--;
    exec(list[op >> 12]);
    debug();
}

int main(int argc, char* argv[])
{
    srand(time(0));
    pollute();
    if(argc != 2)
        return 1;
    load(argv[1]);
    for(;;)
        cycle();
}

void pollute(void)
{
    for(int i = 0; i < VROWS; i++)
        vmem[i] = (u64)1 << (i + (i % 2 ? 0 : 32));
}

void print(u64 line)
{
    int bits = 8 * sizeof(u64);
    char* str = malloc(bits + 1);
    for(int i = bits - 1; i >= 0; i--)
    {
        str[i] = line & 0x1 ? '1' : '0';
        line >>= 1;
    }
    str[bits] = '\0';
    puts(str);
}

void debug(void)
{
    /* Misc */
    printf("pc:0x%04X ", pc);
    printf("exec:0x%04X ", op);
    printf("I:0x%04X ", I);
    printf("dt:0x%02X ", dt);
    printf("st:0x%02X ", st);
    printf("sp:0x%02X ", sp);
    printf("cyc:%llu\n", ++cyc);
    /* Video memory */
    for(int i=0; i < VSIZE; i++)
        printf("  %02X\t", v[i]);
    putchar('\n');
    /* Stack */
    for(int i=0; i < SSIZE; i++)
        printf("%04X\t", s[i]);
    putchar('\n');
    /* General purpose registers */
    for(int i=0; i < VROWS; i++)
        print(vmem[i]);
    printf("Push enter to continue onto the next cycle...");
    while(getchar() != '\n');
}
