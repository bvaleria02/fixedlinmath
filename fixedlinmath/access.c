#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t getElementMatrix(flmmat_t *mat, flmdim_t x,flmdim_t y){
	return (y * mat->width) + x;
}

flmretrieve_t fixedLMRetrieveValue(flmmat_t *mat, flmdim_t x, flmdim_t y){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_ERROR_VALUE);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_MATRIXUNSET, FLM_ERROR_VALUE);
	}

	if(x >= mat->width){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_DIMENSION, FLM_ERROR_VALUE);
	}

	if(y >= mat->height){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_DIMENSION, FLM_ERROR_VALUE);
	}

	uint32_t element = getElementMatrix(mat, x, y);
	flmretrieve_t value = 0;

	FLM_TYPE_EXEC(mat->type, {
		value = ((flmdtype_t *)mat->data)[element];
	}, {
		value = FLM_ERROR_VALUE;	
	});

	return value;
}

FLMErrorCode fixedLMSetValue(flmmat_t *mat, flmdim_t x, flmdim_t y, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(x >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(y >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	uint32_t element = getElementMatrix(mat, x, y);

	FLM_TYPE_EXEC(mat->type, {
		((flmdtype_t *)mat->data)[element] = (flmdtype_t)value;
	}, {
		return FLM_ERROR_TYPE;
	});

	return FLM_NO_ERROR;
}
