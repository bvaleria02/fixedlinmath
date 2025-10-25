#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint32_t getElementMatrix(flmmat_t *mat, flmdim_t col,flmdim_t row){
	flmflag_t isTransposed = fixedLMIsTransposed(mat);
	flmflag_t isView 	   = fixedLMIsView(mat);

	if(fixedLMIsTransposed(mat)){
	}

	uint32_t element;

	if(isTransposed && isView){
		element = ((mat->viewDetails.colOffset + col) * mat->width) + (mat->viewDetails.rowOffset + row);
	} else if(isTransposed && !isView){
		element=  (col * mat->width) + row;
	} else if(!isTransposed && isView){
		element = ((mat->viewDetails.rowOffset + row) * mat->width) + (mat->viewDetails.colOffset + col);
	} else{	
		element = (row * mat->width) + col;
	}

	return element;
}

flmdim_t fixedLMGetWidth(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_DIM_ERROR);
	}
	
	flmflag_t isTransposed = fixedLMIsTransposed(mat);
	flmflag_t isView 	   = fixedLMIsView(mat);

	flmdim_t width;
	if(isTransposed && isView){
		width = mat->viewDetails.viewHeight;
	} else if(isTransposed && !isView){
		width = mat->height;
	} else if(!isTransposed && isView){
		width = mat->viewDetails.viewWidth;
	} else{	
		width = mat->width;
	}

	return width;
}

flmdim_t fixedLMGetHeight(flmmat_t *mat){
	if(mat == NULL){
		FLM_RAISE_RETURN_VALUE(FLM_ERROR_NULLPTR, FLM_DIM_ERROR);
	}

	flmflag_t isTransposed = fixedLMIsTransposed(mat);
	flmflag_t isView 	   = fixedLMIsView(mat);

	flmdim_t height;
	if(isTransposed && isView){
		height = mat->viewDetails.viewWidth;
	} else if(isTransposed && !isView){
		height = mat->width;
	} else if(!isTransposed && isView){
		height = mat->viewDetails.viewHeight;
	} else{	
		height = mat->height;
	}

	return height;
}


flmretrieve_t fixedLMRetrieveValue(flmmat_t *mat, flmdim_t col, flmdim_t row){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t width, height;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width,  col);
	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	uint32_t element = getElementMatrix(mat, col, row);
	flmretrieve_t value = 0;
	//printf("trans: %i\tcol: %i\trow: %i\tIndex: %i\n", mat->isTransposed, col, row, element);

	FLM_TYPE_EXEC(typeMat, {
		value = ((flmdtype_t *)mat->data)[element];
	}, {
		value = FLM_ERROR_VALUE;	
	});

	return value;
}

FLMErrorCode fixedLMSetValue(flmmat_t *mat, flmdim_t col, flmdim_t row, flmretrieve_t value){
	HANDLE_INVALID_MATRIX(mat);

	flmdim_t width, height;
	GET_DIMENSIONS_MATRIX(mat, width, height);

	HANDLE_DIMENSIONS_MATRIX(width,  col);
	HANDLE_DIMENSIONS_MATRIX(height, row);

	flmtype_t typeMat = fixedLMGetType(mat);

	if(fixedLMIsReadOnly(mat)){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_READONLY);
	}

	uint32_t element = getElementMatrix(mat, col, row);

	FLM_TYPE_EXEC(typeMat, {
		((flmdtype_t *)mat->data)[element] = (flmdtype_t)value;
	}, {
		return FLM_ERROR_TYPE;
	});

	return FLM_NO_ERROR;
}
