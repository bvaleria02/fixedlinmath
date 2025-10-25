#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline flmflag_t setFlagCustomValue(flmflag_t base, flmflag_t mask, flmflag_t shift, flmflag_t value){
	return (base & ~(mask)) | ((value << shift) & mask);
}

static inline flmflag_t setFlag(flmflag_t base, flmflag_t mask){
	return base | mask;
}

static inline flmflag_t clearFlag(flmflag_t base, flmflag_t mask){
	return base & ~(mask);
}

static inline flmflag_t getFlag(flmflag_t base, flmflag_t mask){
	return base & mask;
}

flmflag_t fixedLMIsSet(flmmat_t *mat){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return getFlag(mat->config, FLM_FLAG_SET);
}

flmflag_t fixedLMIsTransposed(flmmat_t *mat){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return getFlag(mat->config, FLM_FLAG_TRANSPOSED);
}

flmflag_t fixedLMIsDataAllocated(flmmat_t *mat){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return getFlag(mat->config, FLM_FLAG_DATAALLOCATED);
}

flmflag_t fixedLMIsReadOnly(flmmat_t *mat){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return getFlag(mat->config, FLM_FLAG_READONLY);
}

FLMErrorCode fixedLMSetFlag(flmmat_t *mat, flmflag_t flag){
	HANDLE_NULL_MATRIX_FLAG_RETURN_ERROR(mat);
	mat->config = setFlag(mat->config, flag);
	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMToggleFlag(flmmat_t *mat, flmflag_t flag){
	HANDLE_NULL_MATRIX_FLAG_RETURN_ERROR(mat);
	flmflag_t isFlag = fixedLMGetFlag(mat, flag);
	if(isFlag){
		mat->config = clearFlag(mat->config, flag);
	} else {
		mat->config = setFlag(mat->config, flag);
	}
	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMClearFlag(flmmat_t *mat, flmflag_t flag){
	HANDLE_NULL_MATRIX_FLAG_RETURN_ERROR(mat);
	mat->config = clearFlag(mat->config, flag);
	return FLM_NO_ERROR;
}

flmflag_t    fixedLMGetFlag(flmmat_t *mat, flmflag_t flag){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return getFlag(mat->config, flag);
}

FLMErrorCode fixedLMSetType(flmmat_t *mat, flmtype_t type){
	HANDLE_NULL_MATRIX_FLAG_RETURN_ERROR(mat);
	mat->config = setFlagCustomValue(mat->config, FLM_FLAG_TYPE, 0x0, type);
	return FLM_NO_ERROR;
}

flmtype_t fixedLMGetType(flmmat_t *mat){
	HANDLE_NULL_MATRIX_FLAG(mat);
	return (flmtype_t) getFlag(mat->config, FLM_FLAG_TYPE);
}
