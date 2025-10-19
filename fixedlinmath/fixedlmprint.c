#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include "fixedlmprint.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void fixedLMPrintMatrix(flmmat_t *mat){
	if(mat == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return;
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
