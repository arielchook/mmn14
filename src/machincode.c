#include <machinecode.h>
int dataSection[DATA_SECTION_SIZE];
int IC=0;
int DC=0;

bool writeToDataSection(int value)
{
    if (DC>=DATA_SECTION_SIZE) {
        return false;
    }
    dataSection[DC++]=value;
    return true;
}

void advanceDC(int howmuch)
{
    DC+=howmuch;
}
