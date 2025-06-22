#ifndef NB_SINTAX_PARSER_RULES_H
#define NB_SINTAX_PARSER_RULES_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/core/NBSintaxParserDefs.h"
//
#ifdef __cplusplus
extern "C" {
#endif

//Elems - hints of posible starts

typedef struct STNBSintaxParserElemHintStartNode_ {
	//Parent element
	STNBSintaxParserPartIdx idx;
	UI8						resultMask; //ENSintaxParserValidateElemResultBit
	BOOL					isLeaf;	//Is a leaft node (still can have children)
	//Children elements
	struct STNBSintaxParserElemHintStartNode_* childrn;
	UI32					childrnSz;
} STNBSintaxParserElemHintStartNode;

typedef struct STNBSintaxParserElemHintStartUtf_ {
	char*	val;	//First utf char (if type is 'ENSintaxParserElemType_Literal')
	UI32	valSz;	//First utf char length (if type is 'ENSintaxParserElemType_Literal')
} STNBSintaxParserElemHintStartUtf;
	
typedef struct STNBSintaxParserElemHintStart_ {
	ENSintaxParserElemType				type;
	UI32								iElem;		//Element (if type is not 'ENSintaxParserElemType_Literal')
	STNBSintaxParserElemHintStartUtf	utf;
	STNBSintaxParserElemHintStartNode	posibs; //all posibs tree
} STNBSintaxParserElemHintStart;

BOOL NBCompare_NBSintaxParserElemHintStartByElemIdx(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz);
BOOL NBCompare_NBSintaxParserElemHintStartByUtfChar(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz);

//--------------
//- Sintax rules
//--------------

typedef struct STNBSintaxParserElemDefPart_ {
	UI32	iElem;
	BOOL	isOptional;
} STNBSintaxParserElemDefPart;

typedef struct STNBSintaxParserElemDef_ {
	STNBSintaxParserElemDefPart*	parts;
	UI32							partsSz;
} STNBSintaxParserElemDef;

typedef struct STNBSintaxParserElemHints_ {
	BOOL					canBeEmpty;		//TRUE if this element can have an empty value
	BOOL					canBeRecursive;	//TRUE if this element can start with itself
	STNBArraySorted			starts[ENSintaxParserElemType_Count]; //STNBSintaxParserElemHintStart
} STNBSintaxParserElemHints;
	
typedef struct STNBSintaxParserElem_ {
	ENSintaxParserElemType		type;
	char*						name;		//unique name
	UI32						nameLen;	//name length
	STNBSintaxParserElemDef*	defs;
	UI32						defsSz;
	STNBSintaxParserElemHints	hints;
} STNBSintaxParserElem;

typedef struct STNBSintaxParserRulesParserState_ {
	BOOL		errFnd;				//Error state reached parsing the rules
	BOOL		completed;			//The rules were parsed and validated
	BOOL		isLineComment;		//Found '//' and the rest of the line must be ignored
	STNBString	errDescs;			//Error descriptions
	//
	char		utf8Accum[12];		//Utf8 bytes accumulated
	UI32		utf8AccumExp;		//Utf8 bytes expected
	UI32		utf8AccumCur;		//Utf8 bytes current
	//
	STNBString	tknAccum;			//Token accumulated
	//
	UI32		iCurElem;			//Current STNBSintaxParserElem
	UI32		curRuleTknsCount;	//Current rule/line tokes count
} STNBSintaxParserRulesParserState;

//Rules

typedef struct STNBSintaxParserRules_ {
	STNBArray elems;	//STNBSintaxParserElem
	//Rules parsing
	STNBSintaxParserRulesParserState parser;
} STNBSintaxParserRules;

void NBSintaxParserRules_init(STNBSintaxParserRules* obj);
void NBSintaxParserRules_release(STNBSintaxParserRules* obj);

void NBSintaxParserRules_empty(STNBSintaxParserRules* obj);
STNBSintaxParserElem* NBSintaxParserRules_elemByName(STNBSintaxParserRules* obj, const ENSintaxParserElemType type, const char* name, const BOOL addIfNecessary);

//--------------
//- Sintax rules
//--------------
/*
lexical-element1:
lexical-element2
lexical-element2  [(] lexical-element2(opt) [)]
lexical-element2 [;](opt)
(callback)
...
// "[" and "]" can be used to define a literal element.
//"(callback)" can be used to define the usage of the callback function.
//"(opt)" can be added at the end of an element to define as optional.
*/
void NBSintaxParserRules_feedStart(STNBSintaxParserRules* obj);
BOOL NBSintaxParserRules_feed(STNBSintaxParserRules* obj, const char* syntaxDefs);
BOOL NBSintaxParserRules_feedByte(STNBSintaxParserRules* obj, const char c);
BOOL NBSintaxParserRules_feedBytes(STNBSintaxParserRules* obj, const char* syntaxDefs, const SI32 syntaxDefsSz);
BOOL NBSintaxParserRules_feedEnd(STNBSintaxParserRules* obj);
//
SI32 NBSintaxParserRules_getElemsCount(const STNBSintaxParserRules* obj);
SI32 NBSintaxParserRules_getElemIdx(const STNBSintaxParserRules* obj, const char* name);
BOOL NBSintaxParserRules_getElemHeaderByIdx(const STNBSintaxParserRules* obj, const UI32 iElem, ENSintaxParserElemType* dstype, STNBString* dstName);
BOOL NBSintaxParserRules_concatAsRulesInC(const STNBSintaxParserRules* obj, const char* prefix, STNBString* dst);
BOOL NBSintaxParserRules_concat(const STNBSintaxParserRules* obj, STNBString* dst);
BOOL NBSintaxParserRules_concatByRefs(const STNBSintaxParserRules* obj, STNBString* dst);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
