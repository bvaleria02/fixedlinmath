#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMDivScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	if(value == 0){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIVZERO);
	}

	flmretrieve_t y = getReciprocalByType(type, value);
	return fixedLMMulScalar(m1, m2, type, y);
}

FLMErrorCode fixedLMRowDivScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
	if(value == 0){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIVZERO);
	}

	flmretrieve_t y = getReciprocalByType(type, value);
	return fixedLMRowMulScalar(mat, row, type, y);
}

FLMErrorCode fixedLMColDivScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
	if(value == 0){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIVZERO);
	}

	flmretrieve_t y = getReciprocalByType(type, value);
	return fixedLMColMulScalar(mat, col, type, y);
}
