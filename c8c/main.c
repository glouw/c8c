#include "feed.h"
#include "io.h"
#include "ident.h"
#include "translate.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
        return 1;
    io.init(fopen(argv[1], "r"), stdout);
    feed.init();
    ident.init();
    translate.program();
}
