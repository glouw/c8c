#pragma once

struct errors
{
    void (*handle)(int error, unsigned linenumber);
};

extern struct errors errors;
