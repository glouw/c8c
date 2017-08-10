#pragma once

#include "tree.h"

struct generator
{
    void (*construct)();
    void (*entrance)(const char* entry);
    void (*scan)(bool growing);
};

extern const struct generator generator;
