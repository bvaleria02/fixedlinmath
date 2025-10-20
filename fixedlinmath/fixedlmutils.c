#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

flmretrieve_t getOneValueByType(flmtype_t type){
	fixedLMClearErrno();
	flmretrieve_t oneVal;

	FLM_TYPE_EXEC_SPLIT(type, {
		oneVal = F32_ONE;
	}, {
		oneVal = F32_ONE;
	}, {
		oneVal = F64_ONE;
	}, {
		oneVal = F64_ONE;
	}, {
		oneVal = 0;
	});

	return oneVal;
}

flmretrieve_t getZeroValueByType(flmtype_t type){
	fixedLMClearErrno();
	flmretrieve_t oneVal;

	FLM_TYPE_EXEC_SPLIT(type, {
		oneVal = F32_ZERO;
	}, {
		oneVal = F32_ZERO;
	}, {
		oneVal = F64_ZERO;
	}, {
		oneVal = F64_ZERO;
	}, {
		oneVal = 0;
	});

	return oneVal;
}


flmretrieve_t typeAbstractValueConverterIn(flmtype_t type, flmretrieve_t x){
	/*
		converts from "type" to flmretrieve_t
	*/
	flmretrieve_t y;
	
	FLM_TYPE_EXEC_SPLIT(type, 
		{ y = fixedLShift64((fixed64_t)  x, F32_RSHIFT); }, 
		{ y = ufixedLShift64((ufixed64_t)x, F32_RSHIFT); },
		{ y = (flmretrieve_t) x; },
		{ y = (flmretrieve_t) x; },
		{ y = 0; }
	);

	return y;
}

flmretrieve_t typeAbstractValueConverterOut(flmtype_t type, flmretrieve_t x){
	/*
		Converts from flmretrieve_t to "type"
	*/
	flmretrieve_t y;
	
	FLM_TYPE_EXEC_SPLIT(type, 
		{ y = fixedRShift64((fixed64_t)  x, F32_RSHIFT); }, 
		{ y = ufixedRShift64((ufixed64_t)x, F32_RSHIFT); },
		{ y = (flmretrieve_t) x; },
		{ y = (flmretrieve_t) x; },
		{ y = 0; }
	);

	return y;
}

flmretrieve_t getReciprocalByType(flmtype_t type, flmretrieve_t x){
	flmretrieve_t y;

	FLM_TYPE_EXEC_SPLIT(type, 
		{ y = fixedRecp32(x); },
		{ y = ufixedRecp32(x); },
		{ y = fixedRecp64(x); },
		{ y = ufixedRecp64(x); },
		{ return FLM_ERROR_VALUE; }
	);

	return y;
}

flmdim_t getNonZeroRowFromColumnIndex(flmmat_t *mat, flmdim_t col, flmdim_t row){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_ERROR_VALUE);
	}

	if(col >= mat->width){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_DIMENSION, FLM_ERROR_VALUE);
	}

	if(row >= mat->width){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_DIMENSION, FLM_ERROR_VALUE);
	}

	flmretrieve_t temp1;

	for(flmdim_t i = row; i < mat->height; i++){
		temp1 = fixedLMRetrieveValue(mat, col, i);
		if(temp1 != 0){
			return i;
		}
	}

	FLM_RAISE_RETURN_VALUE(FLM_ERROR_NOTFOUND, FLM_ERROR_VALUE);
}

flmdim_t getNonZeroRowFromColumn(flmmat_t *mat, flmdim_t col){
	return getNonZeroRowFromColumnIndex(mat, col, 0);
}

flmflag_t fixedLMIsMatrixNotEye(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_FLAG_NOTEYE);
	}

	if(mat->isSet == FLM_MATRIX_UNSET){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_MATRIXUNSET, FLM_FLAG_NOTEYE);
	}

	if(mat->width != mat->height){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_RECTANGULAR, FLM_FLAG_NOTEYE);
	}

	flmretrieve_t temp1;
	flmretrieve_t one  = getOneValueByType(mat->type);
	flmretrieve_t zero = getZeroValueByType(mat->type);

	for(flmdim_t i = 0; i < mat->height; i++){
		for(flmdim_t j = 0; j < mat->width; j++){
			temp1 = fixedLMRetrieveValue(mat, j, i);
			//printf("col: %i\trow: %i\tvalue: %016lx\n", j, i, temp1);
			if((i == j) && (temp1 == one)){
				continue;
			} else if ((i != j) && (temp1 == zero)){
				continue;
			}

			return FLM_FLAG_NOTEYE;
		}
	}
	
	return FLM_NO_FLAG;
}

uint8_t getSizeofDataType(flmtype_t type){
	FLM_TYPE_EXEC(type, {
		return sizeof(flmdtype_t);
	}, {
		return 0;
	});

	return 0;
}
