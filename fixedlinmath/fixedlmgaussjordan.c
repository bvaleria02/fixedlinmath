#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "fixedlmprint.h"

FLMErrorCode fixedLMGaussJordan(flmmat_t *msrc, flmmat_t *mdest){
	if(msrc == NULL || mdest == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	if(msrc->isSet == FLM_MATRIX_UNSET || mdest->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);
	}

	if(msrc->height != mdest->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(msrc->width != msrc->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_RECTANGULAR);
	}
	
	FLMErrorCode code;

	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t one = getOneValueByType(msrc->type);

	flmdim_t rowWithoutZero;
	
	for(flmdim_t i = 0; i < msrc->width; i++){
		
		fixedLMClearErrno();
		rowWithoutZero = getNonZeroRowFromColumnIndex(msrc, i, i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		/*
			If no row with non-zero value if found, the matrix is
			not invertible, so this returns early

			The following code assumes there is a non-zero row
		*/

		fixedLMRowSwap(msrc,  rowWithoutZero, i);
		fixedLMRowSwap(mdest, rowWithoutZero, i);

		temp1 = fixedLMRetrieveValue(msrc, i, i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

		code  = fixedLMRowDivScalar(msrc, i, msrc->type, temp1);
		if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) return code;
		code  = fixedLMRowDivScalar(mdest, i, msrc->type, temp1);
		if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) return code;

		// Ensures 1.0 value
		fixedLMSetValue(msrc, i, i, one);

		for(flmdim_t j = i+1; j < msrc->height; j++){
			fixedLMClearErrno();
			temp2 = fixedLMRetrieveValue(msrc, i, j);
			code  = fixedLMRowSubWeighted(msrc,  i, j ,msrc->type,  temp2);
			if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) FLM_RAISE_RETURN_ERROR(code);
			code  = fixedLMRowSubWeighted(mdest, i, j, msrc->type, temp2);
			if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) FLM_RAISE_RETURN_ERROR(code);

			// Ensures 0.0 value
			fixedLMSetValue(msrc, i, j, 0x0);
			// fixedLMPrintMatrix(msrc);
		}

	}

	flmdim_t row;
	flmdim_t col;

	for(flmdim_t i = 0; i < msrc->width; i++){
		
		row   = msrc->width - i - 1;
		col	  = row;
		temp1 = fixedLMRetrieveValue(msrc, row, col);

		for(flmdim_t j = 0; j < row; j++){
			fixedLMClearErrno();

			temp2 = fixedLMRetrieveValue(msrc, col, j);
			code  =	fixedLMRowSubWeighted(msrc,  row, j, msrc->type, temp2);
			if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) FLM_RAISE_RETURN_ERROR(code);
			code  = fixedLMRowSubWeighted(mdest, row, j, msrc->type, temp2);
			if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) FLM_RAISE_RETURN_ERROR(code);

			// Ensures 0.0 value
			fixedLMSetValue(msrc, row, j, 0x0);
		}
	}
	
	flmflag_t isInvertible = fixedLMIsMatrixNotEye(msrc);
	if(isInvertible == FLM_FLAG_NOTEYE){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTINVERTIBLE);
	}

	return FLM_NO_ERROR;
}
