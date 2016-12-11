#pragma once

const struct errors
{
    void (*handle)(const int error, const unsigned linenumber);
}
errors;
