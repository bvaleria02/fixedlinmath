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

	if(msrc->width != mdest->width){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if(msrc->height != mdest->height){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	if((msrc->width != msrc->height) || (msrc->height != mdest->width)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_RECTANGULAR);
	}
	
	FLMErrorCode code;
	code = fixedLMEye(mdest);
	if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);

	flmretrieve_t temp1;
	flmretrieve_t temp2;
	flmretrieve_t temp3;

	flmdim_t rowWithoutZero;
	
	for(flmdim_t i = 0; i < msrc->width; i++){
		
		rowWithoutZero = getNonZeroRowFromColumn(msrc, i);
		printf("i: %i\ta: %i\n", i, rowWithoutZero);

		fixedLMRowSwap(msrc,  rowWithoutZero, i);
		fixedLMRowSwap(mdest, rowWithoutZero, i);
		fixedLMPrintMatrix(msrc);

		temp1 = fixedLMRetrieveValue(msrc, i, i);
		code  = fixedLMGetErrno();
		if(code != FLM_NO_ERROR) FLM_RAISE_RETURN_ERROR(code);
		printf("i: %i\t\tTemp1: %016lx\n", i,temp1);

		code  = fixedLMRowDivScalar(msrc, i, msrc->type, temp1);
		if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) return code;
		code  = fixedLMRowDivScalar(mdest, i, msrc->type, temp1);
		if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) return code;
		fixedLMClearErrno();
		fixedLMPrintMatrix(msrc);
		fixedLMPrintMatrix(mdest);

		temp2 = fixedLMRetrieveValue(msrc, i, i);
		temp2 = typeAbstractValueConverterIn(msrc->type, temp2);

		for(flmdim_t j = i+1; j < msrc->height; j++){
			temp3 = fixedLMRetrieveValue(msrc, i, j);
			printf("i: %i\tj: %i\tTemp3: %016lx\n", i, j, temp3);
			code  = fixedLMRowDivScalar(msrc,  j, msrc->type,  temp3);
			code  = fixedLMRowDivScalar(mdest, j, mdest->type, temp3);
			code  = fixedLMRowSub(msrc, i, j);
			code  = fixedLMRowSub(mdest, i, j);
			/*
			code  = fixedLMSetValue(msrc, i, j, 0x0);
			code  = fixedLMSetValue(mdest, i, j, 0x0);
*/
			fixedLMPrintMatrix(msrc);
			if((code != FLM_NO_ERROR) && (code != FLM_ERROR_DIVZERO)) FLM_RAISE_RETURN_ERROR(code);
			fixedLMClearErrno();
		}

	}

	return FLM_NO_ERROR;
}
