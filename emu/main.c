#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define VROWS     32
#define VCOLS     64
#define BYTES   4096
#define START 0x0200
#define VSIZE     16
#define SSIZE     12
#define BFONT     80

uint64_t vmem[VROWS];
uint16_t pc = START;
uint16_t I;
uint16_t s[SSIZE];
uint16_t op;
uint8_t dt;
uint8_t st;
uint8_t sp;
uint8_t v[VSIZE];
uint8_t mem[BYTES];

uint8_t input(int ms)
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

void _0000(void) { } // No-op
void _00E0(void) { for(uint16_t i = 0; i < VROWS; i++) while(vmem[i] >>= 1); }
void _00EE(void) { pc = s[--sp]; }
void _1NNN(void) { uint16_t nnn = op & 0x0FFF; pc = nnn; }
void _2NNN(void) { uint16_t nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
void _3XNN(void) { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
void _4XNN(void) { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
void _5XY0(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
void _6XNN(void) { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x]  = nn; }
void _7XNN(void) { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
void _8XY0(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x]  = v[y]; }
void _8XY1(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] |= v[y]; }
void _8XY2(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] &= v[y]; }
void _8XY3(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] ^= v[y]; }
void _8XY4(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] + v[y] > 0xFF ? 0x01 : 0x00; v[x] = v[x] + v[y]; }
void _8XY5(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] - v[y] < 0x00 ? 0x00 : 0x01; v[x] = v[x] - v[y]; }
void _8XY7(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[y] - v[x] < 0x00 ? 0x00 : 0x01; v[x] = v[y] - v[x]; }
void _8XY6(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 0) & 0x01; v[x] = v[y] >> 1; }
void _8XYE(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 7) & 0x01; v[x] = v[y] << 1; }
void _9XY0(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
void _ANNN(void) { uint16_t nnn = op & 0x0FFF; I = nnn; }
void _BNNN(void) { uint16_t nnn = op & 0x0FFF; pc = nnn + v[0x0]; }
void _CXNN(void) { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; v[x] = nn & (rand() % 0x100); }
void _DXYN(void) { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4, n = (op & 0x000F);
    uint8_t flag = 0x00;
    for(int i = 0; i < n; i++)
    {
        uint64_t line = (uint64_t)mem[I + i] << (VCOLS - 8);
        line >>= v[x];
        if(!flag) flag = (vmem[v[y] + i] ^ line) != (vmem[v[y] + i] | line);
        vmem[v[y] + i] ^= line;
    }
    v[0xF] = flag;
}
void _EXA1(void) { uint16_t x = (op & 0x0F00) >> 8; uint8_t pressed = input( 0); if(pressed == 0xFF) { pc += 0x0002; return; } if(v[x] != pressed) pc += 0x0002; }
void _EX9E(void) { uint16_t x = (op & 0x0F00) >> 8; uint8_t pressed = input( 0); if(pressed == 0xFF) { pc += 0x0000; return; } if(v[x] == pressed) pc += 0x0002; }
void _FX07(void) { uint16_t x = (op & 0x0F00) >> 8; v[x] = dt; }
void _FX0A(void) { uint16_t x = (op & 0x0F00) >> 8; uint8_t pressed = input(-1); if(pressed == 0xFF) v[x] = 0x00; else v[x] = pressed; }
void _FX15(void) { uint16_t x = (op & 0x0F00) >> 8; dt = v[x]; }
void _FX18(void) { uint16_t x = (op & 0x0F00) >> 8; st = v[x]; }
void _FX1E(void) { uint16_t x = (op & 0x0F00) >> 8; I += v[x]; }
void _FX29(void) { uint16_t x = (op & 0x0F00) >> 8; I = 5 * v[x]; }
void _FX33(void) { uint16_t x = (op & 0x0F00) >> 8;
    #define len(array) (signed)(sizeof(array) / sizeof(*array))
    int lookup[] = { 100, 10, 1 };
    for(int i = 0; i < len(lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
void _FX55(void) { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
void _FX65(void) { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

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

void output(void)
{
    erase();
    for(int i = 0; i < VROWS; i++) {
    for(int j = 0; j < VCOLS; j++)
    {
        uint8_t pixel = (vmem[i] >> (VCOLS - 1 - j)) & 0x1;
        pixel ? color_set(1, NULL) : color_set(2, NULL);
        addch(' ');
    }
    color_set(3, NULL);
    addch('\n');
    }
    refresh();
}

void cycle(void)
{
    if(dt > 0) dt--;
    if(st > 0) st--;
    if(st) beep();
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    (*exec[op >> 12])();
    output();
    napms(10);
}

void load(char* game)
{
    uint8_t ch[BFONT] = {
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
    FILE* fp = fopen(game, "rb");
    if(!fp)
        exit(1);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    uint8_t buf[BYTES];
    fread(buf, 1, size, fp);
    fclose(fp);
    for(uint16_t i = 0; i < BFONT; i++)
        mem[i] = ch[i];
    for(uint16_t i = 0; i < size; i++)
        mem[i + START] = buf[i];
}

void cleanup(void)
{
    endwin();
}

int main(int argc, char* argv[])
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
    char* game = argv[1];
    load(game);
    srand(time(0));
    for(;;)
        cycle();
}
