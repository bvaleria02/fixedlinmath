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

	return FLM_NO_ERROR;
}
