#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMAdd(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	// m3 is the output matrix. If m3 is NULL, it uses m1 as the output
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
	flmretrieve_t x1;
	flmretrieve_t x2;
	flmretrieve_t x3;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(type1, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x2 = fixedLMRetrieveValue(m2, j, i);
			x2 = typeAbstractValueConverterIn(type2, x2);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedAdd64(x1, x2);
			x3 = typeAbstractValueConverterOut(type3, x3);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMAddScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	
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
	if(code != FLM_NO_ERROR) return code;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(type1, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedAdd64(x1, x2);
			x3 = typeAbstractValueConverterOut(type2, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAdd(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(height, rowSrc);
	HANDLE_DIMENSIONS_MATRIX(height, rowDest);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(typeMat, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAdd(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width, colSrc);
	HANDLE_DIMENSIONS_MATRIX(width, colDest);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(typeMat, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAddScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, row);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);

		code = fixedLMSetValue(mat, i, row, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAddScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width, col);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);
		
		code = fixedLMSetValue(mat, col, i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAddWeighted(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(height, rowSrc);
	HANDLE_DIMENSIONS_MATRIX(height, rowDest);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(typeMat, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		
		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAddWeighted(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest, flmtype_t type, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t height, width;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width, colSrc);
	HANDLE_DIMENSIONS_MATRIX(width, colDest);

	flmtype_t typeMat = fixedLMGetType(mat);

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(typeMat, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(typeMat, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(typeMat, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}
