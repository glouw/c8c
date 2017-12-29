#include "files.h"
#include "generator.h"

int main(int argc, char* argv[])
{
    files.construct();
    if(argc != 3)
        exit(1);
    files.open(argv);
    // First pass
    generator.construct();
    generator.scan(true);
    files.reset();
    // Reset vector
    generator.entrance("main");
    // Second pass
    generator.scan(false);
    // Done
    exit(0);
}
