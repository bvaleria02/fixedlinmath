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

		case 4:		code = fixedLMDeterminant4x4(a, type, value);
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

	flmtype_t typeA = fixedLMGetType(a);

	if(widthA < 1){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

	flmretrieve_t temp = fixedLMRetrieveValue(a, 0, 0);
	temp = typeAbstractValueConverterIn(typeA, temp);
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

	flmtype_t typeA = fixedLMGetType(a);

	if(widthA < 2){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

/*
	A = [a b]  det = ad - cb
		[c d]
*/

	FLM_CLEAR_ERROR();

	flmretrieve_t A = fixedLMRetrieveValue(a, 0, 0);
	A = typeAbstractValueConverterIn(typeA, A);

	flmretrieve_t B = fixedLMRetrieveValue(a, 1, 0);
	B = typeAbstractValueConverterIn(typeA, B);

	flmretrieve_t C = fixedLMRetrieveValue(a, 0, 1);
	C = typeAbstractValueConverterIn(typeA, C);

	flmretrieve_t D = fixedLMRetrieveValue(a, 1, 1);
	D = typeAbstractValueConverterIn(typeA, D);
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

	flmtype_t typeA = fixedLMGetType(a);

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
	A = typeAbstractValueConverterIn(typeA, A);

	flmretrieve_t B = fixedLMRetrieveValue(a, 1, 0);
	B = typeAbstractValueConverterIn(typeA, B);

	flmretrieve_t C = fixedLMRetrieveValue(a, 2, 0);
	C = typeAbstractValueConverterIn(typeA, C);

	flmretrieve_t D = fixedLMRetrieveValue(a, 0, 1);
	D = typeAbstractValueConverterIn(typeA, D);

	flmretrieve_t E = fixedLMRetrieveValue(a, 1, 1);
	E = typeAbstractValueConverterIn(typeA, E);

	flmretrieve_t F = fixedLMRetrieveValue(a, 2, 1);
	F = typeAbstractValueConverterIn(typeA, F);

	flmretrieve_t G = fixedLMRetrieveValue(a, 0, 2);
	G = typeAbstractValueConverterIn(typeA, G);

	flmretrieve_t H = fixedLMRetrieveValue(a, 1, 2);
	H = typeAbstractValueConverterIn(typeA, H);

	flmretrieve_t I = fixedLMRetrieveValue(a, 2, 2);
	I = typeAbstractValueConverterIn(typeA, I);

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

	flmtype_t typeA = fixedLMGetType(a);
	flmtype_t typeU = fixedLMGetType(u);

	FLMErrorCode code = fixedLMLUDecomposition(a, l, u);
	if(code != FLM_NO_ERROR) return code;

	flmretrieve_t acc = F64_ONE;
	flmretrieve_t temp = 0;

	for(flmdim_t i = 0; i < widthU; i++){
		temp = fixedLMRetrieveValue(u, i, i);
		temp = typeAbstractValueConverterIn(typeU, temp);
		acc  = fixedMul64(acc, temp);
	}

	acc = typeAbstractValueConverterOut(type, acc);
	(*value) = acc;

	(void) typeA;
	return FLM_NO_ERROR;
}

FLMErrorCode fixedLMDeterminant4x4(flmmat_t *a, flmtype_t type, flmretrieve_t *value){
	if(value == NULL){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);
	}

	HANDLE_INVALID_MATRIX(a);

	flmdim_t widthA, heightA;
	HANDLE_RECTANGLE_MATRIX(a, widthA, heightA);

	flmtype_t typeA = fixedLMGetType(a);

	if(widthA < 4){
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);
	}

