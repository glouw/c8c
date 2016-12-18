#pragma once

#include "tree.h"

const struct generator
{
    void (*construct)(void);
    void (*reset)(char* entry);
    void (*scan)(bool growing);
}
generator;
