#pragma once
#include "common.h"
void memEnd(void);
BOOL configmem(int unit, int unitnumber);
BOOL memInit(void);
void* allocateMemory(const unsigned int nbyte);
void freeMemory(void* freepointer);
void showMem(char* buf);
