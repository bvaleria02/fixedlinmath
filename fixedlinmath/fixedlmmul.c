#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMMul(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	HANDLE_INVALID_MATRIX(m1);
	HANDLE_INVALID_MATRIX(m2);
	HANDLE_INVALID_MATRIX(m3);

	flmdim_t height1, width1;
	GET_DIMENSIONS_MATRIX(m1, width1, height1);

	flmdim_t height2, width2;
	GET_DIMENSIONS_MATRIX(m2, width2, height2);

	flmdim_t height3, width3;
	GET_DIMENSIONS_MATRIX(m3, width3, height3);


	if(width1 != height2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(width2 != width3){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(height1 != height3){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);
	flmtype_t type3 = fixedLMGetType(m3);

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x2;
	flmretrieve_t acc;
	flmretrieve_t x3;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width2; j++){
			
			acc = F64_ZERO;

			for(flmdim_t k = 0; k < width1; k++){
				//printf("i: %i\tj: %i\tk: %i\n", i, j, k);
				x1 = fixedLMRetrieveValue(m1, k, i);
				x1 = typeAbstractValueConverterIn(type1, x1);
				code = fixedLMGetErrno();
				if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

				x2 = fixedLMRetrieveValue(m2, j, k);
				x2 = typeAbstractValueConverterIn(type2, x2);
				code = fixedLMGetErrno();
				if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

				x3  = fixedMul64(x1, x2);
				acc = fixedAdd64(acc, x3);
			}

			x3 = typeAbstractValueConverterOut(type3, acc);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMMulScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	// m2 is the output matrix. If m3 is NULL, it uses m1 as the output
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

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x3;
	flmretrieve_t x2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(type1, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedMul64(x1, x2);
			x3 = typeAbstractValueConverterOut(type2, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowMulScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	x2 = typeAbstractValueConverterIn(type, value);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, row);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedMul64(temp1, x2);
		temp2 = typeAbstractValueConverterOut(typeMat, temp2);

		code = fixedLMSetValue(mat, i, row, temp2);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColMulScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width, col);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	x2 = typeAbstractValueConverterIn(type, value);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedMul64(temp1, x2);
		temp2 = typeAbstractValueConverterOut(typeMat, temp2);

		code = fixedLMSetValue(mat, col, i, temp2);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}
