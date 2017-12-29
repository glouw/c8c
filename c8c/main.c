#include "io.h"
#include "compile.h"

int main(int argc, char* argv[])
{
    if(argc != 3)
        io.bomb("specify input and output files");
    io.init(argv);
    compile.init();
    compile.program();
}
