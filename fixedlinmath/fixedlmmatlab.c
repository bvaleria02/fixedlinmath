#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMFill(flmmat_t *mat, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	FLMErrorCode code = FLM_NO_ERROR;

	for(flmdim_t i = 0; i < width; i++){
		for(flmdim_t j = 0; j < height; j++){
			code = fixedLMSetValue(mat, i, j, value);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return code;
}

FLMErrorCode fixedLMZeros(flmmat_t *mat){
	FLMErrorCode code = FLM_NO_ERROR;
	flmtype_t typeMat = fixedLMGetType(mat);

	fixedLMClearErrno();
	flmretrieve_t zeroVal = getZeroValueByType(typeMat);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	
	code = fixedLMFill(mat, zeroVal);
	return code;
}

FLMErrorCode fixedLMOnes(flmmat_t *mat){
	FLMErrorCode code = FLM_NO_ERROR;
	flmtype_t typeMat = fixedLMGetType(mat);

	fixedLMClearErrno();
	flmretrieve_t oneVal = getOneValueByType(typeMat);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	
	code = fixedLMFill(mat, oneVal);
	return code;
}

FLMErrorCode fixedLMEye(flmmat_t *mat){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	HANDLE_RECTANGLE_MATRIX(mat, width, height);

	flmtype_t typeMat = fixedLMGetType(mat);
	
	FLMErrorCode code = FLM_NO_ERROR;

	code = fixedLMFill(mat, 0);
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	flmretrieve_t oneVal = getOneValueByType(typeMat);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < width; i++){
		code = fixedLMSetValue(mat, i, i, oneVal);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}
