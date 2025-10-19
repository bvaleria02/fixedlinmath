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
