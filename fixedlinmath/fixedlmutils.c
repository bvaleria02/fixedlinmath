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
