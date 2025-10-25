#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t getElementMatrix(flmmat_t *mat, flmdim_t col,flmdim_t row){
	if(fixedLMIsTransposed(mat)){
		return (col * mat->width) + row;
	}
	return (row * mat->width) + col;
}

flmdim_t fixedLMGetWidth(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_DIM_ERROR);
	}

	if(fixedLMIsTransposed(mat)){
		return mat->height;
	}

	return mat->width;
}

flmdim_t fixedLMGetHeight(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_DIM_ERROR);
	}

	if(fixedLMIsTransposed(mat)){
		return mat->width;
	}

	return mat->height;
}


flmretrieve_t fixedLMRetrieveValue(flmmat_t *mat, flmdim_t col, flmdim_t row){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t width, height;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width,  col);
	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	uint32_t element = getElementMatrix(mat, col, row);
	flmretrieve_t value = 0;
	//printf("trans: %i\tcol: %i\trow: %i\tIndex: %i\n", mat->isTransposed, col, row, element);

	FLM_TYPE_EXEC(typeMat, {
		value = ((flmdtype_t *)mat->data)[element];
	}, {
		value = FLM_ERROR_VALUE;	
	});

	return value;
}

FLMErrorCode fixedLMSetValue(flmmat_t *mat, flmdim_t col, flmdim_t row, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t width, height;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width,  col);
	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	if(fixedLMIsReadOnly(mat)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_READONLY);
	}

	uint32_t element = getElementMatrix(mat, col, row);

	FLM_TYPE_EXEC(typeMat, {
		((flmdtype_t *)mat->data)[element] = (flmdtype_t)value;
	}, {
		return FLM_ERROR_TYPE;
	});

	return FLM_NO_ERROR;
}
