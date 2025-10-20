#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMMul(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL || m3 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(m1->width != m2->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m2->width != m3->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->height != m3->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x2;
	flmretrieve_t acc;
	flmretrieve_t x3;

	for(flmdim_t i = 0; i < m1->height; i++){
		for(flmdim_t j = 0; j < m2->width; j++){
			
			acc = F64_ZERO;

			for(flmdim_t k = 0; k < m1->width; k++){
				//printf("i: %i\tj: %i\tk: %i\n", i, j, k);
				x1 = fixedLMRetrieveValue(m1, k, i);
				x1 = typeAbstractValueConverterIn(m1->type, x1);
				code = fixedLMGetErrno();
				if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

				x2 = fixedLMRetrieveValue(m2, j, k);
				x2 = typeAbstractValueConverterIn(m2->type, x2);
				code = fixedLMGetErrno();
				if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

				x3  = fixedMul64(x1, x2);
				acc = fixedAdd64(acc, x3);
			}

			x3 = typeAbstractValueConverterOut(m3->type, acc);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMMulScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	if(m1 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	// m2 is the output matrix. If m3 is NULL, it uses m1 as the output
	if(m2 == NULL){
		m2 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(m1->width != m2->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->height != m2->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x3;
	flmretrieve_t x2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < m1->height; i++){
		for(flmdim_t j = 0; j < m1->width; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedMul64(x1, x2);
			x3 = typeAbstractValueConverterOut(m2->type, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowMulScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(row >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	x2 = typeAbstractValueConverterIn(type, value);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < mat->width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, row);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedMul64(temp1, x2);
		temp2 = typeAbstractValueConverterOut(mat->type, temp2);

		code = fixedLMSetValue(mat, i, row, temp2);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColMulScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(col >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	x2 = typeAbstractValueConverterIn(type, value);
	code  = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedMul64(temp1, x2);
		temp2 = typeAbstractValueConverterOut(mat->type, temp2);

		code = fixedLMSetValue(mat, col, i, temp2);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}
