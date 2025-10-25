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

	fixedLMColAddWeighted(&m3, 0, 1, FIXED32_T, 0x40000);
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

	fixedLMDestroyMatrix(&m1);
	fixedLMDestroyMatrix(&m2);
}
	
void dotCrossProduct(){
	flmmat_t v1, v2, v3;
	fixed32_t data1[3];
	fixed32_t data2[3];
	fixed32_t data3[3];

	fixedLMCreateMatrix(&v1, 1, 3, FIXED32_T, data1);
	fixedLMCreateMatrix(&v2, 1, 3, FIXED32_T, data2);
	fixedLMCreateMatrix(&v3, 1, 3, FIXED32_T, data3);

	fixedLMZeros(&v1);
	fixedLMZeros(&v2);
	fixedLMZeros(&v3);

	fixedLMSetFlag(&v2, FLM_FLAG_TRANSPOSED);
	fixedLMSetValue(&v1, 0, 0, 0x40000);	//  4.0
	fixedLMSetValue(&v1, 0, 2, 0x20000);	//  2.0
	fixedLMSetValue(&v2, 0, 0, 0xA0000);	// 10.0
	fixedLMSetValue(&v2, 1, 0, 0x78000);	//  7.5
	fixedLMClearFlag(&v2, FLM_FLAG_TRANSPOSED);
	fixedLMPrintError();

	flmretrieve_t dot = 0;
	fixedLMDotVectorCol(&v1, &v2, FIXED32_T, &dot);
	printf("Dot product: %lf\n", convertF32ToDouble(dot));

	fixedLMCrossCol3d(&v1, &v2, &v3);
	fixedLMPrintError();

	fixedLMPrintMatrix(&v1);
	fixedLMPrintMatrix(&v2);
	fixedLMPrintMatrix(&v3);

	fixedLMTransposeDeep(&v3);
	fixedLMPrintMatrix(&v3);
}

void virtualTranspose(){
	FLM_CLEAR_ERROR();
	fixed32_t data4[6];
	flmmat_t m4;
	fixedLMCreateMatrix(&m4, 2, 3, FIXED32_T, data4);
	
	for(uint8_t i = 0; i < 6; i++){
		fixedLMSetValue(&m4, i % 2, i / 2, 0x10000 * i);
	}

	fixedLMPrintMatrix(&m4);

	fixedLMTransposeVirtual(&m4);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m4);

	fixedLMRowSwap(&m4, 0, 1);
	fixedLMPrintError();
	fixedLMPrintMatrix(&m4);
}


#define TEST_DIM 4

void LUDecomp(){
	fixed32_t dataA[TEST_DIM * TEST_DIM];
	fixed32_t dataL[TEST_DIM * TEST_DIM];
	fixed32_t dataU[TEST_DIM * TEST_DIM];
	flmmat_t a, l, u;
	
	fixedLMCreateMatrix(&a, TEST_DIM, TEST_DIM, FIXED32_T, dataA);
	fixedLMCreateMatrix(&l, TEST_DIM, TEST_DIM, FIXED32_T, dataL);
	fixedLMCreateMatrix(&u, TEST_DIM, TEST_DIM, FIXED32_T, dataU);

	fixedLMEye(&a);
	fixedLMSetValue(&a, 0, 0, 0x30000);		//  1.0
	fixedLMSetValue(&a, 1, 0, 0x30000);		//  3.0
	fixedLMSetValue(&a, 2, 0, 0x50000);		//  5.0
	fixedLMSetValue(&a, 3, 0, 0x90000);		//  9.0
	fixedLMSetValue(&a, 0, 1, 0X10000);		//  1.0
	fixedLMSetValue(&a, 1, 1, 0x30000);		//  3.0
	fixedLMSetValue(&a, 2, 1, 0x10000);		//  1.0
	fixedLMSetValue(&a, 3, 1, 0x70000);		//  7.0
	fixedLMSetValue(&a, 0, 2, 0x40000);		//  4.0
	fixedLMSetValue(&a, 1, 2, 0x30000);		//  3.0
	fixedLMSetValue(&a, 2, 2, 0x90000);		//  9.0
	fixedLMSetValue(&a, 3, 2, 0x70000);		//  7.0
	fixedLMSetValue(&a, 0, 3, 0x50000);		//  5.0
	fixedLMSetValue(&a, 1, 3, 0x20000);		//  2.0
	fixedLMSetValue(&a, 2, 3, 0x00000);		//  0.0
	fixedLMSetValue(&a, 3, 3, 0x90000);		//  9.0

	fixedLMPrintError();
	fixedLMPrintMatrix(&a);

	fixedLMSetFlag(&u, FLM_FLAG_READONLY);

	flmretrieve_t det4x4;
	flmretrieve_t detLU;
	fixedLMDeterminantLU(&a, &l, &u, FIXED64_T, &detLU);
	fixedLMPrintError();
	fixedLMDeterminant4x4(&a, FIXED64_T, &det4x4);
	fixedLMPrintError();
	printf("Det 4x4: %lf\n", convertF64ToDouble(det4x4));
	printf("Det LU: %lf\n", convertF64ToDouble(detLU));

	flmmat_t view;
	fixedLMCreateView(&view, &a, 1, 1, 3, 2);
	fixedLMPrintMatrix(&view);
	fixedLMPrintError();

	fixedLMSetFlag(&view, FLM_FLAG_TRANSPOSED);
	//fixedLMClearFlag(&view, FLM_FLAG_VIEW);
	fixedLMPrintMatrix(&view);
	fixedLMPrintError();

}

int main(){
	//creationAndBasiOperations();
	//basic_arithmetic();
	//matmul_test();
	//gaussJordan();

	//dotCrossProduct();
	//virtualTranspose();

	LUDecomp();
	return 0;
}
