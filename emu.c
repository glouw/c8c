#include <SDL2/SDL.h>
#include <stdint.h>
#include <time.h>

#define VROWS (32)
#define VCOLS (64)
#define BYTES (4096)
#define START (0x0200)
#define VSIZE (16)
#define SSIZE (12)
#define BFONT (80)

static uint64_t vmem[VROWS];

static uint16_t pc = START;
static uint16_t I;
static uint16_t s[SSIZE];
static uint16_t op;

static uint8_t dt;
static uint8_t st;
static uint8_t sp;
static uint8_t v[VSIZE];
static uint8_t mem[BYTES];
static uint8_t charges[VROWS][VCOLS];

static const uint8_t* key;

static SDL_Window* window;
static SDL_Renderer* renderer;

static int input(const int waiting)
{
    do
    {
        SDL_PumpEvents();
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

static void _0000() { /* no-op */ }
static void _00E0() { for(int j = 0; j < VROWS; j++) while(vmem[j] >>= 1); }
static void _00EE() { pc = s[--sp]; }
static void _1NNN() { uint16_t nnn = op & 0x0FFF; pc = nnn; }
static void _2NNN() { uint16_t nnn = op & 0x0FFF; s[sp++] = pc; pc = nnn; }
static void _3XNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] == nn) pc += 0x0002; }
static void _4XNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; if(v[x] != nn) pc += 0x0002; }
static void _5XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] == v[y]) pc += 0x0002; }
static void _6XNN() { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x]  = nn; }
static void _7XNN() { uint16_t nn = op & 0x00FF, x = (op & 0x0F00) >> 8; v[x] += nn; }
static void _8XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x]  = v[y]; }
static void _8XY1() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] |= v[y]; }
static void _8XY2() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] &= v[y]; }
static void _8XY3() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; v[x] ^= v[y]; }
static void _8XY4() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; uint8_t flag = v[x] + v[y] > 0xFF ? 0x01 : 0x00; v[x] = v[x] + v[y]; v[0xF] = flag; }
static void _8XY5() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; uint8_t flag = v[x] - v[y] < 0x00 ? 0x00 : 0x01; v[x] = v[x] - v[y]; v[0xF] = flag; }
static void _8XY7() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; uint8_t flag = v[y] - v[x] < 0x00 ? 0x00 : 0x01; v[x] = v[y] - v[x]; v[0xF] = flag; }
static void _8XY6() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; uint8_t flag = (v[y] >> 0) & 0x01; v[x] = v[y] >> 1; v[0xF] = flag; }
static void _8XYE() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; uint8_t flag = (v[y] >> 7) & 0x01; v[x] = v[y] << 1; v[0xF] = flag; }
static void _9XY0() { uint16_t x = (op & 0x0F00) >> 8, y = (op & 0x00F0) >> 4; if(v[x] != v[y]) pc += 0x0002; }
static void _ANNN() { uint16_t nnn = op & 0x0FFF; I = nnn; }
static void _BNNN() { uint16_t nnn = op & 0x0FFF; pc = nnn + v[0x0]; }
static void _CXNN() { uint16_t x = (op & 0x0F00) >> 8, nn = op & 0x00FF; v[x] = nn & (rand() % 0x100); }
static void _DXYN() {
    uint16_t x = (op & 0x0F00) >> 8;
    uint16_t y = (op & 0x00F0) >> 4;
    uint16_t n = (op & 0x000F) >> 0;
    uint8_t flag = 0;
    for(int j = 0; j < n; j++)
    {
        uint64_t line = (uint64_t) mem[I + j] << (VCOLS - 8);
        line >>= v[x];
        if((vmem[v[y] + j] ^ line) != (vmem[v[y] + j] | line))
            flag = 1;
        vmem[v[y] + j] ^= line;
    }
    v[0xF] = flag;
}
static void _EXA1() { uint16_t x = (op & 0x0F00) >> 8; if(v[x] != input(0)) pc += 0x0002; }
static void _EX9E() { uint16_t x = (op & 0x0F00) >> 8; if(v[x] == input(0)) pc += 0x0002; }
static void _FX07() { uint16_t x = (op & 0x0F00) >> 8; v[x] = dt; }
static void _FX0A() { uint16_t x = (op & 0x0F00) >> 8; v[x] = input(1); }
static void _FX15() { uint16_t x = (op & 0x0F00) >> 8; dt = v[x]; }
static void _FX18() { uint16_t x = (op & 0x0F00) >> 8; st = v[x]; }
static void _FX1E() { uint16_t x = (op & 0x0F00) >> 8; I += v[x]; }
static void _FX29() { uint16_t x = (op & 0x0F00) >> 8; I = 5 * v[x]; }
static void _FX33() { uint16_t x = (op & 0x0F00) >> 8;
    const int lookup[] = { 100, 10, 1 };
    for(unsigned i = 0; i < sizeof(lookup) / sizeof(*lookup); i++)
        mem[I + i] = v[x] / lookup[i] % 10;
}
static void _FX55() { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) mem[I + i] = v[i]; I += i; }
static void _FX65() { uint16_t x = (op & 0x0F00) >> 8; int i; for(i = 0; i <= x; i++) v[i] = mem[I + i]; I += i; }

