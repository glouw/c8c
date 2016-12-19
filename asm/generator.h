#pragma once

#include "tree.h"

const struct generator
{
    void (*construct)(void);
    void (*entrance)(char* entry);
    void (*scan)(bool growing);
}
generator;
