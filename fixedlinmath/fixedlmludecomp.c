#include "../fixedpoint/fixedpoint.h"
#include "fixedlinmath.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include "fixedlmprint.h"

void createLComponents(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmdim_t col, flmdim_t width, flmretrieve_t one){

	flmretrieve_t a_mn;
	flmretrieve_t l_mk;
	flmretrieve_t u_kn;
	flmretrieve_t u_nn;
	flmretrieve_t acc;
	flmretrieve_t acc2;

	for(flmdim_t row = col; row < width; row++){
		if(row == col){
			fixedLMSetValue(l, col, row, one);
			continue;
		} else if (row < col){
			fixedLMSetValue(l, col, row, 0x0);
			continue;
		}
		
		// {a_{m,n} - sum from {k=1} to {m-1} l_{m,k} cdot u_{k,n}} over {u_{n,n}}
		// m = row	n = col

		a_mn = fixedLMRetrieveValue(a, col, row);
		a_mn = typeAbstractValueConverterIn(a->type, a_mn);

		u_nn = fixedLMRetrieveValue(u, col, col);
		u_nn = getReciprocalByType(u->type, u_nn);
		u_nn = typeAbstractValueConverterIn(u->type, u_nn);

		// Special case: if l_m0, l_m0 = a_mn/u_nn
		if(col == 0){
			acc = fixedMul64(a_mn, u_nn);
			acc = typeAbstractValueConverterOut(l->type, acc);
			fixedLMSetValue(l, col, row, acc);
			continue;
		}

		acc = a_mn;

		for(flmdim_t k = 0; k < col; k++){
			l_mk = fixedLMRetrieveValue(l, k, row);
			l_mk = typeAbstractValueConverterIn(l->type, l_mk);

			u_kn = fixedLMRetrieveValue(u, col, k);
			u_kn = typeAbstractValueConverterIn(u->type, u_kn);
			
			acc2 = fixedMul64(l_mk, u_kn);
			acc  = fixedSub64(acc, acc2);
		}
		
		acc = fixedMul64(acc, u_nn);
		acc = typeAbstractValueConverterOut(l->type, acc);
		fixedLMSetValue(l, col, row, acc);
	}

	(void) width;
}

void createUComponents(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmdim_t row, flmdim_t width){

	flmretrieve_t a_mn;
	flmretrieve_t l_mk;
	flmretrieve_t u_kn;
	flmretrieve_t acc;
	flmretrieve_t acc2;

	for(flmdim_t col = row; col < width; col++){
		if(col < row){
			fixedLMSetValue(l, col, row, 0x0);
			continue;
		}

		// a_{m,n} - sum from k=1 to {m-1} l_{m,k} cdot u_{k,n}
		
		a_mn = fixedLMRetrieveValue(a, col, row);
		a_mn = typeAbstractValueConverterIn(a->type, a_mn);

		// Special case: row = 0, u_0n = a_0n
		if(row == 0){
			acc = typeAbstractValueConverterOut(u->type, a_mn);
			fixedLMSetValue(u, col, row, acc);
			continue;
		}

		acc = a_mn;
		for(flmdim_t k = 0; k < row; k++){
			l_mk = fixedLMRetrieveValue(l, k, row);
			l_mk = typeAbstractValueConverterIn(l->type, l_mk);

			u_kn = fixedLMRetrieveValue(u, col, k);
			u_kn = typeAbstractValueConverterIn(u->type, u_kn);
			
			acc2 = fixedMul64(l_mk, u_kn);
			acc  = fixedSub64(acc, acc2);
		}
		
		acc = typeAbstractValueConverterOut(u->type, acc);
		fixedLMSetValue(u, col, row, acc);
	}
}


FLMErrorCode fixedLMLUDecomposition(flmmat_t *a, flmmat_t *l, flmmat_t *u){
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

	flmretrieve_t oneL = getOneValueByType(l->type);
	fixedLMZeros(l);
	fixedLMZeros(u);
	
	for(flmdim_t i = 0; i < widthA; i++){
		createUComponents(a, l, u, i, widthA);
	//	printf("U: %i\n", i);
	//	fixedLMPrintMatrix(u);

		createLComponents(a, l, u, i, widthA, oneL);
	//	printf("L: %i\n", i);
	//	fixedLMPrintMatrix(l);
	}


	return FLM_NO_ERROR;
}
