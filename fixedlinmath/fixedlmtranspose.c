#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMTranspose(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmmat_t proxy;
	FLMErrorCode code;

	code = fixedLMCreateMatrix(&proxy, mat->height, mat->width, mat->type, mat->data);
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < mat->height; i++){
		for(flmdim_t j = i; j < mat->width; j++){
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
