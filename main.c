#include "fixedlinmath/fixedlinmath.h"
#include "fixedlinmath/fixedlmprint.h"
#include "fixedpoint/fixedpoint.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	fixed32_t data[36];
	flmmat_t mat;
	FLMErrorCode code;

	code = fixedLMCreateMatrix(&mat, 6, 6, FIXED32_T, data);
	printf("Code: %i\n", code);

	code = fixedLMSetValue(&mat, 0, 0, 0x10000);
	printf("Code: %i\n", code);

	fixed32_t a = fixedLMRetrieveValue(&mat, 0, 0);
	printf("Code: %i\n", fixedLMGetErrno(code));
	printf("Value: %lf\n", convertF32ToDouble(a));

	fixedLMPrintMatrix(&mat);

	fixedLMZeros(&mat);
	printf("Code: %i\n", fixedLMGetErrno(code));
	fixedLMPrintMatrix(&mat);
	fixedLMOnes(&mat);
	printf("Code: %i\n", fixedLMGetErrno(code));
	fixedLMPrintMatrix(&mat);
	fixedLMFill(&mat, 0x1FFFFFF);
	printf("Code: %i\n", fixedLMGetErrno(code));
	fixedLMPrintMatrix(&mat);
	fixedLMEye(&mat);
	printf("Code: %i\n", fixedLMGetErrno(code));
	fixedLMPrintMatrix(&mat);

	return 0;
}
