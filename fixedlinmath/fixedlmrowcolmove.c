#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMRowSwap(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	if(mat == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(rowSrc >= mat->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(rowDest >= mat->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	for(flmdim_t i = 0; i < mat->width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	return code;

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	return code;
		
		code = fixedLMSetValue(mat, i, rowSrc, temp2);
		if(code != FLM_NO_ERROR)	return code;
		code = fixedLMSetValue(mat, i, rowDest, temp1);
		if(code != FLM_NO_ERROR)	return code;
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColSwap(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest){
	if(mat == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(colSrc >= mat->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(colDest >= mat->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	for(flmdim_t i = 0; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	return code;

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	return code;
		
		code = fixedLMSetValue(mat, colSrc , i, temp2);
		if(code != FLM_NO_ERROR)	return code;
		code = fixedLMSetValue(mat, colDest, i, temp1);
		if(code != FLM_NO_ERROR)	return code;
	}

	return FLM_NO_ERROR;
}
