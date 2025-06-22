#ifndef NB_SINTAX_PARSER_C_PPROC_UNIT_H
#define NB_SINTAX_PARSER_C_PPROC_UNIT_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/core/NBSintaxParser.h"
#include "nb/core/NBRange.h"
//
#include "nb/core/NBSintaxParserCDefs.h"
#include "nb/core/NBSintaxParserCPProcDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

//Callbacks

typedef BOOL (*NBSintaxParserCPProcUnitMacroFind)(void* userParam, const char* identifier, const UI32 identifierLen, STNBSintaxParserCMacro** dst);

typedef struct STNBSintaxParserCPProcUnitCallbacks_ {
	void*								param;
	NBSintaxParserCPProcUnitMacroFind	macroFind;
} STNBSintaxParserCPProcUnitCallbacks;

//Macro execution

typedef struct STNBSintaxParserCPProcUnitTokens_ {
	const STNBSintaxParserResultNode*	parent;			//Tokens to walk through
	STNBSintaxParserResultNodeRng		range;			//Range of tokens to walk through
	const char*							strAccum;		//string values
	UI32								strAccumSz;		//size of string values
	const STNBSintaxParserCMacroReplaceHint* replaceHints;		//Replace hints (usually only when tranlsating macros)
	const STNBSintaxParserCMacroReplaceHint* replaceHintsAfter;	//Replace hints pointer after last
} STNBSintaxParserCPProcUnitTokens;

typedef struct STNBSintaxParserCPProcUnitStack_ {
	STNBArray							defs;			//STNBSintaxParserCMacro*
} STNBSintaxParserCPProcUnitStack;

typedef enum ENNBSintaxParserCPProcUnitFeedState_ {
	ENNBSintaxParserCPProcUnitFeedState_StartPending = 0,	//Inactive, untill start node is found	
	ENNBSintaxParserCPProcUnitFeedState_StartFound,		//Active, untill afterEnd node is found
	ENNBSintaxParserCPProcUnitFeedState_EndFound,			//Inactive, afterEnd node found
	//Count
	ENNBSintaxParserCPProcUnitFeedState_Count
} ENNBSintaxParserCPProcUnitFeedState;

typedef enum ENNBSintaxParserCPProcUnitMacroType_ {
	ENNBSintaxParserCPProcUnitMacroType_None = 0,	//No macro is open
	ENNBSintaxParserCPProcUnitMacroType_Defined,	//"defined" found	
	ENNBSintaxParserCPProcUnitMacroType_Open,		//Macro found (object or function like)
	//Count
	ENNBSintaxParserCPProcUnitMacroType_Count
} ENNBSintaxParserCPProcUnitMacroType;

//Reference to a token in the recursive feed of a token

typedef struct STNBSintaxParserCPProcUnitTknRefLive_ {
	const STNBSintaxParserResultNode*		node;	//Node
	UI32									iChild;	//Child index
	UI32									iLeaf;	//Leaf seq
	const STNBSintaxParserCMacroReplaceHint* hint;	//Hint
} STNBSintaxParserCPProcUnitTknRefLive;

//Reference to a range of tokens in the recursive feed of a token

typedef struct STNBSintaxParserCPProcUnitTknsRngLive_ {
	STNBSintaxParserCPProcUnitTknRefLive start;	//start
	STNBSintaxParserCPProcUnitTknRefLive after; //after last
} STNBSintaxParserCPProcUnitTknsRngLive;

//Preproc translation unit 

typedef struct STNBSintaxParserCPProcUnit_ {
	STNBSintaxParser*					dst;
	STNBSintaxParserCPProcUnitCallbacks	callbacks;
	ENNBSintaxParserCPProcUnitFeedState	feedState;
	BOOL								allowOpDefined;	//allow 'defined' operator
	//Tokens
	struct {
		const STNBSintaxParserCPProcUnitTokens* def;
		UI32 iPosCur;
	} tokens;
	//Params from parent
	struct {
		const STNBSintaxParserCPProcUnitTokens* tokens;
		STNBSintaxParserCPProcUnitTknsRngLive*	arr;
		UI32									arrSz;
	} params;
	//Parsing macro
	struct {
		ENNBSintaxParserCPProcUnitMacroType		type;
		STNBSintaxParserCMacro*					def;			//macro found
		UI32									parentesisDepth; //level of parenthesis found
		BOOL									paramMustStart;	//Next token starts a param (except for empty params lists)
		BOOL									paramMustClose;	//Next token must be ')'
		STNBSintaxParserCPProcUnitTknsRngLive*	paramCur;		//Current param being filled in 'params' 
		STNBArray								params;			//STNBSintaxParserCPProcUnitTknsRngLive
	} macroOpen;
} STNBSintaxParserCPProcUnit;

//Translation unit (also macros translations)

void NBSintaxParserCPProcUnit_init(STNBSintaxParserCPProcUnit* obj);
void NBSintaxParserCPProcUnit_release(STNBSintaxParserCPProcUnit* obj);

//Config
void NBSintaxParserCPProcUnit_configFromOther(STNBSintaxParserCPProcUnit* obj, const STNBSintaxParserCPProcUnit* other);
void NBSintaxParserCPProcUnit_setAllowOperatorDefined(STNBSintaxParserCPProcUnit* obj, const BOOL allow);
void NBSintaxParserCPProcUnit_setCallbacks(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitCallbacks* callbacks);

//Feed

BOOL NBSintaxParserCPProcUnit_feedTokens(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack);

//Macro run stack

void NBSintaxParserCMacroStack_init(STNBSintaxParserCPProcUnitStack* obj);
void NBSintaxParserCMacroStack_release(STNBSintaxParserCPProcUnitStack* obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
