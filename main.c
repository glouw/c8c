#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>

#define VROWS 32 // video memory rows
#define VCOLS 64 // video memory columns

static uint16_t pc = 0x0200;      // program counter
static uint16_t op;               // opcode
static uint16_t I;                // general purpose pointer
static uint16_t s[16];            // subroutine address stack
static uint8_t dt;                // delay timer
static uint8_t st;                // sound timer
static uint8_t sp;                // stack pointer
static uint8_t rom[4096];         // read only memory
static uint8_t v[16];             // general purpose registers
static uint8_t vram[VROWS][VCOLS];  // video ram
static unsigned long long cycles; // cycles done since start

static inline void _00EE(void) { pc=s[--sp]; }
static inline void _1NNN(void) { uint16_t nnn=op&0x0FFF; pc=nnn; }
static inline void _2NNN(void) { uint16_t nnn=op&0x0FFF; pc=nnn; s[sp++]=pc; }
static inline void _ANNN(void) { uint16_t nnn=op&0x0FFF; I=nnn; }
static inline void _BNNN(void) { uint16_t nnn=op&0x0FFF; pc=nnn+v[0x0]; }
static inline void _6XNN(void) { uint16_t nn=op&0x00FF; uint8_t x=(op&0x0F00)>>8; v[x]=nn;  }
static inline void _7XNN(void) { uint16_t nn=op&0x00FF; uint8_t x=(op&0x0F00)>>8; v[x]+=nn; }
static inline void _3XNN(void) { uint8_t x=(op&0x0F00)>>8; uint16_t nn=op&0x00FF; if(v[x]==nn) pc+=0x0002; }
static inline void _4XNN(void) { uint8_t x=(op&0x0F00)>>8; uint16_t nn=op&0x00FF; if(v[x]!=nn) pc+=0x0002; }
static inline void _CXNN(void) { uint8_t x=(op&0x0F00)>>8; uint16_t nn=op&0x00FF; v[x]=nn&(rand()%256); }
static inline void _5XY0(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]==v[y]) pc+=0x0002; }
static inline void _9XY0(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; if(v[x]!=v[y]) pc+=0x0002; }
static inline void _8XY0(void) { uint8_t x=(op&0x0F00)>>8, y=op&0x00F0; v[x]=v[y]; }
static inline void _8XY1(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]|=v[y]; }
static inline void _8XY2(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]&=v[y]; }
static inline void _8XY3(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[x]^=v[y]; }
static inline void _8XY4(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF] = v[x]+v[y]>0xFF ? 0x01:0x00; v[x]+=v[y]; } /* overflow check */
static inline void _8XY5(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF] = v[x]-v[y]<0x00 ? 0x01:0x00; v[x]-=v[y]; } /* undrflow check */
static inline void _8XY6(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF] = v[y]&0x01; v[x]=v[y]>>1; }
static inline void _8XYE(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF] = v[y]&0x80; v[x]=v[y]<<1; }
static inline void _8XY7(void) { uint8_t x=(op&0x0F00)>>8, y=(op&0x00F0)>>4; v[0xF] = v[y]-v[x]<0x00 ? 0x00:0x01; v[x]=v[y]-v[x]; } /* undrflow check */
static inline void _EX9E(void) { uint8_t x=(op&0x0F00)>>8; int temp; timeout(0); temp=getch(); if(temp!=ERR) if(v[x]==(uint8_t)temp) pc+=0x0002; } /* non blocking */
static inline void _EXA1(void) { uint8_t x=(op&0x0F00)>>8; int temp; timeout(0); temp=getch(); if(temp!=ERR) if(v[x]!=(uint8_t)temp) pc+=0x0002; } /* non blocking */
static inline void _FX0A(void) { uint8_t x=(op&0x0F00)>>8; int temp; timeout(-1); temp=getch(); if(temp!=ERR) v[x]=(uint8_t)temp; }
static inline void _FX07(void) { uint8_t x=(op&0x0F00)>>8; v[x]=dt; }
static inline void _FX15(void) { uint8_t x=(op&0x0F00)>>8; dt=v[x]; }
static inline void _FX18(void) { uint8_t x=(op&0x0F00)>>8; st=v[x]; }
static inline void _FX1E(void) { uint8_t x=(op&0x0F00)>>8; I+=v[x]; }
static inline void _FX29(void) { uint8_t x=(op&0x0F00)>>8; I=5*v[x]; }
static inline void _FX33(void) { uint8_t x=(op&0x0F00)>>8; rom[I]=(v[x]/100)%10, rom[I+1]=(v[x]/10)%10, rom[I+2]=v[x]%10; }
//static inline void _FX55(void) { for(int i=0; i<((op&0x0F00)>>8); i++) rom[I+i]=v[((op&0x0F00)>>8)+i]; }
//static inline void _FX65(void) { for(int i=0; i<((op&0x0F00)>>8); i++) v[((op&0x0F00)>>8)+i]=rom[I+i]; }
static inline void _00E0(void) { for(int i=0; i<VROWS; i++) for(int j=0; j<VCOLS; j++) vram[i][j]=0x00; }
static inline void _DXYN(void)
{
    uint8_t x = v[(op&0x0F00)>>8]; // column
    uint8_t y = v[(op&0x00F0)>>4]; // row
    uint8_t n = op&0x000F;         // number of bytes to xor to the screen
    if(y > VROWS-1) y -= VROWS;      // horizonal wrap around
    if(x > VCOLS-1) x -= VCOLS;      // vertical wrap around
    for(uint8_t i=0; i<n; i++)
    for(uint8_t j=0; j<n; j++)
    {
        uint8_t bit = (rom[I+i] >> (7-j)) & 0x01;       // bits to be xor'd to vram
        v[0xF] = (vram[y+i][x+j] && bit) ? 0x01 : 0x00; // if bit and vram is 1, erase imminent, raise flag
        vram[y+i][x+j] ^= bit;                          // xor bit and vram
    }
    init_pair(1, COLOR_BLACK, COLOR_RED);             // initialize COLOR_PAIR(1)
    for(uint8_t i=0; i<VROWS; i++)
    for(uint8_t j=0; j<VCOLS; j++)
    {
        mvaddch(i, 2*j+0, vram[i][j] ? ' '|COLOR_PAIR(1) : ' '); // use COLOR_PAIR(1)
        mvaddch(i, 2*j+1, vram[i][j] ? ' '|COLOR_PAIR(1) : ' '); // use COLOR_PAIR(1)
    }
    refresh();
}

