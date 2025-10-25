#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMCrossRow3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
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

	HANDLE_EXACTSIZE_MATRIX(height1, width1, 1, 3);
	HANDLE_EXACTSIZE_MATRIX(height2, width2, 1, 3);
	HANDLE_EXACTSIZE_MATRIX(height3, width3, 1, 3);

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);
	flmtype_t type3 = fixedLMGetType(m3);

	FLMErrorCode code;
	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;
	flmretrieve_t temp4;
	flmretrieve_t acc;

	/*
		m1 = [a1, b1, c1]
		m2 = [a2, b2, c2]
		m3 = [a3, b3, c3]

		a3 = b1*c2 - c1*b2
		b3 = c1*a2 - c2*a1
		c3 = a1*b2 - a2*b1

		a3 = b1*c2 - c1*b2
		temp1 = b1
		temp2 = c2
		temp3 = c1
		temp4 = b2
	*/

	FLM_CLEAR_ERROR();
	temp1 = fixedLMRetrieveValue(m1, 1, 0);
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 2, 0);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m1, 2, 0);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m2, 1, 0);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
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
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 0);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m2, 2, 0);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 0);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
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
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 1, 0);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 0);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m1, 1, 0);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
	code  = fixedLMSetValue(m3, 2, 0, acc);
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMCrossCol3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
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

	HANDLE_EXACTSIZE_MATRIX(height1, width1, 3, 1);
	HANDLE_EXACTSIZE_MATRIX(height2, width2, 3, 1);
	HANDLE_EXACTSIZE_MATRIX(height3, width3, 3, 1);

	flmtype_t type1 = fixedLMGetType(m1);
	flmtype_t type2 = fixedLMGetType(m2);
	flmtype_t type3 = fixedLMGetType(m3);

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
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 2);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m1, 0, 2);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m2, 0, 1);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
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
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 0);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 2);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 0);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
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
	temp1 = typeAbstractValueConverterIn(type1, temp1);

	temp2 = fixedLMRetrieveValue(m2, 0, 1);
	temp2 = typeAbstractValueConverterIn(type2, temp2);

	temp3 = fixedLMRetrieveValue(m2, 0, 0);
	temp3 = typeAbstractValueConverterIn(type2, temp3);

	temp4 = fixedLMRetrieveValue(m1, 0, 1);
	temp4 = typeAbstractValueConverterIn(type2, temp4);

	acc   = fixedSub64(fixedMul64(temp1, temp2), fixedMul64(temp3, temp4));
	acc   = typeAbstractValueConverterOut(type3, acc);
	code  = fixedLMSetValue(m3, 0, 2, acc);
	if(code != FLM_NO_ERROR) return code;

	return FLM_NO_ERROR;
}
