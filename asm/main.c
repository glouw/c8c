#include "files.h"
#include "generator.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "error: requires input.asm, output.hex\n");
        return 1;
    }
    files.construct();
    files.open(argv);
    // First pass
    generator.construct();
    generator.scan(true);
    files.reset();
    // Reset vector
    generator.entrance("MAIN");
    // Second pass
    generator.scan(false);
    // Done
    exit(0);
}
