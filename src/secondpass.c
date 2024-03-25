#include <secondpass.h>
#include <stdio.h>
#include <stdbool.h>
#include <unressym.h>

bool secondPass()
{
    if (!resolve_symbols())
        return false;

    return true;
}