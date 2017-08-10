#pragma once

struct io
{
    void (*bomb)(const char* message, ...);
    void (*emit)(const char* message, ...);
};

extern const struct io io;
