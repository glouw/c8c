#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#define VROWS     32
#define VCOLS     64
#define BYTES   4096
#define START 0x0200
#define VSIZE     16
#define SSIZE     12
#define BFONT     80

typedef uint64_t u64;

u64 vmem[VROWS];

typedef uint16_t u16;

u16 pc = START;
u16 I;
u16 s[SSIZE];
u16 op;

typedef uint8_t u8;

u8 dt;
u8 st;
u8 sp;
u8 v[VSIZE];
u8 mem[BYTES];

u8 input(const int ms)
{
    timeout(ms);
    switch(getch())
    {
        case '1': return 0x01; case '2': return 0x02; case '3': return 0x03; case '4': return 0x0C;
        case 'q': return 0x04; case 'w': return 0x05; case 'e': return 0x06; case 'r': return 0x0D;
        case 'a': return 0x07; case 's': return 0x08; case 'd': return 0x09; case 'f': return 0x0E;
        case 'z': return 0x0A; case 'x': return 0x00; case 'c': return 0x0B; case 'v': return 0x0F;
        default:
            return 0xFF;
    }
}

void _0000(void) { /* Possibly an exit? */ }
void _00E0(void) { for(u16 i = 0; i < VROWS; i++) while(vmem[i] >>= 1); }
void _00EE(void) { pc = s[--sp]; }
void _1NNN(void) { const u16 nnn = op & 0x0FFF; pc = nnn; }
void _2NNN(void) { const u16 nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
void _3XNN(void) { const u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
void _4XNN(void) { const u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
void _5XY0(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
void _6XNN(void) { const u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x]  = nn; }
void _7XNN(void) { const u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
void _8XY0(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x]  = v[y]; }
void _8XY1(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] |= v[y]; }
void _8XY2(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] &= v[y]; }
void _8XY3(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] ^= v[y]; }
void _8XY4(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] + v[y] > 0xFF ? 0x01 : 0x00; v[x] = v[x] + v[y]; }
void _8XY5(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] - v[y] < 0x00 ? 0x00 : 0x01; v[x] = v[x] - v[y]; }
void _8XY7(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[y] - v[x] < 0x00 ? 0x00 : 0x01; v[x] = v[y] - v[x]; }
void _8XY6(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 0) & 0x01; v[x] = v[y] >> 1; }
void _8XYE(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 7) & 0x01; v[x] = v[y] << 1; }
void _9XY0(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
void _ANNN(void) { const u16 nnn = op & 0x0FFF; I = nnn; }
void _BNNN(void) { const u16 nnn = op & 0x0FFF; pc = nnn + v[0x0]; }
void _CXNN(void) { const u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; v[x] = nn & (rand() % 0x100); }
void _DXYN(void) { const u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4, n = (op & 0x000F);
    u8 flag = 0x00;
    for(int i = 0; i < n; i++)
    {
        u64 line = (u64)mem[I + i] << (VCOLS - 8);
        line >>= v[x];
        if(!flag) flag = (vmem[v[y] + i] ^ line) != (vmem[v[y] + i] | line);
        vmem[v[y] + i] ^= line;
    }
    v[0xF] = flag;
}
void _EXA1(void) { const u16 x = (op & 0x0F00) >> 8; const u8 pressed = input( 0); if(pressed == 0xFF) { pc += 0x0002; return; } if(v[x] != pressed) pc += 0x0002; }
void _EX9E(void) { const u16 x = (op & 0x0F00) >> 8; const u8 pressed = input( 0); if(pressed == 0xFF) { pc += 0x0000; return; } if(v[x] == pressed) pc += 0x0002; }
void _FX07(void) { const u16 x = (op & 0x0F00) >> 8; v[x] = dt; }
void _FX0A(void) { const u16 x = (op & 0x0F00) >> 8; const u8 pressed = input(-1); if(pressed == 0xFF) v[x] = 0x00; else v[x] = pressed; }
void _FX15(void) { const u16 x = (op & 0x0F00) >> 8; dt = v[x]; }
void _FX18(void) { const u16 x = (op & 0x0F00) >> 8; st = v[x]; }
void _FX1E(void) { const u16 x = (op & 0x0F00) >> 8; I += v[x]; }
void _FX29(void) { const u16 x = (op & 0x0F00) >> 8; I = 5 * v[x]; }
void _FX33(void) { const u16 x = (op & 0x0F00) >> 8;
    #define len(array) (signed)(sizeof(array) / sizeof(*array))
    const int lookup[] = { 100, 10, 1 };
    for(int i = 0; i < len(lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
void _FX55(void) { const u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
void _FX65(void) { const u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

void _0___(void);
void _8___(void);
void _E___(void);
void _F___(void);
void (*opsa[])(void) = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _0000 };
void (*opsb[])(void) = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
void (*opsc[])(void) = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
void (*opsd[])(void) = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
/**********************/ _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
/*                    */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
/*      zZzZzZzZzZ... */ _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* ()()               */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* (-.)               */ _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/* () ()              */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/**********************/ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
void (*exec[])(void) = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
void _0___(void) { (*opsa[op & 0x000F])(); }
void _8___(void) { (*opsb[op & 0x000F])(); }
void _E___(void) { (*opsc[op & 0x000F])(); }
void _F___(void) { (*opsd[op & 0x00FF])(); }

void debug(void)
{
    addch('\n');
    for(int i = 0; i < VSIZE; i++)
    {
        printw("  ");
        printw("%02X", v[i]);
        printw(" ");
    }
    addch('\n');
    for(int i = 0; i < SSIZE; i++)
        printw("%04X ", s[i]);
    addch('\n');
    printw("op: %04X\n", op);
    printw("pc: %04X\n", pc);
    printw("I:  %04X\n", I);
    printw("dt: %02X\n", dt);
    printw("st: %02X\n", st);
    printw("sp: %02X\n", sp);
}

void output(void)
{
    erase();
    for(int i = 0; i < VROWS; i++)
    {
        for(int j = 0; j < VCOLS; j++)
        {
            const u8 pixel = (vmem[i] >> (VCOLS - 1 - j)) & 0x1;
            pixel ? color_set(1, NULL) : color_set(2, NULL);
            addch(' ');
        }
        color_set(3, NULL);
        addch('\n');
    }
    debug();
    refresh();
}

void cycle(void)
{
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    if(dt > 0) dt--;
    if(st > 0) st--;
    if(st) beep();
    (*exec[op >> 12])();
    output();
    napms(10);
}

void load(const char* const game)
{
    const u8 ch[BFONT] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80 
    };
    FILE* const fp = fopen(game, "rb");
    if(!fp)
        exit(2);
    fseek(fp, 0, SEEK_END);
    const long size = ftell(fp);
    rewind(fp);
    u8 buf[BYTES];
    fread(buf, 1, size, fp);
    fclose(fp);
    for(u16 i = 0; i < BFONT; i++)
        mem[i] = ch[i];
    for(u16 i = 0; i < size; i++)
        mem[i + START] = buf[i];
}

void cleanup(void)
{
    endwin();
}

int main(const int argc, const char* const argv[])
{
    atexit(cleanup);
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, 0, 5); // Purple Square
    init_pair(2, 0, 0); // Black square
    init_pair(3, 7, 0); // White text
    if(argc != 2)
        exit(1);
    const char* const game = argv[1];
    load(game);
    srand(time(0));
    for(;;)
        cycle();
}