static void _0___();
static void _8___();
static void _E___();
static void _F___();
static void (*opsa[])() = { _00E0, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _00EE, _0000 };
static void (*opsb[])() = { _8XY0, _8XY1, _8XY2, _8XY3, _8XY4, _8XY5, _8XY6, _8XY7, _0000, _0000, _0000, _0000, _0000, _0000, _8XYE, _0000 };
static void (*opsc[])() = { _0000, _EXA1, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _EX9E, _0000 };
static void (*opsd[])() = { _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX07, _0000, _0000, _FX0A, _0000, _0000, _0000, _0000, _0000,
/*************************/ _0000, _0000, _0000, _0000, _0000, _FX15, _0000, _0000, _FX18, _0000, _0000, _0000, _0000, _0000, _FX1E, _0000,
/*                       */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _FX29, _0000, _0000, _0000, _0000, _0000, _0000,
/*                       */ _0000, _0000, _0000, _FX33, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*         CHIP-8        */ _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                       */ _0000, _0000, _0000, _0000, _0000, _FX55, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*                       */ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000,
/*************************/ _0000, _0000, _0000, _0000, _0000, _FX65, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000, _0000 };
static void (*exec[])() = { _0___, _1NNN, _2NNN, _3XNN, _4XNN, _5XY0, _6XNN, _7XNN, _8___, _9XY0, _ANNN, _BNNN, _CXNN, _DXYN, _E___, _F___ };
static void _0___() { (*opsa[op & 0x000F])(); }
static void _8___() { (*opsb[op & 0x000F])(); }
static void _E___() { (*opsc[op & 0x000F])(); }
static void _F___() { (*opsd[op & 0x00FF])(); }

static void load(const char* game)
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
    {
        fprintf(stderr, "error: binary '%s' not found\n", game);
        exit(1);
    }
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

static void cycle()
{
    if(dt > 0) dt--;
    if(st > 0) st--;
    if(st)
    {
        /* Beep */
    }
    op = (mem[pc] << 8) + (mem[pc + 1] & 0x00FF);
    pc += 0x0002;
    (*exec[op >> 12])();
}

static void output()
{
    for(int j = 0; j < VROWS; j++)
    for(int i = 0; i < VCOLS; i++)
    {
        SDL_SetRenderDrawColor(renderer, charges[j][i], 0x00, 0x00, 0x00);
        const int w = 8;
        const SDL_Rect rect = { i * w + 1, j * w + 1, w - 2, w - 2 };
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

static int charging(const int j, const int i)
{
    return (vmem[j] >> (VCOLS - 1 - i)) & 0x1;
}

static void charge()
{
    for(int j = 0; j < VROWS; j++)
    for(int i = 0; i < VCOLS; i++)
        if(charging(j, i))
            charges[j][i] = 0xFF;
}

static void discharge()
{
    for(int j = 0; j < VROWS; j++)
    for(int i = 0; i < VCOLS; i++)
        if(!charging(j, i))
            charges[j][i] *= 0.997;
}

void dump()
{
    for(int i = 0; i < VSIZE; i++)
        printf("v[%02d]: %d = 0x%02X\n", i, v[i], v[i]);
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "error: too few or too many argmuents\n");
        exit(1);
    }
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(512, 256, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_SetWindowTitle(window, "Emu-1.0");
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    key = SDL_GetKeyboardState(NULL);
    load(argv[1]);
    srand(time(0));
    for(int cycles = 0; !key[SDL_SCANCODE_END] && !key[SDL_SCANCODE_ESCAPE]; cycles++)
    {
        SDL_PumpEvents(); // Cannot poll an SDL_Event -- Too slow!
        charge();
        cycle();
        if(cycles % 15 == 0)
            output();
        discharge();
    }
    SDL_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
