#include "fixedlinmath/fixedlinmath.h"
#include "fixedlinmath/fixedlmprint.h"
#include "fixedpoint/fixedpoint.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void creationAndBasiOperations(){
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
}

void basic_arithmetic(){
	FLMErrorCode code;
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
	printf("Code: %i\n", code);
	fixedLMPrintMatrix(&m3);

	code = fixedLMSubScalar(&m3, NULL, FIXED32_T, 0x00018000);
	printf("Code: %i\n", code);
	fixedLMPrintMatrix(&m3);

	code = fixedLMMulScalar(&m3, NULL, FIXED32_T, 0x00020000);
	printf("Code: %i\n", code);
	fixedLMPrintMatrix(&m3);
}

void matmul_test(){
	FLMErrorCode code;
	flmmat_t m1, m2, m3, m4;
	fixed32_t data1[6];
	fixed32_t data2[6];
	fixed32_t data3[4];

	code = fixedLMCreateMatrix(&m1, 3, 2, FIXED32_T, data1);
	fixedLMPrintError();
	code = fixedLMCreateMatrix(&m2, 2, 3, FIXED32_T, data2);
	fixedLMPrintError();
	code = fixedLMCreateMatrix(&m3, 2, 2, FIXED32_T, data3);
	fixedLMPrintError();

	code = fixedLMOnes(&m1);
	fixedLMPrintError();
	code = fixedLMSetValue(&m1, 1, 1, 0x0000);
	fixedLMPrintMatrix(&m1);

	code = fixedLMZeros(&m2);
	code = fixedLMSetValue(&m2, 0, 0, 0x10000);
	code = fixedLMSetValue(&m2, 1, 1, 0x10000);
	code = fixedLMSetValue(&m2, 0, 2, 0x10000);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);

	code = fixedLMRowSwap(&m2, 1, 2);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);

	code = fixedLMColSwap(&m2, 0, 1);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);

	code = fixedLMColMulScalar(&m2, 1, FIXED32_T, 0x40000);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);

	code = fixedLMColSub(&m2, 1, 0);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);


	code = fixedLMMul(&m1, &m2, &m3);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m3);

	code = fixedLMDivScalar(&m3, NULL, FIXED32_T, 0x20000);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m3);

	code = fixedLMRowSubScalar(&m3, 1, FIXED32_T, 0x38000);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m3);
	
	fixed32_t data4[4];
	code = fixedLMCreateMatrix(&m4, 2, 2, FIXED32_T, data4);
	fixedLMPrintError();
	FLM_CLEAR_ERROR();

	fixedLMColAddWeighted(&m3, 0, 4, FIXED32_T, 0x40000);
	fixedLMPrintError();
	FLM_CLEAR_ERROR();
	fixedLMPrintMatrix(&m3);

	fixedLMPrintError();
	FLM_CLEAR_ERROR();

	(void) code;
}

void gaussJordan(){
	fixed32_t data1[FLM_BUFFER_CAPACITY(3, 3)];
	flmmat_t m1, m2;
	FLMErrorCode code;

	code = fixedLMCreateMatrix(&m1, 3, 3, FIXED32_T, data1);
	printf("Code: %i\n", code);
	code = fixedLMAllocMatrix(&m2, 1, 3, FIXED32_T);
	printf("Code: %i\n", code);

	fixedLMZeros(&m1);
	fixedLMZeros(&m2);

	fixedLMSetValue(&m1, 2, 0, 0x20000);	// 2.0
	fixedLMSetValue(&m1, 1, 1, 0x08000);	// 0.5
	fixedLMSetValue(&m1, 0, 2, 0x48000);	// 4.5
	fixedLMSetValue(&m1, 0, 1, 0x40000);	// 4.0

	//fixedLMEye(&m2);
	fixedLMSetValue(&m2, 0, 0, 0x08000);	// 0.5
	fixedLMSetValue(&m2, 0, 1, 0x74000);	// 7.25
	fixedLMSetValue(&m2, 0, 2, 0x52000);	// 5.125

//	code = fixedLMGaussJordan(&m1, &m2);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m1);
	fixedLMPrintMatrix(&m2);

	fixedLMDestroyMatrix(&m1);
	fixedLMPrintError();
	FLM_CLEAR_ERROR();

/*
	fixedLMDestroyMatrix(&m2);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m2);
	fixedLMPrintError();
*/
	FLM_CLEAR_ERROR();

	fixedLMTranspose(&m1);
	fixedLMPrintMatrix(&m1);
	fixedLMPrintError();

	fixedLMTranspose(&m2);
	fixedLMPrintMatrix(&m2);
	fixedLMPrintError();

}
	


int main(){
	/*
	creationAndBasiOperations();
	basic_arithmetic();
	*/
//	matmul_test();
	gaussJordan();
	return 0;
}
