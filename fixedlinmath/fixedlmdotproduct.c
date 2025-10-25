#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMDotElementWise(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m2 == NULL){
		m2 = m1;
	}

	if(m3 == NULL){
		m3 = m1;
	}

	HANDLE_INVALID_MATRIX(m1);
	HANDLE_INVALID_MATRIX(m2);
	HANDLE_INVALID_MATRIX(m3);

	flmdim_t height1, width1;
	GET_DIMENSIONS_MATRIX(m1, width1, height1);

	flmdim_t height2, width2;
	GET_DIMENSIONS_MATRIX(m2, width2, height2);

	flmdim_t height3, width3;
	GET_DIMENSIONS_MATRIX(m3, width3, height3);

	HANDLE_NONMATCHING_MATRIX(width1, height1, width2, height2);
	HANDLE_NONMATCHING_MATRIX(width1, height1, width3, height3);

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);
	flmtype_t type3 = fixedLMGetType(m3);

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			FLM_CLEAR_ERROR();
			temp1 = fixedLMRetrieveValue(m1, j, i);
			temp1 = typeAbstractValueConverterIn(type1, temp1);

			temp2 = fixedLMRetrieveValue(m2, j, i);
			temp2 = typeAbstractValueConverterIn(type2, temp2);

			temp3 = fixedMul64(temp1, temp2);
			temp3 = typeAbstractValueConverterOut(type3, temp3);
			code  = fixedLMSetValue(m3, j, i, temp3);
			if(code != FLM_NO_ERROR) return code;
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDotVectorRow(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t *value){
	if(m2 == NULL){
		m2 = m1;
	}

	HANDLE_INVALID_MATRIX(m1);
	HANDLE_INVALID_MATRIX(m2);

	flmdim_t height1, width1;
	GET_DIMENSIONS_MATRIX(m1, width1, height1);

	flmdim_t height2, width2;
	GET_DIMENSIONS_MATRIX(m2, width2, height2);

	HANDLE_NONMATCHING_MATRIX(width1, height1, width2, height2);

	HANDLE_NONROWVECT_MATRIX(height1);
	HANDLE_NONROWVECT_MATRIX(height2);

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t acc   = 0;

	for(flmdim_t i = 0; i < width1; i++){
		FLM_CLEAR_ERROR();

		temp1 = fixedLMRetrieveValue(m1, i, 0);
		temp1 = typeAbstractValueConverterIn(type1, temp1);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp2 = fixedLMRetrieveValue(m2, i, 0);
		temp2 = typeAbstractValueConverterIn(type2, temp2);
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
	if(m2 == NULL){
		m2 = m1;
	}

	HANDLE_INVALID_MATRIX(m1);
	HANDLE_INVALID_MATRIX(m2);

	flmdim_t height1, width1;
	GET_DIMENSIONS_MATRIX(m1, width1, height1);

	flmdim_t height2, width2;
	GET_DIMENSIONS_MATRIX(m2, width2, height2);

	HANDLE_NONMATCHING_MATRIX(width1, height1, width2, height2);

	HANDLE_NONCOLVECT_MATRIX(width1);
	HANDLE_NONCOLVECT_MATRIX(width2);

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);

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
		temp1 = typeAbstractValueConverterIn(type1, temp1);
		code = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) return code;

		temp2 = fixedLMRetrieveValue(m2, 0, i);
		temp2 = typeAbstractValueConverterIn(type2, temp2);
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


