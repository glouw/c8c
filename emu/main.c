#include <SDL2/SDL.h>
#include <stdint.h>
#include <time.h>

enum
{
    VROWS = 32, VCOLS = 64, BYTES = 4096, START = 0x0200, VSIZE = 16, SSIZE = 12, BFONT = 80
};

uint64_t vmem[VROWS];

uint16_t pc = START, I, s[SSIZE], op;

uint8_t dt, st, sp, v[VSIZE], mem[BYTES];

const uint8_t* key;

int input(const int waiting)
{
    // Needs "wait for keypress" event.
    SDL_PumpEvents();
    do
    {
        if(key[SDL_SCANCODE_1]) return 0x01;
        if(key[SDL_SCANCODE_2]) return 0x02;
        if(key[SDL_SCANCODE_3]) return 0x03;
        if(key[SDL_SCANCODE_4]) return 0x0C;
        if(key[SDL_SCANCODE_Q]) return 0x04;
        if(key[SDL_SCANCODE_W]) return 0x05;
        if(key[SDL_SCANCODE_E]) return 0x06;
        if(key[SDL_SCANCODE_R]) return 0x0D;
        if(key[SDL_SCANCODE_A]) return 0x07;
        if(key[SDL_SCANCODE_S]) return 0x08;
        if(key[SDL_SCANCODE_D]) return 0x09;
        if(key[SDL_SCANCODE_F]) return 0x0E;
        if(key[SDL_SCANCODE_Z]) return 0x0A;
        if(key[SDL_SCANCODE_X]) return 0x00;
        if(key[SDL_SCANCODE_C]) return 0x0B;
        if(key[SDL_SCANCODE_V]) return 0x0F;
    }
    while(waiting);
    return -1;
}

SDL_Window* window;
SDL_Renderer* renderer;

void output()
{
    for(int i = 0; i < VROWS; i++)
        for(int j = 0; j < VCOLS; j++)
        {
            const uint8_t pixel = (vmem[i] >> (VCOLS - 1 - j)) & 0x1;
            SDL_SetRenderDrawColor(renderer, 0x00, pixel ? 0xFF : 0x00, 0x00, 0x00);
            const int w = 8;
            const SDL_Rect rect = { j * w + 1, i * w + 1, w - 2, w - 2 };
            SDL_RenderFillRect(renderer, &rect);
        }
    SDL_RenderPresent(renderer);
}

void _0000() { }
void _00E0() { for(int i = 0; i < VROWS; i++) while(vmem[i] >>= 1); }
void _00EE() { pc = s[--sp]; }
void _1NNN() { uint16_t nnn = op & 0x0FFF; pc = nnn; }
void _2NNN() { uint16_t nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
void _3XNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
void _4XNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
void _5XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
void _6XNN() { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x]  = nn; }
void _7XNN() { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
void _8XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x]  = v[y]; }
void _8XY1() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] |= v[y]; }
void _8XY2() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] &= v[y]; }
void _8XY3() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] ^= v[y]; }
void _8XY4() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] + v[y] > 0xFF ? 0x01 : 0x00; v[x] = v[x] + v[y]; }
void _8XY5() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[x] - v[y] < 0x00 ? 0x00 : 0x01; v[x] = v[x] - v[y]; }
void _8XY7() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = v[y] - v[x] < 0x00 ? 0x00 : 0x01; v[x] = v[y] - v[x]; }
void _8XY6() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 0) & 0x01; v[x] = v[y] >> 1; }
void _8XYE() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[0xF] = (v[y] >> 7) & 0x01; v[x] = v[y] << 1; }
void _9XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
void _ANNN() { uint16_t nnn = op & 0x0FFF; I = nnn; }
void _BNNN() { uint16_t nnn = op & 0x0FFF; pc = nnn + v[0x0]; }
void _CXNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; v[x] = nn & (rand() % 0x100); }
void _DXYN() {
    uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4, n = (op & 0x000F);
    uint8_t flag = 0x00;
    for(int i = 0; i < n; i++)
    {
        uint64_t line = (uint64_t) mem[I + i] << (VCOLS - 8);
        line >>= v[x];
        if(!flag)
            flag = (vmem[v[y] + i] ^ line) != (vmem[v[y] + i] | line);
        vmem[v[y] + i] ^= line;
    }
    v[0xF] = flag;
    output();
}
void _EXA1() { uint16_t x = (op & 0x0F00) >> 8; if(v[x] != input(0)) pc += 0x0002; }
void _EX9E() { uint16_t x = (op & 0x0F00) >> 8; if(v[x] == input(0)) pc += 0x0002; }
void _FX07() { uint16_t x = (op & 0x0F00) >> 8; v[x] = dt; }
void _FX0A() { uint16_t x = (op & 0x0F00) >> 8; v[x] = input(1); }
void _FX15() { uint16_t x = (op & 0x0F00) >> 8; dt = v[x]; }
void _FX18() { uint16_t x = (op & 0x0F00) >> 8; st = v[x]; }
void _FX1E() { uint16_t x = (op & 0x0F00) >> 8; I += v[x]; }
void _FX29() { uint16_t x = (op & 0x0F00) >> 8; I = 5 * v[x]; }
void _FX33() { uint16_t x = (op & 0x0F00) >> 8;
    int lookup[] = { 100, 10, 1 };
    for(unsigned i = 0; i < sizeof(lookup) / sizeof(*lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
void _FX55() { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
void _FX65() { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

void _0___();
void _8___();
void _E___();
void _F___();
void (*opsa[])() = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _0000 };
void (*opsb[])() = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
void (*opsc[])() = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
void (*opsd[])() = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
/******************/ _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
/*                */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
/*                */ _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*     CHIP-8     */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                */ _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/******************/ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
void (*exec[])() = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
void _0___() { (*opsa[op & 0x000F])(); }
void _8___() { (*opsb[op & 0x000F])(); }
void _E___() { (*opsc[op & 0x000F])(); }
void _F___() { (*opsd[op & 0x00FF])(); }

void load(const char* game)
{
    const uint8_t ch[BFONT] = {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80,
    };
    FILE* const fp = fopen(game, "rb");
    if(fp == NULL)
        exit(1);
    fseek(fp, 0, SEEK_END);
    const long size = ftell(fp);
    rewind(fp);
    uint8_t buf[BYTES];
    fread(buf, 1, size, fp);
    fclose(fp);
    for(int i = 0; i < BFONT; i++)
        mem[i] = ch[i];
    for(int i = 0; i < size; i++)
        mem[i + START] = buf[i];
}

void cycle()
{
    if(dt > 0) dt--;
    if(st > 0) st--;
    if(st)
    {
        // Beep.
    }
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    (*exec[op >> 12])();
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(512, 256, 0, &window, &renderer);
    key = SDL_GetKeyboardState(NULL);
    if(argc != 2)
        exit(1);
    load(argv[1]);
    srand(time(0));
    for(int cycles = 0; !key[SDL_SCANCODE_END]; cycles++)
    {
        SDL_PumpEvents();
        cycle();
        SDL_Delay(1);
    }
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
