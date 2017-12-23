#pragma once

struct compile
{
    void (*init)(void);
    void (*program)(void);
};

extern const struct compile compile;
