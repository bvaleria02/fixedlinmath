#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMRowSwap(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(height, rowSrc);
	HANDLE_DIMENSIONS_MATRIX(height, rowDest);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;

	for(flmdim_t i = 0; i < width; i++){
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
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width, colSrc);
	HANDLE_DIMENSIONS_MATRIX(width, colDest);

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
	if(matDest == NULL){
		matDest = matSrc;
	}

	HANDLE_INVALID_MATRIX(matSrc);
	HANDLE_INVALID_MATRIX(matDest);

	flmdim_t heightSrc, widthSrc;
	GET_DIMENSIONS_MATRIX(matSrc, widthSrc, heightSrc);

	flmdim_t heightDest, widthDest;
	GET_DIMENSIONS_MATRIX(matDest, widthDest, heightDest);

	HANDLE_DIMENSIONS_MATRIX(widthSrc,   colSrc);
	HANDLE_DIMENSIONS_MATRIX(widthDest,  colDest);
	HANDLE_DIMENSIONS_MATRIX(heightSrc,  rowSrc);
	HANDLE_DIMENSIONS_MATRIX(heightDest, rowDest);

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
