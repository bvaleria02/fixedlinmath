#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

FLMErrorCode fixedLMDeterminant(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);
	
	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	FLMErrorCode code = FLM_NO_ERROR;

	switch(widthA){
		case 0:		code = fixedLMDeterminant0x0(a, type, value);
					break;

		case 1:		code = fixedLMDeterminant1x1(a, type, value);
					break;

		case 2:		code = fixedLMDeterminant2x2(a, type, value);
					break;

		case 3:		code = fixedLMDeterminant3x3(a, type, value);
					break;

		default: 	code = fixedLMDeterminantLU(a, l, u, type, value);
					break;

	}

	return code;
}

FLMErrorCode fixedLMDeterminant0x0(flmmat_t *a, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	(*value) = 0x0;

	(void) a;
	(void) type;

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDeterminant1x1(flmmat_t *a, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);

	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	if(widthA < 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	flmretrieve_t temp = fixedLMRetrieveValue(a, 0, 0);
	temp = typeAbstractValueConverterIn(a->type, temp);
	FLMErrorCode code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	(*value) = typeAbstractValueConverterOut(type, temp);

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDeterminant2x2(flmmat_t *a, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);

	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	if(widthA < 2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

/*
	A = [a b]  det = ad - cb
		[c d]
*/

	FLM_CLEAR_ERROR();

	flmretrieve_t A = fixedLMRetrieveValue(a, 0, 0);
	A = typeAbstractValueConverterIn(a->type, A);

	flmretrieve_t B = fixedLMRetrieveValue(a, 1, 0);
	B = typeAbstractValueConverterIn(a->type, B);

	flmretrieve_t C = fixedLMRetrieveValue(a, 0, 1);
	C = typeAbstractValueConverterIn(a->type, C);

	flmretrieve_t D = fixedLMRetrieveValue(a, 1, 1);
	D = typeAbstractValueConverterIn(a->type, D);
	FLMErrorCode code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	flmretrieve_t AD = fixedMul64(A, D);
	flmretrieve_t CB = fixedMul64(C, B);

	flmretrieve_t det = fixedSub64(AD, CB);
	(*value) = typeAbstractValueConverterOut(type, det);

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDeterminant3x3(flmmat_t *a, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);

	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	if(widthA < 3){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

/*
	A = [a b c]  det = a(ei - hf) - b(di - gf) + c(dh - ge)
		[d e f]
		[g h i]

	"I hereby declare I'm not going to use Sarrus' method"
*/

	FLM_CLEAR_ERROR();

	flmretrieve_t A = fixedLMRetrieveValue(a, 0, 0);
	A = typeAbstractValueConverterIn(a->type, A);

	flmretrieve_t B = fixedLMRetrieveValue(a, 1, 0);
	B = typeAbstractValueConverterIn(a->type, B);

	flmretrieve_t C = fixedLMRetrieveValue(a, 2, 0);
	C = typeAbstractValueConverterIn(a->type, C);

	flmretrieve_t D = fixedLMRetrieveValue(a, 0, 1);
	D = typeAbstractValueConverterIn(a->type, D);

	flmretrieve_t E = fixedLMRetrieveValue(a, 1, 1);
	E = typeAbstractValueConverterIn(a->type, E);

	flmretrieve_t F = fixedLMRetrieveValue(a, 2, 1);
	F = typeAbstractValueConverterIn(a->type, F);

	flmretrieve_t G = fixedLMRetrieveValue(a, 0, 2);
	G = typeAbstractValueConverterIn(a->type, G);

	flmretrieve_t H = fixedLMRetrieveValue(a, 1, 2);
	H = typeAbstractValueConverterIn(a->type, H);

	flmretrieve_t I = fixedLMRetrieveValue(a, 2, 2);
	I = typeAbstractValueConverterIn(a->type, I);

	FLMErrorCode code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

	flmretrieve_t EI = fixedMul64(E, I);
	flmretrieve_t HF = fixedMul64(H, F);
	flmretrieve_t DI = fixedMul64(D, I);
	flmretrieve_t GF = fixedMul64(G, F);
	flmretrieve_t DH = fixedMul64(D, H);
	flmretrieve_t GE = fixedMul64(G, E);

	flmretrieve_t EIHF = fixedSub64(EI, HF);
	flmretrieve_t DIGF = fixedSub64(DI, GF);
	flmretrieve_t DHGE = fixedSub64(DH, GE);

	flmretrieve_t AEIHF = fixedMul64(A, EIHF);
	flmretrieve_t BDIGF = fixedMul64(B, DIGF);
	flmretrieve_t CDHGE = fixedMul64(C, DHGE);

	flmretrieve_t acc   = fixedSub64(AEIHF, BDIGF);
	acc					= fixedAdd64(acc,   CDHGE);

	(*value) = typeAbstractValueConverterOut(type, acc);

	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDeterminantLU(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);
	HANDLE_INVALID_MATRIX(l);
	HANDLE_INVALID_MATRIX(u);
	
	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	flmdim_t widthL, heightL;
	HANDLE_RECTANGLE_MATRIX(l, widthL, heightL);

	flmdim_t widthU, heightU;
	HANDLE_RECTANGLE_MATRIX(u, widthU, heightU);

	HANDLE_NONMATCHING_MATRIX(widthA, heightA, widthL, heightL);
	HANDLE_NONMATCHING_MATRIX(widthA, heightA, widthU, heightU);

	FLMErrorCode code = fixedLMLUDecomposition(a, l, u);
	if(code != FLM_NO_ERROR) return code;

	flmretrieve_t acc = F64_ONE;
	flmretrieve_t temp = 0;

	for(flmdim_t i = 0; i < widthU; i++){
		temp = fixedLMRetrieveValue(u, i, i);
		temp = typeAbstractValueConverterIn(u->type, temp);
		acc  = fixedMul64(acc, temp);
	}

	acc = typeAbstractValueConverterOut(type, acc);
	(*value) = acc;

	return FLM_NO_ERROR;
}
