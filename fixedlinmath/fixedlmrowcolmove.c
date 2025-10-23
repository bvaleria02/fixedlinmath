#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMRowSwap(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(rowSrc >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowDest >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		
		code = fixedLMSetValue(mat, i, rowSrc, temp2);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		code = fixedLMSetValue(mat, i, rowDest, temp1);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColSwap(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(colSrc >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(colDest >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		
		code = fixedLMSetValue(mat, colSrc , i, temp2);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		code = fixedLMSetValue(mat, colDest, i, temp1);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMValueSwap(flmmat_t *matSrc, flmmat_t *matDest, flmdim_t colSrc, flmdim_t rowSrc, flmdim_t colDest, flmdim_t rowDest){
	if(matSrc == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(matDest == NULL){
		matDest = matSrc;
	}

	flmdim_t heightSrc = fixedLMGetHeight(matSrc);
	flmdim_t widthSrc = fixedLMGetWidth(matSrc);
	flmdim_t heightDest = fixedLMGetHeight(matDest);
	flmdim_t widthDest = fixedLMGetWidth(matDest);

	if(matSrc->isSet == FLM_MATRIX_UNSET || matDest->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(colSrc >= widthSrc || colDest >= widthDest){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowSrc >= heightSrc || rowDest >= heightDest){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	temp1 = fixedLMRetrieveValue(matSrc,  colSrc,  rowSrc);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(code);

	temp2 = fixedLMRetrieveValue(matDest, colDest, rowDest);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(code);
		
	code = fixedLMSetValue(matSrc,  colSrc,  rowSrc,  temp2);
	if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(code);
	code = fixedLMSetValue(matDest, colDest, rowDest, temp1);
	if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(code);

	return FLM_NO_ERROR;
}
