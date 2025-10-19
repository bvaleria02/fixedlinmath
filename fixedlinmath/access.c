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
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_VALUE;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_VALUE;
	}

	if(x >= mat->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_VALUE;
	}

	if(y >= mat->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_VALUE;
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
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(x >= mat->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(y >= mat->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	uint32_t element = getElementMatrix(mat, x, y);
/*
	switch(mat->type){
		case FIXED32_T:		((fixed32_t *) mat->data)[element] = (fixed32_t) value;
							break;
		case UFIXED32_T:	((ufixed32_t *) mat->data)[element] = (ufixed32_t) value;
							break;
		case FIXED64_T:		((fixed64_t *) mat->data)[element] = (fixed64_t) value;
							break;
		case UFIXED64_T:	((ufixed64_t *) mat->data)[element] = (ufixed64_t) value;
							break;
		default:			fixedLMSetErrno(FLM_ERROR_TYPE);
							return FLM_ERROR_TYPE;
	}
*/

	FLM_TYPE_EXEC(mat->type, {
		((flmdtype_t *)mat->data)[element] = (flmdtype_t)value;
	}, {
		return FLM_ERROR_TYPE;
	});

	return FLM_NO_ERROR;
}
