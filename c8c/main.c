#include "feed.h"
#include "io.h"
#include "translate.h"

int main()
{
    io.init(stdin, stdout);
    feed.init();
    translate.ident();
    translate.ident();
    translate.print_names();
}
