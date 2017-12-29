#pragma once

struct io
{
    void  (*print)(const char* msg, ...);
    void  (*bomb)(const char* msg, ...);
    void  (*next)(void);
    void  (*skip)(void);
    void  (*init)(char* argv[]);
    char  (*peek)(void);
    char* (*peeks)(void);
    int   (*isendexpr)(void);
    int   (*isendop)(void);
    void  (*match)(const int c);
    char* (*gname)(void);
    char* (*gnum)(void);
    char* (*gop)(void);
    int   (*eof)(void);
    void  (*matches)(const char* s);
};

extern const struct io io;
