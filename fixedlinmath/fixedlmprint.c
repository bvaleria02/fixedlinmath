#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include "fixedlmprint.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void fixedLMPrintMatrix(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, );
		return;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_MATRIXUNSET, );
	}

	flmretrieve_t value;
	double valueF;

	printf("Columns: %i\tRows: %i\n", mat->width, mat->height);

	for(flmdim_t i = 0; i < mat->height; i++){
		printf("Row: %i:\t", i+1);

		for(flmdim_t j = 0; j < mat->width; j++){
			value = fixedLMRetrieveValue(mat, j, i);

			FLM_TYPE_EXEC_SPLIT(mat->type, {
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
