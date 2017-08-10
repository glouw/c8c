#pragma once

const struct io
{
    void (*bomb)(char* message, ...);
    void (*emit)(char* message, ...);
}
io;
