#ifndef FIXED_LIN_MATH_H
#define FIXED_LIN_MATH_H

/*
	This library is still WIP
*/

#include "../fixedpoint/fixedpoint.h"
#include <stdint.h>

typedef uint16_t flmdim_t;
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
	FLM_ERROR_RECTANGULAR	= 6,
	FLM_ERROR_NOTFOUND		= 7,
	FLM_ERROR_NOTINVERTIBLE	= 8,
	FLM_ERROR_DIVZERO		= 9
} FLMErrorCode;

typedef enum{
	FLM_MATRIX_UNSET		= 0,
	FLM_MATRIX_SET			= 1
} FLMMatrixSet;

typedef const char *FLMFunctionName;
typedef const char *FLMFileName;
typedef int FLMLineNumber;
typedef const char *FLMErrnoString;
#define FLMERRNOSTRING_SIZE 64

extern _Thread_local FLMErrorCode flm_errno;
extern _Thread_local FLMFunctionName flm_funcname;
extern _Thread_local FLMFileName flm_filename;
extern _Thread_local FLMLineNumber flm_linenumber;

#define FLM_ERROR_VALUE 0
#define FLM_DIM_ERROR 0

#define FLM_RAISE_ERROR(__errorCode) do{	\
	fixedLMSetErrno(__errorCode);			\
	flm_funcname = __func__;				\
	flm_filename = __FILE__;				\
	flm_linenumber = __LINE__;				\
} while(0)

#define FLM_RAISE_RETURN_ERROR(__errorCode) do{	\
	FLM_RAISE_ERROR(__errorCode);				\
	return __errorCode;							\
} while(0)

#define FLM_RAISE_RETURN_VALUE(__errorCode, __value) do{	\
	FLM_RAISE_ERROR(__errorCode);							\
	return __value;										\
} while(0)

#define FLM_CLEAR_ERROR() do{		\
	fixedLMSetErrno(FLM_NO_ERROR);	\
	flm_funcname = NULL;			\
	flm_filename = NULL;			\
	flm_linenumber = 0;				\
} while(0)

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
flmretrieve_t typeAbstractValueConverterIn(flmtype_t type, flmretrieve_t x);
flmretrieve_t typeAbstractValueConverterOut(flmtype_t type, flmretrieve_t x);
flmretrieve_t getReciprocalByType(flmtype_t type, flmretrieve_t x);

flmdim_t getNonZeroRowFromColumn(flmmat_t *mat, flmdim_t col);

// fixedlinmath/fixedlmadd.c
FLMErrorCode fixedLMAdd(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);
FLMErrorCode fixedLMAddScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowAdd(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest);
FLMErrorCode fixedLMColAdd(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest);
FLMErrorCode fixedLMRowAddScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColAddScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowAddWeighted(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColAddWeighted(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest, flmtype_t type, flmretrieve_t value);

// fixedlinmath/fixedlmsub.c
FLMErrorCode fixedLMSub(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);
FLMErrorCode fixedLMSubScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowSub(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest);
FLMErrorCode fixedLMColSub(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest);
FLMErrorCode fixedLMRowSubScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColSubScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowSubWeighted(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColSubWeighted(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest, flmtype_t type, flmretrieve_t value);

// fixedlinmath/fixedlmmul.c
FLMErrorCode fixedLMMul(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);
FLMErrorCode fixedLMMulScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowMulScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColMulScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value);

// fixedlinmath/fixedlmdiv.c
FLMErrorCode fixedLMDivScalar(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMRowDivScalar(flmmat_t *mat, flmdim_t row, flmtype_t type, flmretrieve_t value);
FLMErrorCode fixedLMColDivScalar(flmmat_t *mat, flmdim_t col, flmtype_t type, flmretrieve_t value);

// fixedlinmath/fixedrowcolmove.c
FLMErrorCode fixedLMRowSwap(flmmat_t *mat, flmdim_t rowSrc, flmdim_t rowDest);
FLMErrorCode fixedLMColSwap(flmmat_t *mat, flmdim_t colSrc, flmdim_t colDest);

// fixedlinmath/fixedlmgaussjordan.c
FLMErrorCode fixedLMGaussJordan(flmmat_t *msrc, flmmat_t *mdest);

#endif // FIXED_LIN_MATH_H
