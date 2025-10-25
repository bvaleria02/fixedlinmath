//#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMCreateMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type, void *data){
	HANDLE_INVALID_MATRIX(mat);

	HANDLE_ZERODIM_MATRIX(width);
	HANDLE_ZERODIM_MATRIX(height);

	mat->width 	= width;
	mat->height = height;
	mat->data	= data;
	mat->config	= fixedLMSetType(mat, type);

	FLMErrorCode code = fixedLMConfigureView(mat, 0, 0, width, height);
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMAllocMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type){
	HANDLE_INVALID_MATRIX(mat);

	HANDLE_ZERODIM_MATRIX(width);
	HANDLE_ZERODIM_MATRIX(height);
	
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

FLMErrorCode fixedLMConfigureView(flmmat_t *mat, flmdim_t colOffset, flmdim_t rowOffset, flmdim_t viewWidth, flmdim_t viewHeight){
	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	printf("width: %i\tcolOffset: %i\tviewWidth: %i\n", width, colOffset, viewWidth);

	HANDLE_ZERODIM_MATRIX(viewWidth);
	HANDLE_ZERODIM_MATRIX(viewHeight);

	HANDLE_DIMENSIONS_MATRIX(width, colOffset);
	HANDLE_DIMENSIONS_MATRIX(height, rowOffset);

	HANDLE_DIMENSIONS_MATRIX(width,  colOffset + viewWidth - 1);
	HANDLE_DIMENSIONS_MATRIX(height, rowOffset + viewHeight - 1);

	mat->viewDetails.colOffset  = colOffset;
	mat->viewDetails.rowOffset  = rowOffset;
	mat->viewDetails.viewWidth  = viewWidth;
	mat->viewDetails.viewHeight = viewHeight;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMCreateView(flmmat_t *child, flmmat_t *parent, flmdim_t colOffset, flmdim_t rowOffset, flmdim_t viewWidth, flmdim_t viewHeight){
	HANDLE_INVALID_MATRIX(parent);
	HANDLE_INVALID_MATRIX(child);

	child->width  = parent->width;
	child->height = parent->height;
	child->data	  = parent->data;
	child->config = parent->config;

	FLMErrorCode code;
	code = fixedLMConfigureView(child, colOffset, rowOffset, viewWidth, viewHeight);
	if(code != FLM_NO_ERROR) return code;

	code = fixedLMSetFlag(child, FLM_FLAG_VIEW | FLM_FLAG_READONLY);
	if(code != FLM_NO_ERROR) return code;


	return FLM_NO_ERROR;
}
