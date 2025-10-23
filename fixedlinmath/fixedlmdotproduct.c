#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMDotElementWise(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m2 == NULL){
		m2 = m1;
	}

	if(m3 == NULL){
		m3 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height1 = fixedLMGetHeight(m1);
	flmdim_t width1 = fixedLMGetWidth(m1);
	flmdim_t height2 = fixedLMGetHeight(m2);
	flmdim_t width2 = fixedLMGetWidth(m2);
	flmdim_t height3 = fixedLMGetHeight(m3);
	flmdim_t width3 = fixedLMGetWidth(m3);

	if((width1 != width2) || (width1 != width3)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if((height1 != height2) || (height1 != height3)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			FLM_CLEAR_ERROR();
			temp1 = fixedLMRetrieveValue(m1, j, i);
			temp1 = typeAbstractValueConverterIn(m1->type, temp1);

			temp2 = fixedLMRetrieveValue(m2, j, i);
			temp2 = typeAbstractValueConverterIn(m2->type, temp2);

			temp3 = fixedMul64(temp1, temp2);
			temp3 = typeAbstractValueConverterOut(m3->type, temp3);
			code  = fixedLMSetValue(m3, j, i, temp3);
			if(code != FLM_NO_ERROR) return code;
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDotVectorRow(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t *value){
	if(m1 == NULL || value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m2 == NULL){
		m2 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height1 = fixedLMGetHeight(m1);
	flmdim_t width1 = fixedLMGetWidth(m1);
	flmdim_t height2 = fixedLMGetHeight(m2);
	flmdim_t width2 = fixedLMGetWidth(m2);

	if(width1 != width2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(height1 != height2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(height1 != 1 || height2 != 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);
	}

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t acc   = 0;

	for(flmdim_t i = 0; i < width1; i++){
		FLM_CLEAR_ERROR();

		temp1 = fixedLMRetrieveValue(m1, i, 0);
		temp1 = typeAbstractValueConverterIn(m1->type, temp1);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp2 = fixedLMRetrieveValue(m2, i, 0);
		temp2 = typeAbstractValueConverterIn(m2->type, temp2);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp3 = fixedMul64(temp1, temp2);
		acc   = fixedAdd64(acc, temp3);
	}

	(*value) = typeAbstractValueConverterOut(type, acc);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDotVectorCol(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t *value){
	if(m1 == NULL || value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m2 == NULL){
		m2 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height1 = fixedLMGetHeight(m1);
	flmdim_t width1 = fixedLMGetWidth(m1);
	flmdim_t height2 = fixedLMGetHeight(m2);
	flmdim_t width2 = fixedLMGetWidth(m2);

	if(width1 != width2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(height1 != height2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(width1 != 1 || width2 != 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);
	}

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t acc   = 0;

	for(flmdim_t i = 0; i < height1; i++){
		FLM_CLEAR_ERROR();

		temp1 = fixedLMRetrieveValue(m1, 0, i);
		temp1 = typeAbstractValueConverterIn(m1->type, temp1);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp2 = fixedLMRetrieveValue(m2, 0, i);
		temp2 = typeAbstractValueConverterIn(m2->type, temp2);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp3 = fixedMul64(temp1, temp2);
		acc   = fixedAdd64(acc, temp3);
	}

	(*value) = typeAbstractValueConverterOut(type, acc);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}


