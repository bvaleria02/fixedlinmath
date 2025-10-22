#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMCrossRow3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL || m3 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(m1->width != m2->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->height != m2->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->height != 1 || m2->height != 1 || m3->height != 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);
	}

	if(m1->width != 3 || m2->width != 3 || m3->width != 3){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t temp4;
	flmretrieve_t acc;

	// m1 = [a1, b1, c1]
	// m2 = [a2, b2, c2]
	// m3 = [a3, b3, c3]

	// a3 = b1*c2 - c1*b2
	// b3 = c1*a2 - c2*a1
	// c3 = a1*b2 - a2*b1


	/*
		a3 = b1*c2 - c1*b2
		temp1 = b1
		temp2 = c2
		temp3 = c1
		temp4 = b2
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 1, 0);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 2, 0);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m1, 2, 0);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m2, 1, 0);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 0, 0, acc);
	if(code != FLM_NO_ERROR) return code;

	/*
		b3 = c1*a2 - c2*a1
		temp1 = c1
		temp2 = a2
		temp3 = c2
		temp4 = a1
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 2, 0);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 0);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m2, 2, 0);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 0);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 1, 0, acc);
	if(code != FLM_NO_ERROR) return code;

	/*
		c3 = a1*b2 - a2*b1
		temp1 = a1
		temp2 = b2
		temp3 = a2
		temp4 = b1
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 0, 0);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 1, 0);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 0);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m1, 1, 0);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 2, 0, acc);
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMCrossCol3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL || m3 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(m1->width != m2->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->height != m2->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(m1->width != 1 || m2->width != 1 || m3->width != 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);
	}

	if(m1->height != 3 || m2->height != 3 || m3->height != 3){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t temp4;
	flmretrieve_t acc;

	// m1 = [a1, b1, c1]
	// m2 = [a2, b2, c2]
	// m3 = [a3, b3, c3]

	// a3 = b1*c2 - c1*b2
	// b3 = c1*a2 - c2*a1
	// c3 = a1*b2 - a2*b1


	/*
		a3 = b1*c2 - c1*b2
		temp1 = b1
		temp2 = c2
		temp3 = c1
		temp4 = b2
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 0, 1);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 2);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m1, 0, 2);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m2, 0, 1);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 0, 0, acc);
	if(code != FLM_NO_ERROR) return code;

	/*
		b3 = c1*a2 - c2*a1
		temp1 = c1
		temp2 = a2
		temp3 = c2
		temp4 = a1
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 0, 2);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 0);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 2);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 0);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 0, 1, acc);
	if(code != FLM_NO_ERROR) return code;

	/*
		c3 = a1*b2 - a2*b1
		temp1 = a1
		temp2 = b2
		temp3 = a2
		temp4 = b1
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 0, 0);
	temp1 = typeAbstractValueConverterIn(m1->type, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 1);
	temp2 = typeAbstractValueConverterIn(m2->type, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 0);
	temp3 = typeAbstractValueConverterIn(m2->type, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 1);
	temp4 = typeAbstractValueConverterIn(m2->type, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(m3->type, acc);
	code  = fixedLMSetValue(m3, 0, 2, acc);
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}
