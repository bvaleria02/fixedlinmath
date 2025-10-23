//#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMCreateMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type, void *data){
	if(mat == NULL || data == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_SET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXSET);
	}

	mat->width 	= width;
	mat->height = height;
	mat->type	= type;
	mat->data	= data;
	mat->isSet	= FLM_MATRIX_SET;
	mat->isDataAllocated = FLM_DATA_MANUAL;
	mat->isTransposed = FLM_MATRIX_NOTTRANSPOSED;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMAllocMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_SET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXSET);
	}
	
	uint8_t dataSize = getSizeofDataType(type);
	void *data = malloc(width * height * dataSize);
	if(data == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MALLOC);
	}

	FLMErrorCode code = fixedLMCreateMatrix(mat, width, height, type, data);
	if(code != FLM_NO_ERROR){
		goto cleanup;
	}

	mat->isDataAllocated = FLM_DATA_ALLOCATED;
	return FLM_NO_ERROR;

cleanup:
	if(data != NULL){
		free(data);
	}

	mat->data = NULL;
	return code;
}

FLMErrorCode fixedLMResetMatrix(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	mat->data = NULL;
	mat->width = 0;
	mat->height = 0;
	mat->isSet = FLM_MATRIX_UNSET;
	mat->isDataAllocated = FLM_DATA_MANUAL;
	mat->isTransposed = FLM_MATRIX_NOTTRANSPOSED;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDestroyMatrix(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->data == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isDataAllocated == FLM_DATA_MANUAL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTHEAPMEMORY);
	}
	
	free(mat->data);
	fixedLMResetMatrix(mat);

	return FLM_NO_ERROR;
}
