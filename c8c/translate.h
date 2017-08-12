#pragma once

struct translate
{
    void (*expression)();
    void (*ident)();
    void (*print_names)();
};

extern const struct translate translate;
