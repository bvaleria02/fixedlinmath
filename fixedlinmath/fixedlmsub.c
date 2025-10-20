#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMSub(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3){
	if(m1 == NULL || m2 == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	// m3 is the output matrix. If m3 is NULL, it uses m1 as the output
	if(m3 == NULL){
		m3 = m1;
	}

	if(m1->isSet == FLM_MATRIX_UNSET || m2->isSet == FLM_MATRIX_UNSET || m3->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if((m1->width != m2->width) || (m1->width != m3->width)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if((m1->height != m2->height) || (m1->height != m3->height)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode code;
	flmretrieve_t x1;
	flmretrieve_t x2;
	flmretrieve_t x3;

	for(flmdim_t i = 0; i < m1->height; i++){
		for(flmdim_t j = 0; j < m1->width; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

			x2 = fixedLMRetrieveValue(m2, j, i);
			x2 = typeAbstractValueConverterIn(m2->type, x2);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

			x3 = fixedSub64(x1, x2);
			x3 = typeAbstractValueConverterOut(m3->type, x3);

			code = fixedLMSetValue(m3, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMSubScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value){
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
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

	for(flmdim_t i = 0; i < m1->height; i++){
		for(flmdim_t j = 0; j < m1->width; j++){
			x1 = fixedLMRetrieveValue(m1, j, i);
			x1 = typeAbstractValueConverterIn(m1->type, x1);
			code = fixedLMGetErrno();
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

			x3 = fixedSub64(x1, x2);
			x3 = typeAbstractValueConverterOut(m2->type, x3);

			code = fixedLMSetValue(m2, j, i, x3);
			if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		}
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowSub(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(rowSrc >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowDest >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < mat->width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		
		temp3 = fixedSub64(temp2, temp1);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColSub(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(colSrc >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(colDest >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	for(flmdim_t i = 0; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp3 = fixedSub64(temp2, temp1);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowSubScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value){
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
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

	for(flmdim_t i = 0; i < mat->width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, row);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp3 = fixedSub64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, row, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColSubScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value){
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
	flmretrieve_t 	temp3;
	flmretrieve_t   temp2 = typeAbstractValueConverterIn(type, value);
	code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

	for(flmdim_t i = 0; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp3 = fixedSub64(temp1, temp2);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, col, i, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMRowSubWeighted(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(rowSrc >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(rowDest >= mat->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < mat->width; i++){
		temp1 = fixedLMRetrieveValue(mat, i, rowSrc );
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, i, rowDest);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
		
		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedSub64(temp2, temp1);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);

		code = fixedLMSetValue(mat, i, rowDest, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMColSubWeighted(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest, flmtype_t type, flmretrieve_t value){
	if(mat == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(colSrc >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(colDest >= mat->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	FLMErrorCode 	code;
	flmretrieve_t 	temp1;
	flmretrieve_t 	temp2;
	flmretrieve_t 	temp3;

	flmretrieve_t 	value2 = typeAbstractValueConverterIn(type, value);

	for(flmdim_t i = 0; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, colSrc , i);
		temp1 = typeAbstractValueConverterIn(mat->type, temp1);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp2 = fixedLMRetrieveValue(mat, colDest, i);
		temp2 = typeAbstractValueConverterIn(mat->type, temp2);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);

		temp1 = fixedMul64(temp1, value2);
		temp3 = fixedSub64(temp2, temp1);
		temp3 = typeAbstractValueConverterOut(mat->type, temp3);
		
		code = fixedLMSetValue(mat, colDest , i, temp3);
		if(code != FLM_NO_ERROR)	FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	return FLM_NO_ERROR;
}
