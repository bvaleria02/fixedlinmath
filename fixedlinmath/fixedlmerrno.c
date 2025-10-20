#include "fixedlinmath.h"
#include <stdio.h>
#include <stdlib.h>

_Thread_local FLMErrorCode flm_errno = FLM_NO_ERROR;
_Thread_local FLMFunctionName flm_funcname;
_Thread_local FLMFileName flm_filename;
_Thread_local FLMLineNumber flm_linenumber;

FLMErrorCode fixedLMGetErrno(){
	return flm_errno;
}

void fixedLMClearErrno(){
	flm_errno = FLM_NO_ERROR;
}

void fixedLMSetErrno(FLMErrorCode code){
	flm_errno = code;
}
