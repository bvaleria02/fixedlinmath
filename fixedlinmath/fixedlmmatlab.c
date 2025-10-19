#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMFill(flmmat_t *mat, flmretrieve_t value){
	if(mat == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	FLMErrorCode code = FLM_NO_ERROR;

	for(flmdim_t i = 0; i < mat->width; i++){
		for(flmdim_t j = 0; j < mat->height; j++){
			code = fixedLMSetValue(mat, i, j, value);
			if(code != FLM_NO_ERROR){
				return code;
			}
		}
	}

	return code;
}

FLMErrorCode fixedLMZeros(flmmat_t *mat){
	FLMErrorCode code = FLM_NO_ERROR;

	fixedLMClearErrno();
	flmretrieve_t zeroVal = getZeroValueByType(mat->type);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR){
		return code;
	}
	
	code = fixedLMFill(mat, zeroVal);
	return code;
}

FLMErrorCode fixedLMOnes(flmmat_t *mat){
	FLMErrorCode code = FLM_NO_ERROR;

	fixedLMClearErrno();
	flmretrieve_t oneVal = getOneValueByType(mat->type);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR){
		return code;
	}
	
	code = fixedLMFill(mat, oneVal);
	return code;
}

FLMErrorCode fixedLMEye(flmmat_t *mat){
	if(mat == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(mat->width != mat->height){
		fixedLMSetErrno(FLM_ERROR_RECTANGULAR);
		return FLM_ERROR_RECTANGULAR;
	}
	
	FLMErrorCode code = FLM_NO_ERROR;

	code = fixedLMFill(mat, 0);
	if(code != FLM_NO_ERROR){
		return code;
	}

	flmretrieve_t oneVal = getOneValueByType(mat->type);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR){
		return code;
	}

	for(flmdim_t i = 0; i < mat->width; i++){
		code = fixedLMSetValue(mat, i, i, oneVal);
		if(code != FLM_NO_ERROR){
			return code;
		}
	}

	return FLM_NO_ERROR;
}
