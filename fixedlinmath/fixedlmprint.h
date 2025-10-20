#ifndef FIXED_LIN_MATH_PRINT_H
#define FIXED_LIN_MATH_PRINT_H

#include "fixedlinmath.h"

extern FLMErrnoString errnoStringConsts[FLMERRNOSTRING_SIZE];

// fixedlinmath/fixedlmprint.c
void fixedLMPrintMatrix(flmmat_t *mat);
FLMErrnoString fixedLMGetErrnoString(FLMErrorCode code);
void fixedLMPrintErrorCode(FLMErrorCode code);
void fixedLMPrintError();

#endif // FIXED_LIN_MATH_PRINT_H
