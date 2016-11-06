#pragma once

struct errors
{
    void (*handle)(int error, unsigned linenumber);
};

extern const struct errors errors;
