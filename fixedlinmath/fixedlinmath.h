#ifndef FIXED_LIN_MATH_H
#define FIXED_LIN_MATH_H

/*
	This library is still WIP
*/

#include "../fixedpoint/fixedpoint.h"
#include <stdint.h>

typedef uint32_t flmdim_t;
typedef uint8_t flmtype_t;
typedef uint8_t flmflag_t;
typedef uint64_t flmretrieve_t;

typedef enum {
	FIXED32_T	= 0,	
	UFIXED32_T	= 1,	
	FIXED64_T	= 2,	
	UFIXED64_T	= 3,	
} FLMDataType;


typedef struct{
	flmdim_t colOffset;
	flmdim_t rowOffset;
	flmdim_t viewWidth;
	flmdim_t viewHeight;
} flmviewdetails_t;

typedef struct {
	flmflag_t config;
	/*
	Deprecated
	flmflag_t isSet;
	flmflag_t isDataAllocated;
	flmflag_t isTransposed;
	*/
	flmdim_t height;
	flmdim_t width;
	// Deprecated: flmtype_t type;
	flmviewdetails_t viewDetails;
	void *data;
} flmmat_t;

/*	
	flmmat_t->config bit struct:
	b7:		isView
	b6:		isReadOnly
	b5:		isDataAllocated
	b4:		isTransposed
	b3:		isSet
	b0-b2:	matrix type (fixed32_t, ufixed32_t, fixed64_t or ufixed64_t)
*/

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
	FLM_ERROR_DIVZERO		= 9,
	FLM_ERROR_MALLOC		= 10,
	FLM_ERROR_NOTHEAPMEMORY = 11,
	FLM_ERROR_NOTVECTOR		= 12,
	FLM_ERROR_READONLY		= 13,
	FLM_ERROR_ZERODIM		= 14,
} FLMErrorCode;

typedef enum{
	FLM_NO_FLAG				= 0,
	FLM_FLAG_NOTEYE			= 1
} FLMFlag;

#define FLM_FLAG_TYPE 			0x07
#define FLM_FLAG_SET  			0x08
#define FLM_FLAG_TRANSPOSED  	0x10
#define FLM_FLAG_DATAALLOCATED	0x20
#define FLM_FLAG_READONLY		0x40
#define FLM_FLAG_VIEW			0x80

typedef const char *FLMFunctionName;
typedef const char *FLMFileName;
typedef int FLMLineNumber;
typedef const char *FLMErrnoString;
#define FLMERRNOSTRING_SIZE 64

extern _Thread_local FLMErrorCode flm_errno;
extern _Thread_local FLMFunctionName flm_funcname;
extern _Thread_local FLMFileName flm_filename;
extern _Thread_local FLMLineNumber flm_linenumber;

#define FLM_ERROR_VALUE -1
#define FLM_DIM_ERROR   ((flmdim_t) -1)

#define FLM_BUFFER_CAPACITY(cols, rows) (cols * rows)

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

#define HANDLE_INVALID_MATRIX(mat)	do{						\
	if(mat == NULL){										\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NULLPTR);			\
	}														\
															\
	if(fixedLMIsSet(mat)){						\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_MATRIXUNSET);		\
	}														\
} while(0)

#define HANDLE_INVALID_MATRIX_VOID(mat)	do{					\
	if(mat == NULL){										\
		FLM_RAISE_ERROR(FLM_ERROR_NULLPTR);					\
		return;												\
	}														\
															\
	if(fixedLMIsSet(mat)){									\
		FLM_RAISE_ERROR(FLM_ERROR_MATRIXUNSET);				\
		return;												\
	}														\
} while(0)

#define HANDLE_RECTANGLE_MATRIX(mat, width, height) do{		\
	width = fixedLMGetWidth(mat);							\
	height = fixedLMGetHeight(mat);							\
															\
	if(width != height){									\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_RECTANGULAR);		\
	}														\
} while(0)

#define GET_DIMENSIONS_MATRIX(mat, width, height) do{		\
	width = fixedLMGetWidth(mat);							\
	height = fixedLMGetHeight(mat);							\
} while(0)

#define HANDLE_NONMATCHING_MATRIX(widthA, heightA, widthB, heightB) do{		\
	if(widthA != widthB){									\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);		\
	}														\
															\
	if(heightA != heightB){									\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);		\
	}														\
} while(0)

#define HANDLE_NULL_MATRIX_FLAG(mat)	do{					\
	if(mat == NULL){										\
		FLM_RAISE_ERROR(FLM_ERROR_NULLPTR);					\
		return 0x0;											\
	}														\
} while(0)

#define HANDLE_NULL_MATRIX_FLAG_RETURN_ERROR(mat)	do{		\
	if(mat == NULL){										\
		FLM_RAISE_ERROR(FLM_ERROR_NULLPTR);					\
		return FLM_ERROR_NULLPTR;							\
	}														\
} while(0)

#define HANDLE_DIMENSIONS_MATRIX(_limit, _value) do{		\
	if(_limit <= _value){									\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);		\
	}														\
} while(0)

