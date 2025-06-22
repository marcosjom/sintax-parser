#ifndef NB_SINTAX_PARSER_C_PPROC_DEFS_H
#define NB_SINTAX_PARSER_C_PPROC_DEFS_H

#include "nb/NBFrameworkDefs.h"
//
//#include "nb/core/NBArray.h"
//#include "nb/core/NBArraySorted.h"
//#include "nb/core/NBSintaxParser.h"
//#include "nb/core/NBRange.h"
//
//#include "nb/core/NBSintaxParserCDefs.h"
//#include "nb/core/NBSintaxParserCTokens.h"
//#include "nb/core/NBSintaxParserCPProcUnit.h"

#ifdef __cplusplus
extern "C" {
#endif

//Macro definition

typedef enum ENNBSintaxParserCMacroReplaceHintType_ {
	ENNBSintaxParserCMacroReplaceHintType_Ignore = 0,	//Token must be ignore (usually "#" or "##")
	ENNBSintaxParserCMacroReplaceHintType_Token,		//Token
	ENNBSintaxParserCMacroReplaceHintType_ParamRef,		//Is a reference to a param 
} ENNBSintaxParserCMacroReplaceHintType;

typedef struct STNBSintaxParserCMacroReplaceHint_ {
	ENNBSintaxParserCMacroReplaceHintType type;
	UI32								iLeafReplace;		//Leaf in replacement-list
	UI32								iLeafParam;			//Correspondance in parameter-list
	UI32								iParam;				//Param index
	BOOL								isParamToLiteral;	//Is a reference to a param preceded by '#'
	BOOL								isConcatStart;		//The token or param is followed by '##'
	BOOL								isConcatContinue;	//The token or param is preceded by '##'
} STNBSintaxParserCMacroReplaceHint;

typedef struct STNBSintaxParserCMacro_ {
	//Definition line
	struct {
		STNBString						str;
		STNBSintaxParserResultNode		node;
		//Reference inside 'node'
		struct {
			const char*					name;			//macro name
			UI32						nameLen;		//maro name length
			BOOL						isFunction;		//object-like or function-like (params != NULL || paramElipsis != NULL)
			BOOL						isVaArgs;		//... at params '__VA_ARGS__' is defined as anything after last param
			STNBSintaxParserResultNode* params;			//identifier-list
			UI32						paramsCount;	//count of identifiers in params 'identifier-list'
			STNBSintaxParserResultNode*	replacement;	//replacement-list
			UI32						replacementLeavesCount; //count of leaves in replacement node
			STNBArray					replacementHints; //STNBSintaxParserCMacroReplaceHint, sequence of index of leaf nodes in replacement list that are parameters
		} refs;
	} def;
} STNBSintaxParserCMacro;


#ifdef __cplusplus
} //extern "C"
#endif

#endif
