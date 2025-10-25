//#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMCreateMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type, void *data){
	HANDLE_INVALID_MATRIX(mat);

	mat->width 	= width;
	mat->height = height;
	mat->data	= data;
	mat->config	= fixedLMSetType(mat, type);

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMAllocMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type){
	HANDLE_INVALID_MATRIX(mat);
	
	uint8_t dataSize = getSizeofDataType(type);
	void *data = malloc(width * height * dataSize);
	if(data == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MALLOC);
	}

	FLMErrorCode code = fixedLMCreateMatrix(mat, width, height, type, data);
	if(code != FLM_NO_ERROR){
		goto cleanup;
	}

	fixedLMSetFlag(mat, FLM_FLAG_DATAALLOCATED);
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
	mat->config = 0x0;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDestroyMatrix(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->data == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(!fixedLMIsDataAllocated(mat)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTHEAPMEMORY);
	}
	
	free(mat->data);
	fixedLMResetMatrix(mat);

	return FLM_NO_ERROR;
}