/*
	A = [a b c d]  det = a * (f(kp- ol) - g(jp - nl) + h(jo - nk))
		[e f g h]		-b * (e(kp- ol) - g(ip - ml) + h(io - mk))
		[i j k l]       +c * (e(jp- nl) - f(ip - ml) + h(in - mj))
		[m n o p]		-d * (e(jo- nk) - f(io - mk) + g(in - mj))


	"Please, pray for me and my mental sanity, I'm not going to do the 5x5 unroll... or maybe?"
*/

	FLM_CLEAR_ERROR();

	flmretrieve_t A = fixedLMRetrieveValue(a, 0, 0);
	A = typeAbstractValueConverterIn(typeA, A);

	flmretrieve_t B = fixedLMRetrieveValue(a, 1, 0);
	B = typeAbstractValueConverterIn(typeA, B);

	flmretrieve_t C = fixedLMRetrieveValue(a, 2, 0);
	C = typeAbstractValueConverterIn(typeA, C);

	flmretrieve_t D = fixedLMRetrieveValue(a, 3, 0);
	D = typeAbstractValueConverterIn(typeA, D);

	flmretrieve_t E = fixedLMRetrieveValue(a, 0, 1);
	E = typeAbstractValueConverterIn(typeA, E);

	flmretrieve_t F = fixedLMRetrieveValue(a, 1, 1);
	F = typeAbstractValueConverterIn(typeA, F);

	flmretrieve_t G = fixedLMRetrieveValue(a, 2, 1);
	G = typeAbstractValueConverterIn(typeA, G);

	flmretrieve_t H = fixedLMRetrieveValue(a, 3, 1);
	H = typeAbstractValueConverterIn(typeA, H);

	flmretrieve_t I = fixedLMRetrieveValue(a, 0, 2);
	I = typeAbstractValueConverterIn(typeA, I);

	flmretrieve_t J = fixedLMRetrieveValue(a, 1, 2);
	J = typeAbstractValueConverterIn(typeA, J);

	flmretrieve_t K = fixedLMRetrieveValue(a, 2, 2);
	K = typeAbstractValueConverterIn(typeA, K);

	flmretrieve_t L = fixedLMRetrieveValue(a, 3, 2);
	L = typeAbstractValueConverterIn(typeA, L);

	flmretrieve_t M = fixedLMRetrieveValue(a, 0, 3);
	M = typeAbstractValueConverterIn(typeA, M);

	flmretrieve_t N = fixedLMRetrieveValue(a, 1, 3);
	N = typeAbstractValueConverterIn(typeA, N);

	flmretrieve_t O = fixedLMRetrieveValue(a, 2, 3);
	O = typeAbstractValueConverterIn(typeA, O);

	flmretrieve_t P = fixedLMRetrieveValue(a, 3, 3);
	P = typeAbstractValueConverterIn(typeA, P);

	FLMErrorCode code = fixedLMGetErrno();
	if(code != FLM_NO_ERROR) return code;

