#include "fixedlinmath.h"
#include <stdio.h>
#include <stdlib.h>

_Thread_local FLMErrno flm_errno = FLM_NO_ERROR;

FLMErrorCode fixedLMGetErrno(){
	return flm_errno;
}

void fixedLMClearErrno(){
	flm_errno = FLM_NO_ERROR;
}

void fixedLMSetErrno(FLMErrorCode code){
	flm_errno = code;
}
