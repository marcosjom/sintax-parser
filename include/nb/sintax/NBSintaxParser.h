#ifndef NB_SINTAX_PARSER_H
#define NB_SINTAX_PARSER_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/sintax/NBSintaxParserDefs.h"
#include "nb/sintax/NBSintaxParserRules.h"
#include "nb/sintax/NBSintaxParserResults.h"

#ifdef __cplusplus
extern "C" {
#endif

//Callbacks

typedef UI32 /*ENSintaxParserValidateElemResultBit*/ (*NBSintaxParserValidateElemFunc)(void* userParam, const SI32 iElem, const char* name, const char* preContent, const UI32 preContentSz, const UI32 utfCharPos, const char* utfChar, const UI32 utfCharSz);
typedef UI32 (*NBSintaxParserConsumeResults)(void* userParam, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz);
typedef BOOL (*NBSintaxParserUnconsumedChar)(void* userParam, const char* utfChar, const UI32 utfCharSz);
typedef void (*NBSintaxParserErrorFound)(void* userParam, const char* errDesc);

typedef struct STNBSintaxParserCallbacks_ {
	void* userParam;
	NBSintaxParserValidateElemFunc	validateElem;
	NBSintaxParserConsumeResults	consumeResults;
	NBSintaxParserUnconsumedChar	unconsumedChar;
	NBSintaxParserErrorFound		errorFound;
} STNBSintaxParserCallbacks;

//

typedef enum ENSintaxParserResultsMode_ {
	ENSintaxParserResultsMode_LongestsOnly = 0,			//Only the largests will be accumulated
	ENSintaxParserResultsMode_LongestsPerRootElem,		//Largest from each root-elem will be accumulated
	ENSintaxParserResultsMode_LongestsPerRootChildElem,	//Largest from each first-child-elem root-elem will be accumulated
} ENSintaxParserResultsMode;


//--------------
//- Content
//--------------


/*
 PartNode
 |   |-NextPartNode
 |- Posb0
 |- Posb1
    |   |-NextPartNode
    |- Posb0
    |- Posb1
    |- Posb0
 |- Posb0
 ...
 */

struct STNBSintaxParserPartNode_;

typedef struct STNBSintaxParserPartNodeLeafProps_ {
	UI8			resultMskLast;	//ENSintaxParserValidateElemResultBit_*
	UI8			resultMskCur;	//ENSintaxParserValidateElemResultBit_*
	BOOL		isClosed;		//The node is closed, should not continue growing.
	UI32		iByteStart;		//Byte start in 'relAccum'
	UI32		iByteAfter;		//Byte after in 'relAccum', size = (iByteAfter - iByteStart);
	//Child redundancy (NULL if is not a redundancy reference)
	struct STNBSintaxParserPartNode_*	childLeafRef;
	//Branch parent and previous leaf hints
	struct STNBSintaxParserPartNode_*	prevLeaf;			//Previous leaf
	UI32								prevByteAfterIdx;	//Previous leaf, byte after in 'relAccum', size = (iByteAfter - iByteStart);
} STNBSintaxParserPartNodeLeafProps;

typedef struct STNBSintaxParserPartNodeCompleteProps_ {
	UI32		rootElemIdx;		//Type of root
	UI32		rootChildElemIdx;	//Type of first child of root
} STNBSintaxParserPartNodeCompleteProps;
	
typedef struct STNBSintaxParserPartNode_ {
	struct STNBSintaxParserPartNode_*	parent;			//Parent node (relative to leaf)
	struct STNBSintaxParserPartNode_*	previous;		//Previous node (definition sequence)
	STNBSintaxParserPartIdx				partIdx;		//Part index
	STNBSintaxParserPartNodeLeafProps*	leafProps;		//Leaf props (NULL if not a leaf)
	STNBSintaxParserPartNodeCompleteProps* completedProps; //Completed posib props.
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	//Debug
	struct {
		STNBArray						chldrn;			//STNBSintaxParserPartNode*, reverse for 'parent' references
		STNBArray						nexts;			//STNBSintaxParserPartNode*, reverse for 'previous' references
		STNBArray						parentRootsRefs;//STNBSintaxParserPartNode*, reverse for 'childLeafRef' references
		//leafProps
		STNBArray						nextLeaves;		//STNBSintaxParserPartNode*, reverse for 'leafProps.prevLeaf'
	} dbg;
#	endif
	//
	SI32								retainCount;	//References cunt
} STNBSintaxParserPartNode;
	
typedef struct STNBSintaxParserState_ {
	BOOL						errFnd;				//Error state reached parsing the rules
	BOOL						completed;			//The rules were parsed and validated
	//Utf char
	BOOL						utf8IsAnalyzable;	//Content inside 'utf8Accum' was fed as analyzable or not?
	char						utf8Accum[12];		//Utf8 bytes accumulated
	UI32						utf8AccumExp;		//Utf8 bytes expected
	UI32						utf8AccumCur;		//Utf8 bytes current
	//Tokens parsing
	UI32						iByteCur;			//Current byte in 'feed stream'
	STNBString					relAccum;			//Current values accumulated
	STNBString					strRemain;			//Values remain not processed
	//Posibs tree
	struct {
		//Root
		struct {
			STNBArray			elemsIdxs;		//UI32, the idx of elements root of the tree
			STNBSintaxParserPartNode* node;		//Root node for all leafs
		} root;
		//Unconsumed chars
		struct {
			STNBArray			allowed;		//STNBRangeI
			STNBString			str;
		} unconsumed;
		//Leaves
		struct {
			ENSintaxParserResultsMode resultsMode; //How to preserve completed leaves
			STNBArray			open;			//STNBSintaxParserPartNode*, leaf nodes still open
			STNBArray			completed;		//STNBSintaxParserPartNode*, left nodes of completed branches
		} leaves;
	} tree;
} STNBSintaxParserState;

//--------
//- Parser
//--------

typedef struct STNBSintaxParserConfig_ {
	//Name of elements to search
	const char**	rootElems;
	UI32			rootElemsSz;
	//Allowed unconsumed chars
	const char**	unconsumedCharsAllowed;
	UI32			unconsumedCharsAllowedSz;
	//Results accumulation mode
	BOOL			resultsKeepHistory;		//Keep a copy of the results in 'resultsHist' (notify and discard is the default)
	ENSintaxParserResultsMode resultsMode;
} STNBSintaxParserConfig;

typedef struct STNBSintaxParserResultsHistRecord_ {
	STNBSintaxParserResults		results;
	STNBString					strAccum;
} STNBSintaxParserResultsHistRecord;

typedef struct STNBSintaxParserResultsHist_ {
	BOOL						keepHist;			//Keep a copy of the results in 'resultsHist' (notify and discard is the default)
	STNBArray					records;			//STNBSintaxParserResultsHistRecord
} STNBSintaxParserResultsHist;

typedef struct STNBSintaxParser_ {
	//Preprocessed defs
	BOOL						rulesAreRef; 
	STNBSintaxParserRules*		rules;
	//Callbacks
	STNBSintaxParserCallbacks	callbacks;
	//Parser
	STNBSintaxParserState		parser;
	//Results packs
	STNBSintaxParserResultsHist	resultsHist;
} STNBSintaxParser;

void NBSintaxParser_init(STNBSintaxParser* obj);
void NBSintaxParser_release(STNBSintaxParser* obj);

//-----------
//- Callbacks
//-----------
void NBSintaxParser_setCallbacks(STNBSintaxParser* obj, const STNBSintaxParserCallbacks* callbacks);

//--------------
//- Feed rules
//--------------

void NBSintaxParser_rulesFeedStart(STNBSintaxParser* obj);
BOOL NBSintaxParser_rulesFeed(STNBSintaxParser* obj, const char* syntaxDefs);
BOOL NBSintaxParser_rulesFeedByte(STNBSintaxParser* obj, const char c);
BOOL NBSintaxParser_rulesFeedBytes(STNBSintaxParser* obj, const char* syntaxDefs, const SI32 syntaxDefsSz);
BOOL NBSintaxParser_rulesFeedEnd(STNBSintaxParser* obj);
BOOL NBSintaxParser_rulesFeedAsRefOf(STNBSintaxParser* obj, STNBSintaxParser* other);
//
BOOL NBSintaxParser_rulesAreLoaded(const STNBSintaxParser* obj);
SI32 NBSintaxParser_rulesGetElemIdx(const STNBSintaxParser* obj, const char* name);
SI32 NBSintaxParser_rulesGetElemsCount(const STNBSintaxParser* obj);
BOOL NBSintaxParser_rulesGetElemHeaderByIdx(const STNBSintaxParser* obj, const UI32 iElem, ENSintaxParserElemType* dstype, STNBString* dstName);
BOOL NBSintaxParser_rulesConcatAsRulesInC(const STNBSintaxParser* obj, const char* prefix, STNBString* dst);
BOOL NBSintaxParser_rulesConcat(const STNBSintaxParser* obj, STNBString* dst);
BOOL NBSintaxParser_rulesConcatByRefs(const STNBSintaxParser* obj, STNBString* dst);

//--------------
//- Feed content
//--------------

BOOL NBSintaxParser_isEmpty(const STNBSintaxParser* obj);
BOOL NBSintaxParser_feedStart(STNBSintaxParser* obj, const STNBSintaxParserConfig* config);
BOOL NBSintaxParser_feedRestart(STNBSintaxParser* obj);
BOOL NBSintaxParser_feed(STNBSintaxParser* obj, const char* data);
BOOL NBSintaxParser_feedByte(STNBSintaxParser* obj, const char c);
BOOL NBSintaxParser_feedBytes(STNBSintaxParser* obj, const char* data, const SI32 dataSz);
BOOL NBSintaxParser_feedToken(STNBSintaxParser* obj, const UI32 iElem, const char* data);
BOOL NBSintaxParser_feedTokenBytes(STNBSintaxParser* obj, const UI32 iElem, const char* data, const SI32 dataSz);
BOOL NBSintaxParser_feedUnanalyzable(STNBSintaxParser* obj, const char* data);
BOOL NBSintaxParser_feedUnanalyzableByte(STNBSintaxParser* obj, const char c);
BOOL NBSintaxParser_feedUnanalyzableBytes(STNBSintaxParser* obj, const char* data, const SI32 dataSz);
BOOL NBSintaxParser_feedFlush(STNBSintaxParser* obj);
BOOL NBSintaxParser_feedEnd(STNBSintaxParser* obj);
void NBSintaxParser_feedConcatRemain(STNBSintaxParser* obj, STNBString* dst);

//--------------
//- Posibs
//--------------
void NBSintaxParser_posibsConcat(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, STNBString* dst);

//--------------
//- Results
//--------------
UI32 NBSintaxParser_resultsHistoryCount(const STNBSintaxParser* obj);
BOOL NBSintaxParser_resultsHistoryGet(const STNBSintaxParser* obj, const UI32 index, const STNBSintaxParserResults** dstResultPtr, const char** dstStrAccum, UI32* dstStrAccumdst);
void NBSintaxParser_resultsHistoryEmpty(STNBSintaxParser* obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
