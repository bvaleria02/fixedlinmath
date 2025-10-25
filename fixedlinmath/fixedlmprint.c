#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include "fixedlmprint.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void fixedLMPrintMatrix(flmmat_t *mat){
	HANDLE_INVALID_MATRIX_VOID(mat);

	flmretrieve_t value;
	double valueF;

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);
	flmtype_t typeMat = fixedLMGetType(mat);

	printf("Columns: %i\tRows: %i\n",width, height);

	for(flmdim_t i = 0; i < height; i++){
		printf("Row: %i:\t", i+1);

		for(flmdim_t j = 0; j < width; j++){
			value = fixedLMRetrieveValue(mat, j, i);

			FLM_TYPE_EXEC_SPLIT(typeMat, {
				valueF = convertF32ToDouble(value);
			}, {
				valueF = convertUF32ToDouble(value);
			}, {
				valueF = convertF64ToDouble(value);
			}, {
				valueF = convertUF64ToDouble(value);
			}, {
				valueF = 0;
			});

			printf("%10.4le\t", valueF);
		}
		printf("\n");
	}
}

FLMErrnoString errnoStringConsts[FLMERRNOSTRING_SIZE] = {
	"Void pointer passed as argument where non-void value is expected.",
	"Dimensions passed to matrix are too big for it.",
	"The matrix has already been set, can't re-initialize.",
	"Matrix must be set (initialized) before it can be used.",
	"Unknown type",
	"Expected square matrix (MxM), but rectangular was passed (MxN).",
	"Expected value not found on current matrix.",
	"The matrix isn't invertible.",
	"Attempted a division by zero.",
	"Error allocating heap memory (malloc)",
	"The matrix data is static, not allocated in heap",
	"The matrix is not a vector (Nx1)",
	"Trying to change a value in a matrix marked as \"Read Only\"",
	"Passed width=0 or height=0 to create a matrix or a view",
};

FLMErrnoString fixedLMGetErrnoString(FLMErrorCode code){
	if(code == 0){
		return "No error\n";
	}

	return errnoStringConsts[code - 1];
}


void fixedLMPrintErrorCode(FLMErrorCode code){
	if(code == FLM_NO_ERROR){
		printf("No errors found :)\n");
		return;
	}

	FLMErrnoString errnoString = fixedLMGetErrnoString(code);
	printf("[ERROR] %s at <%s> in file <%s> at line %i\n", errnoString, flm_funcname, flm_filename, flm_linenumber);
}


void fixedLMPrintError(){
	FLMErrorCode code = fixedLMGetErrno();
	return fixedLMPrintErrorCode(code);
}
