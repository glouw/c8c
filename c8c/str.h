#pragma once

struct str
{
    int (*eql)(const char* a, const char* b);
    char* (*dup)(const char* s);
};

extern const struct str str;
