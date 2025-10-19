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

	fixed32_t data1[9];
	flmmat_t m1;
	code = fixedLMCreateMatrix(&m1, 3, 3, FIXED32_T, data1);
	fixedLMFill(&m1, 0x10000);
	fixedLMPrintMatrix(&m1);

	fixed64_t data2[9];
	flmmat_t m2;
	code = fixedLMCreateMatrix(&m2, 3, 3, FIXED64_T, data2);
	fixedLMEye(&m2);
	fixedLMPrintMatrix(&m2);

	fixed32_t data3[9];
	flmmat_t m3;
	code = fixedLMCreateMatrix(&m3, 3, 3, FIXED32_T, data3);

	code = fixedLMAdd(&m1, &m2, &m3);
	fixedLMPrintMatrix(&m3);

	code = fixedLMSubScalar(&m3, NULL, FIXED32_T, 0x00008000);
	fixedLMPrintMatrix(&m3);
	return 0;
}
