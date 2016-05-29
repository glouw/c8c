#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

u8 input(int ms);

void _0000(void) { }
void _00E0(void) { for(u16 i = 0; i < VROWS; i++) while(vmem[i] >>= 1); }
void _00EE(void) { pc = s[--sp]; }
void _00EF(void) { endwin(); exit(0); }
void _1NNN(void) { u16 nnn = op & 0x0FFF; pc = nnn; }
void _2NNN(void) { u16 nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
void _3XNN(void) { u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
void _4XNN(void) { u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
void _5XY0(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
void _6XNN(void) { u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x]  = nn; }
void _7XNN(void) { u16 nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
void _8XY0(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x]  = v[y]; }
void _8XY1(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] |= v[y]; }
void _8XY2(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] &= v[y]; }
void _8XY3(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] ^= v[y]; }
void _8XY4(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] + v[y] > 0xFF ? 0x01 : 0x00; v[x] = v[x] + v[y]; }
void _8XY5(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] - v[y] < 0x00 ? 0x00 : 0x01; v[x] = v[x] - v[y]; }
void _8XY7(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[y] - v[x] < 0x00 ? 0x00 : 0x01; v[x] = v[y] - v[x]; }
void _8XY6(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 0) & 0x01; v[x] = v[y] >> 1; }
void _8XYE(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 7) & 0x01; v[x] = v[y] << 1; }
void _9XY0(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
void _ANNN(void) { u16 nnn = op & 0x0FFF; I = nnn; }
void _BNNN(void) { u16 nnn = op & 0x0FFF; pc = nnn + v[0x0]; }
void _CXNN(void) { u16 x = (op & 0x0F00) >> 8, nn = op & 0x00FF; v[x] = nn & (rand() % 0x100); }
void _DXYN(void) { u16 x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4, n = (op & 0x000F);
    u8 flag = 0x00;
    for(int i = 0; i < n; i++) {
        u64 line = (u64)mem[I + i] << (VCOLS - 8);
        line >>= v[x];
        if(!flag) flag = (vmem[v[y] + i] ^ line) != (vmem[v[y] + i] | line);
        vmem[v[y] + i] ^= line;
    }
    v[0xF] = flag;
}
void _EXA1(void) { u16 x = (op & 0x0F00) >> 8; u8 pressed = input(+0); if(pressed == 0xFF) return; if(v[x] != pressed) pc += 2; }
void _EX9E(void) { u16 x = (op & 0x0F00) >> 8; u8 pressed = input(+0); if(pressed == 0xFF) return; if(v[x] == pressed) pc += 2; }
void _FX07(void) { u16 x = (op & 0x0F00) >> 8; v[x] = dt; }
void _FX0A(void) { u16 x = (op & 0x0F00) >> 8; u8 pressed = input(-1); if(pressed == 0xFF) return; v[x] = pressed; }
void _FX15(void) { u16 x = (op & 0x0F00) >> 8; dt = v[x]; }
void _FX18(void) { u16 x = (op & 0x0F00) >> 8; st = v[x]; }
void _FX1E(void) { u16 x = (op & 0x0F00) >> 8; I += v[x]; }
void _FX29(void) { u16 x = (op & 0x0F00) >> 8; I = 5 * v[x]; }
void _FX33(void) { u16 x = (op & 0x0F00) >> 8;
    #define len(array) (signed)(sizeof(array) / sizeof(*array))
    int i, lookup[] = { 100, 10, 1 };
    for(i = 0; i < len(lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
void _FX55(void) { u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
void _FX65(void) { u16 x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

void _0___(void);
void _8___(void);
void _E___(void);
void _F___(void);
void (*opsa[])(void) = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _00EF };
void (*opsb[])(void) = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
void (*opsc[])(void) = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
void (*opsd[])(void) = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
                         _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
                         _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
void (*list[])(void) = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
void exec(void (*opcode)(void)) { opcode(); }
void _0___(void) { exec(opsa[op & 0x000F]); }
void _8___(void) { exec(opsb[op & 0x000F]); }
void _E___(void) { exec(opsc[op & 0x000F]); }
void _F___(void) { exec(opsd[op & 0x00FF]); }

u8 input(int ms)
{
    timeout(ms);
    switch(getch())
    {
        case '1': return 0x01; case '2': return 0x02; case '3': return 0x03; case '4': return 0x0C;
        case 'q': return 0x04; case 'w': return 0x05; case 'e': return 0x06; case 'r': return 0x0D;
        case 'a': return 0x07; case 's': return 0x08; case 'd': return 0x09; case 'f': return 0x0E;
        case 'z': return 0x0A; case 'x': return 0x00; case 'c': return 0x0B; case 'v': return 0x0F;
    }
    return 0xFF;
}

void output(void)
{
    erase();
    for(int i = 0; i < VROWS; i++) {
    for(int j = 0; j < VCOLS; j++)
    {
        u8 pixel = (vmem[i] >> (VCOLS - 1 - j)) & 0x1;
        pixel ?
            color_set(1, NULL):
            color_set(2, NULL);
        addstr(" ");
    }
    color_set(3, NULL);
    addch('\n');
    }
    addch('\n');
    for(int i = 0; i < VSIZE; i++)
    {
        printw("  ");
        printw("%02X ", v[i]);
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
    refresh();
}

void cycle(void)
{
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    if(dt > 0) dt--;
    if(st > 0) st--;
    if(st) beep();
    exec(list[op >> 12]);
    output();
    napms(10);
}

void load(char* str)
{
    u8 ch[BFONT] =
    {
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
    FILE* game = fopen(str, "rb");
    if(!game)
    {
        endwin();
        fprintf(stderr, "%s not found\n", str);
        exit(1);
    }
    fseek(game, 0, SEEK_END);
    long size = ftell(game);
    rewind(game);
    u8 buf[BYTES];
    fread(buf, 1, size, game);
    fclose(game);
    for(u16 i = 0; i < BFONT; i++)
        mem[i] = ch[i];
    for(u16 i = 0; i < size; i++)
        mem[i + START] = buf[i];
}

int main(int argc, char* argv[])
{
    if(argc != 2)
        return 1;
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, 0, 5); // Purple Square
    init_pair(2, 0, 0); // Black square
    init_pair(3, 7, 0); // White text
    load(argv[1]);
    srand(time(0));
    for(;;)
        cycle();
}
