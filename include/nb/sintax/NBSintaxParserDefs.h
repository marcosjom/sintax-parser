#ifndef NB_SINTAX_PARSER_DEFS_H
#define NB_SINTAX_PARSER_DEFS_H

#include "nb/NBFrameworkDefs.h"
//

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------
//- Preprocessed definition (interal)
//-----------------------------------

typedef enum ENSintaxParserElemType_ {
	ENSintaxParserElemType_Literal = 0,	//An explicit text
	ENSintaxParserElemType_Callback,	//Use the callback function
	ENSintaxParserElemType_Elem,		//A syntax element
	ENSintaxParserElemType_Count,
} ENSintaxParserElemType;

//Elems - hints of posible starts

typedef enum ENSintaxParserValidateElemResultBit_ {
	ENSintaxParserValidateElemResultBit_None		= 0,	//Not valid
	ENSintaxParserValidateElemResultBit_Partial		= 1,	//Is partial valid
	ENSintaxParserValidateElemResultBit_Complete	= 2,	//Is complete
	ENSintaxParserValidateElemResultBit_All			= (ENSintaxParserValidateElemResultBit_Partial | ENSintaxParserValidateElemResultBit_Complete)
} ENSintaxParserValidateElemResultBit;

typedef struct STNBSintaxParserPartIdx_ {
	UI32	iElem;			//Element index
	UI32	iDef;			//Definition index
	UI32	iPart;			//Part index
} STNBSintaxParserPartIdx;

#ifdef __cplusplus
} //extern "C"
#endif

#endif
