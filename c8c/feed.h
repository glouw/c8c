#pragma once

struct feed
{
    char* (*name)();
    int (*number)();
    void (*init)();
    int (*isop)();
    int (*peek)();
    void (*match)(char x);
    int (*end)();
    void (*matches)(const char* const str);
    int (*lines)();
};

extern const struct feed feed;
