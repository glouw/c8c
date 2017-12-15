#pragma once

struct feed
{
    char* (*name)(void);
    int (*number)(void);
    void (*init)(void);
    int (*isop)(void);
    int (*peek)(void);
    void (*match)(char x);
    int (*end)(void);
    void (*matches)(const char* const str);
    int (*lines)(void);
};

extern const struct feed feed;
