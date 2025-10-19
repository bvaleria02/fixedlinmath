#ifndef FIXED_LIN_MATH_H
#define FIXED_LIN_MATH_H

/*
	This library is still WIP
*/

#include <stdint.h>

typedef uint8_t flmdim_t;
typedef uint8_t flmtype_t;
typedef uint8_t flmflag_t;
typedef uint64_t flmretrieve_t;

typedef enum _DATA_TYPE_ {
	FIXED32_T	= 0,	
	UFIXED32_T	= 1,	
	FIXED64_T	= 2,	
	UFIXED64_T	= 3,	
} FLMDataType;

typedef struct {
	flmflag_t isSet;
	flmdim_t height;
	flmdim_t width;
	flmtype_t type;
	void *data;
} flmmat_t;

typedef enum {
	FLM_NO_ERROR			= 0,
	FLM_ERROR_NULLPTR		= 1,
	FLM_ERROR_DIMENSION		= 2,
	FLM_ERROR_MATRIXSET		= 3,
	FLM_ERROR_MATRIXUNSET	= 4,
	FLM_ERROR_TYPE			= 5,
	FLM_ERROR_RECTANGULAR	= 6
} FLMErrorCode;

typedef enum{
	FLM_MATRIX_UNSET		= 0,
	FLM_MATRIX_SET			= 1
} FLMMatrixSet;

typedef uint8_t FLMErrno;
extern _Thread_local FLMErrno flm_errno;

#define FLM_ERROR_VALUE 0

#define FLM_TYPE_EXEC_SPLIT(__type, __sFixed32, __sUfixed32, __sFixed64, __sUfixed64, __error)			\
	switch(__type){											\
		case FIXED32_T:	{									\
							typedef fixed32_t flmdtype_t;	\
							__sFixed32;						\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case UFIXED32_T:	{								\
							typedef ufixed32_t flmdtype_t;	\
							__sUfixed32;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case FIXED64_T:	{									\
							typedef fixed64_t flmdtype_t;	\
							__sFixed64;						\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case UFIXED64_T:	{								\
							typedef ufixed64_t flmdtype_t;	\
							__sUfixed64;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		default:		fixedLMSetErrno(FLM_ERROR_TYPE);	\
						__error;							\
						break;								\
	}


/*
	__type: data type of the matrix
		(FIXED32_T, UFIXED64_T, etc)

	__statement: what should do if type if valid, for example:
		((flmdtype_t *)mat->data)[element] = (flmdtype_t)value;

	__error: what should do if type is invalid, for example:
		return FLM_ERROR_TYPE;

	Use flmdtype_t as the type in the __statement.

*/
#define FLM_TYPE_EXEC(__type, __statement, __error)			\
	switch(__type){											\
		case FIXED32_T:	{									\
							typedef fixed32_t flmdtype_t;	\
							__statement;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case UFIXED32_T:	{								\
							typedef ufixed32_t flmdtype_t;	\
							__statement;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case FIXED64_T:	{									\
							typedef fixed64_t flmdtype_t;	\
							__statement;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		case UFIXED64_T:	{								\
							typedef ufixed64_t flmdtype_t;	\
							__statement;					\
							(void) (flmdtype_t) 0;			\
						}									\
						break;								\
		default:		fixedLMSetErrno(FLM_ERROR_TYPE);	\
						__error;							\
						break;								\
	}


// fixedlinmath/creatematrix.c
FLMErrorCode fixedLMCreateMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type, void *data);

// fixedlinmath/access.c
flmretrieve_t fixedLMRetrieveValue(flmmat_t *mat, flmdim_t x, flmdim_t y);
FLMErrorCode fixedLMSetValue(flmmat_t *mat, flmdim_t x, flmdim_t y, flmretrieve_t value);

// fixedlinmath/fixedlmerrno.c
FLMErrorCode fixedLMGetErrno();
void fixedLMClearErrno();
void fixedLMSetErrno(FLMErrorCode code);

// fixedlinmath/fixedlmmatlab.c
FLMErrorCode fixedLMFill(flmmat_t *mat, flmretrieve_t value);
FLMErrorCode fixedLMZeros(flmmat_t *mat);
FLMErrorCode fixedLMOnes(flmmat_t *mat);
FLMErrorCode fixedLMEye(flmmat_t *mat);

// fixedlinmath/fixedlmutils.c
flmretrieve_t getZeroValueByType(flmtype_t type);
flmretrieve_t getOneValueByType(flmtype_t type);

#endif // FIXED_LIN_MATH_H
