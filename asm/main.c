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
    // Reset vector
    generator.reset("MAIN");
    // Second pass
    generator.scan(false);
    // All's well that ends well
    exit(0);
}
