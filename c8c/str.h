#pragma once

struct str
{
    int (*eql)(const char* a, const char* b);
    char* (*dup)(const char* s);
    int (*ispostfix)(const char* s);
    int (*isassign)(const char* s);
    int (*ischain)(const char* s);
    int (*isname)(const char* lv);
};

extern const struct str str;
