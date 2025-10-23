#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMTransposeDeep(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	flmmat_t proxy;
	proxy.isSet = 0;
	FLMErrorCode code;

	code = fixedLMCreateMatrix(&proxy, height, width, mat->type, mat->data);
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < height; i++){
		for(flmdim_t j = i; j < width; j++){
			if(i == j) continue;

			code = fixedLMValueSwap(
						/* matSrc */ 	mat,
						/* matDest */ 	&proxy,
						/* colSrc */ 	i,
						/* rowSrc */	j,
						/* colDest */	j,
						/* rowDest */   i
					);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	mat->height = proxy.height;
	mat->width	= proxy.width;

	return FLM_NO_ERROR;
}

// Legacy use, to be deprecated.
FLMErrorCode fixedLMTranspose(flmmat_t *mat){
	return fixedLMTransposeDeep(mat);
}

FLMErrorCode fixedLMTransposeVirtual(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	mat->isTransposed = !(mat->isTransposed);

	return FLM_NO_ERROR;
}
