#pragma once

struct feed
{
    char* (*name)(void);
    int (*number)(void);
    void (*init)(void);
    int (*peek)(void);
    void (*match)(char x);
    int (*end)(void);
    void (*matches)(const char* const str);
    int (*lines)(void);
    int (*isends)(void);
    int (*farpeek)(void);
};

extern const struct feed feed;