static void cycle(void)
{
    /* emulate */
    op = (rom[pc]<<8)+(rom[pc+1]&0x00FF);
    pc += 0x0002;
    if(dt>0) dt--; // does this go after the loop or before? */
    if(st>0) st--; /* a timer value of 01 will have no audible effect */
    switch(op&0xF000)
    {
            case 0x1000: _1NNN(); break;
            case 0x2000: _2NNN(); break;
            case 0x3000: _3XNN(); break;
          //case 0x4000: _4XNN(); break;
          //case 0x5000: _5XY0(); break;
            case 0x6000: _6XNN(); break;
            case 0x7000: _7XNN(); break;
          //case 0x9000: _9XY0(); break;
            case 0xA000: _ANNN(); break;
          //case 0xB000: _BNNN(); break;
            case 0xC000: _CXNN(); break;
            case 0xD000: _DXYN(); break;
        case 0x0000:
            switch(op&0x000F)
            {
              //case 0x00EE: _00EE(); break;
            }
            break;
        case 0x8000:
            switch(op&0x000F)
            {
                case 0x0000: _8XY0(); break;
              //case 0x0001: _8XY1(); break;
              //case 0x0002: _8XY2(); break;
              //case 0x0003: _8XY3(); break;
              //case 0x0004: _8XY4(); break;
              //case 0x0005: _8XY5(); break;
              //case 0x0006: _8XY6(); break;
              //case 0x0007: _8XY7(); break;
              //case 0x000E: _8XYE(); break;
            }
            break;
        case 0xE000:
            switch(op&0x00FF)
            {
              //case 0x009E: _EX9E(); break;
              //case 0x00A1: _EXA1(); break;
            }
            break;
        case 0xF000:
            switch(op&0x00FF)
            {
              //case 0x0007: _FX07(); break;
              //case 0x000A: _FX0A(); break;
              //case 0x0015: _FX15(); break;
              //case 0x0018: _FX18(); break;
              //case 0x001E: _FX1E(); break;
              //case 0x0029: _FX29(); break;
              //case 0x0033: _FX33(); break;
              //case 0x0055: _FX55(); break;
              //case 0x0065: _FX65(); break;
            }
            break;
    }
    napms(1);
}

long load(const char* str)
{
    uint8_t ch[80] =
    {
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
    FILE* fp = fopen(str, "rb");
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);
    uint8_t buf[4096];
    fread(buf, 1, sz, fp);
    fclose(fp);
    for(uint16_t i=0; i<80; i++) rom[i+0x0000] = ch[i];
    for(uint16_t i=0; i<sz; i++) rom[i+0x0200] = buf[i];
    return sz;
}

void dumprom(long sz)
{
    printf("Program Size: %d Bytes\n", sz);
    printf(".FONT\n");
    for(uint16_t i=0; i<80; i+=5)
        printf("%02X %02X %02X %02X %02X\n", rom[i], rom[i+1], rom[i+2], rom[i+3], rom[i+4]);
    printf(".PROGRAM\n");
    for(uint16_t i=0x0200; i<0x0200+sz; i+=2)
        printf("0x%04X: %02X%02X\n", i, rom[i], rom[i+1]);
    printf("ROWS: %d\n", LINES);
    printf("COLS: %d\n", COLS);
}

int main(int argc, char* argv[])
{
    if(argc!=2) return 1;
    srand(time(NULL)); // seed random number generator for opcode _CXNN
    long sz = load(argv[1]);
    initscr();         // start ncurses
    #define DEBUG 1
    #if DEBUG==1
    goto end;
    #endif
    timeout(-1);       // blocking keyboard input default 
    start_color();     // enable color
    curs_set(0);       // disable cursor
    for(;;) cycle();
end:endwin();          // end ncurses
    dumprom(sz);       // put ROM contents on ncurses display
    return 0;
}