/*
Just a little intermission while you are wondering why I unrolled
the 4x4 matrix determinant formula:

It's too high to get over (yeah, yeah)
You're too low to get under (yeah, yeah)
You're stuck in the middle (yeah, yeah)
And the pain is thunder (yeah, yeah)

Lift your head up high and scream out to the world
"I know I am someone, " and let the truth unfurl (hee-ha)
No one can hurt you now, because you know what's true
Yes, I believe in me, so you believe in you
Help me sing it

Ma-ma-se, ma-ma-sa, ma-ma-coo-sa (hoo-hoo)
Ma-ma-se, ma-ma-sa, ma-ma-coo-sa (hee-hee-hee)
Ma-ma-se, ma-ma-sa, ma-ma-coo-sa (hee-hee-hee)
Ma-ma-se, ma-ma-sa, ma-ma-coo-sa

Intermission done!
*/

	// a * ...
	flmretrieve_t KP = fixedMul64(K, P);
	flmretrieve_t OL = fixedMul64(O, L);
	flmretrieve_t JP = fixedMul64(J, P);
	flmretrieve_t NL = fixedMul64(N, L);
	flmretrieve_t JO = fixedMul64(J, O);
	flmretrieve_t NK = fixedMul64(N, K);

	// b * ...
	// KP, OL
	flmretrieve_t IP = fixedMul64(I, P);
	flmretrieve_t ML = fixedMul64(M, L);
	flmretrieve_t IO = fixedMul64(I, O);
	flmretrieve_t MK = fixedMul64(M, K);

	// c * ...
	// JP, NL, IP, ML
	flmretrieve_t IN = fixedMul64(I, N);
	flmretrieve_t MJ = fixedMul64(M, J);

	// d * ...
	// NK, IO, MK, IN, MJ
	//flmretrieve_t JO = fixedMul64(K, P);

	// a * 
	flmretrieve_t KPOL = fixedSub64(KP, OL);
	flmretrieve_t JPNL = fixedSub64(JP, NL);
	flmretrieve_t JONK = fixedSub64(JO, NK);

	// b * 
	// KPOL, 
	flmretrieve_t IPML = fixedSub64(IP, ML);
	flmretrieve_t IOMK = fixedSub64(IO, MK);

	// c * 
	// JPNL, IPML
	flmretrieve_t INMJ = fixedSub64(IN, MJ);

	// d*
	// JONK, IOMK, INMJ

	flmretrieve_t FKPOL = fixedMul64(F, KPOL);
	flmretrieve_t GJPNL = fixedMul64(G, JPNL);
	flmretrieve_t HJONK = fixedMul64(H, JONK);

	flmretrieve_t EKPOL = fixedMul64(E, KPOL);
	flmretrieve_t GIPML = fixedMul64(G, IPML);
	flmretrieve_t HIOMK = fixedMul64(H, IOMK);

	flmretrieve_t EJPNL = fixedMul64(E, JPNL);
	flmretrieve_t FIPML = fixedMul64(F, IPML);
	flmretrieve_t HINMJ = fixedMul64(H, INMJ);

	flmretrieve_t EJONK = fixedMul64(E, JONK);
	flmretrieve_t FIOMK = fixedMul64(F, IOMK);
	flmretrieve_t GINMJ = fixedMul64(G, INMJ);

	flmretrieve_t FKPOL_GJPNL_HJONK = fixedSub64(FKPOL, GJPNL);
	FKPOL_GJPNL_HJONK = fixedAdd64(FKPOL_GJPNL_HJONK, HJONK);
	FKPOL_GJPNL_HJONK = fixedMul64(FKPOL_GJPNL_HJONK, A);

	flmretrieve_t EKPOL_GIPML_HIOMK = fixedSub64(EKPOL, GIPML);
	EKPOL_GIPML_HIOMK = fixedAdd64(EKPOL_GIPML_HIOMK, HIOMK);
	EKPOL_GIPML_HIOMK = fixedMul64(EKPOL_GIPML_HIOMK, B);

	flmretrieve_t EJPNL_FIPML_HINMJ = fixedSub64(EJPNL, FIPML);
	EJPNL_FIPML_HINMJ = fixedAdd64(EJPNL_FIPML_HINMJ, HINMJ);
	EJPNL_FIPML_HINMJ = fixedMul64(EJPNL_FIPML_HINMJ, C);

	flmretrieve_t EJONK_FIOMK_GINMJ = fixedSub64(EJONK, FIOMK);
	EJONK_FIOMK_GINMJ = fixedAdd64(EJONK_FIOMK_GINMJ, GINMJ);
	EJONK_FIOMK_GINMJ = fixedMul64(EJONK_FIOMK_GINMJ, D);

	flmretrieve_t acc   = fixedSub64(FKPOL_GJPNL_HJONK, EKPOL_GIPML_HIOMK);
	acc					= fixedAdd64(acc,   EJPNL_FIPML_HINMJ);
	acc					= fixedSub64(acc,   EJONK_FIOMK_GINMJ);

	(*value) = typeAbstractValueConverterOut(type, acc);

	return FLM_NO_ERROR;
}
