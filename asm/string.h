#pragma once

struct string
{
    char* (*duplicate)(char* s);
};

extern const struct string string;
