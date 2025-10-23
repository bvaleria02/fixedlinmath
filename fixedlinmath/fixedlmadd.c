#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMAdd(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	// m3 is the output matrix. If m3 is NULL, it uses m1 as the output
	if(m3 == NULL){
		m3 = m1;
	}

	flmdim_t height1 = fixedLMGetHeight(m1);
	flmdim_t width1 = fixedLMGetWidth(m1);
	flmdim_t height2 = fixedLMGetHeight(m2);
	flmdim_t width2 = fixedLMGetWidth(m2);
	flmdim_t height3 = fixedLMGetHeight(m3);
	flmdim_t width3 = fixedLMGetWidth(m3);

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if((width1 != width2) || (width1 != width3)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if((height1 != height2) || (height1 != height3)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x2;
	flmretrieve_t x3;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x2 = fixedLMRetrieveValue(m2, j, i);
			x2 = typeAbstractValueConverterIn(m2->type, x2);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedAdd64(x1, x2);
			x3 = typeAbstractValueConverterOut(m3->type, x3);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMAddScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
	if(m1 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	// m2 is the output matrix. If m3 is NULL, it uses m1 as the output
	if(m2 == NULL){
		m2 = m1;
	}

	flmdim_t height1 = fixedLMGetHeight(m1);
	flmdim_t width1 = fixedLMGetWidth(m1);
	flmdim_t height2 = fixedLMGetHeight(m2);
	flmdim_t width2 = fixedLMGetWidth(m2);

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(width1 != width2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(height1 != height2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x3;
	flmretrieve_t x2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	for(flmdim_t i = 0; i < height1; i++){
		for(flmdim_t j = 0; j < width1; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

			x3 = fixedAdd64(x1, x2);
			x3 = typeAbstractValueConverterOut(m2->type, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAdd(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(rowSrc >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowDest >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAdd(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(colSrc >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(colDest >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAddScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(row >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, row);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, row, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAddScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(col >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, col, i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowAddWeighted(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(rowSrc >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowDest >= height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		
		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColAddWeighted(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	flmdim_t height = fixedLMGetHeight(mat);
	flmdim_t width = fixedLMGetWidth(mat);

	if(colSrc >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(colDest >= width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedAdd64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
	}

	return FLM_NO_ERROR;
}
