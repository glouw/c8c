#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define VROWS     32
#define BYTES   4096
#define START 0x0200
#define VSIZE     16
#define FSIZE     16

typedef uint64_t u64;

u64 cyc;         // Cycles since boot
u64 vmem[VROWS]; // Video memory

typedef uint16_t u16;

u16 pc = START; // Program counter
u16 I;          // General purpose pointer
u16 f[FSIZE];   // Frames
u16 op;         // Opcode

typedef uint8_t u8;

u8 dt;         // Delay timer
u8 st;         // Sound timer
u8 fp;         // Frame pointer
u8 v[VSIZE];   // General purpose registers
u8 mem[BYTES]; // General purpose memory

// Opcodes
void _0000(void) { }
void _00E0(void) { /*  CLS */ puts("exexuting CLS");  for(u16 i=0; i<VROWS; i++) while(vmem[i] >>= 1); }
void _00EE(void) { /*  RET */ puts("executing RET");  pc=f[--fp]; }
void _00EF(void) { /*  END */ puts("executing END");  exit(0); }
void _1NNN(void) { /*   JP */ puts("executing JP");   u16 nnn=op&0x0FFF; pc=nnn; }
void _2NNN(void) { /* CALL */ puts("executing CALL"); u16 nnn=op&0x0FFF; f[fp++]=pc; pc=nnn; }
void _3XNN(void) { /*   SE */ puts("executing SE");   u16 x=(op&0x0F00)>>8, nn=op&0x00FF; if(v[x]==nn) pc+=0x0002; }
void _4XNN(void) { /*  SNE */ puts("executing SNE");  u16 x=(op&0x0F00)>>8, nn=op&0x00FF; if(v[x]!=nn) pc+=0x0002; }
void _5XY0(void) { /*   SE */ puts("executing SE");   u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]==v[y]) pc+=0x0002; }
void _6XNN(void) { /*   LD */ puts("executing LD");   u16 nn=op&0x00FF, x=(op&0x0F00)>>8; v[x]=nn; }
void _7XNN(void) { } // u16 nn=op&0x00FF, x=(op&0x0F00)>>8; v[x]+=nn; } // add nn to a register */
void _8XY0(void) { } // u16 x=(op&0x0F00)>>8, y=op&0x00F0; v[x]=v[y]; } // assign a register to that of another */
void _8XY1(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]|=v[y]; } //  or and assign registers */
void _8XY2(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]&=v[y]; } // and and assign registers */
void _8XY3(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]^=v[y]; } // xor and assign registers */
void _8XY4(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]+v[y]>0xFF?0x01:0x00; v[x]+=v[y]; } */
void _8XY5(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[x]-v[y]<0x00?0x01:0x00; v[x]-=v[y]; } */
void _8XY6(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x01; v[x]=v[y]>>1; } // shift right */
void _8XY7(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]-v[x]<0x00?0x00:0x01; v[x]=v[y]-v[x]; } */
void _8XYE(void) { } // u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF]=v[y]&0x80; v[x]=v[y]<<1; } // shift left */
void _9XY0(void) { /*  SNE */ puts("executing SNE");  u16 x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]!=v[y]) pc+=0x0002; }
void _ANNN(void) { } // u16 nnn=op&0x0FFF; I=nnn; } // assign address nnn to general purpose pointer I */
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
void _FX07(void) { } //u16 x=(op&0x0F00)>>8; v[x]=dt; } // store delay timer
void _FX0A(void) { }
void _FX15(void) { } //u16 x=(op&0x0F00)>>8; dt=v[x]; } // assign delay timer
void _FX18(void) { } //u16 x=(op&0x0F00)>>8; st=v[x]; } // assign sound timer
void _FX1E(void) { } //u16 x=(op&0x0F00)>>8; I+=v[x]; } // add to register to general purpose pointer
void _FX29(void) { } //u16 x=(op&0x0F00)>>8; I=5*v[x]; } // point to character for difplay difplay
void _FX33(void) { } //u16 x=(op&0x0F00)>>8; mem[I]=(v[x]/100)%10, mem[I+1]=(v[x]/10)%10, mem[I+2]=v[x]%10; } // BCD
void _FX55(void) { } //u16 x=(op&0x0F00)>>8, i; for(i=0; i<x; i++) mem[I+i]=v[i]; I+=i; } // save all general purpose registers
void _FX65(void) { } //u16 x=(op&0x0F00)>>8, i; for(i=0; i<x; i++) v[i]=mem[I+i]; I+=i; } // get all general purpose registers

// Opcode map
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
void _0___(void) { exec(opsa[op&0x000F]); }
void _8___(void) { exec(opsb[op&0x000F]); }
void _E___(void) { exec(opsc[op&0x000F]); }
void _F___(void) { exec(opsd[op&0x00FF]); }

void load(char* str)
{
    u8 ch[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    FILE* game = fopen(str, "rb");
    fseek(game, 0, SEEK_END);
    long sz = ftell(game);
    rewind(game);
    u8 buf[BYTES];
    fread(buf, 1, sz, game);
    fclose(game);
    for(u16 i=0; i<80; i++) mem[i] = ch[i];
    for(u16 i=0; i<sz; i++) mem[i+START] = buf[i];
    srand(time(0));
}

void debug(void); // TODO: Remove

void cycle(void)
{
    op = (mem[pc]<<8)+(mem[pc+1]&0x00FF); // Get opcode
    pc += 0x0002;  // Point pc to next opcode for the next cycle
    if(dt>0) dt--; // Decrement delay timer
    if(st>0) st--; // Decrement sound timer
    exec(list[op>>12]); // Execute opcode
    debug();
}

void pollute(void); // TODO: Remove

int main(int argc, char* argv[])
{
    pollute();
    if(argc != 2)
        return 1;
    load(argv[1]);
    for(;;)
        cycle();
}

void pollute(void) // TODO: Remove
{
    for(int i = 0; i < VROWS; i++)
        vmem[i] = (u64)1 << (i + (i % 2 ? 0 : 32));
}

void print(u64 line) // TODO: Remove
{
    char* str = malloc(65);
    for(int i=63; i>=0; i--)
    {
        str[i] = line & 0x1 ? '1' : '0';
        line >>= 1;
    }
    str[64] = '\0';
    puts(str);
}

void debug(void) // TODO: Remove
{
    // Most things
    printf("pc:%04X\texecuted:%04X\tI:%04X\tdt:%02X\tst:%02X\tfp:%02X\tcycles done:%llu\n", pc, op, I, dt, st, fp, ++cyc);
    // General Purpose register dump
    for(int i=0; i<VSIZE; i++) printf("%04X\t", v[i]);
    putchar('\n');
    // Frame dump
    for(int i=0; i<VSIZE; i++) printf("%04X\t", f[i]);
    putchar('\n');
    // Video memory dump
    for(int i=0; i<VROWS; i++) print(vmem[i]);
    printf("Push enter to continue onto the next cycle...");
    while(getchar() != '\n');
}
