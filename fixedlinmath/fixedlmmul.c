#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMMul(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL || m3 == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(m1->width != m2->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(m2->width != m3->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(m1->height != m3->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
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
				if(code != FLM_NO_ERROR) return code;

				x2 = fixedLMRetrieveValue(m2, j, k);
				x2 = typeAbstractValueConverterIn(m2->type, x2);
				code = fixedLMGetErrno();
				if(code != FLM_NO_ERROR) return code;

				x3  = fixedMul64(x1, x2);
				acc = fixedAdd64(acc, x3);
			}

			x3 = typeAbstractValueConverterOut(m3->type, acc);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR){
				return code;
			}
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMMulScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	if(m1 == NULL){
		fixedLMSetErrno(FLM_ERROR_NULLPTR);
		return FLM_ERROR_NULLPTR;
	}

	// m2 is the output matrix. If m3 is NULL, it uses m1 as the output
	if(m2 == NULL){
		m2 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET){
		fixedLMSetErrno(FLM_ERROR_MATRIXUNSET);
		return FLM_ERROR_MATRIXUNSET;
	}

	if(m1->width != m2->width){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	if(m1->height != m2->height){
		fixedLMSetErrno(FLM_ERROR_DIMENSION);
		return FLM_ERROR_DIMENSION;
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x3;
	flmretrieve_t x2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	for(flmdim_t i = 0; i < m1->height; i++){
		for(flmdim_t j = 0; j < m1->width; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) return code;

			x3 = fixedMul64(x1, x2);
			x3 = typeAbstractValueConverterOut(m2->type, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR){
				return code;
			}
		}
	}

	return FLM_NO_ERROR;
}