#define HANDLE_EXACTSIZE_MATRIX(_heightMat, _widthMat, _heightExpected, _widthExpected) do{		\
	if(_widthExpected != _widthMat || _heightExpected != _heightMat){							\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_DIMENSION);											\
	}																							\
} while(0)

#define HANDLE_NONCOLVECT_MATRIX(width) do{					\
	if(width != 1){											\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);		\
	}														\
} while(0)

#define HANDLE_NONROWVECT_MATRIX(height) do{				\
	if(height != 1){										\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_NOTVECTOR);		\
	}														\
} while(0)

#define HANDLE_ZERODIM_MATRIX(_size) do{				\
	if(_size == 0){										\
		FLM_RAISE_RETURN_ERROR(FLM_ERROR_ZERODIM);		\
	}													\
} while(0)

// fixedlinmath/creatematrix.c
FLMErrorCode fixedLMCreateMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type, void *data);
FLMErrorCode fixedLMAllocMatrix(flmmat_t *mat, flmdim_t width, flmdim_t height, flmtype_t type);
FLMErrorCode fixedLMResetMatrix(flmmat_t *mat);
FLMErrorCode fixedLMDestroyMatrix(flmmat_t *mat);
FLMErrorCode fixedLMConfigureView(flmmat_t *mat, flmdim_t colOffset, flmdim_t rowOffset, flmdim_t viewWidth, flmdim_t viewHeight);
FLMErrorCode fixedLMCreateView(flmmat_t *child, flmmat_t *parent, flmdim_t colOffset, flmdim_t rowOffset, flmdim_t viewWidth, flmdim_t viewHeight);

// fixedlinmath/access.c
flmretrieve_t fixedLMRetrieveValue(flmmat_t *mat, flmdim_t x, flmdim_t y);
FLMErrorCode fixedLMSetValue(flmmat_t *mat, flmdim_t x, flmdim_t y, flmretrieve_t value);
flmdim_t fixedLMGetWidth(flmmat_t *mat);
flmdim_t fixedLMGetHeight(flmmat_t *mat);

// fixedlinmath/fixedlmflags.c
flmflag_t fixedLMIsSet(flmmat_t *mat);
flmflag_t fixedLMIsTransposed(flmmat_t *mat);
flmflag_t fixedLMIsDataAllocated(flmmat_t *mat);
flmflag_t fixedLMIsReadOnly(flmmat_t *mat);
flmflag_t fixedLMIsView(flmmat_t *mat);

FLMErrorCode fixedLMSetFlag(flmmat_t *mat, flmflag_t flag);
FLMErrorCode fixedLMClearFlag(flmmat_t *mat, flmflag_t flag);
flmflag_t    fixedLMGetFlag(flmmat_t *mat, flmflag_t flag);

FLMErrorCode fixedLMSetType(flmmat_t *mat, flmtype_t type);
flmtype_t fixedLMGetType(flmmat_t *mat);

FLMErrorCode fixedLMToggleFlag(flmmat_t *mat, flmflag_t flag);

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
flmdim_t getNonZeroRowFromColumnIndex(flmmat_t *mat, flmdim_t col, flmdim_t row);

flmflag_t fixedLMIsMatrixNotEye(flmmat_t *mat);

uint8_t getSizeofDataType(flmtype_t type);

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
FLMErrorCode fixedLMValueSwap(flmmat_t *matSrc, flmmat_t *matDest, flmdim_t colSrc, flmdim_t rowSrc, flmdim_t colDest, flmdim_t rowDest);

// fixedlinmath/fixedlmgaussjordan.c
FLMErrorCode fixedLMGaussJordan(flmmat_t *msrc, flmmat_t *mdest);

// fixedlinmath/fixedlmtranspose.c
FLMErrorCode fixedLMTranspose(flmmat_t *mat);
FLMErrorCode fixedLMTransposeDeep(flmmat_t *mat);
FLMErrorCode fixedLMTransposeVirtual(flmmat_t *mat);

// fixedlinmath/fixedlmdotproduct.c
FLMErrorCode fixedLMDotElementWise(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);
FLMErrorCode fixedLMDotVectorRow(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDotVectorCol(flmmat_t *m1, flmmat_t *m2, flmtype_t type, flmretrieve_t *value);

// fixedlinmath/fixedlmcrossproduct.c
FLMErrorCode fixedLMCrossRow3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);
FLMErrorCode fixedLMCrossCol3d(flmmat_t *m1, flmmat_t *m2, flmmat_t *m3);

// fixedlinmath/fixedlmludecomp.c
FLMErrorCode fixedLMLUDecomposition(flmmat_t *a, flmmat_t *l, flmmat_t *u);

// fixedlinmath/fixedlmdeterminant.c
FLMErrorCode fixedLMDeterminant(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminant0x0(flmmat_t *a, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminant1x1(flmmat_t *a, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminant2x2(flmmat_t *a, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminant3x3(flmmat_t *a, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminant4x4(flmmat_t *a, flmtype_t type, flmretrieve_t *value);
FLMErrorCode fixedLMDeterminantLU(flmmat_t *a, flmmat_t *l, flmmat_t *u, flmtype_t type, flmretrieve_t *value);

#endif // FIXED_LIN_MATH_H
