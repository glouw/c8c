#pragma once

struct translate
{
    void (*expression)();
    void (*ident)();
    void (*names)();
};

extern const struct translate translate;
