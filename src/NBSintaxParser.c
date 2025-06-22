
#include "nb/NBFrameworkPch.h"
#include "nb/core/NBSintaxParser.h"
//
#include "nb/core/NBMemory.h"
#include "nb/core/NBThreadMutex.h"
#include "nb/core/NBEncoding.h"
#include "nb/core/NBRange.h"
//

//Test code
#define NB_TEST_FUNC(P1, P2)	"nb/core/NBRange.h"
/*#if NB_TEST_FUNC(1, 2)
//
#endif*/

#include NB_TEST_FUNC(1, 2)
//#include NB_TEST_FUNC(,)

//Print debug
//#define NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS		//If defined, then the tree is printed after every action
//Print-params
#define NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_PARTS_IDXS		FALSE
#define NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_PARTS_IDXS		FALSE
#define NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RANGES			FALSE
#define NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RETAIN_COUNT	FALSE
//
#ifdef NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS
#	define NBSINTAX_PARSER_DEBUG_PRINT_TREE(OBJ, UTFCHAR, UTFCHARSZ, NODE_TO_POINT_OUT, TEXT) \
	{ \
		STNBString str; \
		NBString_initWithSz(&str, 1024, 4096, 0.10f); \
		/*NBSintaxParser_dbgTreeConcatDeflated*/ \
		NBSintaxParser_treeConcat(OBJ, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_PARTS_IDXS, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RANGES, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RETAIN_COUNT, UTFCHAR, UTFCHARSZ, NODE_TO_POINT_OUT, &str); \
		PRINTF_INFO("NBSintaxParser, tree " TEXT ":\n%s\n", str.str); \
		NBString_release(&str); \
	}
#else
#	define NBSINTAX_PARSER_DEBUG_PRINT_TREE(OBJ, UTFCHAR, UTFCHARSZ, NODE_TO_POINT_OUT, TEXT)
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgValidateTree(const STNBSintaxParser* obj, const UI32 nextUtfSz);
//void NBSintaxParser_dbgValidateTreeNodePosibs(const STNBSintaxParser* obj, const BOOL isPosibCompleted, const STNBSintaxParserPartNode* posibStart, const STNBSintaxParserPartNode* parent, const BOOL partentIsLastNodeH, const UI32 strAccumSz, STNBArray* arrNodes /*(STNBSintaxParserPartNode*)*/);
//void NBSintaxParser_dbgValidateTreeNodeAtArray(STNBArray* arrNodes /*(STNBSintaxParserPartNode*)*/, const STNBSintaxParserPartNode* node, const BOOL mustNotBeFound, const BOOL addIfNecesary);
//void NBSintaxParser_dbgValidateTreeNode(const STNBSintaxParser* obj, STNBArray* arrNodes /*(STNBSintaxParserPartNode*)*/, const STNBSintaxParserPartNode* node, const BOOL isPosibCompleted, const STNBSintaxParserPartNode* parent, const BOOL partentIsLastNodeH, const UI32 strAccumSz);
#endif

//Feed
BOOL NBSintaxParser_feedTokenBytes_(STNBSintaxParser* obj, const UI32 iElem, const char* data, const SI32 dataSz);
BOOL NBSintaxParser_feedByte_(STNBSintaxParser* obj, const char c, const BOOL isAnalyzable);
void NBSintaxParser_feedFlush_(STNBSintaxParser* obj);

//Tree

void NBSintaxParser_treeAddStartPosibsUtf(STNBSintaxParser* obj, STNBSintaxParserPartNode* parent, const STNBSintaxParserElemHintStartNode* posibs, const UI32 bytesToAdd, const UI32 baseResultMsk /*ENSintaxParserValidateElemResultBit*/, const BOOL closeNewFarLeaves, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx, BOOL* dstConsumed);
void NBSintaxParser_treeAddStartPosibsToken(STNBSintaxParser* obj, STNBSintaxParserPartNode* parent, const STNBSintaxParserElemHintStartNode* posibs, const UI32 iElemToken, const char* token, const UI32 tokenSz, const UI32 baseResultMsk /*ENSintaxParserValidateElemResultBit*/, const BOOL closeNewFarLeaves, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx, BOOL* dstConsumed);
void NBSintaxParser_treeFeedFirstUtf(STNBSintaxParser* obj, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedFirstUtfElem(STNBSintaxParser* obj, STNBSintaxParserPartNode** parentRef, STNBSintaxParserPartNode** prevLeafRef, const UI32 prevLeafByteAfterIdx, const UI32 iElem, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedFirstToken(STNBSintaxParser* obj, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedFirstTokenElem(STNBSintaxParser* obj, STNBSintaxParserPartNode** parentRef, STNBSintaxParserPartNode** prevLeafRef, const UI32 prevLeafByteAfterIdx, const UI32 iElem, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedNextUtf(STNBSintaxParser* obj, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedNextPartWithUtf(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedNextToken(STNBSintaxParser* obj, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed);
void NBSintaxParser_treeFeedNextPartWithToken(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed);
BOOL NBSintaxParser_treeBranchIsCompletedFromFarLeaf(const STNBSintaxParser* obj, const STNBSintaxParserPartNode* nLeafBottom);
void NBSintaxParser_treeAddCompletedBranch(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const UI32 iElemRoot, const UI32 iElemRootChild);
//Concat tree reducing repetitions of nodes
#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgTreeConcatDeflated(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz,  const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst);
void NBSintaxParser_dbgTreeConcatDeflatedPosib(const STNBSintaxParser* obj, const char colChar, const STNBSintaxParserPartNode* posibStart, const BOOL includeRanges, const BOOL includePartIdxs, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst);
void NBSintaxParser_dbgTreeConcatDeflatedPosib_(const STNBSintaxParser* obj, const char colChar, const STNBSintaxParserPartNode* posibStart, const char* leftStrForPosib, const UI32 leftStrForPosibSz, const char* leftStrForChild, const UI32 leftStrForChildSz, const BOOL includeRanges, const BOOL includePartIdxs, const BOOL includeRetainCount, const char* strAccum, const UI32 strAccumSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst);
#endif
//Concat tree posibs (repeating nodes when necesary)
void NBSintaxParser_treeConcat(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst);

//Tree nodes

STNBSintaxParserPartNode* NBSintaxParser_nodeCreate(void);
void NBSintaxParser_nodeRetain(STNBSintaxParserPartNode* obj);
void NBSintaxParser_nodeRelease(STNBSintaxParserPartNode* obj);
//
void NBSintaxParser_nodeSetParent(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* parent);
void NBSintaxParser_nodeSetPrevious(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* prev);
void NBSintaxParser_nodeSetPrevLeaf(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx);
void NBSintaxParser_nodeSetChildLeafRef(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* childLeafRef);

//Results

void NBSintaxParser_resultsCurClone_(const STNBSintaxParser* obj, const BOOL reuseNamesPtrs, STNBSintaxParserResults* dst, STNBString* dstStrAccum);
UI32 NBSintaxParser_resultsCurConsume_(STNBSintaxParser* obj);

//

void NBSintaxParser_init(STNBSintaxParser* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParser);
	//Preprocessed defs
	obj->rulesAreRef	= FALSE;
	obj->rules			= NULL;
	//Parser
	{
		STNBSintaxParserState* parser = &obj->parser;
		//
		NBString_initWithSz(&parser->relAccum, 4096, 4096, 0.10f);
		NBString_init(&parser->strRemain);
		//Posibilities tree
		{
			//rootElems
			{
				NBArray_init(&parser->tree.root.elemsIdxs, sizeof(UI32), NULL);
			}
			//Unconsumed chars
			{
				NBArray_init(&parser->tree.unconsumed.allowed, sizeof(STNBRangeI), NULL);
				NBString_init(&parser->tree.unconsumed.str);
			}
			//Leaf nodes
			{
				NBArray_init(&parser->tree.leaves.open, sizeof(STNBSintaxParserPartNode*), NBCompare_PtrVoid);
				NBArray_init(&parser->tree.leaves.completed, sizeof(STNBSintaxParserPartNode*), NBCompare_PtrVoid);
			}
			//Posibs
			{
				parser->tree.root.node = NBSintaxParser_nodeCreate();
			}
		}
	}
	//Results history
	{
		obj->resultsHist.keepHist = FALSE;
		NBArray_init(&obj->resultsHist.records, sizeof(STNBSintaxParserResultsHistRecord), NULL);
	}
}

void NBSintaxParser_release(STNBSintaxParser* obj){
	//Preprocessed defs
	if(obj->rules != NULL){
		if(!obj->rulesAreRef){
			NBSintaxParserRules_release(obj->rules);
			NBMemory_free(obj->rules);
			obj->rules = NULL;
		}
		obj->rulesAreRef = FALSE;
		obj->rules = NULL;
	}
	//Parser
	{
		STNBSintaxParserState* parser = &obj->parser;
		//
		NBString_release(&parser->relAccum);
		NBString_release(&parser->strRemain);
		//Posibilities tree
		{
			//rootElems
			{
				NBArray_empty(&parser->tree.root.elemsIdxs);
				NBArray_release(&parser->tree.root.elemsIdxs);
			}
			//Unconsumed chars
			{
				NBArray_empty(&parser->tree.unconsumed.allowed);
				NBArray_release(&parser->tree.unconsumed.allowed);
				NBString_release(&parser->tree.unconsumed.str);
			}
			//Leaves
			{
				//Open
				{
					STNBArray* leaves = &parser->tree.leaves.open;
					SI32 i; for(i = 0; i < leaves->use; i++){
						STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
						NBSintaxParser_nodeRelease(nn);
					}
					NBArray_empty(leaves);
					NBArray_release(leaves);
				}
				//Completed
				{
					STNBArray* completed = &parser->tree.leaves.completed;
					SI32 i; for(i = 0; i < completed->use; i++){
						STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(completed, STNBSintaxParserPartNode*, i);
						NBSintaxParser_nodeRelease(nn);
					}
					NBArray_empty(completed);
					NBArray_release(completed);
				}
			}
			//Posibs
			if(parser->tree.root.node != NULL){
				NBASSERT(parser->tree.root.node->retainCount == 1) //Must be empty
				NBSintaxParser_nodeRelease(parser->tree.root.node);
				parser->tree.root.node = NULL;
			}
		}
	}
	//Results packs
	{
		NBSintaxParser_resultsHistoryEmpty(obj);
		NBArray_release(&obj->resultsHist.records);
	}
}

//-----------
//- Callbacks
//-----------

void NBSintaxParser_setCallbacks(STNBSintaxParser* obj, const STNBSintaxParserCallbacks* callbacks){
	if(callbacks == NULL){
		NBMemory_setZeroSt(obj->callbacks, STNBSintaxParserCallbacks);
	} else {
		obj->callbacks = *callbacks;
	}
}

//--------------
//- Feed rules
//--------------

void NBSintaxParser_rulesFeedStart(STNBSintaxParser* obj){
	if(obj->rules == NULL || obj->rulesAreRef){
		obj->rulesAreRef	= FALSE;
		obj->rules			= NBMemory_allocType(STNBSintaxParserRules);
		NBSintaxParserRules_init(obj->rules);
	}
	NBASSERT(obj->rules != NULL)
	NBSintaxParserRules_feedStart(obj->rules);
}

BOOL NBSintaxParser_rulesFeed(STNBSintaxParser* obj, const char* syntaxDefs){
	BOOL r = FALSE;
	if(obj->rules != NULL && !obj->rulesAreRef){
		r = NBSintaxParserRules_feed(obj->rules, syntaxDefs);
	}
	return r;
}

BOOL NBSintaxParser_rulesFeedByte(STNBSintaxParser* obj, const char c){
	BOOL r = FALSE;
	if(obj->rules != NULL && !obj->rulesAreRef){
		r = NBSintaxParserRules_feedByte(obj->rules, c);
	}
	return r;
}

BOOL NBSintaxParser_rulesFeedBytes(STNBSintaxParser* obj, const char* syntaxDefs, const SI32 syntaxDefsSz){
	BOOL r = FALSE;
	if(obj->rules != NULL && !obj->rulesAreRef){
		r = NBSintaxParserRules_feedBytes(obj->rules, syntaxDefs, syntaxDefsSz);
	}
	return r;
}

BOOL NBSintaxParser_rulesFeedEnd(STNBSintaxParser* obj){
	BOOL r = FALSE;
	if(obj->rules != NULL && !obj->rulesAreRef){
		r = NBSintaxParserRules_feedEnd(obj->rules);
	}
	return r;
}

BOOL NBSintaxParser_rulesFeedAsRefOf(STNBSintaxParser* obj, STNBSintaxParser* other){
	BOOL r = FALSE;
	if(other->rules != NULL){
		if(obj->rules != NULL && !obj->rulesAreRef){
			NBSintaxParserRules_release(obj->rules);
			NBMemory_free(obj->rules);
			obj->rules = NULL;
		}
		//
		obj->rulesAreRef = TRUE;
		obj->rules = other->rules;
		r = TRUE;
	}
	return r;
}

//

BOOL NBSintaxParser_rulesAreLoaded(const STNBSintaxParser* obj){
	return (obj->rules != NULL);
}

SI32 NBSintaxParser_rulesGetElemIdx(const STNBSintaxParser* obj, const char* name){
	SI32 r = -1;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_getElemIdx(obj->rules, name);
	}
	return r;
}

SI32 NBSintaxParser_rulesGetElemsCount(const STNBSintaxParser* obj){
	SI32 r = -1;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_getElemsCount(obj->rules);
	}
	return r;
}

BOOL NBSintaxParser_rulesGetElemHeaderByIdx(const STNBSintaxParser* obj, const UI32 iElem, ENSintaxParserElemType* dstype, STNBString* dstName){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_getElemHeaderByIdx(obj->rules, iElem, dstype, dstName);
	}
	return r;
}

BOOL NBSintaxParser_rulesConcatAsRulesInC(const STNBSintaxParser* obj, const char* prefix, STNBString* dst){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_concatAsRulesInC(obj->rules, prefix, dst);
	}
	return r;
}

BOOL NBSintaxParser_rulesConcat(const STNBSintaxParser* obj, STNBString* dst){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_concat(obj->rules, dst);
	}
	return r;
}

BOOL NBSintaxParser_rulesConcatByRefs(const STNBSintaxParser* obj, STNBString* dst){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		r = NBSintaxParserRules_concatByRefs(obj->rules, dst);
	}
	return r;
}

//-----------------
// Feed
//-----------------

BOOL NBSintaxParser_isEmpty(const STNBSintaxParser* obj){
	//No bytes in buffer, no leaves open nor closed
	return (obj->parser.utf8AccumCur == 0 && obj->parser.tree.leaves.open.use == 0 && obj->parser.tree.leaves.completed.use == 0);
}

BOOL NBSintaxParser_feedStart(STNBSintaxParser* obj, const STNBSintaxParserConfig* config){
	BOOL r = FALSE;
	//Restarts parse state
	if(NBSintaxParser_feedRestart(obj)){
		STNBSintaxParserState* parser	= &obj->parser;
		//Posibilities tree
		if(config == NULL){
			PRINTF_ERROR("NBSintaxParser, no config provided.\n");
			parser->errFnd = TRUE;
			if(obj->callbacks.errorFound != NULL){
				(obj->callbacks.errorFound)(obj->callbacks.userParam, "No config provided.");
			}
		} else {
			STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
			STNBSintaxParserResultsHist* resultsHist = &obj->resultsHist;
			//rootElems
			{
				NBArray_empty(&parser->tree.root.elemsIdxs);
				if(config->rootElems == NULL || config->rootElemsSz <= 0){
					PRINTF_ERROR("NBSintaxParser, no root elems defined.\n");
					parser->errFnd = TRUE;
					if(obj->callbacks.errorFound != NULL){
						(obj->callbacks.errorFound)(obj->callbacks.userParam, "No root elems defined.");
					}
				} else {
					UI32 i; for(i = 0; i < config->rootElemsSz; i++){
						const char* rootElem = config->rootElems[i];
						const STNBSintaxParserElem* ee = NBSintaxParserRules_elemByName(obj->rules, ENSintaxParserElemType_Elem, rootElem, FALSE);
						if(ee == NULL){
							PRINTF_ERROR("NBSintaxParser, rules for '%s' not found.\n", rootElem);
							parser->errFnd = TRUE;
							if(obj->callbacks.errorFound != NULL){
								STNBString str;
								NBString_init(&str);
								NBString_concat(&str, "Rules for '");
								NBString_concat(&str, rootElem);
								NBString_concat(&str, "' not found.");
								(obj->callbacks.errorFound)(obj->callbacks.userParam, str.str);
								NBString_release(&str);
							}
						} else {
							const UI32 eeIdx = (UI32)(ee - NBArray_dataPtr(&rules->elems, STNBSintaxParserElem)); 
							NBASSERT(eeIdx >= 0 && eeIdx < rules->elems.use)
							NBArray_addValue(&parser->tree.root.elemsIdxs, eeIdx);
						}
					}
				}
			}
			//Unconsumed chars
			{
				NBArray_empty(&parser->tree.unconsumed.allowed);
				NBString_empty(&parser->tree.unconsumed.str);
				if(config->unconsumedCharsAllowed != NULL && config->unconsumedCharsAllowedSz > 0){
					UI32 i; for(i = 0; i < config->unconsumedCharsAllowedSz; i++){
						const char* cc = config->unconsumedCharsAllowed[i];
						if(cc != NULL){
							const UI8 bytesExp = NBEncoding_utf8BytesExpected(*cc);
							if(bytesExp != 0){
								STNBRangeI rng;
								rng.start	= parser->tree.unconsumed.str.length;
								rng.size	= (SI32)bytesExp;
								NBArray_addValue(&parser->tree.unconsumed.allowed, rng);
								NBString_concatBytes(&parser->tree.unconsumed.str, cc, rng.size);
							}
						}
					}
				}
			}
			//Results modes
			{
				parser->tree.leaves.resultsMode = config->resultsMode;
			}
			//Results history
			{
				NBSintaxParser_resultsHistoryEmpty(obj);
				resultsHist->keepHist = config->resultsKeepHistory;
			}
		}
		//Result
		r = (!parser->errFnd);
	}
	return r;
}

BOOL NBSintaxParser_feedRestart(STNBSintaxParser* obj){
	BOOL r = FALSE;
	//Restarts parse state
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed){
			parser->errFnd			= FALSE;
			parser->completed		= FALSE;
			//
			parser->utf8AccumExp	= 0;
			parser->utf8AccumCur	= 0;
			//Empty strAccum
			parser->iByteCur		= 0;
			NBString_empty(&parser->relAccum);
			NBString_empty(&parser->strRemain);
			//Posibilities tree
			{
				//Leaves
				{
					//Open
					{
						STNBArray* leaves = &parser->tree.leaves.open;
						SI32 i; for(i = 0; i < leaves->use; i++){
							STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
							NBSintaxParser_nodeRelease(nn);
						}
						NBArray_empty(leaves);
					}
					//Completed
					{
						STNBArray* completed = &parser->tree.leaves.completed;
						SI32 i; for(i = 0; i < completed->use; i++){
							STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(completed, STNBSintaxParserPartNode*, i);
							if(nn->retainCount > 1){
								PRINTF_INFO("NBSintaxParser, completed-leaf retained %d times at end (expected 1).", nn->retainCount);
							}
							NBSintaxParser_nodeRelease(nn);
						}
						NBArray_empty(completed);
					}
				}
				//Empty tokens posibilities
				NBASSERT(parser->tree.root.node != NULL)
				NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, NULL, 0, NULL, "at feed restart")
				NBASSERT(parser->tree.root.node->retainCount == 1) //Must be empty
			}
			//Result
			r = (!parser->errFnd);
		}
	}
	return r;
}
	
BOOL NBSintaxParser_feed(STNBSintaxParser* obj, const char* data){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL && !parser->errFnd){
				const char* c = data;
				while(*c != '\0'){
					if(!NBSintaxParser_feedByte_(obj, *c, TRUE)){
						parser->errFnd = TRUE;
						break;
					}
					c++;
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedBytes(STNBSintaxParser* obj, const char* data, const SI32 dataSz){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL &&  dataSz > 0 && !parser->errFnd){
				const char* c = data;
				const char* cAfterEnd = c + dataSz;
				while(c < cAfterEnd){
					if(!NBSintaxParser_feedByte_(obj, *c, TRUE)){
						parser->errFnd = TRUE;
						break;
					}
					c++;
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedToken(STNBSintaxParser* obj, const UI32 iElem, const char* data){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL){
				r = NBSintaxParser_feedTokenBytes(obj, iElem, data, NBString_strLenBytes(data));
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedTokenBytes(STNBSintaxParser* obj, const UI32 iElem, const char* data, const SI32 dataSz){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL && dataSz > 0){
				if(!NBSintaxParser_feedTokenBytes_(obj, iElem, data, dataSz)){
					parser->errFnd = TRUE;
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

//
	
BOOL NBSintaxParser_feedByte(STNBSintaxParser* obj, const char c){
	return NBSintaxParser_feedByte_(obj, c, TRUE);
}

BOOL NBSintaxParser_feedUnanalyzable(STNBSintaxParser* obj, const char* data){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL && !parser->errFnd){
				const char* c = data;
				while(*c != '\0'){
					if(!NBSintaxParser_feedByte_(obj, *c, FALSE)){
						parser->errFnd = TRUE;
						break;
					}
					c++;
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedUnanalyzableByte(STNBSintaxParser* obj, const char c){
	return NBSintaxParser_feedByte_(obj, c, FALSE);
}

BOOL NBSintaxParser_feedUnanalyzableBytes(STNBSintaxParser* obj, const char* data, const SI32 dataSz){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			if(data != NULL &&  dataSz > 0 && !parser->errFnd){
				const char* c = data;
				const char* cAfterEnd = c + dataSz;
				while(c < cAfterEnd){
					if(!NBSintaxParser_feedByte_(obj, *c, FALSE)){
						parser->errFnd = TRUE;
						break;
					}
					c++;
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedByte_(STNBSintaxParser* obj, const char c, const BOOL isAnalyzable){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			//Detect change in analyzable
			if(parser->utf8IsAnalyzable != isAnalyzable && parser->utf8AccumCur != 0){
				parser->errFnd = TRUE;
				if(obj->callbacks.errorFound != NULL){
					(obj->callbacks.errorFound)(obj->callbacks.userParam, "Analyzable-flag changed at incompleted utf char.");
				}
			} else {
				//Detect the size of the utf char
				if(!parser->errFnd && parser->utf8AccumExp == 0){
					if((parser->utf8AccumExp = NBEncoding_utf8BytesExpected(c)) <= 0){
						parser->errFnd = TRUE;
						if(obj->callbacks.errorFound != NULL){
							(obj->callbacks.errorFound)(obj->callbacks.userParam, "Utf8 malformation found.");
						}
					}
				}
				//Accumulate byte
				if(!parser->errFnd){
					parser->utf8Accum[parser->utf8AccumCur++] = c;
					//Flush character
					if(parser->utf8AccumCur == parser->utf8AccumExp){
						//Process
						NBASSERT(parser->tree.root.node != NULL)
						BOOL consumed = FALSE;
						STNBArray* leaves = &parser->tree.leaves.open;
						//Print current utfchar
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						/*{
							char utf8Accum[12];
							if(parser->utf8AccumCur > 0){
								NBMemory_copy(utf8Accum, parser->utf8Accum, parser->utf8AccumCur);
							}
							utf8Accum[parser->utf8AccumCur] = '\0';
							PRINTF_INFO("NBSintaxParser, feeding '%s' (%d leaves open, %d completed).\n", utf8Accum, obj->parser.tree.leaves.open.use, obj->parser.tree.leaves.completed.use);
						}*/
#						endif
						//Feed
						if(leaves->use == 0 && parser->tree.leaves.completed.use == 0){
							if(isAnalyzable){
								//First char (optimization)
								NBSintaxParser_treeFeedFirstUtf(obj, parser->utf8Accum, parser->utf8AccumCur, &consumed);
								//Notify char as not valid
								if(!consumed){
									//Callback
									if(obj->callbacks.unconsumedChar != NULL){
										char utfCharCopy[12];
										const UI32 utfCharSz = parser->utf8AccumCur;
										NBMemory_copy(utfCharCopy, parser->utf8Accum, utfCharSz);
										utfCharCopy[utfCharSz] = '\0';
										//Consume char before notifying
										{
											parser->utf8AccumExp = 0;
											parser->utf8AccumCur = 0;
										}
										if(!(*obj->callbacks.unconsumedChar)(obj->callbacks.userParam, utfCharCopy, utfCharSz)){
											PRINTF_ERROR("NBSintaxParser, 'unconsumedChar' returned FALSE.\n");
											parser->errFnd = TRUE;
										}
									} else {
										BOOL found = FALSE;
										//Compare with allowed unconsumed chars
										{
											const char* unconsumedStr = parser->tree.unconsumed.str.str;
											SI32 i; for(i = (SI32)parser->tree.unconsumed.allowed.use - 1; i >= 0; i--){
												const STNBRangeI* rng = NBArray_itmPtrAtIndex(&parser->tree.unconsumed.allowed, STNBRangeI, i);
												if(NBString_strIsEqualBytes(&unconsumedStr[rng->start], rng->size, parser->utf8Accum, parser->utf8AccumCur)){
													found = TRUE;
													break;
												}
											}
										}
										if(found){
											//Just accumulate char
											consumed = TRUE;
										} else {
                                            PRINTF_ERROR("NBSintaxParser, unconsumed char found: '%.*s'\n", parser->tree.unconsumed.str.str);
											parser->errFnd = TRUE;
										}
									}
								}
							} else {
								//Just accumulate char
								consumed = TRUE;
							}
						} else {
							//Next chars
							if(isAnalyzable){
								NBSintaxParser_treeFeedNextUtf(obj, parser->utf8Accum, parser->utf8AccumCur, &consumed);
							} else {
								//Close all completed leaves
								SI32 i; for(i = (SI32)leaves->use - 1; i >= 0; i--){
									STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
									NBASSERT(n->parent != NULL) //All leaves must have a parent node defined
									NBASSERT(n->partIdx.iElem >= 0 && n->partIdx.iElem < obj->rules->elems.use)
									NBASSERT(n->leafProps != NULL)
									BOOL consumed2 = FALSE;
									if(n->leafProps != NULL){
										STNBSintaxParserPartNodeLeafProps* nProps = n->leafProps;
										if(nProps->isClosed || (nProps->resultMskCur & ENSintaxParserValidateElemResultBit_Complete) != 0){
											nProps->isClosed = TRUE;
											consumed2 = TRUE;
										}
									}
									//Result
									if(consumed2){
										consumed = TRUE;
									} else {
										//Remove leaf
										NBSintaxParser_nodeRelease(n);
										NBArray_removeItemAtIndex(leaves, i);
									}
								}
							}
						}
						//Post process (if not consumed yet)
						if(parser->utf8AccumCur != 0){
							//Concat UTF
							{
								//Concat utf
								NBString_concatBytes(&parser->relAccum, parser->utf8Accum, parser->utf8AccumCur);
								//Increase counter
								parser->iByteCur	+= parser->utf8AccumCur;
								//Start next char
								parser->utf8AccumExp = 0;
								parser->utf8AccumCur = 0;
							}
							//Print current tree
#							if defined(NB_CONFIG_INCLUDE_ASSERTS) && defined(NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS)
							if(consumed){
								NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, parser->utf8Accum, parser->utf8AccumCur, NULL, "after feeding byte")
							}
#							endif
							//Flush if not consumed
							if(!consumed){
								NBSintaxParser_feedFlush_(obj);
							}
						}
					}
				}
				parser->utf8IsAnalyzable = isAnalyzable;
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedTokenBytes_(STNBSintaxParser* obj, const UI32 iElem, const char* data, const SI32 dataSz){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			//Detect if utfChar is in queue
			if(parser->utf8AccumCur != 0){
				parser->errFnd = TRUE;
				if(obj->callbacks.errorFound != NULL){
					(obj->callbacks.errorFound)(obj->callbacks.userParam, "Feeding token while incompleted utf char in queue.");
				}
			} else if(data != NULL && dataSz > 0){
				//Process
				NBASSERT(parser->tree.root.node != NULL)
				BOOL consumed			= FALSE, ignorePostProcess = FALSE;
				STNBArray* leaves		= &parser->tree.leaves.open;
				STNBArray* completed	= &parser->tree.leaves.completed;
				//Print current utfchar
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				/*{
					const STNBSintaxParserElem* eeTokn = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, iElem);
					STNBString tmp;
					NBString_init(&tmp);
					NBString_concatBytes(&tmp, data, dataSz);
					PRINTF_INFO("NBSintaxParser, feeding token('%s')->('%s') (%d leaves open, %d completed).\n", eeTokn->name, tmp.str, obj->parser.tree.leaves.open.use, obj->parser.tree.leaves.completed.use);
					NBString_release(&tmp);
				}*/
#				endif
				//Feed
				if(leaves->use == 0 && completed->use == 0){
					//First char (optimization)
					NBSintaxParser_treeFeedFirstToken(obj, iElem, data, dataSz, &consumed);
					//Notify char as not valid
					if(!consumed){
						//Consume char before notifying
						ignorePostProcess = TRUE;
						//Callback
						if(obj->callbacks.unconsumedChar != NULL){
							STNBString tmp;
							NBString_init(&tmp);
							NBString_concatBytes(&tmp, data, dataSz);
							{
								UI8 utfCharSz = 0;
								const char* c = tmp.str;
								const char* cAfter = c + tmp.length;
								while(c < cAfter){
									utfCharSz = NBEncoding_utf8BytesExpected(*c);
									if(!(*obj->callbacks.unconsumedChar)(obj->callbacks.userParam, c, utfCharSz)){
										PRINTF_ERROR("NBSintaxParser, 'unconsumedChar' returned FALSE.\n");
										parser->errFnd = TRUE;
										break;
									}
									c += utfCharSz;
								}
							}
							NBString_release(&tmp);
						} else {
							SI32 i, iStart = (SI32)parser->tree.unconsumed.allowed.use - 1;
							UI8 utfCharSz = 0; BOOL found = FALSE;
							const char* c	= data;
							const char* cAfter = c + dataSz;
							const char* unconsumedStr = parser->tree.unconsumed.str.str;
							while(c < cAfter){
								utfCharSz	= NBEncoding_utf8BytesExpected(*c);
								found		= FALSE;
								//Compare with allowed unconsumed chars
								for(i = iStart; i >= 0; i--){
									const STNBRangeI* rng = NBArray_itmPtrAtIndex(&parser->tree.unconsumed.allowed, STNBRangeI, i);
									if(NBString_strIsEqualBytes(&unconsumedStr[rng->start], rng->size, c, utfCharSz)){
										found = TRUE;
										break;
									}
								}
								if(!found){
                                    PRINTF_ERROR("NBSintaxParser, unconsumed char found: '%.*s'\n", parser->tree.unconsumed.str.str);
									parser->errFnd = TRUE;
									break;
								}
								c += utfCharSz;
							}
						}
					}
				} else {
					//Next chars
					NBSintaxParser_treeFeedNextToken(obj, iElem, data, dataSz, &consumed);
				}
				//Post process (if not consumed yet)
				if(!ignorePostProcess){
					//Concat UTF
					{
						//Concat utf
						NBString_concatBytes(&parser->relAccum, data, dataSz);
						//Increase counter
						parser->iByteCur += dataSz;
						//Start next char
						NBASSERT(parser->utf8AccumExp == 0)
						NBASSERT(parser->utf8AccumCur == 0)
					}
					//Print current tree
#					if defined(NB_CONFIG_INCLUDE_ASSERTS) && defined(NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS)
					if(consumed){
						NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, data, dataSz, NULL, "after feeding byte")
					}
#					endif
					//Flush if not consumed
					if(!consumed){
						NBSintaxParser_feedFlush_(obj);
					}
				}
			}
			r = (!parser->errFnd);
		}
	}
	return r;
}

void NBSintaxParser_feedFlush_(STNBSintaxParser* obj){
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	STNBArray* leaves		= &parser->tree.leaves.open;
	STNBArray* completed	= &parser->tree.leaves.completed;
	SI32 iStrRemainPos		= 0;
	STNBString strRemain, tmp;
	NBString_init(&strRemain);
	NBString_init(&tmp);
	//Accumuate current accum
	while(!parser->errFnd){
		//Complete pending results
		{
			SI32 i; for(i = (SI32)leaves->use - 1; i >= 0; i--){
				STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
				NBASSERT(n->parent != NULL) //All leaves must have a parent node defined
				NBASSERT(n->partIdx.iElem >= 0 && n->partIdx.iElem < obj->rules->elems.use)
				NBASSERT(n->leafProps != NULL)
				if(NBSintaxParser_treeBranchIsCompletedFromFarLeaf(obj, n)){
					UI32 iElemRoot = n->partIdx.iElem;
					UI32 iElemRootChild = rules->elems.use;
					//Determine root elems
					const STNBSintaxParserPartNode* topRootNode = obj->parser.tree.root.node; 
					STNBSintaxParserPartNode* nLeafCur = n;
					while(nLeafCur != NULL && nLeafCur->parent != topRootNode){
						nLeafCur = nLeafCur->parent;
						if(nLeafCur != NULL){
							iElemRootChild	= iElemRoot;
							iElemRoot		= nLeafCur->partIdx.iElem;
						}
					} NBASSERT(iElemRoot == nLeafCur->partIdx.iElem)
					//Add as result
					NBSintaxParser_treeAddCompletedBranch(obj, n, iElemRoot, iElemRootChild);
				}
				//Remove leaf
				NBSintaxParser_nodeRelease(n);
				NBArray_removeItemAtIndex(leaves, i);
			}
		}
		//Send current completed result
		if(completed->use > 0){
			//--------
			//- At least one completed posibility found;
			//- notify results, flush used chars and retry.
			//--------
			//Notify posibilities
			{
				//PRINTF_INFO("Notifying with strAccum('%s').\n", parser->relAccum.str);
				const UI32 bytesConsumed = NBSintaxParser_resultsCurConsume_(obj);
				NBASSERT(bytesConsumed <= parser->relAccum.length)
				if(bytesConsumed == 0){
					PRINTF_ERROR("NBSintaxParser, resultsNotify consumed zero bytes.\n");
					parser->errFnd = TRUE;
					break;
				} else if(bytesConsumed > parser->relAccum.length){
					PRINTF_ERROR("NBSintaxParser, resultsNotify consumed zero bytes.\n");
					parser->errFnd = TRUE;
					break;
				} else {
					//Build new remaining content
					NBString_empty(&tmp);
					//Concat remaining on accum
					if(bytesConsumed < parser->relAccum.length){
						const SI32 countRemain = (parser->relAccum.length - bytesConsumed);
						NBASSERT(parser->iByteCur >= countRemain)
						parser->iByteCur -= countRemain;
						NBString_concatBytes(&tmp, &parser->relAccum.str[bytesConsumed], countRemain);
					}
					//Concat not processed
					if(iStrRemainPos < strRemain.length){
						NBString_concatBytes(&tmp, &strRemain.str[iStrRemainPos], (strRemain.length - iStrRemainPos));
					}
					//Set new strRemain
					{
						iStrRemainPos = 0;
						NBString_swapContent(&strRemain, &tmp);
					}
				}
				//PRINTF_INFO("Notified with strRemain('%s') strAccum('%s').\n", &strRemain.str[iStrRemainPos], parser->relAccum.str);
			}
		} else {
			//--------
			//- No completed posibilities found;
			//- flush first utfChar and retry.
			//--------
			//Remove first utfchar and try again
			//PRINTF_INFO("Unconsuming with strAccum('%s').\n", parser->relAccum.str);
			if(parser->relAccum.length > 0){
				const UI8 utfLen = NBEncoding_utf8BytesExpected(*parser->relAccum.str);
				if(utfLen <= 0 || utfLen > parser->relAccum.length){
					PRINTF_ERROR("NBSintaxParser, invalid utf content.\n");
					parser->errFnd = TRUE;
					break;
				} else {
					//Build new remaining content
					{
						NBASSERT(utfLen <= parser->relAccum.length)
						NBString_empty(&tmp);
						//Concat remaining on accum
						if(utfLen < parser->relAccum.length){
							const SI32 countRemain = (parser->relAccum.length - utfLen);
							NBASSERT(parser->iByteCur >= countRemain)
							parser->iByteCur -= countRemain;
							NBString_concatBytes(&tmp, &parser->relAccum.str[utfLen], countRemain);
						}
						//Concat not processed
						if(iStrRemainPos < strRemain.length){
							NBString_concatBytes(&tmp, &strRemain.str[iStrRemainPos], (strRemain.length - iStrRemainPos));
						}
						//Set new strRemain
						{
							iStrRemainPos = 0;
							NBString_swapContent(&strRemain, &tmp);
						}
					}
					//Notify or verify char as non-consumed
					{
						if(obj->callbacks.unconsumedChar != NULL){
							char utfCharCopy[12];
							NBMemory_copy(utfCharCopy, parser->relAccum.str, utfLen);
							utfCharCopy[utfLen] = '\0';
							if(!(*obj->callbacks.unconsumedChar)(obj->callbacks.userParam, utfCharCopy, utfLen)){
								PRINTF_ERROR("NBSintaxParser, 'unconsumedChar' returned FALSE.\n");
								parser->errFnd = TRUE;
								break;
							}
						} else {
							BOOL found = FALSE;
							//Compare with allowed unconsumed chars
							{
								const char* unconsumedStr = parser->tree.unconsumed.str.str;
								SI32 i; for(i = (SI32)parser->tree.unconsumed.allowed.use - 1; i >= 0; i--){
									const STNBRangeI* rng = NBArray_itmPtrAtIndex(&parser->tree.unconsumed.allowed, STNBRangeI, i);
									if(NBString_strIsEqualBytes(&unconsumedStr[rng->start], rng->size, parser->relAccum.str, utfLen)){
										found = TRUE;
										break;
									}
								}
							}
							if(!found){
								PRINTF_ERROR("NBSintaxParser, unconsumed char found: '%s'\n", parser->tree.unconsumed.str.str);
								parser->errFnd = TRUE;
								break;
							}
						}
					}
				}
			}
			//PRINTF_INFO("Unconsumed with strRemain('%s') strAccum('%s').\n", &strRemain.str[iStrRemainPos], parser->relAccum.str);
		}
		//Restart feed
		NBSintaxParser_feedRestart(obj);
		//Try again
		if(iStrRemainPos >= strRemain.length){
			//End flush
			//PRINTF_INFO("Exiting cicle before refeeding.\n");
			break;
		} else {
			//Re-feed remaining
			BOOL consumed = FALSE;
			//Feed again
			while(!parser->errFnd && iStrRemainPos < strRemain.length){
				const char* utfChar = &strRemain.str[iStrRemainPos];
				const UI8 utfLen = NBEncoding_utf8BytesExpected(*utfChar);
				if(utfLen <= 0 || utfLen > strRemain.length){
					PRINTF_ERROR("NBSintaxParser, invalid utf content.\n");
					parser->errFnd = TRUE;
					break;
				} else {
					//Feed char
					//Print current utfchar
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					/*{
						char utf8Accum[12];
						if(utfChar > 0){
							NBMemory_copy(utf8Accum, utfChar, utfLen);
						}
						utf8Accum[utfLen] = '\0';
						PRINTF_INFO("NBSintaxParser, refeeding '%s' (%d leaves open, %d completed).\n", utf8Accum, obj->parser.tree.leaves.open.use, obj->parser.tree.leaves.completed.use);
					}*/
#					endif
					//Feed
					if(parser->tree.leaves.open.use == 0 && parser->tree.leaves.completed.use == 0){
						//First char (optimization)
						NBSintaxParser_treeFeedFirstUtf(obj, utfChar, utfLen, &consumed);
					} else {
						//Next chars
						NBSintaxParser_treeFeedNextUtf(obj, utfChar, utfLen, &consumed);
					}
					//Concat UTF
					{
						//Concat utf
						NBString_concatBytes(&parser->relAccum, utfChar, utfLen);
						//Increase counter
						parser->iByteCur += utfLen;
						iStrRemainPos += utfLen;
					}
					//Print current tree
#					if defined(NB_CONFIG_INCLUDE_ASSERTS) && defined(NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS)
					if(consumed){
						NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, NULL, 0, NULL, "after refeeding char")
					}
#					endif
					//Stop and reprocess
					if(!consumed){
						break;
					}
				}
			}
			//PRINTF_INFO("Refeeding with strRemain('%s') strAccum('%s') consumed(%s).\n", &strRemain.str[iStrRemainPos], parser->relAccum.str, (consumed ? "YES" : "NO"));
			//Remove procesed chars
			if(consumed && iStrRemainPos == strRemain.length){
				//All processed
				//PRINTF_INFO("Exiting cicle after refeeding.\n");
				break;
			}
		}
	};
	NBString_release(&strRemain);
	NBString_release(&tmp);
}

BOOL NBSintaxParser_feedFlush(STNBSintaxParser* obj){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			NBSintaxParser_feedFlush_(obj);
			r = (!parser->errFnd);
		}
	}
	return r;
}

BOOL NBSintaxParser_feedEnd(STNBSintaxParser* obj){
	BOOL r = FALSE;
	if(obj->rules != NULL){
		STNBSintaxParserState* parser	= &obj->parser;
		STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		if(rules->parser.completed && !parser->completed){
			//Flush
			NBSintaxParser_feedFlush_(obj);
			//Post-process
			if(!parser->errFnd){
				//ToDo: implement
			}
			//Add bytes to remain
			if(parser->utf8AccumCur > 0){
				NBString_concatBytes(&parser->strRemain, parser->utf8Accum, parser->utf8AccumCur);
				parser->utf8AccumCur = parser->utf8AccumExp = 0;
			}
			//
			parser->completed = TRUE;
			r = (!parser->errFnd);
		}
	}
	return r;
}

void NBSintaxParser_feedConcatRemain(STNBSintaxParser* obj, STNBString* dst){
	STNBSintaxParserState* parser = &obj->parser;
	if(dst != NULL){
		if(parser->strRemain.length > 0){
			NBString_concatBytes(dst, parser->strRemain.str, parser->strRemain.length);
		}
	}
}

//-----------------
//Tree nodes
//-----------------

STNBSintaxParserPartNode* NBSintaxParser_nodeCreate(void){
	STNBSintaxParserPartNode* r = NBMemory_allocType(STNBSintaxParserPartNode);
	NBMemory_setZeroSt(*r, STNBSintaxParserPartNode);
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	//Debug
	{
		NBArray_init(&r->dbg.chldrn, sizeof(STNBSintaxParserPartNode*), NULL);
		NBArray_init(&r->dbg.nexts, sizeof(STNBSintaxParserPartNode*), NULL);
		NBArray_init(&r->dbg.parentRootsRefs, sizeof(STNBSintaxParserPartNode*), NULL);
		//leafProps
		NBArray_init(&r->dbg.nextLeaves, sizeof(STNBSintaxParserPartNode*), NULL);
	}
#	endif
	//References count
	r->retainCount = 1;
	//
	return r;
}

void NBSintaxParser_nodeRetain(STNBSintaxParserPartNode* obj){
	NBASSERT(obj->retainCount > 0)
	obj->retainCount++;
}

void NBSintaxParser_nodeRelease(STNBSintaxParserPartNode* obj){
	NBASSERT(obj->retainCount > 0)
	obj->retainCount--;
	if(obj->retainCount == 0){
		//Empty
		NBSintaxParser_nodeSetParent(obj, NULL);
		NBSintaxParser_nodeSetPrevious(obj, NULL);
		NBSintaxParser_nodeSetChildLeafRef(obj, NULL);
		NBSintaxParser_nodeSetPrevLeaf(obj, NULL, 0);
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		//Debug
		{
			NBASSERT(obj->dbg.chldrn.use == 0)
			NBASSERT(obj->dbg.nexts.use == 0)
			NBASSERT(obj->dbg.parentRootsRefs.use == 0)
			NBASSERT(obj->dbg.nextLeaves.use == 0)
			NBArray_release(&obj->dbg.chldrn);
			NBArray_release(&obj->dbg.nexts);
			NBArray_release(&obj->dbg.parentRootsRefs);
			NBArray_release(&obj->dbg.nextLeaves);
		}
#		endif
		//Leaf props
		if(obj->leafProps != NULL){
			NBASSERT(obj->leafProps->prevLeaf == NULL)
			NBASSERT(obj->leafProps->childLeafRef == NULL)
			NBMemory_free(obj->leafProps);
			obj->leafProps = NULL;
		}
		//Completed props
		if(obj->completedProps != NULL){
			NBMemory_free(obj->completedProps);
			obj->completedProps = NULL;
		}
		//Release memory
		NBASSERT(obj->parent == NULL)
		NBASSERT(obj->previous == NULL)
		NBMemory_free(obj);
		obj = NULL;
	}
}

//

void NBSintaxParser_nodeSetParent(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* parent){
	NBASSERT(obj != NULL)
	NBASSERT(obj != parent) //Avoid direct recursivity
	//Retain
	if(obj->parent != parent){
		//Add new
		if(parent != NULL){
			NBSintaxParser_nodeRetain(parent);
			//Debug
			{
				IF_NBASSERT(NBArray_addValue(&parent->dbg.chldrn, obj);)
			}
		}
		//Release current
		if(obj->parent != NULL){
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			//Debug
			{
				BOOL wasFnd = FALSE;
				SI32 i; for(i = 0; i < obj->parent->dbg.chldrn.use; i++){
					const STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(&obj->parent->dbg.chldrn, STNBSintaxParserPartNode*, i);
					if(nn == obj){
						NBArray_removeItemAtIndex(&obj->parent->dbg.chldrn, i);
						wasFnd = TRUE;
						break;
					}
				}
				NBASSERT(wasFnd) //Reference to child must be found
			}
#			endif
			NBSintaxParser_nodeRelease(obj->parent);
		}
		//Set
		obj->parent = parent;
	}
}

void NBSintaxParser_nodeSetPrevious(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* prev){
	NBASSERT(obj != NULL)
	NBASSERT(obj != prev) //Avoid direct recursivity
	//Apply change
	if(obj->previous != prev){
		//Retain
		if(prev != NULL){
			NBSintaxParser_nodeRetain(prev);
			IF_NBASSERT(NBArray_addValue(&prev->dbg.nexts, obj);)
		}
		//Release current
		if(obj->previous != NULL){
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			{
				BOOL wasFnd = FALSE;
				SI32 i; for(i = 0; i < obj->previous->dbg.nexts.use; i++){
					const STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(&obj->previous->dbg.nexts, STNBSintaxParserPartNode*, i);
					if(nn == obj){
						NBArray_removeItemAtIndex(&obj->previous->dbg.nexts, i);
						wasFnd = TRUE;
						break;
					}
				}
				NBASSERT(wasFnd) //Reference to child must be found
			}
#			endif
			NBSintaxParser_nodeRelease(obj->previous);
		}
		//Set
		obj->previous = prev;
	}
}

void NBSintaxParser_nodeSetPrevLeaf(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx){
	NBASSERT(obj != NULL)
	NBASSERT(obj != prevLeaf) //Avoid direct recursivity
	//Set prev leaf
	{
		BOOL apply = FALSE;
		if(prevLeaf == NULL){
			if(obj->leafProps != NULL){
				//Apply change
				if(obj->leafProps->prevLeaf != prevLeaf){
					apply = TRUE;
				}
			}
		} else {
			if(obj->leafProps == NULL){
				obj->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
				NBMemory_setZeroSt(*obj->leafProps, STNBSintaxParserPartNodeLeafProps);;
			}
			if(obj->leafProps->prevLeaf != prevLeaf){
				apply = TRUE;
			}
		}
		//Apply
		if(apply){
			NBASSERT(obj->leafProps != NULL)
			if(obj->leafProps != NULL){
				STNBSintaxParserPartNodeLeafProps* nProps = obj->leafProps;
				//Retain
				if(prevLeaf != NULL){
					NBSintaxParser_nodeRetain(prevLeaf);
					IF_NBASSERT(NBArray_addValue(&prevLeaf->dbg.nextLeaves, obj);)
				}
				//Release current
				if(nProps->prevLeaf != NULL){
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					{
						BOOL wasFnd = FALSE;
						SI32 i; for(i = 0; i < nProps->prevLeaf->dbg.nextLeaves.use; i++){
							const STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(&nProps->prevLeaf->dbg.nextLeaves, STNBSintaxParserPartNode*, i);
							if(nn == obj){
								NBArray_removeItemAtIndex(&nProps->prevLeaf->dbg.nextLeaves, i);
								wasFnd = TRUE;
								break;
							}
						}
						NBASSERT(wasFnd) //Reference to child must be found
					}
#					endif
					NBSintaxParser_nodeRelease(nProps->prevLeaf);
				}
				//Set
				nProps->prevLeaf			= prevLeaf;
				nProps->prevByteAfterIdx	= prevLeafByteAfterIdx;
			}
		}
	}
}

void NBSintaxParser_nodeSetChildLeafRef(STNBSintaxParserPartNode* obj, STNBSintaxParserPartNode* childLeafRef){
	//Set parent
	NBASSERT(obj != NULL)
	NBASSERT(obj != childLeafRef) //Avoid direct recursivity
	//Set childLeafRef
	{
		BOOL apply = FALSE;
		if(childLeafRef == NULL){
			if(obj->leafProps != NULL){
				//Apply change
				if(obj->leafProps->childLeafRef != childLeafRef){
					apply = TRUE;
				}
			}
		} else {
			if(obj->leafProps == NULL){
				obj->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
				NBMemory_setZeroSt(*obj->leafProps, STNBSintaxParserPartNodeLeafProps);;
			}
			if(obj->leafProps->childLeafRef != childLeafRef){
				apply = TRUE;
			}
		}
		//Apply
		if(apply){
			NBASSERT(obj->leafProps != NULL)
			if(obj->leafProps != NULL){
				STNBSintaxParserPartNodeLeafProps* nProps = obj->leafProps;
				//Retain
				if(childLeafRef != NULL){
					NBSintaxParser_nodeRetain(childLeafRef);
					IF_NBASSERT(NBArray_addValue(&childLeafRef->dbg.parentRootsRefs, obj);)
				}
				//Release current
				if(nProps->childLeafRef != NULL){
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					{
						BOOL wasFnd = FALSE;
						SI32 i; for(i = 0; i < nProps->childLeafRef->dbg.parentRootsRefs.use; i++){
							const STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(&nProps->childLeafRef->dbg.parentRootsRefs, STNBSintaxParserPartNode*, i);
							if(nn == obj){
								NBArray_removeItemAtIndex(&nProps->childLeafRef->dbg.parentRootsRefs, i);
								wasFnd = TRUE;
								break;
							}
						}
						NBASSERT(wasFnd) //Reference to child must be found
					}
#					endif
					NBSintaxParser_nodeRelease(nProps->childLeafRef);
				}
				//Set
				nProps->childLeafRef = childLeafRef;
			}
		}
	}
}

//-----------------
//Tree
//-----------------

void NBSintaxParser_treeAddStartPosibsUtf(STNBSintaxParser* obj, STNBSintaxParserPartNode* parent, const STNBSintaxParserElemHintStartNode* posibs, const UI32 bytesToAdd, const UI32 baseResultMsk /*ENSintaxParserValidateElemResultBit*/, const BOOL closeNewFarLeaves, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	if(posibs->childrn != NULL && posibs->childrnSz > 0){
		UI32 i; for(i = 0; i < posibs->childrnSz; i++){
			const STNBSintaxParserElemHintStartNode* chld = &posibs->childrn[i];
			NBASSERT(chld->isLeaf == (chld->childrn == NULL && chld->childrnSz == 0)) //Must be leaf if no children
			//Create new posib (if necesary)
			STNBSintaxParserPartNode* chldN = NBSintaxParser_nodeCreate();
			//Add as posibility
			{
				chldN->partIdx = chld->idx;
				NBSintaxParser_nodeSetParent(chldN, parent);
			}
			//Add as leaf
			if(chld->isLeaf){
				//Set initial props maks
				{
					if(chldN->leafProps == NULL){
						chldN->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
						NBMemory_setZeroSt(*chldN->leafProps, STNBSintaxParserPartNodeLeafProps);;
					}
					{
						STNBSintaxParserPartNodeLeafProps* nProps = chldN->leafProps;
						nProps->iByteStart		= obj->parser.relAccum.length;
						nProps->iByteAfter		= nProps->iByteStart + bytesToAdd;
						nProps->resultMskCur	|= (baseResultMsk | chld->resultMask);
						nProps->resultMskLast	|= (baseResultMsk | chld->resultMask);
						nProps->isClosed		= closeNewFarLeaves;
						NBASSERT(nProps->resultMskCur == nProps->resultMskLast) //Should be the same at this stage
					}
				}
				//Register as leaf node
				{
					NBSintaxParser_nodeSetPrevLeaf(chldN, prevLeaf, prevLeafByteAfterIdx);
					NBArray_addValue(&obj->parser.tree.leaves.open, chldN);
					consumed = TRUE;
					chldN = NULL; //retained by leaves array
				}
			}
			//Add children
			if(chld->childrn != NULL && chld->childrnSz > 0){
				//Create node (if necesary)
				if(chldN == NULL){
					chldN = NBSintaxParser_nodeCreate();
					{
						chldN->partIdx = chld->idx;
						NBSintaxParser_nodeSetParent(chldN, parent);
					}
				}
				//Add children
				{
					BOOL consumed2 = FALSE;
					NBSintaxParser_treeAddStartPosibsUtf(obj, chldN, chld, bytesToAdd, baseResultMsk, closeNewFarLeaves, prevLeaf, prevLeafByteAfterIdx, &consumed2);
					if(consumed2){
						consumed = TRUE;
					}
				}
			}
			//Release (is retained by children)
			if(chldN != NULL){
				NBSintaxParser_nodeRelease(chldN);
				chldN = NULL;
			}
		}
	}
	//
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

void NBSintaxParser_treeAddStartPosibsToken(STNBSintaxParser* obj, STNBSintaxParserPartNode* parent, const STNBSintaxParserElemHintStartNode* posibs, const UI32 iElemToken, const char* token, const UI32 tokenSz, const UI32 baseResultMsk /*ENSintaxParserValidateElemResultBit*/, const BOOL closeNewFarLeaves, STNBSintaxParserPartNode* prevLeaf, const UI32 prevLeafByteAfterIdx, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	if(posibs->childrn != NULL && posibs->childrnSz > 0){
		UI32 i; for(i = 0; i < posibs->childrnSz; i++){
			const STNBSintaxParserElemHintStartNode* chld = &posibs->childrn[i];
			//Create new posib (if necesary)
			STNBSintaxParserPartNode* chldN = NBSintaxParser_nodeCreate();
			//Add as posibility
			{
				chldN->partIdx = chld->idx;
				NBSintaxParser_nodeSetParent(chldN, parent);
			}
			//Add as leaf
			if(chld->isLeaf){
				//NBASSERT(chldN->partIdx.iElem == iElemToken) //Leaf must be same type as token
				//Validate leaf is iElemToken type
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				{
					STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
					NBASSERT(chld->idx.iElem >= 0 && chld->idx.iElem < rules->elems.use)
					if(chld->idx.iElem >= 0 && chld->idx.iElem < rules->elems.use){
						const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, chld->idx.iElem);
						switch(ee->type) {
						case ENSintaxParserElemType_Literal:
							NBASSERT(NBString_strIsEqualBytes(ee->name, ee->nameLen, token, tokenSz))
							break;
						case ENSintaxParserElemType_Callback:
							NBASSERT(FALSE)
							break;
						case ENSintaxParserElemType_Elem:
							NBASSERT(chld->idx.iDef >= 0 && chld->idx.iDef < ee->defsSz)
							if(chld->idx.iDef >= 0 && chld->idx.iDef < ee->defsSz){
								const STNBSintaxParserElemDef* def = &ee->defs[chld->idx.iDef];
								NBASSERT(chld->idx.iPart >= 0 && chld->idx.iPart < def->partsSz)
								if(chld->idx.iPart >= 0 && chld->idx.iPart < def->partsSz){
									const STNBSintaxParserElemDefPart* part = &def->parts[chld->idx.iPart];
									NBASSERT(part->iElem >= 0 && part->iElem < rules->elems.use)
									if(part->iElem >= 0 && part->iElem < rules->elems.use){
										const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
										NBASSERT(part->iElem == iElemToken)
										NBASSERT(ee2->nameLen > 0)
										NBASSERT(!NBString_strIsEmpty(ee2->name))
									}
								}
							}
							break;
						default:
							NBASSERT(FALSE)
							break;
						}
					}
				}
#				endif
				//Set initial props maks
				{
					if(chldN->leafProps == NULL){
						chldN->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
						NBMemory_setZeroSt(*chldN->leafProps, STNBSintaxParserPartNodeLeafProps);;
					}
					{
						STNBSintaxParserPartNodeLeafProps* nProps = chldN->leafProps;
						nProps->iByteStart		= obj->parser.relAccum.length;
						nProps->iByteAfter		= nProps->iByteStart + tokenSz;
						nProps->resultMskCur	|= (baseResultMsk | chld->resultMask);
						nProps->resultMskLast	|= (baseResultMsk | chld->resultMask);
						nProps->isClosed		= closeNewFarLeaves;
						NBASSERT(nProps->resultMskCur == nProps->resultMskLast) //Should be the same at this stage
					}
				}
				//Register as leaf node
				{
					NBSintaxParser_nodeSetPrevLeaf(chldN, prevLeaf, prevLeafByteAfterIdx);
					NBArray_addValue(&obj->parser.tree.leaves.open, chldN);
					consumed = TRUE;
					chldN = NULL; //retained by leaves array
				}
			}
			//Add children
			if(chld->childrn != NULL && chld->childrnSz > 0){
				//Create node (if necesary)
				if(chldN == NULL){
					chldN = NBSintaxParser_nodeCreate();
					{
						chldN->partIdx = chld->idx;
						NBSintaxParser_nodeSetParent(chldN, parent);
					}
				}
				//Add children
				{
					BOOL consumed2 = FALSE;
					NBSintaxParser_treeAddStartPosibsToken(obj, chldN, chld, iElemToken, token, tokenSz, baseResultMsk, closeNewFarLeaves, prevLeaf, prevLeafByteAfterIdx, &consumed2);
					if(consumed2){
						consumed = TRUE;
					}
				}
			}
			//Release (is retained by children)
			if(chldN != NULL){
				NBSintaxParser_nodeRelease(chldN);
				chldN = NULL;
			}
		}
	}
	//
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

//First utfChar

void NBSintaxParser_treeFeedFirstUtf(STNBSintaxParser* obj, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser = &obj->parser;
	NBASSERT(parser->tree.leaves.open.use == 0)
	NBASSERT(parser->tree.leaves.completed.use == 0)
	//
	{
		//Add start parts
		UI32 i; for(i = 0; i < parser->tree.root.elemsIdxs.use; i++){
			const UI32 iElem = NBArray_itmValueAtIndex(&parser->tree.root.elemsIdxs, UI32, i);
			NBASSERT(iElem >= 0 && iElem < obj->rules->elems.use)
			BOOL consumed2 = FALSE;
			NBSintaxParser_treeFeedFirstUtfElem(obj, &parser->tree.root.node, &parser->tree.root.node, 0, iElem, utfChar, utfCharSz, &consumed2);
			if(consumed2){
				consumed = TRUE;
			}
		}
	}
	//
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

void NBSintaxParser_treeFeedFirstUtfElem(STNBSintaxParser* obj, STNBSintaxParserPartNode** parentRef, STNBSintaxParserPartNode** prevLeafRef, const UI32 prevLeafByteAfterIdx, const UI32 iElem, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	STNBSintaxParserPartNode* parent = NULL;
	STNBSintaxParserPartNode* prevLeaf = NULL;
	BOOL parentMustRelease			= FALSE;
	BOOL prevLeafMustRelease		= FALSE;
	if(parentRef != NULL) parent = *parentRef;
	if(prevLeafRef != NULL) prevLeaf = *prevLeafRef;
	//
	{
		//Add start parts
		NBASSERT(iElem < obj->rules->elems.use)
		if(iElem < obj->rules->elems.use){
			const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, iElem);
			//Literal starts (binary seach)
			{
				const STNBArraySorted* arr = &ee->hints.starts[ENSintaxParserElemType_Literal];
				if(arr->use > 0){
					STNBSintaxParserElemHintStart srch;
					srch.type		= ENSintaxParserElemType_Literal;
					srch.utf.val	= (char*)utfChar;
					srch.utf.valSz	= utfCharSz;
					{
						const SI32 iFnd = NBArraySorted_indexOf(arr, &srch, sizeof(srch), NULL);
						if(iFnd >= 0){
							const STNBSintaxParserElemHintStart* start = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, iFnd);
							NBASSERT(start->type == ENSintaxParserElemType_Literal)
							NBASSERT(NBString_strIsEqualBytes(start->utf.val, start->utf.valSz, utfChar, utfCharSz))
							BOOL consumed2 = FALSE;
							//Create parent node (if necesary)
							if(parent == NULL){
								parent = NBSintaxParser_nodeCreate();
								parentMustRelease = TRUE;
							}
							if(prevLeaf == NULL){
								prevLeaf = NBSintaxParser_nodeCreate();
								prevLeafMustRelease = TRUE;
							}
							//Add posibs
							NBSintaxParser_treeAddStartPosibsUtf(obj, parent, &start->posibs, utfCharSz, ENSintaxParserValidateElemResultBit_None, FALSE, prevLeaf, prevLeafByteAfterIdx, &consumed2);
							if(consumed2){
								consumed = TRUE;
							}
							NBASSERT(consumed2);
						}
					}
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					if(parent == obj->parser.tree.root.node){
						NBSintaxParser_dbgValidateTree(obj, utfCharSz);
					}
#					endif
				}
			}
			//Callback starts
			{
				const STNBArraySorted* arr = &ee->hints.starts[ENSintaxParserElemType_Callback];
				if(arr->use > 0){
					SI32 i; for(i = (SI32)arr->use - 1; i >= 0; i--){
						const STNBSintaxParserElemHintStart* start = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, i);
						NBASSERT(start->type == ENSintaxParserElemType_Callback)
						NBASSERT(start->iElem >= 0 && start->iElem < obj->rules->elems.use)
						if(obj->callbacks.validateElem == NULL){
							PRINTF_ERROR("NBSintaxParser, validateElem callback not set.\n");
							parser->errFnd = TRUE;
							if(obj->callbacks.errorFound != NULL){
								(obj->callbacks.errorFound)(obj->callbacks.userParam, "ValidateElem callback not set.");
							}
							break;
						} else {
							NBASSERT(start->iElem >= 0 && start->iElem < obj->rules->elems.use)
							const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, start->iElem);
							const UI32 resultMsk = (*obj->callbacks.validateElem)(obj->callbacks.userParam, start->iElem, ee2->name, parser->relAccum.str, parser->relAccum.length, 0, utfChar, utfCharSz);
							if((resultMsk & (ENSintaxParserValidateElemResultBit_Partial | ENSintaxParserValidateElemResultBit_Complete)) != 0){
								BOOL consumed2 = FALSE;
								//Create parent node (if necesary)
								if(parent == NULL){
									parent = NBSintaxParser_nodeCreate();
									parentMustRelease = TRUE;
								}
								if(prevLeaf == NULL){
									prevLeaf = NBSintaxParser_nodeCreate();
									prevLeafMustRelease = TRUE;
								}
								//Add posibs
								NBSintaxParser_treeAddStartPosibsUtf(obj, parent, &start->posibs, utfCharSz, resultMsk, FALSE, prevLeaf, prevLeafByteAfterIdx, &consumed2);
								if(consumed2){
									consumed = TRUE;
								}
								NBASSERT(consumed2);
							}
						}
					}
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					if(parent == obj->parser.tree.root.node){
						NBSintaxParser_dbgValidateTree(obj, utfCharSz);
					}
#					endif
				}
			}
		}
	}
	//Release nodes created here
	if(parent != NULL && parentMustRelease){
		NBSintaxParser_nodeRelease(parent);
	}
	if(prevLeaf != NULL && prevLeafMustRelease){
		NBSintaxParser_nodeRelease(prevLeaf);
	}
	//
	if(parentRef != NULL) *parentRef = parent;
	if(prevLeafRef != NULL) *prevLeafRef = prevLeaf;
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

//First token

void NBSintaxParser_treeFeedFirstToken(STNBSintaxParser* obj, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	NBASSERT(parser->tree.leaves.open.use == 0)
	NBASSERT(parser->tree.leaves.completed.use == 0)
	//Add start parts by iElemTkn
	{
		UI32 i; for(i = 0; i < parser->tree.root.elemsIdxs.use; i++){
			const UI32 iElem = NBArray_itmValueAtIndex(&parser->tree.root.elemsIdxs, UI32, i);
			NBASSERT(iElem >= 0 && iElem < obj->rules->elems.use)
			const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, iElem);
			const STNBArraySorted* starts = &ee->hints.starts[ENSintaxParserElemType_Elem];
			if(starts->use > 0){
				const STNBSintaxParserElemHintStart* arrStarts = NBArraySorted_dataPtr(starts, STNBSintaxParserElemHintStart);
				SI32 iStart; for(iStart = 0; iStart < starts->use; iStart++){
					const STNBSintaxParserElemHintStart* start = &arrStarts[iStart];
					NBASSERT(start->type == ENSintaxParserElemType_Elem)
					NBASSERT(start->iElem >= 0 && start->iElem < rules->elems.use)
					BOOL isMatch = (start->iElem == iElemToken);
					if(!isMatch){
						const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, start->iElem);
						if(ee2->type == ENSintaxParserElemType_Literal){
							if(NBString_strIsEqualBytes(ee2->name, ee2->nameLen, token, tokenSz)){
								isMatch = TRUE;
							}
						}
					}
					if(isMatch){
						BOOL consumed2 = FALSE;
						NBSintaxParser_treeFeedFirstTokenElem(obj, &parser->tree.root.node, &parser->tree.root.node, 0, iElem, start->iElem, token, tokenSz, &consumed2);
						if(consumed2){
							consumed = TRUE;
						}
					}
				}
			}
		}
	}
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

void NBSintaxParser_treeFeedFirstTokenElem(STNBSintaxParser* obj, STNBSintaxParserPartNode** parentRef, STNBSintaxParserPartNode** prevLeafRef, const UI32 prevLeafByteAfterIdx, const UI32 iElem, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	//STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	STNBSintaxParserPartNode* parent = NULL;
	STNBSintaxParserPartNode* prevLeaf = NULL;
	BOOL parentMustRelease			= FALSE;
	BOOL prevLeafMustRelease		= FALSE;
	if(parentRef != NULL) parent = *parentRef;
	if(prevLeafRef != NULL) prevLeaf = *prevLeafRef;
	//
	{
		//Add start parts
		NBASSERT(iElem < obj->rules->elems.use)
		if(iElem < obj->rules->elems.use){
			const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, iElem);
			//Elem starts (binary seach)
			{
				const STNBArraySorted* arr = &ee->hints.starts[ENSintaxParserElemType_Elem];
				if(arr->use > 0){
					STNBSintaxParserElemHintStart srch;
					srch.type		= ENSintaxParserElemType_Elem;
					srch.iElem		= iElemToken;
					{
						const SI32 iFnd = NBArraySorted_indexOf(arr, &srch, sizeof(srch), NULL);
						if(iFnd >= 0){
							const STNBSintaxParserElemHintStart* start = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, iFnd);
							NBASSERT(start->type == ENSintaxParserElemType_Elem)
							NBASSERT(start->iElem == srch.iElem)
							BOOL consumed2 = FALSE;
							//Create parent node (if necesary)
							if(parent == NULL){
								parent = NBSintaxParser_nodeCreate();
								parentMustRelease = TRUE;
							}
							if(prevLeaf == NULL){
								prevLeaf = NBSintaxParser_nodeCreate();
								prevLeafMustRelease = TRUE;
							}
							//Add posibs
							NBSintaxParser_treeAddStartPosibsToken(obj, parent, &start->posibs, iElemToken, token, tokenSz, ENSintaxParserValidateElemResultBit_None, TRUE, prevLeaf, prevLeafByteAfterIdx, &consumed2);
							if(consumed2){
								consumed = TRUE;
							}
							//NBASSERT(consumed2); //ToDo: remove
						}
					}
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					if(parent == obj->parser.tree.root.node){
						NBSintaxParser_dbgValidateTree(obj, tokenSz);
					}
#					endif
				}
			}
		}
	}
	//Release nodes created here
	if(parent != NULL && parentMustRelease){
		NBSintaxParser_nodeRelease(parent);
	}
	if(prevLeaf != NULL && prevLeafMustRelease){
		NBSintaxParser_nodeRelease(prevLeaf);
	}
	//
	if(parentRef != NULL) *parentRef = parent;
	if(prevLeafRef != NULL) *prevLeafRef = prevLeaf;
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

//Next utfChar

void NBSintaxParser_treeFeedNextUtf(STNBSintaxParser* obj, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	STNBArray* leaves = &parser->tree.leaves.open;
	NBASSERT(parser->tree.leaves.open.use != 0 || parser->tree.leaves.completed.use != 0)
	//Analyze leaves
	//Calculate new states
	if(!parser->errFnd){
		SI32 i; for(i = (SI32)leaves->use - 1; i >= 0; i--){
			STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
			NBASSERT(n->leafProps != NULL)
			if(n->leafProps != NULL){
				STNBSintaxParserPartNodeLeafProps* nProps = n->leafProps;
				NBASSERT(n->parent != NULL) //All leaves must have a parent node defined
				NBASSERT(n->partIdx.iElem >= 0 && n->partIdx.iElem < obj->rules->elems.use)
				const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, n->partIdx.iElem);
				//Set the old state
				nProps->resultMskLast = nProps->resultMskCur;
				if(nProps->isClosed){
					//Not allowed to grow anymore
					nProps->resultMskCur = ENSintaxParserValidateElemResultBit_None;
				} else {
					//Set the new state
					switch (ee2->type) {
						case ENSintaxParserElemType_Literal:
							{
								const UI32 curSz	= (nProps->iByteAfter - nProps->iByteStart);
								const UI32 aftrSz	= (curSz + utfCharSz); 
								nProps->resultMskCur = ENSintaxParserValidateElemResultBit_None;
								if(aftrSz <= ee2->nameLen){
									//Compare
									if(NBString_strIsEqualBytes(utfChar, utfCharSz, &ee2->name[curSz], utfCharSz)){
										nProps->resultMskCur = (aftrSz == ee2->nameLen ? ENSintaxParserValidateElemResultBit_Complete : ENSintaxParserValidateElemResultBit_Partial);
										consumed = TRUE;
									}
								}
							}
							break;
						case ENSintaxParserElemType_Callback:
							if(n->parent == NULL){
								//Expected a parent node
								PRINTF_ERROR("NBSintaxParser, expected a parent for callback node.\n");
								parser->errFnd = TRUE;
								if(obj->callbacks.errorFound != NULL){
									(obj->callbacks.errorFound)(obj->callbacks.userParam, "Expected a parent for callback node.");
								}
							} else if(obj->callbacks.validateElem == NULL){
								//Expected a callback
								PRINTF_ERROR("NBSintaxParser, callback 'validateElem' is not set.\n");
								parser->errFnd = TRUE;
								if(obj->callbacks.errorFound != NULL){
									(obj->callbacks.errorFound)(obj->callbacks.userParam, "Callback 'validateElem' is not set.");
								}
							} else {
								const UI32 curSz		= (nProps->iByteAfter - nProps->iByteStart);
								const STNBSintaxParserElem* ee3 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, n->parent->partIdx.iElem);
								nProps->resultMskCur	= (UI8)(*obj->callbacks.validateElem)(obj->callbacks.userParam, n->parent->partIdx.iElem, ee3->name, parser->relAccum.str, parser->relAccum.length, curSz, utfChar, utfCharSz);
							}
							break;
						case ENSintaxParserElemType_Elem:
							NBASSERT(FALSE) //Testing
							break;
						default:
							break;
					}
				}
			}
		}
	}
	//Print current tree
	NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, utfChar, utfCharSz, NULL, "after calculating states")
	//Apply states changes
	if(!parser->errFnd){
		//Progress previous completed posibilities
		SI32 i; for(i = (SI32)leaves->use - 1; i >= 0; i--){
			STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
			NBASSERT(n->leafProps != NULL)
			if(n->leafProps != NULL){
				STNBSintaxParserPartNodeLeafProps* nProps = n->leafProps;
				NBASSERT(n->parent != NULL) //All leaves must have a parent node defined
				NBASSERT(n->partIdx.iElem >= 0 && n->partIdx.iElem < obj->rules->elems.use)
				const BOOL isMatch			= (((nProps->resultMskCur & (ENSintaxParserValidateElemResultBit_Partial | ENSintaxParserValidateElemResultBit_Complete))) != 0);
				const BOOL wasComplete		= ((nProps->resultMskLast & ENSintaxParserValidateElemResultBit_Complete) != 0); 
				const BOOL isNotComplete	= ((nProps->resultMskCur & ENSintaxParserValidateElemResultBit_Complete) == 0);
				//Print parents path
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				/*{
					UI32 pCount = 0;
					STNBSintaxParserPartNode* leaf2 = n;
					while(leaf2->parent != NULL){
						const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, leaf2->partIdx.iElem);
						PRINTF_INFO("NBSintaxParser, parent-%d '%s'.\n", pCount, ee2->name);
						leaf2 = leaf2->parent;
						pCount++;
					}
				}*/
#				endif
				if(isMatch){
					nProps->iByteAfter		+= utfCharSz;
				}
				if(wasComplete && isNotComplete){
					//const BOOL isPartial	= ((n->resultMskCur & ENSintaxParserValidateElemResultBit_Partial) != 0);
					//const BOOL isUniqPosib = (leaves->use == 1);
					//BOOL cloneFirstPosib	= (isPartial || !isUniqPosib);
					BOOL consumed2			= FALSE;
					NBSintaxParser_treeFeedNextPartWithUtf(obj, n, utfChar, utfCharSz, &consumed2);
					if(consumed2){
						consumed = TRUE;
					} 
				}
				//
				if(isMatch){
					consumed = TRUE;
				} else {
					//Remove leaf
					nProps->resultMskLast = nProps->resultMskCur = ENSintaxParserValidateElemResultBit_None;
					NBSintaxParser_nodeRelease(n);
					NBArray_removeItemAtIndex(leaves, i);
				} 
			}
		}
		//Print current tree
		NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, utfChar, utfCharSz, NULL, "after applying states")
		IF_NBASSERT(NBSintaxParser_dbgValidateTree(obj, utfCharSz);)
	}
	//
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

void NBSintaxParser_treeFeedNextPartWithUtf(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const char* utfChar, const UI32 utfCharSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	NBASSERT(nLeafBottom->leafProps != NULL)
	if(nLeafBottom->leafProps != NULL){
		STNBSintaxParserPartNodeLeafProps* nLeafBottomProps = nLeafBottom->leafProps;
		STNBSintaxParserPartNode* nLeafCur = nLeafBottom;
		UI32 iPartNext = (nLeafCur->partIdx.iPart + 1);
		UI32 iElemRoot = nLeafCur->partIdx.iElem;
		UI32 iElemRootChild = rules->elems.use;
		while(nLeafCur != NULL){
			//Obtain elem
			NBASSERT(nLeafCur->partIdx.iElem >= 0 && nLeafCur->partIdx.iElem < obj->rules->elems.use)
			const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->partIdx.iElem);
			const STNBSintaxParserElemDef* def = NULL;
			UI32 partsSz = 0;
			if(ee->type == ENSintaxParserElemType_Elem){
				NBASSERT(nLeafCur->partIdx.iDef >= 0 && nLeafCur->partIdx.iDef < ee->defsSz)
				//Obtain definition
				def = &ee->defs[nLeafCur->partIdx.iDef];
				//Define next part
				NBASSERT(nLeafCur->partIdx.iPart >= 0 && nLeafCur->partIdx.iPart < def->partsSz)
				NBASSERT(iPartNext >= 0 && iPartNext <= def->partsSz)
				partsSz = def->partsSz;
			}
			if(iPartNext >= partsSz){
				//Advance leaf content //ToDo: remove
				//nLeafCur->iByteAfter = nLeafBottom->iByteAfter; //obj->parser.relAccum.length;
				//Move to parent
				if(nLeafCur->parent == parser->tree.root.node){
					//Add as completed
					NBASSERT(iElemRoot == nLeafCur->partIdx.iElem)
					NBSintaxParser_treeAddCompletedBranch(obj, nLeafBottom, iElemRoot, iElemRootChild);
					//End cicle
					nLeafCur = NULL;
				} else {
					//Analyze recursivity
					if(ee->hints.canBeRecursive){
						if(ee->defs != NULL && ee->defsSz > 0 ){
							UI32 iDef; for(iDef = 0; iDef < ee->defsSz; iDef++){
								const STNBSintaxParserElemDef* def = &ee->defs[iDef];
								if(def->parts != NULL && def->partsSz > 0){
									SI32 iRecursivePart = -1; BOOL nonOptionalFnd = FALSE;
									UI32 iPart; for(iPart = 0; iPart < def->partsSz; iPart++){
										const STNBSintaxParserElemDefPart* part = &def->parts[iPart];
										//Analyze
										if(iRecursivePart != -1){
											//Try recursive posibility
											BOOL consumed2 = FALSE;
											STNBSintaxParserPartNode* newPosib = NULL;
											STNBSintaxParserPartNode* newNextNode = NULL;
											NBSintaxParser_treeFeedFirstUtfElem(obj, &newNextNode, &newPosib, nLeafBottomProps->iByteAfter, part->iElem, utfChar, utfCharSz, &consumed2);
											NBASSERT((newPosib == NULL && !consumed2) || (newPosib != NULL && consumed2))
											NBASSERT((newNextNode == NULL && !consumed2) || (newNextNode != NULL && consumed2))
											if(consumed2){
												consumed = TRUE;
											}
											if(newPosib != NULL && newNextNode != NULL){
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, cloning recursivity.\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//Config mid posib
												{
													newPosib->partIdx.iElem = nLeafCur->partIdx.iElem;
													newPosib->partIdx.iDef	= iDef;
													newPosib->partIdx.iPart	= iRecursivePart;
													NBSintaxParser_nodeSetParent(newPosib, nLeafCur->parent); NBASSERT(nLeafCur->parent != NULL)
													NBSintaxParser_nodeSetChildLeafRef(newPosib, nLeafCur);
													//Set initial props maks
													{
														if(newPosib->leafProps == NULL){
															newPosib->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
															NBMemory_setZeroSt(*newPosib->leafProps, STNBSintaxParserPartNodeLeafProps);;
														}
														{
															STNBSintaxParserPartNodeLeafProps* nProps = newPosib->leafProps;
															nProps->iByteStart		= nLeafBottomProps->iByteAfter;
															nProps->iByteAfter		= nLeafBottomProps->iByteAfter;
														}
														NBSintaxParser_nodeSetPrevLeaf(newPosib, nLeafBottom, nLeafBottomProps->iByteAfter);
													}
#													ifdef NB_CONFIG_INCLUDE_ASSERTS
													/*{
														NBASSERT(nLeafCur->partIdx.iElem >= 0 && nLeafCur->partIdx.iElem < obj->rules->elems.use)
														const STNBSintaxParserElem* eeP = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->partIdx.iElem);
														PRINTF_INFO("NBSintaxParser, leaf of recursive: '%s' iDef(%d) iPart(%d).\n", eeP->name, nLeafCur->partIdx.iDef, nLeafCur->partIdx.iPart);
													}*/
													/*{
														NBASSERT(nLeafCur->parent->partIdx.iElem >= 0 && nLeafCur->parent->partIdx.iElem < obj->rules->elems.use)
														const STNBSintaxParserElem* eeP = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->parent->partIdx.iElem);
														PRINTF_INFO("NBSintaxParser, parent of recursive: '%s' iDef(%d) iPart(%d).\n", eeP->name, nLeafCur->parent->partIdx.iDef, nLeafCur->parent->partIdx.iPart);
													}*/
#													endif
												}
												//Config next part
												{
													newNextNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
													newNextNode->partIdx.iDef	= iDef;
													newNextNode->partIdx.iPart	= iPart;
													NBSintaxParser_nodeSetParent(newNextNode, nLeafCur->parent);
													NBSintaxParser_nodeSetPrevious(newNextNode, newPosib);
												}
												//Print next part
#												if defined(NB_CONFIG_INCLUDE_ASSERTS) && defined(NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS)
												{
													STNBString str;
													NBString_init(&str);
													NBSintaxParser_dbgTreeConcatDeflatedPosib(obj, '?', newNextNode, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RANGES, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_PARTS_IDXS, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RETAIN_COUNT, utfChar, utfCharSz, NULL, &str);
													//PRINTF_INFO("NBSintaxParser, next clone:\n%s\n", str.str);
													NBString_release(&str);
												}
#												endif
												newPosib = NULL;
												newNextNode = NULL;
											}
										}
										//Stop
										if(!part->isOptional){
											nonOptionalFnd = TRUE;
										}
										//Start recursivity
										if(part->iElem == nLeafCur->partIdx.iElem){
											iRecursivePart = iPart;
										} else if(nonOptionalFnd){
											//Stop
											break;
										}
									}
								}
							}
						}
					}
					//Move to parent
					nLeafCur = nLeafCur->parent;
					if(nLeafCur != NULL){
						iElemRootChild	= iElemRoot;
						iElemRoot		= nLeafCur->partIdx.iElem;
						//
						iPartNext		= (nLeafCur->partIdx.iPart + 1);
					}
				}
			} else {
				//Try next parts
				const STNBSintaxParserElemDefPart* part = &def->parts[iPartNext];
				const STNBSintaxParserElem* eePart = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
				//if(eePart->hints.canBeEmpty){
				//	PRINTF_INFO("Part '%s' can be empty.\n", eePart->name);
				//}
				//PRINTF_INFO("Trying next part '%s' with utf '%s'.\n", eePart->name, utfChar);
				{
					BOOL consumed2 = FALSE;
					STNBSintaxParserPartNode* newNode = NULL;
					NBSintaxParser_treeFeedFirstUtfElem(obj, &newNode, &nLeafBottom, nLeafBottomProps->iByteAfter, part->iElem, utfChar, utfCharSz, &consumed2);
					NBASSERT((newNode == NULL && !consumed2) || (newNode != NULL && consumed2))
					if(consumed2){
						consumed = TRUE;
					}
					if(newNode != NULL){
						//Config node
						newNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
						newNode->partIdx.iDef	= nLeafCur->partIdx.iDef;
						newNode->partIdx.iPart	= iPartNext;
						NBSintaxParser_nodeSetParent(newNode, nLeafCur->parent);
						NBSintaxParser_nodeSetPrevious(newNode, nLeafCur);
						newNode = NULL;
					}
				}
				//Next step
				if(part->isOptional || eePart->hints.canBeEmpty){
					iPartNext++;
				} else {
					//End cicle
					nLeafCur = NULL;
				}
			}
		}
	}
	//
	if(dstConsumed != NULL){
		*dstConsumed = consumed;
	}
}

//Next token

void NBSintaxParser_treeFeedNextToken(STNBSintaxParser* obj, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	IF_NBASSERT(STNBSintaxParserRules* rules = obj->rules;) NBASSERT(rules != NULL)
	STNBArray* leaves = &parser->tree.leaves.open;
	NBASSERT(parser->tree.leaves.open.use != 0 || parser->tree.leaves.completed.use != 0)
	//Apply states changes
	SI32 i; for(i = (SI32)leaves->use - 1; i >= 0; i--){
		STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
		NBASSERT(n->leafProps != NULL)
		if(n->leafProps != NULL){
			STNBSintaxParserPartNodeLeafProps* nProps = n->leafProps;
			NBASSERT(n->parent != NULL) //All leaves must have a parent node defined
			NBASSERT(n->partIdx.iElem >= 0 && n->partIdx.iElem < obj->rules->elems.use)
			//Set the old state
			nProps->resultMskLast			= nProps->resultMskCur;
			//Not allowed to grow anymore
			nProps->resultMskCur			= ENSintaxParserValidateElemResultBit_None;
			nProps->isClosed				= TRUE;
			{
				const BOOL isMatch			= FALSE;
				const BOOL wasComplete		= ((nProps->resultMskLast & ENSintaxParserValidateElemResultBit_Complete) != 0); 
				const BOOL isNotComplete	= TRUE;
				//Print parents path
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				/*{
					UI32 pCount = 0;
					STNBSintaxParserPartNode* leaf2 = n;
					while(leaf2->parent != NULL){
						const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, leaf2->partIdx.iElem);
						PRINTF_INFO("NBSintaxParser, parent-%d '%s'.\n", pCount, ee2->name);
						leaf2 = leaf2->parent;
						pCount++;
					}
				}*/
#				endif
				if(isMatch){
					nProps->iByteAfter		+= tokenSz;
				}
				if(wasComplete && isNotComplete){
					//const BOOL isPartial	= ((n->resultMskCur & ENSintaxParserValidateElemResultBit_Partial) != 0);
					//const BOOL isUniqPosib = (leaves->use == 1);
					//BOOL cloneFirstPosib	= (isPartial || !isUniqPosib);
					BOOL consumed2			= FALSE;
					NBSintaxParser_treeFeedNextPartWithToken(obj, n, iElemToken, token, tokenSz, &consumed2);
					if(consumed2){
						consumed = TRUE;
					} 
				}
				//
				if(isMatch){
					consumed = TRUE;
				} else {
					//Remove leaf
					nProps->resultMskLast = nProps->resultMskCur = ENSintaxParserValidateElemResultBit_None;
					NBSintaxParser_nodeRelease(n);
					NBArray_removeItemAtIndex(leaves, i);
				}
			}
		}
	}
	//Print current tree
	NBSINTAX_PARSER_DEBUG_PRINT_TREE(obj, token, tokenSz, NULL, "after applying states")
	IF_NBASSERT(NBSintaxParser_dbgValidateTree(obj, tokenSz);)
	//
	if(dstConsumed != NULL) *dstConsumed = consumed;
}

void NBSintaxParser_treeFeedNextPartWithToken(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const UI32 iElemToken, const char* token, const UI32 tokenSz, BOOL* dstConsumed){
	BOOL consumed = FALSE;
	STNBSintaxParserState* parser	= &obj->parser;
	STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	NBASSERT(nLeafBottom->leafProps != NULL)
	if(nLeafBottom->leafProps != NULL){
		STNBSintaxParserPartNodeLeafProps* nLeafBottomProps = nLeafBottom->leafProps;
		STNBSintaxParserPartNode* nLeafCur = nLeafBottom;
		UI32 iPartNext = (nLeafCur->partIdx.iPart + 1);
		UI32 iElemRoot = nLeafCur->partIdx.iElem;
		UI32 iElemRootChild = rules->elems.use;
		while(nLeafCur != NULL){
			//Obtain elem
			NBASSERT(nLeafCur->partIdx.iElem >= 0 && nLeafCur->partIdx.iElem < obj->rules->elems.use)
			const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->partIdx.iElem);
			const STNBSintaxParserElemDef* def = NULL;
			UI32 partsSz = 0;
			if(ee->type == ENSintaxParserElemType_Elem){
				NBASSERT(nLeafCur->partIdx.iDef >= 0 && nLeafCur->partIdx.iDef < ee->defsSz)
				//Obtain definition
				def = &ee->defs[nLeafCur->partIdx.iDef];
				//Define next part
				NBASSERT(nLeafCur->partIdx.iPart >= 0 && nLeafCur->partIdx.iPart < def->partsSz)
				NBASSERT(iPartNext >= 0 && iPartNext <= def->partsSz)
				partsSz = def->partsSz;
			}
			if(iPartNext >= partsSz){
				//Advance leaf content //ToDo: remove
				//nLeafCur->iByteAfter = nLeafBottom->iByteAfter; //obj->parser.relAccum.length;
				//Move to parent
				if(nLeafCur->parent == parser->tree.root.node){
					//Add as completed
					NBASSERT(iElemRoot == nLeafCur->partIdx.iElem)
					NBSintaxParser_treeAddCompletedBranch(obj, nLeafBottom, iElemRoot, iElemRootChild);
					//End cicle
					nLeafCur = NULL;
				} else {
					//Analyze recursivity
					if(ee->hints.canBeRecursive){
						if(ee->defs != NULL && ee->defsSz > 0 ){
							UI32 iDef; for(iDef = 0; iDef < ee->defsSz; iDef++){
								const STNBSintaxParserElemDef* def = &ee->defs[iDef];
								if(def->parts != NULL && def->partsSz > 0){
									SI32 iRecursivePart = -1; BOOL nonOptionalFnd = FALSE;
									UI32 iPart; for(iPart = 0; iPart < def->partsSz; iPart++){
										const STNBSintaxParserElemDefPart* part = &def->parts[iPart];
										//Analyze
										if(iRecursivePart != -1){
											//Try recursive posibility
											BOOL consumed2 = FALSE;
											STNBSintaxParserPartNode* newPosib = NULL;
											STNBSintaxParserPartNode* newNextNode = NULL;
											NBSintaxParser_treeFeedFirstTokenElem(obj, &newNextNode, &newPosib, nLeafBottomProps->iByteAfter, part->iElem, iElemToken, token, tokenSz, &consumed2);
											//ToDo: analyze literals
											NBASSERT((newPosib == NULL && !consumed2) || (newPosib != NULL && consumed2))
											NBASSERT((newNextNode == NULL && !consumed2) || (newNextNode != NULL && consumed2))
											if(consumed2){
												consumed = TRUE;
											}
											if(newPosib != NULL && newNextNode != NULL){
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, cloning recursivity.\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//PRINTF_INFO("NBSintaxParser, -------------------\n");
												//Config mid posib
												{
													newPosib->partIdx.iElem = nLeafCur->partIdx.iElem;
													newPosib->partIdx.iDef	= iDef;
													newPosib->partIdx.iPart	= iRecursivePart;
													NBSintaxParser_nodeSetParent(newPosib, nLeafCur->parent); NBASSERT(nLeafCur->parent != NULL)
													NBSintaxParser_nodeSetChildLeafRef(newPosib, nLeafCur);
													//Set initial props maks
													{
														if(newPosib->leafProps == NULL){
															newPosib->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
															NBMemory_setZeroSt(*newPosib->leafProps, STNBSintaxParserPartNodeLeafProps);;
														}
														{
															STNBSintaxParserPartNodeLeafProps* nProps = newPosib->leafProps;
															nProps->iByteStart		= nLeafBottomProps->iByteAfter;
															nProps->iByteAfter		= nLeafBottomProps->iByteAfter;
														}
														NBSintaxParser_nodeSetPrevLeaf(newPosib, nLeafBottom, nLeafBottomProps->iByteAfter);
													}
#													ifdef NB_CONFIG_INCLUDE_ASSERTS
													/*{
														NBASSERT(nLeafCur->partIdx.iElem >= 0 && nLeafCur->partIdx.iElem < obj->rules->elems.use)
														const STNBSintaxParserElem* eeP = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->partIdx.iElem);
														PRINTF_INFO("NBSintaxParser, leaf of recursive: '%s' iDef(%d) iPart(%d).\n", eeP->name, nLeafCur->partIdx.iDef, nLeafCur->partIdx.iPart);
													}*/
													/*{
														NBASSERT(nLeafCur->parent->partIdx.iElem >= 0 && nLeafCur->parent->partIdx.iElem < obj->rules->elems.use)
														const STNBSintaxParserElem* eeP = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->parent->partIdx.iElem);
														PRINTF_INFO("NBSintaxParser, parent of recursive: '%s' iDef(%d) iPart(%d).\n", eeP->name, nLeafCur->parent->partIdx.iDef, nLeafCur->parent->partIdx.iPart);
													}*/
#													endif
												}
												//Config next part
												{
													newNextNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
													newNextNode->partIdx.iDef	= iDef;
													newNextNode->partIdx.iPart	= iPart;
													NBSintaxParser_nodeSetParent(newNextNode, nLeafCur->parent);
													NBSintaxParser_nodeSetPrevious(newNextNode, newPosib);
												}
												//Print next part
#												if defined(NB_CONFIG_INCLUDE_ASSERTS) && defined(NBSINTAX_PARSER_DEBUG_PRINT_TREE_AFTER_ACTIONS)
												{
													STNBString str;
													NBString_init(&str);
													NBSintaxParser_dbgTreeConcatDeflatedPosib(obj, '?', newNextNode, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RANGES, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_PARTS_IDXS, NBSINTAX_PARSER_DEBUG_PRINT_TREE_INCLUDE_RETAIN_COUNT, token, tokenSz, NULL, &str);
													//PRINTF_INFO("NBSintaxParser, next clone:\n%s\n", str.str);
													NBString_release(&str);
												}
#												endif
												newPosib = NULL;
												newNextNode = NULL;
											}
										}
										//Stop
										if(!part->isOptional){
											nonOptionalFnd = TRUE;
										}
										//Start recursivity
										if(part->iElem == nLeafCur->partIdx.iElem){
											iRecursivePart = iPart;
										} else if(nonOptionalFnd){
											//Stop
											break;
										}
									}
								}
							}
						}
					}
					//Move to parent
					nLeafCur = nLeafCur->parent;
					if(nLeafCur != NULL){
						iElemRootChild	= iElemRoot;
						iElemRoot		= nLeafCur->partIdx.iElem;
						//
						iPartNext		= (nLeafCur->partIdx.iPart + 1);
					}
				}
			} else {
				//Try next parts
				const STNBSintaxParserElemDefPart* part = &def->parts[iPartNext];
				const STNBSintaxParserElem* eePart = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
				//if(eePart->hints.canBeEmpty){
				//	PRINTF_INFO("Part '%s' can be empty.\n", eePart->name);
				//}
				//PRINTF_INFO("Trying next part '%s' with token '%s'.\n", eePart->name, token);
				switch (eePart->type) {
					case ENSintaxParserElemType_Literal:
						//Compare literal
						if(NBString_strIsEqualBytes(eePart->name, eePart->nameLen, token, tokenSz)){
							STNBSintaxParserPartNode* partNode = NBSintaxParser_nodeCreate();
							//Config part node
							{
								partNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
								partNode->partIdx.iDef	= nLeafCur->partIdx.iDef;
								partNode->partIdx.iPart	= iPartNext;
								NBSintaxParser_nodeSetParent(partNode, nLeafCur->parent);
								NBSintaxParser_nodeSetPrevious(partNode, nLeafCur);
							}
							{
								if(partNode->leafProps == NULL){
									partNode->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
									NBMemory_setZeroSt(*partNode->leafProps, STNBSintaxParserPartNodeLeafProps);;
								}
								STNBSintaxParserPartNodeLeafProps* nProps = partNode->leafProps;
								nProps->iByteStart		= obj->parser.relAccum.length;
								nProps->iByteAfter		= nProps->iByteStart + tokenSz;
								nProps->resultMskCur	|= ENSintaxParserValidateElemResultBit_Complete;
								nProps->resultMskLast	|= ENSintaxParserValidateElemResultBit_Complete;
								nProps->isClosed		= TRUE;
								NBASSERT(nProps->resultMskCur == nProps->resultMskLast) //Should be the same at this stage
							}
							//Register as leaf node
							{
								NBSintaxParser_nodeSetPrevLeaf(partNode, nLeafBottom, nLeafBottomProps->iByteAfter);
								NBArray_addValue(&obj->parser.tree.leaves.open, partNode);
								consumed = TRUE;
							}
							partNode = NULL; //retained by litrlNode
						}
						break;
					case ENSintaxParserElemType_Callback:
						NBASSERT(FALSE)
						break;
					case ENSintaxParserElemType_Elem:
						//Add by type
						if(part->iElem == iElemToken){
							STNBSintaxParserPartNode* partNode = NBSintaxParser_nodeCreate();
							//Config part node
							{
								partNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
								partNode->partIdx.iDef	= nLeafCur->partIdx.iDef;
								partNode->partIdx.iPart	= iPartNext;
								NBSintaxParser_nodeSetParent(partNode, nLeafCur->parent);
								NBSintaxParser_nodeSetPrevious(partNode, nLeafCur);
							}
							{
								if(partNode->leafProps == NULL){
									partNode->leafProps = NBMemory_allocType(STNBSintaxParserPartNodeLeafProps);
									NBMemory_setZeroSt(*partNode->leafProps, STNBSintaxParserPartNodeLeafProps);;
								}
								STNBSintaxParserPartNodeLeafProps* nProps = partNode->leafProps;
								nProps->iByteStart		= obj->parser.relAccum.length;
								nProps->iByteAfter		= nProps->iByteStart + tokenSz;
								nProps->resultMskCur	|= ENSintaxParserValidateElemResultBit_Complete;
								nProps->resultMskLast	|= ENSintaxParserValidateElemResultBit_Complete;
								nProps->isClosed		= TRUE;
								NBASSERT(nProps->resultMskCur == nProps->resultMskLast) //Should be the same at this stage
							}
							//Register as leaf node
							{
								NBSintaxParser_nodeSetPrevLeaf(partNode, nLeafBottom, nLeafBottomProps->iByteAfter);
								NBArray_addValue(&obj->parser.tree.leaves.open, partNode);
								consumed = TRUE;
							}
							partNode = NULL; //retained by litrlNode
						} else {
							BOOL consumed2 = FALSE;
							STNBSintaxParserPartNode* newNode = NULL;
							NBSintaxParser_treeFeedFirstTokenElem(obj, &newNode, &nLeafBottom, nLeafBottomProps->iByteAfter, part->iElem, iElemToken, token, tokenSz, &consumed2);
							NBASSERT((newNode == NULL && !consumed2) || (newNode != NULL && consumed2))
							if(consumed2){
								consumed = TRUE;
							}
							if(newNode != NULL){
								//Config node
								newNode->partIdx.iElem	= nLeafCur->partIdx.iElem;
								newNode->partIdx.iDef	= nLeafCur->partIdx.iDef;
								newNode->partIdx.iPart	= iPartNext;
								NBSintaxParser_nodeSetParent(newNode, nLeafCur->parent);
								NBSintaxParser_nodeSetPrevious(newNode, nLeafCur);
								newNode = NULL;
							}
						}
						break;
					default:
						NBASSERT(FALSE)
						break;
				}
				//Next step
				if(part->isOptional || eePart->hints.canBeEmpty){
					iPartNext++;
				} else {
					//End cicle
					nLeafCur = NULL;
				}
			}
		}
	}
	//
	if(dstConsumed != NULL){
		*dstConsumed = consumed;
	}
}

//

BOOL NBSintaxParser_treeBranchIsCompletedFromFarLeaf(const STNBSintaxParser* obj, const STNBSintaxParserPartNode* nLeafBottom){
	BOOL r = FALSE;
	const STNBSintaxParserState* parser			= &obj->parser;
	const STNBSintaxParserRules* rules			= obj->rules; NBASSERT(rules != NULL)
	const STNBSintaxParserPartNode* nLeafCur	= nLeafBottom;
	NBASSERT(nLeafCur->leafProps != NULL)
	if(nLeafCur->leafProps != NULL){
		const STNBSintaxParserPartNodeLeafProps* nProps = nLeafCur->leafProps;
		const BOOL isLeafCompleted = (nProps->isClosed || (nProps->resultMskCur & ENSintaxParserValidateElemResultBit_Complete) != 0);
		if(isLeafCompleted){
			UI32 iPartNext = (nLeafCur->partIdx.iPart + 1); 
			while(nLeafCur != NULL){
				NBASSERT(nLeafCur->partIdx.iElem >= 0 && nLeafCur->partIdx.iElem < obj->rules->elems.use)
				const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nLeafCur->partIdx.iElem);
				const STNBSintaxParserElemDef* def = NULL;
				UI32 partsSz = 0;
				if(ee->type == ENSintaxParserElemType_Elem){
					NBASSERT(nLeafCur->partIdx.iDef >= 0 && nLeafCur->partIdx.iDef < ee->defsSz)
					//Obtain definition
					def = &ee->defs[nLeafCur->partIdx.iDef];
					//Define next part
					NBASSERT(nLeafCur->partIdx.iPart >= 0 && nLeafCur->partIdx.iPart < def->partsSz)
					NBASSERT(iPartNext >= 0 && iPartNext <= def->partsSz)
					partsSz = def->partsSz;
				}
				if(iPartNext >= partsSz){
					//Advance leaf content
					//nLeafCur->iByteAfter = nLeafBottom->iByteAfter;
					//Move to parent
					if(nLeafCur->parent == parser->tree.root.node){
						//Posib tree is complete
						r = TRUE;
						//End cicle
						nLeafCur = NULL;
					} else {
						//Move to parent
						nLeafCur = nLeafCur->parent;
						if(nLeafCur != NULL){
							iPartNext = (nLeafCur->partIdx.iPart + 1);
						}
					}
				} else {
					//Try next parts
					const STNBSintaxParserElemDefPart* part = &def->parts[iPartNext];
					//Next step
					if(part->isOptional){
						iPartNext++;
					} else {
						//End cicle
						nLeafCur = NULL;
					}
				}
			}
		}
	}
	//
	return r;
}

void NBSintaxParser_treeAddCompletedBranch(STNBSintaxParser* obj, STNBSintaxParserPartNode* nLeafBottom, const UI32 iElemRoot, const UI32 iElemRootChild){
	NBASSERT(nLeafBottom->leafProps != NULL)
	const ENSintaxParserResultsMode resultsMode = obj->parser.tree.leaves.resultsMode;
	STNBSintaxParserPartNodeLeafProps* nProps = nLeafBottom->leafProps;
	STNBArray* completed	= &obj->parser.tree.leaves.completed;
	//Remove previous shorter results
	SI32 i2; for(i2 = (SI32)completed->use -1; i2 >= 0; i2--){
		STNBSintaxParserPartNode* nLeaf2 = NBArray_itmValueAtIndex(completed, STNBSintaxParserPartNode*, i2);
		NBASSERT(nLeaf2->leafProps != NULL)
		NBASSERT(nLeaf2->completedProps != NULL)
		if(nLeaf2->leafProps != NULL && nLeaf2->completedProps != NULL){
			STNBSintaxParserPartNodeLeafProps* nProps2 = nLeaf2->leafProps;
			const STNBSintaxParserPartNodeCompleteProps* cProps2 = nLeaf2->completedProps;
			if(nProps2->iByteAfter < nProps->iByteAfter){
				if(resultsMode == ENSintaxParserResultsMode_LongestsOnly
				|| (resultsMode == ENSintaxParserResultsMode_LongestsPerRootElem && cProps2->rootElemIdx == iElemRoot)
				|| (resultsMode == ENSintaxParserResultsMode_LongestsPerRootChildElem && cProps2->rootChildElemIdx == iElemRootChild)
				   ){
					NBSintaxParser_nodeRelease(nLeaf2);
					NBArray_removeItemAtIndex(completed, i2);
				}
			}
		}
	}
	//Move leaf to completed
	{
		if(nLeafBottom->completedProps == NULL){
			nLeafBottom->completedProps = NBMemory_allocType(STNBSintaxParserPartNodeCompleteProps);
			NBMemory_setZeroSt(*nLeafBottom->completedProps, STNBSintaxParserPartNodeCompleteProps);;
		}
		nLeafBottom->completedProps->rootElemIdx = iElemRoot;
		nLeafBottom->completedProps->rootChildElemIdx = iElemRootChild;
		//
		NBSintaxParser_nodeRetain(nLeafBottom);
		NBArray_addValue(completed, nLeafBottom);
	}
}

//-----------------
//Concat
//-----------------

void NBSintaxParser_treeConcatNodeStates(const UI32 resultMskLast, const UI32 resultMskCur, const char* baseState, STNBString* dst){
	if(dst != NULL){
		STNBString strState;
		NBString_init(&strState);
		if(!NBString_strIsEmpty(baseState)){
			if(strState.length > 0){
				NBString_concatByte(&strState, ' ');
			}
			NBString_concat(&strState, baseState);
		}
		if((resultMskLast & ENSintaxParserValidateElemResultBit_Complete) != 0){
			if((resultMskCur & ENSintaxParserValidateElemResultBit_Complete) == 0){
				if(strState.length > 0){
					NBString_concatByte(&strState, ' ');
				}
				NBString_concat(&strState, "completed-before");
			}
		} else if((resultMskLast & ENSintaxParserValidateElemResultBit_Partial) != 0){
			if((resultMskCur & ENSintaxParserValidateElemResultBit_Partial) == 0){
				if(strState.length > 0){
					NBString_concatByte(&strState, ' ');
				}
				NBString_concat(&strState, "discarted");
			}
		}
		if((resultMskCur & ENSintaxParserValidateElemResultBit_Partial) != 0){
			if(strState.length > 0){
				NBString_concatByte(&strState, ' ');
			}
			NBString_concat(&strState, "partial");
		}
		if((resultMskCur & ENSintaxParserValidateElemResultBit_Complete) != 0){
			if(strState.length > 0){
				NBString_concatByte(&strState, ' ');
			}
			NBString_concat(&strState, "complete");
		}
		if(strState.length > 0){
			NBString_concat(dst, " (");
			NBString_concat(dst, strState.str);
			NBString_concat(dst, ")");
		}
		NBString_release(&strState);
	}
}

//Concat tree reducing repetitions of nodes

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgTreeConcatDeflatedNode_(const STNBSintaxParser* obj, const char colChar, const STNBSintaxParserPartNode* nodeH, const STNBSintaxParserElemDefPart* part, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const char* strAccum, const UI32 strAccumSz, const char* strLeftChildDef, const UI32 strLeftChildDefSz, const char* strLeftChildBtm, const UI32 strLeftChildBtmSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dstPosibStr, STNBArray* dstSubstrs){
	NBASSERT(part->iElem >= 0 && part->iElem < obj->rules->elems.use)
	const STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	const STNBSintaxParserElem* ee		= NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
	//PartIdx
	if(includePartIdxs){
		NBString_concat(dstPosibStr, "(");
		NBString_concatUI32(dstPosibStr, nodeH->partIdx.iElem);
		NBString_concat(dstPosibStr, ",");
		NBString_concatUI32(dstPosibStr, nodeH->partIdx.iDef);
		NBString_concat(dstPosibStr, ",");
		NBString_concatUI32(dstPosibStr, nodeH->partIdx.iPart);
		NBString_concat(dstPosibStr, ")");
	}
	//Name
	switch (ee->type) {
		case ENSintaxParserElemType_Literal:
			NBString_concatByte(dstPosibStr, '[');
			NBString_concat(dstPosibStr, ee->name);
			NBString_concatByte(dstPosibStr, ']');
			break;
		case ENSintaxParserElemType_Callback:
			NBString_concat(dstPosibStr, "(callback)");
			break;
		case ENSintaxParserElemType_Elem:
			NBString_concat(dstPosibStr, ee->name);
			break;
		default:
			NBASSERT(FALSE)
			break;
	}
	//Point
	if(optNodeToPointOut == nodeH){
		NBString_concat(dstPosibStr, "(HERE!)");
	}
	//RetainCount
	if(includeRetainCount){
		NBString_concat(dstPosibStr, "(*");
		NBString_concatSI32(dstPosibStr, nodeH->retainCount);
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		if(nodeH->dbg.chldrn.use > 0){
			NBString_concat(dstPosibStr, ",");	
			NBString_concatSI32(dstPosibStr, nodeH->dbg.chldrn.use);
			NBString_concat(dstPosibStr, "-chldrn");
		}
		if(nodeH->dbg.nextLeaves.use > 0){
			NBString_concat(dstPosibStr, ",");	
			NBString_concatSI32(dstPosibStr, nodeH->dbg.nextLeaves.use);
			NBString_concat(dstPosibStr, "-nextLeaves");
		}
		if(nodeH->dbg.nexts.use > 0){
			NBString_concat(dstPosibStr, ",");	
			NBString_concatSI32(dstPosibStr, nodeH->dbg.nexts.use);
			NBString_concat(dstPosibStr, "-nexts");
		}
		if(nodeH->dbg.parentRootsRefs.use > 0){
			NBString_concat(dstPosibStr, ",");	
			NBString_concatSI32(dstPosibStr, nodeH->dbg.parentRootsRefs.use);
			NBString_concat(dstPosibStr, "-parentRootsRefs");
		}
#		endif
		NBString_concat(dstPosibStr, ")");
	}
	//Ranges
	if(includeRanges){
		if(nodeH->leafProps != NULL){
			const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
			NBString_concat(dstPosibStr, "(");
			NBString_concatUI32(dstPosibStr, nProps->iByteStart);
			NBString_concat(dstPosibStr, ",+");
			NBString_concatUI32(dstPosibStr, (nProps->iByteAfter - nProps->iByteStart));
			NBString_concat(dstPosibStr, ")");
		}
	}
	//Add curr mask
	if(nodeH->leafProps != NULL){
		const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
		NBSintaxParser_treeConcatNodeStates(nProps->resultMskLast, nProps->resultMskCur, NULL, dstPosibStr);
	}
	//Add posibs
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	if(nodeH->leafProps != NULL){
		NBASSERT(!(nodeH->dbg.chldrn.use != 0 && nodeH->leafProps->childLeafRef != NULL)) //Only-real-children or only-child-ref should be at one time.
	}
#	endif 
	{
		const STNBSintaxParserPartNode* nodeP = nodeH;
		const STNBSintaxParserPartNode* posibFilter = NULL;
		//Define if is a referenced leaf
		if(nodeH->leafProps != NULL){
			if(nodeH->leafProps->childLeafRef){
				nodeP = nodeH->leafProps->childLeafRef->parent;
				posibFilter = nodeH->leafProps->childLeafRef;
				while(posibFilter->previous != NULL){
					posibFilter = posibFilter->previous;
				}
			}
		}
		//
		if(nodeP != NULL){
			UI32 childrenCount = 0;
			SI32 i; for(i = 0; i < nodeP->dbg.chldrn.use; i++){
				STNBSintaxParserPartNode* nodeV = NBArray_itmValueAtIndex(&nodeP->dbg.chldrn, STNBSintaxParserPartNode*, i);
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				if(nodeH->leafProps == NULL){
					NBASSERT(nodeV->partIdx.iElem == part->iElem) //Must be a definition of this part
				} else {
					NBASSERT(nodeV->partIdx.iElem == part->iElem || nodeH->leafProps->childLeafRef != NULL) //Must be a definition of this part or a clone
				}
#				endif
				if(nodeV->previous == NULL && (posibFilter == NULL || posibFilter == nodeV)){ //Process only start-of-posibs
					//Deflate by ignoring posibs that are refered in other posibs
					BOOL posibIsRedundant = FALSE;
					if(posibFilter == NULL){
						SI32 i2; for(i2 = 0; i2 < nodeP->dbg.chldrn.use; i2++){
							if(i != i2){
								STNBSintaxParserPartNode* nodeV2 = NBArray_itmValueAtIndex(&nodeP->dbg.chldrn, STNBSintaxParserPartNode*, i2);
								if(nodeV2->leafProps != NULL){
									if(nodeV2->leafProps->childLeafRef != NULL){
										STNBSintaxParserPartNode* childPosibRef = nodeV2->leafProps->childLeafRef;
										while(childPosibRef->previous != NULL){
											childPosibRef = childPosibRef->previous;
										}
										if(childPosibRef == nodeV){
											posibIsRedundant = TRUE;
											break;
										}
									}
								}
							}
						}
					}
					if(!posibIsRedundant){
						const char* leftPathChild	= (childrenCount == 0 ? strLeftChildBtm : strLeftChildDef);
						const SI32 leftPathChildSz	= (childrenCount == 0 ? strLeftChildBtmSz : strLeftChildDefSz);			
						STNBString subStr;
						NBString_init(&subStr);
						//Add substring
						NBSintaxParser_dbgTreeConcatDeflatedPosib_(obj, colChar, nodeV, strLeftChildDef, strLeftChildDefSz, leftPathChild, leftPathChildSz, includeRanges, includePartIdxs, includeRetainCount, strAccum, strAccumSz, optNodeToPointOut, &subStr);
						NBArray_addValue(dstSubstrs, subStr);
						childrenCount++;
					}
				}
			}
		}
	}
}
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgTreeConcatDeflatedPosib_(const STNBSintaxParser* obj, const char colChar, const STNBSintaxParserPartNode* posibStart, const char* leftStrForPosib, const UI32 leftStrForPosibSz, const char* leftStrForChild, const UI32 leftStrForChildSz, const BOOL includeRanges, const BOOL includePartIdxs, const BOOL includeRetainCount, const char* strAccum, const UI32 strAccumSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst){
	if(obj != NULL && posibStart != NULL){
		const STNBSintaxParserPartNode* nodeH = posibStart;
		NBASSERT(nodeH->partIdx.iElem >= 0 && nodeH->partIdx.iElem < obj->rules->elems.use)
		const STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
		const STNBSintaxParserElem* eee		= NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nodeH->partIdx.iElem);
		STNBString myPath;
		STNBArray subStrs, subStrs2;
		NBString_init(&myPath);
		NBArray_init(&subStrs, sizeof(STNBString), NULL);
		NBArray_init(&subStrs2, sizeof(STNBString), NULL);
		//
		if(!NBString_strIsEmpty(leftStrForPosib) && leftStrForPosibSz > 0){
			NBString_concatBytes(&myPath, leftStrForPosib, leftStrForPosibSz);
		}
		//
		{
			switch (eee->type) {
				case ENSintaxParserElemType_Literal:
					{
						NBASSERT(nodeH->retainCount > 0)
						NBASSERT(nodeH->leafProps != NULL)
						//
						NBASSERT(nodeH->partIdx.iDef == 0)
						NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart <= eee->nameLen)
						//
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						if(nodeH->leafProps != NULL){
							const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
							NBASSERT(nProps->iByteStart >= 0)
							NBASSERT(nProps->iByteStart <= nProps->iByteAfter)
							NBASSERT(nProps->iByteAfter <= strAccumSz)
							NBASSERT((nProps->iByteAfter - nProps->iByteStart) <= eee->nameLen)
						}
#						endif
						//
						NBASSERT(nodeH->dbg.chldrn.use == 0)
						NBASSERT(nodeH->dbg.nexts.use == 0)
						//
						NBString_concat(&myPath, (nodeH->previous != NULL ? ", " : (myPath.length > 0 ? "-" : "")));
						//Part idx
						if(includePartIdxs){
							NBString_concat(&myPath, "(");
							NBString_concatUI32(&myPath, nodeH->partIdx.iElem);
							NBString_concat(&myPath, ")");
						}
						//Concat value
						{
							NBString_concatByte(&myPath, '\'');
							if(nodeH->leafProps != NULL){
								const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
								if(nProps->iByteStart < nProps->iByteAfter){
									UI32 i; for(i = nProps->iByteStart; i < nProps->iByteAfter; i++){
										const char c = strAccum[i];
										switch(c) {
											case '\t': NBString_concatBytes(&myPath, "\\t", 2); break;
											case '\r': NBString_concatBytes(&myPath, "\\r", 2); break;
											case '\n': NBString_concatBytes(&myPath, "\\n", 2); break;
											default: NBString_concatByte(&myPath, c); break;
										}
									}
								}
							}
							NBString_concatByte(&myPath, '\'');
						}
						//RetainCount
						if(includeRetainCount){
							NBString_concat(&myPath, "(*");
							NBString_concatSI32(&myPath, nodeH->retainCount);
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							if(nodeH->dbg.chldrn.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.chldrn.use);
								NBString_concat(&myPath, "-chldrn");
							}
							if(nodeH->dbg.nextLeaves.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.nextLeaves.use);
								NBString_concat(&myPath, "-nextLeaves");
							}
							if(nodeH->dbg.nexts.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.nexts.use);
								NBString_concat(&myPath, "-nexts");
							}
							if(nodeH->dbg.parentRootsRefs.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.parentRootsRefs.use);
								NBString_concat(&myPath, "-parentRootsRefs");
							}
#							endif
							NBString_concat(&myPath, ")");
						}
						//Ranges
						if(includeRanges){
							if(nodeH->leafProps != NULL){
								const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
								NBString_concat(&myPath, "(");
								NBString_concatUI32(&myPath, nProps->iByteStart);
								NBString_concat(&myPath, ",+");
								NBString_concatUI32(&myPath, (nProps->iByteAfter - nProps->iByteStart));
								NBString_concat(&myPath, ")");
							}
						}
						//Add curr mask
						if(nodeH->leafProps != NULL){
							const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
							NBSintaxParser_treeConcatNodeStates(nProps->resultMskLast, nProps->resultMskCur, NULL, &myPath);
						}
					}
					break;
				case ENSintaxParserElemType_Callback:
					{
						NBASSERT(nodeH->retainCount > 0)
						NBASSERT(nodeH->leafProps != NULL)
						//
						NBASSERT(nodeH->partIdx.iDef == 0)
						//
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						if(nodeH->leafProps != NULL){
							const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
							NBASSERT(nProps->iByteStart >= 0)
							NBASSERT(nProps->iByteStart <= nProps->iByteAfter)
							NBASSERT(nProps->iByteAfter <= strAccumSz)
						}
#						endif
						//
						NBASSERT(nodeH->dbg.chldrn.use == 0)
						NBASSERT(nodeH->dbg.nexts.use == 0)
						//
						NBString_concat(&myPath, (nodeH->previous != NULL ? ", " : (myPath.length > 0 ? "-" : "")));
						//ElemIds
						if(includePartIdxs){
							NBString_concat(&myPath, "(");
							NBString_concatUI32(&myPath, nodeH->partIdx.iElem);
							NBString_concat(&myPath, ")");
						}
						//Concat value
						{
							//NBString_concatByte(&myPath, 'c');
							NBString_concatByte(&myPath, '\'');
							if(nodeH->leafProps != NULL){
								const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
								if(nProps->iByteStart < nProps->iByteAfter){
									UI32 i; for(i = nProps->iByteStart; i < nProps->iByteAfter; i++){
										const char c = strAccum[i];
										switch(c) {
											case '\t': NBString_concatBytes(&myPath, "\\t", 2); break;
											case '\r': NBString_concatBytes(&myPath, "\\r", 2); break;
											case '\n': NBString_concatBytes(&myPath, "\\n", 2); break;
											default: NBString_concatByte(&myPath, c); break;
										}
									}
								}
							}
							NBString_concatByte(&myPath, '\'');
						}
						//RetainCount
						if(includeRetainCount){
							NBString_concat(&myPath, "(*");
							NBString_concatSI32(&myPath, nodeH->retainCount);
#							ifdef NB_CONFIG_INCLUDE_ASSERTS
							if(nodeH->dbg.chldrn.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.chldrn.use);
								NBString_concat(&myPath, "-chldrn");
							}
							if(nodeH->dbg.nextLeaves.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.nextLeaves.use);
								NBString_concat(&myPath, "-nextLeaves");
							}
							if(nodeH->dbg.nexts.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.nexts.use);
								NBString_concat(&myPath, "-nexts");
							}
							if(nodeH->dbg.parentRootsRefs.use > 0){
								NBString_concat(&myPath, ",");	
								NBString_concatSI32(&myPath, nodeH->dbg.parentRootsRefs.use);
								NBString_concat(&myPath, "-parentRootsRefs");
							}
#							endif
							NBString_concat(&myPath, ")");
						}
						//Ranges
						if(includeRanges){
							if(nodeH->leafProps != NULL){
								const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
								NBString_concat(&myPath, "(");
								NBString_concatUI32(&myPath, nProps->iByteStart);
								NBString_concat(&myPath, ",+");
								NBString_concatUI32(&myPath, (nProps->iByteAfter - nProps->iByteStart));
								NBString_concat(&myPath, ")");
							}
						}
						//Add curr mask
						if(nodeH->leafProps != NULL){
							const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
							NBSintaxParser_treeConcatNodeStates(nProps->resultMskLast, nProps->resultMskCur, NULL, &myPath);
						}
					}
					break;
				case ENSintaxParserElemType_Elem:
					{
						STNBString childrenPath, childrenPath2;
						NBString_init(&childrenPath);
						NBString_init(&childrenPath2);
						//
						if(!NBString_strIsEmpty(leftStrForChild) && leftStrForChildSz > 0){
							NBString_concatBytes(&childrenPath, leftStrForChild, leftStrForChildSz);
						}
						{
							NBASSERT(nodeH->partIdx.iDef >= 0 && nodeH->partIdx.iDef < eee->defsSz)
							const STNBSintaxParserElemDef* def = &eee->defs[nodeH->partIdx.iDef];
							UI32 iPartLast = nodeH->partIdx.iPart;
							while(nodeH != NULL){
								NBASSERT(nodeH->retainCount > 0)
								//
								NBASSERT(nodeH->partIdx.iElem == posibStart->partIdx.iElem) //Must be same elem
								NBASSERT(nodeH->partIdx.iDef == posibStart->partIdx.iDef)	//Must be same def
								NBASSERT(iPartLast < nodeH->partIdx.iPart || nodeH == posibStart)	//Must be sequence of parts
								NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart < def->partsSz)
								//
#								ifdef NB_CONFIG_INCLUDE_ASSERTS
								if(nodeH->leafProps != NULL){
									const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
									NBASSERT(nProps->iByteStart >= 0)
									NBASSERT(nProps->iByteStart <= nProps->iByteAfter)
									NBASSERT(nProps->iByteAfter <= strAccumSz)
								}
#								endif
								//
								if(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart < def->partsSz){
									const STNBSintaxParserElemDefPart* part = &def->parts[nodeH->partIdx.iPart];
									NBASSERT(part->iElem >= 0 && part->iElem < obj->rules->elems.use)
									//My Path
									NBString_concat(&myPath, (nodeH->previous != NULL ? ", " : (myPath.length > 0 ? "-" : "")));
									//Path for children
									{
										const STNBSintaxParserPartNode* childLeafRef = NULL;
										if(nodeH->leafProps != NULL){
											childLeafRef = nodeH->leafProps->childLeafRef;
										}
										NBString_concatRepeatedByte(&childrenPath, ' ', (myPath.length - childrenPath.length));
										NBString_setBytes(&childrenPath2, childrenPath.str, childrenPath.length);
										NBString_concatByte(&childrenPath, (nodeH->dbg.chldrn.use <= 0 && childLeafRef == NULL ? ' ' : colChar));
										NBString_concatByte(&childrenPath2, ' ');
									}
									//
									{
										NBSintaxParser_dbgTreeConcatDeflatedNode_(obj, colChar, nodeH, part, includePartIdxs, includeRanges, includeRetainCount, strAccum, strAccumSz, childrenPath.str, childrenPath.length, childrenPath2.str, childrenPath2.length, optNodeToPointOut, &myPath, &subStrs);
									}
								}
								//Save last partIdx
								iPartLast = nodeH->partIdx.iPart;
								//Next
								if(nodeH->dbg.nexts.use <= 0){
									nodeH = NULL;
								} else {
									//Print other posibs
									if(nodeH->dbg.nexts.use > 1){
										STNBString leftStr2, leftStrChild2;
										NBString_init(&leftStr2);
										NBString_init(&leftStrChild2);
										//
										{
											NBString_concatBytes(&leftStr2, myPath.str, myPath.length);
											if(leftStr2.length > 0) leftStr2.str[leftStr2.length - 1] = '.';
											if(leftStr2.length > 1) leftStr2.str[leftStr2.length - 2] = '.';
											if(leftStr2.length > 2) leftStr2.str[leftStr2.length - 3] = '.';
										}
										//
										{
											NBString_concatBytes(&leftStrChild2, leftStrForChild, leftStrForChildSz);
											if(leftStrChild2.length < leftStr2.length){
												NBString_concatRepeatedByte(&leftStrChild2, ' ', leftStr2.length - leftStrChild2.length);
											}
										}
										{
											SI32 i; for(i = 1; i < nodeH->dbg.nexts.use; i++){
												const STNBSintaxParserPartNode* nodeH2 = NBArray_itmValueAtIndex(&nodeH->dbg.nexts, STNBSintaxParserPartNode*, i);
												STNBString str;
												NBString_init(&str);
												NBSintaxParser_dbgTreeConcatDeflatedPosib_(obj, colChar, nodeH2, leftStr2.str, leftStr2.length, leftStrChild2.str, leftStrChild2.length, includeRanges, includePartIdxs, includeRetainCount, strAccum, strAccumSz, optNodeToPointOut, &str);
												NBArray_addValue(&subStrs2, str);
											}
										}
										NBString_release(&leftStr2);
										NBString_release(&leftStrChild2);
									}
									//Set next node
									nodeH = NBArray_itmValueAtIndex(&nodeH->dbg.nexts, STNBSintaxParserPartNode*, 0);
								}
							}
						}
						NBString_release(&childrenPath);
						NBString_release(&childrenPath2);
					}
					break;
				default:
					break;
			}
			//Add my path
			if(dst != NULL){
				NBString_concatBytes(dst, myPath.str, myPath.length);
				NBString_concatByte(dst, '\n');
			}
			//Add sub paths
			{
				SI32 i; for(i = subStrs.use - 1; i >= 0; i--){
					STNBString* str = NBArray_itmPtrAtIndex(&subStrs, STNBString, i);
					NBString_concatBytes(dst, str->str, str->length);
				}
			}
			//Add sub-alternatives
			{
				SI32 i; for(i = 0; i < subStrs2.use; i++){
					STNBString* str = NBArray_itmPtrAtIndex(&subStrs2, STNBString, i);
					NBString_concatBytes(dst, str->str, str->length);
				}
			}
			//Release subpaths
			{
				SI32 i; for(i = 0; i < subStrs.use; i++){
					STNBString* str = NBArray_itmPtrAtIndex(&subStrs, STNBString, i);
					NBString_release(str);
				}
				NBArray_empty(&subStrs);
			}
			//Release sub-alternatives
			{
				SI32 i; for(i = 0; i < subStrs2.use; i++){
					STNBString* str = NBArray_itmPtrAtIndex(&subStrs2, STNBString, i);
					NBString_release(str);
				}
				NBArray_empty(&subStrs2);
			}
			NBArray_release(&subStrs);
			NBArray_release(&subStrs2);
		}
		NBString_release(&myPath);
	}
}
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgTreeConcatDeflatedPosib(const STNBSintaxParser* obj, const char colChar, const STNBSintaxParserPartNode* posibStart, const BOOL includeRanges, const BOOL includePartIdxs, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst){
	if(dst != NULL){
		const STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
		STNBString strAccumTmp;
		NBString_init(&strAccumTmp);
		{
			const char* strAccum = obj->parser.relAccum.str;
			UI32 strAccumSz = obj->parser.relAccum.length;
			//Use tmp string with new char added
			if(nextUtf != NULL && nextUtfSz > 0){
				NBString_concatBytes(&strAccumTmp, obj->parser.relAccum.str, obj->parser.relAccum.length);
				NBString_concatBytes(&strAccumTmp, nextUtf, nextUtfSz);
				strAccum	= strAccumTmp.str;
				strAccumSz	= strAccumTmp.length;
			}
			//Process
			{
				NBASSERT(posibStart->partIdx.iElem >= 0 && posibStart->partIdx.iElem < obj->rules->elems.use)
				const char* leftStrForPosib		= &colChar;
				const UI32 leftStrForPosibSz	= sizeof(colChar);
				const char* leftStrForChild		= &colChar;
				const UI32 leftStrForChildSz	= sizeof(colChar);
				const STNBSintaxParserElem* eee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, posibStart->partIdx.iElem);
				NBString_concat(dst, "(");
				NBString_concat(dst, eee->name);
				NBString_concat(dst, ")\n");
				NBSintaxParser_dbgTreeConcatDeflatedPosib_(obj, colChar, posibStart, leftStrForPosib, leftStrForPosibSz, leftStrForChild, leftStrForChildSz, includeRanges, includePartIdxs, includeRetainCount, strAccum, strAccumSz, optNodeToPointOut, dst);
			}
		}
		NBString_release(&strAccumTmp);
	}
}
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgTreeConcatDeflated(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst){
	const STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
	STNBString strAccumTmp;
	NBString_init(&strAccumTmp);
	{
		const char* strAccum = obj->parser.relAccum.str;
		UI32 strAccumSz = obj->parser.relAccum.length;
		//Use tmp string with new char added
		if(nextUtf != NULL && nextUtfSz > 0){
			NBString_concatBytes(&strAccumTmp, obj->parser.relAccum.str, obj->parser.relAccum.length);
			NBString_concatBytes(&strAccumTmp, nextUtf, nextUtfSz);
			strAccum	= strAccumTmp.str;
			strAccumSz	= strAccumTmp.length;
		}
		//Process children
		{
			UI32 lastIdxElem = rules->elems.use;
			const STNBSintaxParserState* parser = &obj->parser;
			SI32 i; for(i = 0; i < parser->tree.root.node->dbg.chldrn.use; i++){
				const STNBSintaxParserPartNode* nodeV = NBArray_itmValueAtIndex(&parser->tree.root.node->dbg.chldrn, STNBSintaxParserPartNode*, i);
				if(nodeV->previous == NULL){ //Process only start-of-posibs
					const BOOL includeRanges		= FALSE;
					const char colChar				= '|';
					const char colCharChild			= (i == (parser->tree.root.node->dbg.chldrn.use - 1) ? ' ' : colChar);
					const char* leftStrForPosib		= &colChar;
					const UI32 leftStrForPosibSz	= sizeof(colChar);
					const char* leftStrForChild		= &colCharChild;
					const UI32 leftStrForChildSz	= sizeof(colCharChild);
					if(lastIdxElem != nodeV->partIdx.iElem){
						//Concat name
						NBASSERT(nodeV->partIdx.iElem >= 0 && nodeV->partIdx.iElem < obj->rules->elems.use)
						const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nodeV->partIdx.iElem);
						NBString_concat(dst, "(");
						NBString_concat(dst, ee->name);
						NBString_concat(dst, ")\n");
						//Set new
						lastIdxElem = nodeV->partIdx.iElem;
					}
					NBSintaxParser_dbgTreeConcatDeflatedPosib_(obj, colChar, nodeV, leftStrForPosib, leftStrForPosibSz, leftStrForChild, leftStrForChildSz, includeRanges, includePartIdxs, includeRetainCount, strAccum, strAccumSz, optNodeToPointOut, dst);
				}
			}
		}
	}
	NBString_release(&strAccumTmp);
}
#endif

//Concat tree posibs (repeating nodes when necesary)

void NBSintaxParser_treeConcatChildrenLiteral_(const STNBSintaxParser* obj, const char colChar, const BOOL includePartIdxs, const BOOL includeRanges, const char* baseState, const STNBSintaxParserPartNode* nodeH, const UI32 iByteStart, const UI32 iByteAfter, const char* strAccum, const UI32 strAccumSz, STNBArray* dstLines){
	STNBString str;
	NBString_init(&str);
	NBString_concatByte(&str, colChar); //My spacing
	NBString_concatByte(&str, '-'); //My spacing
	//Part idx
	if(includePartIdxs){
		NBString_concat(&str, "(");
		NBString_concatUI32(&str, nodeH->partIdx.iElem);
		NBString_concat(&str, ",");
		NBString_concatUI32(&str, nodeH->partIdx.iDef);
		NBString_concat(&str, ",");
		NBString_concatUI32(&str, nodeH->partIdx.iPart);
		NBString_concat(&str, ")");
	}
	//Concat Value
	{
		NBString_concatByte(&str, '\'');
		if(iByteStart < iByteAfter){
			UI32 i; for(i = iByteStart; i < iByteAfter; i++){
				const char c = strAccum[i];
				switch(c) {
					case '\t': NBString_concatBytes(&str, "\\t", 2); break;
					case '\r': NBString_concatBytes(&str, "\\r", 2); break;
					case '\n': NBString_concatBytes(&str, "\\n", 2); break;
					default: NBString_concatByte(&str, c); break;
				}
			}
		}
		NBString_concatByte(&str, '\'');
	}
	//Ranges
	if(includeRanges){
		if(nodeH->leafProps != NULL){
			const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
			NBString_concat(&str, "(");
			NBString_concatUI32(&str, nProps->iByteStart);
			NBString_concat(&str, ",+");
			NBString_concatUI32(&str, (nProps->iByteAfter - nProps->iByteStart));
			NBString_concat(&str, ")");
		}
	}
	//Add curr mask
	if(nodeH->leafProps != NULL){
		const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
		NBSintaxParser_treeConcatNodeStates(nProps->resultMskLast, nProps->resultMskCur, baseState, &str);
	}
	NBArray_addValue(dstLines, str);
}

typedef struct NBSintaxParserConcatLinePartDef_ {
	UI32 preNameLen;	// usually: '|-' or ', '
	UI32 nameLen;		// the name only: '[...]' or '...'
	UI32 fullLen;		// complete part '|-...' or ', ...'
	UI32 subLinesCount;	//lines of children
} NBSintaxParserConcatLinePartDef;

void NBSintaxParser_treeConcatChildren_(const STNBSintaxParser* obj, const char colChar, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const STNBSintaxParserPartNode* parent, const STNBSintaxParserPartNode** farLeafCurRef, UI32* farLeafCurRefByteStartIdx, UI32* farLeafCurRefByteAfterIdx, UI32* dstRootElemIdx, const char* strAccum, const UI32 strAccumSz, STNBArray* dstLines){
	const STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
	const STNBSintaxParserPartNode* farLeafCur = NULL;
	UI32 farLeafCurByteStartIdx = 0, farLeafCurByteAfterIdx = 0;
	UI32 iElemLast = 0; 
	//
	if(farLeafCurRef != NULL) farLeafCur = *farLeafCurRef;
	if(farLeafCurRefByteStartIdx != NULL) farLeafCurByteStartIdx = *farLeafCurRefByteStartIdx;
	if(farLeafCurRefByteAfterIdx != NULL) farLeafCurByteAfterIdx = *farLeafCurRefByteAfterIdx;
	if(dstRootElemIdx != NULL) iElemLast = *dstRootElemIdx;
	//
	if(dstLines != NULL){
		BOOL partElemFound = FALSE;
		if(parent != NULL && farLeafCur != NULL){
			NBASSERT(farLeafCur->leafProps != NULL)
			if(farLeafCur->leafProps != NULL){
				//Build external branch of posibilities (and recursively inner branches)
				const SI32 linesCountBefore = dstLines->use;
				const UI32 farLeafByteAfterIdxMax = farLeafCurByteAfterIdx;
				const STNBSintaxParserPartNode* posibLeaf = farLeafCur;
				STNBString strTmp;
				NBString_init(&strTmp);
				while(posibLeaf != NULL && posibLeaf != parent){
					NBASSERT(posibLeaf->partIdx.iElem >= 0 && posibLeaf->partIdx.iElem < obj->rules->elems.use)
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, posibLeaf->partIdx.iElem);
					//Literals and callbacks cant appear on top of elem_nodes.
					NBASSERT(!partElemFound || ee->type == ENSintaxParserElemType_Elem)
					//FarLeaf cannot be NULL nor rootNode
					NBASSERT(farLeafCur != NULL && farLeafCur != obj->parser.tree.root.node)
					NBASSERT(farLeafCur->leafProps != NULL)
					//Ignore literals and callbacks
					switch(ee->type) {
						case ENSintaxParserElemType_Literal:
							//Add value
							{
								NBASSERT(posibLeaf->previous == NULL)
								NBASSERT(farLeafCurByteStartIdx <= farLeafByteAfterIdxMax)
								const UI32 iByteStart	= farLeafCurByteStartIdx;
								const UI32 iByteAfter	= farLeafByteAfterIdxMax;
								NBSintaxParser_treeConcatChildrenLiteral_(obj, colChar, includePartIdxs, includeRanges, NULL, posibLeaf, iByteStart, iByteAfter, strAccum, strAccumSz, dstLines);
							}
							break;
						case ENSintaxParserElemType_Callback:
							//Add value
							{
								NBASSERT(posibLeaf->previous == NULL)
								NBASSERT(farLeafCurByteStartIdx <= farLeafByteAfterIdxMax)
								const UI32 iByteStart	= farLeafCurByteStartIdx;
								const UI32 iByteAfter	= farLeafByteAfterIdxMax;
								NBSintaxParser_treeConcatChildrenLiteral_(obj, colChar, includePartIdxs, includeRanges, NULL, posibLeaf, iByteStart, iByteAfter, strAccum, strAccumSz, dstLines);
							}
							break;
						case ENSintaxParserElemType_Elem:
							{
								NBASSERT(posibLeaf->partIdx.iDef >= 0 && posibLeaf->partIdx.iDef < ee->defsSz)
								const STNBSintaxParserElemDef* def = &ee->defs[posibLeaf->partIdx.iDef];
								const STNBSintaxParserPartNode* nodeH = posibLeaf;
								STNBArray arrParts, arrSublines;
								STNBString strLine, strPart;
								NBArray_init(&arrParts, sizeof(NBSintaxParserConcatLinePartDef), NULL);
								NBArray_init(&arrSublines, sizeof(STNBString), NULL);
								NBString_init(&strLine);
								NBString_init(&strPart);
								//Build line
								while(nodeH != NULL){
									NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart < def->partsSz)
									const STNBSintaxParserElemDefPart* part = &def->parts[nodeH->partIdx.iPart];
									NBASSERT(part->iElem >= 0 && part->iElem < obj->rules->elems.use)
									const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
									UI32 partLenBeforeName = 0, partLenAfterName = 0, linesBeforePart = arrSublines.use;
									NBSintaxParserConcatLinePartDef partDef;
									NBMemory_setZeroSt(partDef, NBSintaxParserConcatLinePartDef);
									NBString_empty(&strPart);
									if(nodeH->previous == NULL){
										NBString_concatByte(&strPart, colChar); //My spacing
										NBString_concatByte(&strPart, '-'); //My spacing
									} else {
										NBString_concat(&strPart, ", ");
									}
									partLenBeforeName = strPart.length;
									//Part idx
									if(includePartIdxs){
										NBString_concat(&strPart, "(");
										NBString_concatUI32(&strPart, nodeH->partIdx.iElem);
										NBString_concat(&strPart, ",");
										NBString_concatUI32(&strPart, nodeH->partIdx.iDef);
										NBString_concat(&strPart, ",");
										NBString_concatUI32(&strPart, nodeH->partIdx.iPart);
										NBString_concat(&strPart, ")");
									}
									//Concat Value
									switch (ee->type) {
									case ENSintaxParserElemType_Literal:
										NBString_concat(&strPart, "[");
										NBString_concat(&strPart, ee->name);
										NBString_concat(&strPart, "]");
										break;
									case ENSintaxParserElemType_Callback:
										NBString_concat(&strPart, "(callback)");
										break;	
									case ENSintaxParserElemType_Elem:
										NBString_concat(&strPart, ee->name);
										break;
									default:
										NBASSERT(FALSE)
										break;
									}
									//Ranges
									if(includeRanges){
										if(nodeH->leafProps != NULL){
											const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
											NBString_concat(&strPart, "(");
											NBString_concatUI32(&strPart, nProps->iByteStart);
											NBString_concat(&strPart, ",+");
											NBString_concatUI32(&strPart, (nProps->iByteAfter - nProps->iByteStart));
											NBString_concat(&strPart, ")");
										}
									}
									//Add curr mask
									if(nodeH->leafProps != NULL){
										const STNBSintaxParserPartNodeLeafProps* nProps = nodeH->leafProps;
										NBSintaxParser_treeConcatNodeStates(nProps->resultMskLast, nProps->resultMskCur, NULL, &strPart);
									}
									partLenAfterName		= strPart.length;
									{
										partDef.preNameLen	= partLenBeforeName;
										partDef.nameLen		= (strPart.length - partLenBeforeName);
										partDef.fullLen		= strPart.length;
										NBASSERT(partDef.preNameLen == 2)
										NBASSERT((partDef.preNameLen + partDef.nameLen) == partDef.fullLen)
									}
									//Add to line
									{
										NBString_empty(&strTmp);
										NBString_concatBytes(&strTmp, strPart.str, strPart.length);
										NBString_concatBytes(&strTmp, strLine.str, strLine.length);
										NBString_swapContent(&strLine, &strTmp);
									}
									//Process node
									if(nodeH == posibLeaf){
										//Move all current lines as children of this node
										if(linesCountBefore < dstLines->use){
											STNBString* strs = NBArray_dataPtr(dstLines, STNBString);
											NBArray_addItems(&arrSublines, &strs[linesCountBefore], sizeof(strs[0]), dstLines->use - linesCountBefore);
											NBArray_removeItemsAtIndex(dstLines, linesCountBefore, dstLines->use - linesCountBefore);
										}
									} else {
										//Move to prev farLeaf
										NBASSERT(farLeafCur->leafProps != NULL)
										NBASSERT(farLeafCur->leafProps->prevLeaf != NULL)
										NBASSERT(farLeafCur->leafProps->prevLeaf != obj->parser.tree.root.node)
										if(farLeafCur->leafProps != NULL){
											const STNBSintaxParserPartNodeLeafProps* farLeafCurProps = farLeafCur->leafProps;
											NBASSERT(farLeafCurProps->prevLeaf != NULL)
											NBASSERT(farLeafCurProps->prevLeaf->leafProps != NULL)
											NBASSERT(farLeafCurProps->prevLeaf->leafProps->iByteStart <= farLeafCurProps->prevByteAfterIdx)
											NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteStartIdx)
											NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteAfterIdx)
											farLeafCur				= farLeafCurProps->prevLeaf;
											farLeafCurByteStartIdx	= farLeafCurProps->prevLeaf->leafProps->iByteStart;
											farLeafCurByteAfterIdx	= farLeafCurProps->prevByteAfterIdx;
										}
										//Process children
										if(farLeafCur != nodeH){
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											if(nodeH->leafProps != NULL){
												NBASSERT(nodeH->leafProps->childLeafRef == NULL) //Must not be a reference
											}
#											endif
											//Process child branch
											const STNBSintaxParserPartNode* parent2 = nodeH;
											const STNBSintaxParserPartNode* farLeafCur2 = farLeafCur;
											UI32 farLeafCurByteStartIdx2	= farLeafCurByteStartIdx;
											UI32 farLeafCurByteAfterIdx2	= farLeafCurByteAfterIdx;
											UI32 rootElemIdx2				= farLeafCur->partIdx.iElem;
											{
												NBSintaxParser_treeConcatChildren_(obj, colChar, includePartIdxs, includeRanges, includeRetainCount, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &rootElemIdx2, strAccum, strAccumSz, &arrSublines);
												NBASSERT(rootElemIdx2 == part->iElem)
											}
											//Move to prev farLeaf
											NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteAfterIdx2)
											NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteStartIdx)
											NBASSERT(farLeafCurByteAfterIdx2 <= farLeafCurByteAfterIdx)
											farLeafCurByteStartIdx	= farLeafCurByteStartIdx2;
											farLeafCurByteAfterIdx	= farLeafCurByteAfterIdx2;
											farLeafCur				= farLeafCur2;
										}
									}
									//Process childRef
									if(nodeH->leafProps != NULL){
										if(farLeafCur == nodeH){
											if(nodeH->leafProps->childLeafRef == NULL){
												//Add value reference node
												const UI32 iByteStart	= farLeafCurByteStartIdx;
												const UI32 iByteAfter	= farLeafCurByteAfterIdx;
												NBSintaxParser_treeConcatChildrenLiteral_(obj, colChar, includePartIdxs, includeRanges, "not-a-node", nodeH, iByteStart, iByteAfter, strAccum, strAccumSz, &arrSublines);
											} else {
												//Move to prev farLeaf
												NBASSERT(farLeafCur->leafProps != NULL)
												NBASSERT(farLeafCur->leafProps->prevLeaf != NULL)
												NBASSERT(farLeafCur->leafProps->prevLeaf != obj->parser.tree.root.node)
												if(farLeafCur->leafProps != NULL){
													const STNBSintaxParserPartNodeLeafProps* farLeafCurProps = farLeafCur->leafProps;
													NBASSERT(farLeafCurProps->prevLeaf != NULL)
													NBASSERT(farLeafCurProps->prevLeaf->leafProps != NULL)
													NBASSERT(farLeafCurProps->prevLeaf->leafProps->iByteStart <= farLeafCurProps->prevByteAfterIdx)
													NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteStartIdx)
													NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteAfterIdx)
													farLeafCur				= farLeafCurProps->prevLeaf;
													farLeafCurByteStartIdx	= farLeafCurProps->prevLeaf->leafProps->iByteStart;
													farLeafCurByteAfterIdx	= farLeafCurProps->prevByteAfterIdx;
												}
												//Clone referenced branch
												{
													NBASSERT(nodeH->dbg.chldrn.use == 0)			//No children must be added
													NBASSERT(nodeH->leafProps != NULL)				//Must have leaf props
													NBASSERT(nodeH->leafProps->prevLeaf != NULL)	//Must have prev farLeaf
													NBASSERT(nodeH->leafProps->childLeafRef->parent != NULL)	//ChildRef must have a real-parent. 
													//Process child branch
													const STNBSintaxParserPartNode* parent2 = nodeH->leafProps->childLeafRef->parent;
													const STNBSintaxParserPartNode* farLeafCur2 = farLeafCur;
													UI32 farLeafCurByteStartIdx2	= farLeafCurByteStartIdx;
													UI32 farLeafCurByteAfterIdx2	= farLeafCurByteAfterIdx;
													UI32 rootElemIdx2				= farLeafCur->partIdx.iElem;
													//PRINTF_INFO("Cloning branchRef byPart(%llu) parent(%llu) farLeaf(%llu)-(%d,+%d).\n", (UI64)nodeH, (UI64)parent2, (UI64)farLeafCur, farLeafCurByteStartIdx, (farLeafCurByteAfterIdx - farLeafCurByteStartIdx));
													{
														NBSintaxParser_treeConcatChildren_(obj, colChar, includePartIdxs, includeRanges, includeRetainCount, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &rootElemIdx2, strAccum, strAccumSz, &arrSublines);
														NBASSERT(rootElemIdx2 == part->iElem)
													}
													//Move to prev farLeaf
													NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteAfterIdx2)
													NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteStartIdx)
													NBASSERT(farLeafCurByteAfterIdx2 <= farLeafCurByteAfterIdx)
													farLeafCurByteStartIdx	= farLeafCurByteStartIdx2;
													farLeafCurByteAfterIdx	= farLeafCurByteAfterIdx2;
													farLeafCur				= farLeafCur2;
												}
											}
										}
									}
									//Add part definition
									{
										partDef.subLinesCount = (arrSublines.use - linesBeforePart);
										NBArray_addValue(&arrParts, partDef);
									}
									//Prev part
									nodeH = nodeH->previous;
								}
								//Push spaces to sublines
								{
									STNBString leftStr2;
									NBString_init(&leftStr2);
									{
										SI32 iLineAfter = (SI32)arrSublines.use;
										SI32 iPart; for(iPart = (SI32)arrParts.use - 1; iPart >= 0; iPart--){
											const NBSintaxParserConcatLinePartDef* partDef = NBArray_itmPtrAtIndex(&arrParts, NBSintaxParserConcatLinePartDef, iPart);
											//PRINTF_INFO("NBSintaxParser, #%d of %d parts with %d lines.\n", (iPart + 1), arrParts.use, partDef->subLinesCount);
											//Align to part
											UI32 charsAdded = 0;
											if(partDef->preNameLen > 0){
												NBASSERT(partDef->preNameLen == 2)
												NBString_concatRepeatedByte(&leftStr2, ' ', partDef->preNameLen);
												charsAdded += partDef->preNameLen;
											}
											//Apply to sublines
											if(partDef->subLinesCount > 0){
												SI32 iLine2; for(iLine2 = (iLineAfter - partDef->subLinesCount); iLine2 < iLineAfter; iLine2++){
													STNBString* line = NBArray_itmPtrAtIndex(&arrSublines, STNBString, iLine2);
													//PRINTF_INFO("NBSintaxParser, adding padding to %d lines (#%d of %d parts): '%s'.\n", partDef->subLinesCount, (iPart + 1), arrParts.use, leftStr2.str);
													NBString_empty(&strTmp);
													NBString_concatBytes(&strTmp, leftStr2.str, leftStr2.length);
													NBString_concatBytes(&strTmp, line->str, line->length);
													NBString_swapContent(line, &strTmp);
													NBArray_addValue(dstLines, *line);
												}
												iLineAfter -= partDef->subLinesCount;
											}
											//Align to next part
											if(iPart > 0){ 
												if(charsAdded < partDef->fullLen){
													//PRINTF_INFO("Adding %d spaces to next column.\n", partDef->fullLen - charsAdded);
													//Add '|'
													if(partDef->subLinesCount > 0){
														NBString_concatByte(&leftStr2, '|');
														charsAdded++;
													}
													//Add spaces
													if(charsAdded < partDef->fullLen){
														const UI32 remainLen = partDef->fullLen - charsAdded;
														NBString_concatRepeatedByte(&leftStr2, ' ', remainLen);
														NBASSERT((charsAdded + remainLen) == partDef->fullLen)
													}
												}
											}
										} NBASSERT(iLineAfter == 0) //All sublines must be processed
									}
									NBString_release(&leftStr2);
								}
								//Add line
								{
									NBArray_addValue(dstLines, strLine);
								}
								NBString_release(&strPart);
								NBArray_release(&arrSublines);
								NBArray_release(&arrParts);
								//Keep track of last
								partElemFound = TRUE;
							}
							break;
						default:
							NBASSERT(FALSE)
							break;
					}
					//Keep last elemIdx
					iElemLast = posibLeaf->partIdx.iElem;
					//Parent posib
					NBASSERT(posibLeaf->parent != NULL) //All branches should lead to the root node (never NULL)
					posibLeaf = posibLeaf->parent;
				}
				NBString_release(&strTmp);
			}
		}
	}
	//
	if(farLeafCur != NULL) *farLeafCurRef = farLeafCur;
	if(farLeafCurRefByteStartIdx != NULL) *farLeafCurRefByteStartIdx = farLeafCurByteStartIdx;
	if(farLeafCurRefByteAfterIdx != NULL) *farLeafCurRefByteAfterIdx = farLeafCurByteAfterIdx;
	if(dstRootElemIdx != NULL) *dstRootElemIdx = iElemLast;
}

void NBSintaxParser_treeConcat(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, const char* nextUtf, const UI32 nextUtfSz, const STNBSintaxParserPartNode* optNodeToPointOut, STNBString* dst){
	const STNBSintaxParserState* parser = &obj->parser;
	const STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	const STNBArray* leaves = &parser->tree.leaves.open;
	const STNBArray* completed = &parser->tree.leaves.completed;
	const STNBSintaxParserPartNode* topRootNode = obj->parser.tree.root.node;
	STNBString strAccumTmp;
	NBString_init(&strAccumTmp);
	{
		const char* strAccum = obj->parser.relAccum.str;
		UI32 strAccumSz = obj->parser.relAccum.length;
		//Use tmp string with new char added
		if(nextUtf != NULL && nextUtfSz > 0){
			NBString_concatBytes(&strAccumTmp, obj->parser.relAccum.str, obj->parser.relAccum.length);
			NBString_concatBytes(&strAccumTmp, nextUtf, nextUtfSz);
			strAccum	= strAccumTmp.str;
			strAccumSz	= strAccumTmp.length;
		}
		//Process children
		{
			UI32 rootAddedCount = 0;
			UI32 rootElemIdxLast = rules->elems.use;
			SI32 iLeafMode = 0; //0=open, 1=completed
			for(iLeafMode = 0; iLeafMode < 2; iLeafMode++){
				const STNBArray* arrNodes = (iLeafMode == 0 ? leaves : completed);
				SI32 i; for(i = 0; i < arrNodes->use; i++){
					const STNBSintaxParserPartNode* nLeaf = NBArray_itmValueAtIndex(arrNodes, STNBSintaxParserPartNode*, i);
					NBASSERT(nLeaf->leafProps != NULL)
					if(nLeaf->leafProps != NULL){
						STNBSintaxParserPartNodeLeafProps* nProps	= nLeaf->leafProps;
						UI32 farLeafCurByteStartIdx2				= nProps->iByteStart;
						const STNBSintaxParserPartNode* parent2		= topRootNode;
						const STNBSintaxParserPartNode* farLeafCur2	= nLeaf;
						UI32 farLeafCurByteAfterIdx2				= nProps->iByteAfter;
						UI32 rootElemIdx							= nLeaf->partIdx.iElem;
						const char colChar							= (arrNodes == completed ? '+' : '|'); 
						STNBArray lines;
						NBArray_init(&lines, sizeof(STNBString), NULL);
						//Build lines
						{
							NBSintaxParser_treeConcatChildren_(obj, colChar, includePartIdxs, includeRanges, includeRetainCount, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &rootElemIdx, strAccum, strAccumSz, &lines);
						}
						//Concat type
						/*if(rootElemIdxLast != rootElemIdx)*/{
							NBASSERT(rootElemIdx >= 0 && rootElemIdx < obj->rules->elems.use)
							const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, rootElemIdx);
							if(rootAddedCount != 0){
								NBString_concatByte(dst, '\n');
							}
							NBString_concatByte(dst, '(');
							NBString_concatBytes(dst, ee->name, ee->nameLen);
							NBString_concatByte(dst, ')');
							NBString_concatByte(dst, '\n');
							rootElemIdxLast = rootElemIdx;
							rootAddedCount++;
						}
						//Concat lines
						{
							SI32 i; for(i = (SI32)lines.use - 1; i >= 0; i--){
								STNBString* line = NBArray_itmPtrAtIndex(&lines, STNBString, i);
								NBString_concatBytes(dst, line->str, line->length);
								NBString_concatByte(dst, '\n');
							}
						}
						//Release strings
						{
							SI32 i; for(i = 0; i < lines.use; i++){
								STNBString* line = NBArray_itmPtrAtIndex(&lines, STNBString, i);
								NBString_release(line);
							}
							NBArray_empty(&lines);
						}
						NBArray_release(&lines);
					}
				}
			}
		}
	}
	NBString_release(&strAccumTmp);
}

void NBSintaxParser_posibsConcat(const STNBSintaxParser* obj, const BOOL includePartIdxs, const BOOL includeRanges, const BOOL includeRetainCount, STNBString* dst){
	NBSintaxParser_treeConcat(obj, includePartIdxs, includeRanges, includeRetainCount, NULL, 0, NULL, dst);
	//IF_NBASSERT(NBSintaxParser_dbgTreeConcatDeflated(obj, includePartIdxs, includeRanges, includeRetainCount, NULL, 0, NULL, dst);)
}


//-----------------
//Results
//-----------------

void NBSintaxParser_resultsCloneChildren_(const STNBSintaxParser* obj, const BOOL reuseNamesPtrs, const STNBSintaxParserPartNode* parent, const STNBSintaxParserPartNode** farLeafCurRef, UI32* farLeafCurRefByteStartIdx, UI32* farLeafCurRefByteAfterIdx, ENSintaxParserElemType* dstType, UI32* dstElemIdx, STNBSintaxParserResultNode** dstParts, UI32* dstPartsSz){
	const STNBSintaxParserRules* rules = obj->rules; NBASSERT(rules != NULL)
	const STNBSintaxParserPartNode* farLeafCur = NULL;
	UI32 farLeafCurByteStartIdx = 0, farLeafCurByteAfterIdx = 0;
	ENSintaxParserElemType typeLast = ENSintaxParserElemType_Elem;
	UI32 elemIdxLast = 0;
	//
	if(farLeafCurRef != NULL) farLeafCur = *farLeafCurRef;
	if(farLeafCurRefByteStartIdx != NULL) farLeafCurByteStartIdx = *farLeafCurRefByteStartIdx;
	if(farLeafCurRefByteAfterIdx != NULL) farLeafCurByteAfterIdx = *farLeafCurRefByteAfterIdx;
	if(dstType != NULL) typeLast = *dstType;
	if(dstElemIdx != NULL) elemIdxLast = *dstElemIdx;
	//
	if(dstParts != NULL && dstPartsSz != NULL){
		STNBSintaxParserResultNode* partsCloneLast = NULL;
		UI32 partsCloneSzLast = 0;
		if(parent != NULL && farLeafCur != NULL){
			NBASSERT(farLeafCur->leafProps != NULL)
			if(farLeafCur->leafProps != NULL){
				//Build external branch of posibilities (and recursively inner branches)
				const UI32 farLeafByteAfterIdxMax = farLeafCurByteAfterIdx;
				const STNBSintaxParserPartNode* posibLeaf = farLeafCur;
				while(posibLeaf != NULL && posibLeaf != parent){
					NBASSERT(posibLeaf->partIdx.iElem >= 0 && posibLeaf->partIdx.iElem < obj->rules->elems.use)
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, posibLeaf->partIdx.iElem);
					//Literals and callbacks cant appear on top of elem_nodes.
					NBASSERT(partsCloneLast == NULL || ee->type == ENSintaxParserElemType_Elem)
					//FarLeaf cannot be NULL nor rootNode
					NBASSERT(farLeafCur != NULL && farLeafCur != obj->parser.tree.root.node)
					NBASSERT(farLeafCur->leafProps != NULL)
					//Ignore literals and callbacks
					if(ee->type == ENSintaxParserElemType_Elem){
						NBASSERT(posibLeaf->partIdx.iDef >= 0 && posibLeaf->partIdx.iDef < ee->defsSz)
						const STNBSintaxParserElemDef* def = &ee->defs[posibLeaf->partIdx.iDef];
						ENSintaxParserElemType type = ee->type;
						UI32 elemIdx = posibLeaf->partIdx.iElem;
						STNBSintaxParserResultNode* partsClone = NULL;
						UI32 partsCloneSz = 0;
						//Count posib parts
						{
							const STNBSintaxParserPartNode* nPart = posibLeaf;
							UI32 nPartIdxLast = posibLeaf->partIdx.iPart + 1;
							while(nPart != NULL){
								NBASSERT(nPart->partIdx.iElem == posibLeaf->partIdx.iElem)
								NBASSERT(nPart->partIdx.iDef == posibLeaf->partIdx.iDef)
								NBASSERT(nPart->partIdx.iPart < nPartIdxLast)
								partsCloneSz++;
								//Prev
								nPartIdxLast	= nPart->partIdx.iPart;
								nPart			= nPart->previous;
							}
							NBASSERT(partsCloneSz > 0)
						}
						//Clone posib parts
						partsClone = NBMemory_allocTypes(STNBSintaxParserResultNode, partsCloneSz);
						{
							SI32 iPart = (SI32)partsCloneSz;
							const STNBSintaxParserPartNode* nPart = posibLeaf;
							while(nPart != NULL){
								NBASSERT(nPart->partIdx.iPart >= 0 && nPart->partIdx.iPart < def->partsSz)
								const STNBSintaxParserElemDefPart* part = &def->parts[nPart->partIdx.iPart];
								STNBSintaxParserResultNode* pp = &partsClone[--iPart];
								NBMemory_setZeroSt(*pp, STNBSintaxParserResultNode);
								NBASSERT(part->iElem >= 0 && part->iElem < obj->rules->elems.use)
								const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, part->iElem);
								{
									//Base values
									pp->type	= ee->type;
									pp->iElem	= part->iElem;
									if(ee->type == ENSintaxParserElemType_Elem){
										if(ee->name != NULL){
											if(reuseNamesPtrs){
												pp->name = ee->name;
											} else {
												pp->name = NBString_strNewBuffer(ee->name);
											} 
										}
									}
									//Process node
									if(nPart == posibLeaf){
										//In leaf nodes, use the min-max current limits.
										NBASSERT(farLeafCurByteStartIdx <= farLeafByteAfterIdxMax)
										pp->iByteStart	= farLeafCurByteStartIdx;
										pp->iByteAfter	= farLeafByteAfterIdxMax;
										//In leaf nodes, use previous parts
										pp->parts		= partsCloneLast;
										pp->partsSz		= partsCloneSzLast;
									} else {
										NBASSERT(farLeafCurByteStartIdx <= farLeafCurByteAfterIdx)
										//Move to prev farLeaf
										NBASSERT(farLeafCur->leafProps != NULL)
										NBASSERT(farLeafCur->leafProps->prevLeaf != NULL)
										NBASSERT(farLeafCur->leafProps->prevLeaf != obj->parser.tree.root.node)
										if(farLeafCur->leafProps != NULL){
											const STNBSintaxParserPartNodeLeafProps* farLeafCurProps = farLeafCur->leafProps;
											NBASSERT(farLeafCurProps->prevLeaf != NULL)
											NBASSERT(farLeafCurProps->prevLeaf->leafProps != NULL)
											NBASSERT(farLeafCurProps->prevLeaf->leafProps->iByteStart <= farLeafCurProps->prevByteAfterIdx)
											NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteStartIdx)
											NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteAfterIdx)
											farLeafCur				= farLeafCurProps->prevLeaf;
											farLeafCurByteStartIdx	= farLeafCurProps->prevLeaf->leafProps->iByteStart;
											farLeafCurByteAfterIdx	= farLeafCurProps->prevByteAfterIdx;
										}
										NBASSERT(farLeafCurByteStartIdx <= farLeafCurByteAfterIdx)
										pp->iByteStart	= farLeafCurByteStartIdx;
										pp->iByteAfter	= farLeafCurByteAfterIdx;
										//Process children
										if(farLeafCur != nPart){
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											if(nPart->leafProps != NULL){
												NBASSERT(nPart->leafProps->childLeafRef == NULL) //Must not be a reference
											}
#											endif
											//Process child branch
											const STNBSintaxParserPartNode* parent2 = nPart;
											const STNBSintaxParserPartNode* farLeafCur2 = farLeafCur;
											UI32 farLeafCurByteStartIdx2	= farLeafCurByteStartIdx;
											UI32 farLeafCurByteAfterIdx2	= farLeafCurByteAfterIdx;
											ENSintaxParserElemType type2 	= pp->type;
											UI32 elemIdx2					= pp->iElem;
											{
												pp->iByteAfter	= farLeafCurByteAfterIdx;
												NBSintaxParser_resultsCloneChildren_(obj, reuseNamesPtrs, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &type2, &elemIdx2, &pp->parts, &pp->partsSz);
												pp->iByteStart	= farLeafCurByteStartIdx2;
												NBASSERT(type2 == pp->type)		//Top child must be a definition of this type
												NBASSERT(elemIdx2 == pp->iElem)	//Top child must be a definition of this type
											}
											//Move to prev farLeaf
											NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteAfterIdx2)
											NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteStartIdx)
											NBASSERT(farLeafCurByteAfterIdx2 <= farLeafCurByteAfterIdx)
											farLeafCurByteStartIdx	= farLeafCurByteStartIdx2;
											farLeafCurByteAfterIdx	= farLeafCurByteAfterIdx2;
											farLeafCur				= farLeafCur2;
										}
									}
									//Process childRef
									if(nPart->leafProps != NULL){
										if(nPart->leafProps->childLeafRef != NULL){
											NBASSERT(farLeafCur == nPart)					//In redundant nodes, the node is the leaf.
											if(farLeafCur == nPart){
												//Move to prev farLeaf
												NBASSERT(farLeafCur->leafProps != NULL)
												NBASSERT(farLeafCur->leafProps->prevLeaf != NULL)
												NBASSERT(farLeafCur->leafProps->prevLeaf != obj->parser.tree.root.node)
												if(farLeafCur->leafProps != NULL){
													const STNBSintaxParserPartNodeLeafProps* farLeafCurProps = farLeafCur->leafProps;
													NBASSERT(farLeafCurProps->prevLeaf != NULL)
													NBASSERT(farLeafCurProps->prevLeaf->leafProps != NULL)
													NBASSERT(farLeafCurProps->prevLeaf->leafProps->iByteStart <= farLeafCurProps->prevByteAfterIdx)
													NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteStartIdx)
													NBASSERT(farLeafCurProps->prevByteAfterIdx <= farLeafCurByteAfterIdx)
													farLeafCur				= farLeafCurProps->prevLeaf;
													farLeafCurByteStartIdx	= farLeafCurProps->prevLeaf->leafProps->iByteStart;
													farLeafCurByteAfterIdx	= farLeafCurProps->prevByteAfterIdx;
												}
												//Clone referenced branch
												{
													NBASSERT(nPart->dbg.chldrn.use == 0)			//No children must be added
													NBASSERT(nPart->leafProps != NULL)				//Must have leaf props
													NBASSERT(nPart->leafProps->prevLeaf != NULL)	//Must have prev farLeaf
													NBASSERT(nPart->leafProps->childLeafRef->parent != NULL)	//ChildRef must have a real-parent. 
													NBASSERT(pp->parts == NULL && pp->partsSz == 0)	//Must not have children yet
													//Process child branch
													const STNBSintaxParserPartNode* parent2 = nPart->leafProps->childLeafRef->parent;
													const STNBSintaxParserPartNode* farLeafCur2 = farLeafCur;
													UI32 farLeafCurByteStartIdx2	= farLeafCurByteStartIdx;
													UI32 farLeafCurByteAfterIdx2	= farLeafCurByteAfterIdx;
													ENSintaxParserElemType type2 	= pp->type;
													UI32 elemIdx2					= pp->iElem;
													//PRINTF_INFO("Cloning branchRef byPart(%llu) parent(%llu) farLeaf(%llu)-(%d,+%d).\n", (UI64)nPart, (UI64)parent2, (UI64)farLeafCur, farLeafCurByteStartIdx, (farLeafCurByteAfterIdx - farLeafCurByteStartIdx));
													{
														pp->iByteAfter	= farLeafCurByteAfterIdx;
														NBSintaxParser_resultsCloneChildren_(obj, reuseNamesPtrs, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &type2, &elemIdx2, &pp->parts, &pp->partsSz);
														pp->iByteStart	= farLeafCurByteStartIdx2;
														NBASSERT(type2 == pp->type)		//Top child must be a definition of this type
														NBASSERT(elemIdx2 == pp->iElem)	//Top child must be a definition of this type
													}
													//Move to prev farLeaf
													NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteAfterIdx2)
													NBASSERT(farLeafCurByteStartIdx2 <= farLeafCurByteStartIdx)
													NBASSERT(farLeafCurByteAfterIdx2 <= farLeafCurByteAfterIdx)
													farLeafCurByteStartIdx	= farLeafCurByteStartIdx2;
													farLeafCurByteAfterIdx	= farLeafCurByteAfterIdx2;
													farLeafCur				= farLeafCur2;
												}
											}
										}
									}
								}
								//Prev part
								nPart = nPart->previous;
							}
							NBASSERT(iPart == 0) //All part must be filled
						}
						//Keep track of last
						NBASSERT(partsClone != NULL)
						typeLast			= type;
						elemIdxLast			= elemIdx;
						partsCloneLast		= partsClone;
						partsCloneSzLast	= partsCloneSz;
					}
					//Parent posib
					NBASSERT(posibLeaf->parent != NULL) //All branches should lead to the root node (never NULL)
					posibLeaf = posibLeaf->parent;
				}
			}
		}
		//Set results
		if(partsCloneLast != NULL){
			NBASSERT(*dstParts == NULL)
			NBASSERT(*dstPartsSz == 0)
			*dstParts	= partsCloneLast;
			*dstPartsSz	= partsCloneSzLast;
		}
	}
	//
	if(farLeafCur != NULL) *farLeafCurRef = farLeafCur;
	if(farLeafCurRefByteStartIdx != NULL) *farLeafCurRefByteStartIdx = farLeafCurByteStartIdx;
	if(farLeafCurRefByteAfterIdx != NULL) *farLeafCurRefByteAfterIdx = farLeafCurByteAfterIdx;
	if(dstType != NULL) *dstType = typeLast;
	if(dstElemIdx != NULL) *dstElemIdx = elemIdxLast;
}
	
void NBSintaxParser_resultsCurClone_(const STNBSintaxParser* obj, const BOOL reuseNamesPtrs, STNBSintaxParserResults* dst, STNBString* dstStrAccum){
	const STNBSintaxParserState* parser = &obj->parser;
	const STNBSintaxParserRules* rules	= obj->rules; NBASSERT(rules != NULL)
	const STNBArray* completed = &parser->tree.leaves.completed;
	//Release current results
	NBSintaxParserResults_release(dst);
	NBSintaxParserResults_init(dst);
	//Clone
	if(completed->use > 0){
		dst->results 	= NBMemory_allocTypes(STNBSintaxParserResultNode, completed->use);
		dst->resultsSz	= 0;
		{
			const STNBSintaxParserPartNode* topRootNode = obj->parser.tree.root.node;
			SI32 i; for(i = 0; i < completed->use; i++){
				const STNBSintaxParserPartNode* nLeaf = NBArray_itmValueAtIndex(completed, STNBSintaxParserPartNode*, i);
				NBASSERT(nLeaf->leafProps != NULL)
				if(nLeaf->leafProps != NULL){
					STNBSintaxParserPartNodeLeafProps* nProps = nLeaf->leafProps;
					//
					ENSintaxParserElemType type = ENSintaxParserElemType_Elem;
					UI32 elemIdx = 0;
					STNBSintaxParserResultNode* partsArr = NULL;
					UI32 partsArrSz = 0;
					UI32 farLeafCurByteStartIdx2 = nProps->iByteStart;
					{
						const STNBSintaxParserPartNode* parent2		= topRootNode;
						const STNBSintaxParserPartNode* farLeafCur2	= nLeaf;
						UI32 farLeafCurByteAfterIdx2				= nProps->iByteAfter;
						NBSintaxParser_resultsCloneChildren_(obj, reuseNamesPtrs, parent2, &farLeafCur2, &farLeafCurByteStartIdx2, &farLeafCurByteAfterIdx2, &type, &elemIdx, &partsArr, &partsArrSz);
					}
					//Add root node
					if(partsArr != NULL && partsArrSz > 0){
						NBASSERT(dst->resultsSz < completed->use)
						STNBSintaxParserResultNode* rootClone = &dst->results[dst->resultsSz++];
						NBMemory_setZeroSt(*rootClone, STNBSintaxParserResultNode);
						//Populate
						rootClone->type		= type;
						rootClone->iElem	= elemIdx;
						NBASSERT(rootClone->type == ENSintaxParserElemType_Elem)
						if(rootClone->type == ENSintaxParserElemType_Elem){
							NBASSERT(rootClone->iElem >= 0 && rootClone->iElem < obj->rules->elems.use)
							const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, rootClone->iElem);
							if(ee->name != NULL){
								if(reuseNamesPtrs){
									rootClone->name = ee->name;
								} else {
									rootClone->name = NBString_strNewBuffer(ee->name);
								} 
							}
						}
						rootClone->iByteStart	= farLeafCurByteStartIdx2;
						rootClone->iByteAfter	= nProps->iByteAfter;
						NBASSERT(rootClone->iByteStart <= rootClone->iByteAfter)
						//
						rootClone->parts		= partsArr;
						rootClone->partsSz		= partsArrSz;
						//Length
						if(dst->resultsSz == 1){
							//Is first
							dst->iByteAfterMin = dst->iByteAfterMax = rootClone->iByteAfter;
						} else {
							//Is after first
							if(dst->iByteAfterMin > rootClone->iByteAfter) dst->iByteAfterMin = rootClone->iByteAfter;
							if(dst->iByteAfterMax < rootClone->iByteAfter) dst->iByteAfterMax = rootClone->iByteAfter;
						}
					}
				}
			}
		}
	}
	//strAccum
	if(dstStrAccum != NULL){
		NBString_empty(dstStrAccum);
		NBString_concatBytes(dstStrAccum, parser->relAccum.str, parser->relAccum.length);
	}
}

void NBSintaxParser_resultsLocalNullifyNamesAtChildren(STNBSintaxParserResultNode* nn){
	if(nn->parts != NULL && nn->partsSz > 0){
		UI32 i; for(i = 0; i < nn->partsSz; i++){
			STNBSintaxParserResultNode* nn2 = &nn->parts[i];
			nn2->name = NULL;
			if(nn2->parts != NULL && nn2->partsSz > 0){
				NBSintaxParser_resultsLocalNullifyNamesAtChildren(nn2);
			}
		}
	}
}

UI32 NBSintaxParser_resultsCurConsume_(STNBSintaxParser* obj){
	UI32 r = 0;
	STNBSintaxParserResultsHist* resultsHist = &obj->resultsHist;
	if(resultsHist->keepHist || obj->callbacks.consumeResults != NULL){
		STNBSintaxParserResults rr;
		NBSintaxParserResults_init(&rr);
		//Clone (using names references)
		NBSintaxParser_resultsCurClone_(obj, TRUE, &rr, NULL);
		NBASSERT(rr.iByteAfterMin >= 0 && rr.iByteAfterMin <= obj->parser.relAccum.length)
		NBASSERT(rr.iByteAfterMax >= 0 && rr.iByteAfterMax <= obj->parser.relAccum.length)
		//Notify or full-consume
		if(obj->callbacks.consumeResults == NULL){
			//Consume all
			r = rr.iByteAfterMax;
		} else {
			//Consume according callback
			r = (*obj->callbacks.consumeResults)(obj->callbacks.userParam, &rr, obj->parser.relAccum.str, rr.iByteAfterMax);
			//Remove non consumed results
			if(r > 0){
				if(!resultsHist->keepHist){
					//Validate consumed size
					if(r != rr.iByteAfterMin && r != rr.iByteAfterMax){ //Optimization: quick-validate against already known values.
						BOOL fnd = FALSE;
						if(rr.results != NULL && rr.resultsSz > 0){
							SI32 i; for(i = (SI32)rr.resultsSz - 1; i >= 0; i--){
								const STNBSintaxParserResultNode* nn = &rr.results[i];
								if(nn->iByteAfter == r){
									fnd = TRUE;
									break;
								}
							}
						}
						if(!fnd){
							PRINTF_ERROR("NBSintaxParser, consumeResults callback must return the length of one of the provided results.\n");
							r = 0;
						}
					}
				} else {
					if(rr.results != NULL && rr.resultsSz > 0){
						SI32 i; for(i = (SI32)rr.resultsSz - 1; i >= 0; i--){
							STNBSintaxParserResultNode* nn = &rr.results[i];
							if(nn->iByteAfter != r){
								//Nullify names
								nn->name = NULL;
								if(nn->parts != NULL && nn->partsSz > 0){
									NBSintaxParser_resultsLocalNullifyNamesAtChildren(nn);
								}
								//Release
								NBSintaxParserResults_nodeRelease(nn);
								//Remove it
								rr.resultsSz--;
								{
									SI32 i2; for(i2 = i; i2 < rr.resultsSz; i2++){
										rr.results[i2] = rr.results[i2 + 1]; 
									}
								}
							}
						}
						if(rr.resultsSz == 0){
							PRINTF_ERROR("NBSintaxParser, consumeResults callback must return the length of one of the provided results.\n");
							r = 0;
						}
						rr.iByteAfterMin = rr.iByteAfterMax = r;
					}
				}
			}
		}
		//Keeop or release
		if(resultsHist->keepHist && r > 0 && rr.results != NULL && rr.resultsSz > 0){
			STNBSintaxParserResultsHistRecord record;
			NBMemory_setZeroSt(record, STNBSintaxParserResultsHistRecord);
			record.results	= rr;
			NBString_initWithStrBytes(&record.strAccum, obj->parser.relAccum.str, r);
			NBArray_addValue(&resultsHist->records, record);
		} else {
			//Nullify names referenes before relasing
			if(rr.results != NULL && rr.resultsSz > 0){
				UI32 i; for(i = 0; i < rr.resultsSz; i++){
					STNBSintaxParserResultNode* nn = &rr.results[i];
					nn->name = NULL;
					if(nn->parts != NULL && nn->partsSz > 0){
						NBSintaxParser_resultsLocalNullifyNamesAtChildren(nn);
					}
				}
			}
			//Release
			NBSintaxParserResults_release(&rr);
		}
	}
	return r;
}

UI32 NBSintaxParser_resultsHistoryCount(const STNBSintaxParser* obj){
	return (UI32)obj->resultsHist.records.use;
}

BOOL NBSintaxParser_resultsHistoryGet(const STNBSintaxParser* obj, const UI32 index, const STNBSintaxParserResults** dstResultPtr, const char** dstStrAccum, UI32* dstStrAccumLen){
	BOOL r = FALSE;
	if(index < obj->resultsHist.records.use){
		STNBSintaxParserResultsHistRecord* hh = NBArray_itmPtrAtIndex(&obj->resultsHist.records, STNBSintaxParserResultsHistRecord, index);
		if(dstResultPtr != NULL) *dstResultPtr = &hh->results;
		if(dstStrAccum != NULL) *dstStrAccum = hh->strAccum.str;
		if(dstStrAccumLen != NULL) *dstStrAccumLen = hh->strAccum.length;
		r = TRUE;
	}
	return r;
}

void NBSintaxParser_resultsHistoryEmpty(STNBSintaxParser* obj){
	SI32 i; for(i = (SI32)obj->resultsHist.records.use - 1; i >= 0; i--){
		STNBSintaxParserResultsHistRecord* hh = NBArray_itmPtrAtIndex(&obj->resultsHist.records, STNBSintaxParserResultsHistRecord, i);
		//Nullify names referenes before relasing
		{
			STNBSintaxParserResults* rr = &hh->results;
			if(rr->results != NULL && rr->resultsSz > 0){
				UI32 i; for(i = 0; i < rr->resultsSz; i++){
					STNBSintaxParserResultNode* nn = &rr->results[i];
					nn->name = NULL;
					if(nn->parts != NULL && nn->partsSz > 0){
						NBSintaxParser_resultsLocalNullifyNamesAtChildren(nn);
					}
				}
			}
		}
		NBSintaxParserResults_release(&hh->results);
		NBString_release(&hh->strAccum);
	}
	NBArray_empty(&obj->resultsHist.records);
}

//-----------------
//Debug
//-----------------

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParser_dbgValidateTree(const STNBSintaxParser* obj, const UI32 utfCharSz){
	/*const STNBSintaxParserState* parser = &obj->parser;
	//Forward validation
	{
		STNBArray arrNodes;
		NBArray_init(&arrNodes, sizeof(STNBSintaxParserPartNode*), NULL);
		{
			SI32 i; for(i = 0; i < parser->tree.root.node->posibs.use; i++){
				STNBSintaxParserPartNode* nodeV	= NBArray_itmValueAtIndex(&parser->tree.root.node->posibs, STNBSintaxParserPartNode*, i);
				const BOOL isPosibCompleted		= FALSE; //NBSintaxParser_resultsPosibExists(obj, nodeV);
				const BOOL partentIsLastNodeH	= TRUE;
				NBSintaxParser_dbgValidateTreeNodePosibs(obj, isPosibCompleted, nodeV, parser->tree.root.node, partentIsLastNodeH, obj->parser.relAccum.length + utfCharSz, &arrNodes);
			}
		}
		NBArray_release(&arrNodes);
	}
	//Leaf reverse validation
	{
		const STNBArray* leaves = &parser->tree.leaves.open;
		SI32 i; for(i = 0; i < leaves->use; i++){
			const STNBSintaxParserPartNode* n = NBArray_itmValueAtIndex(leaves, STNBSintaxParserPartNode*, i);
			const STNBSintaxParserPartNode* leaf = n;
			const STNBSintaxParserPartNode* lastLeaf = NULL;
			while(leaf != NULL){
				NBASSERT(leaf->parent != NULL || leaf == parser->tree.root.node) //All leaves must have a parent node defined
				NBASSERT(leaf->partIdx.iElem >= 0 && leaf->partIdx.iElem < obj->rules->elems.use)
				lastLeaf = leaf;
				leaf = leaf->parent;
			}
			NBASSERT(lastLeaf == parser->tree.root.node) //Must be the root
		}
	}*/
}
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
/*void NBSintaxParser_dbgValidateTreeNodeAtArray(STNBArray* arrNodes / *(STNBSintaxParserPartNode*)* /, const STNBSintaxParserPartNode* node, const BOOL mustNotBeFound, const BOOL addIfNecesary){
	if(arrNodes != NULL){
		BOOL fnd = FALSE;
		SI32 i; for(i = 0; i < arrNodes->use; i++){
			const STNBSintaxParserPartNode* nn = NBArray_itmValueAtIndex(arrNodes, STNBSintaxParserPartNode*, i);
			if(nn == node){
				fnd = TRUE;
				break;
			}
		}
		NBASSERT(!(fnd && mustNotBeFound)) //Must not be found
		if(addIfNecesary){
			NBArray_addValue(arrNodes, node);
		}
	}
}*/
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
/*void NBSintaxParser_dbgValidateTreeNode(const STNBSintaxParser* obj, STNBArray* arrNodes / *(STNBSintaxParserPartNode*)* /, const STNBSintaxParserPartNode* node, const BOOL isPosibCompleted, const STNBSintaxParserPartNode* parent, const BOOL partentIsLastNodeH, const UI32 strAccumSz){
	const BOOL isLastNodeH	= (node->next == NULL);
	const BOOL isLastNodeV	= (node->posibs.use == 0);
	const BOOL mustBeLeaf	= (!isPosibCompleted && partentIsLastNodeH && isLastNodeH && isLastNodeV);
	const BOOL mustBePathOfLeaf = (!isPosibCompleted && partentIsLastNodeH && isLastNodeH);
	//
	NBASSERT(node->retainCount > 0)
	NBASSERT(parent->retainCount > 0)
	NBASSERT(node->parent == parent || node->parent == NULL)
	//
	NBASSERT(node->iByteStart >= 0)
	NBASSERT(node->iByteStart <= node->iByteAfter)
	NBASSERT(node->iByteAfter <= strAccumSz)
	//Validate leaf
	if(mustBeLeaf){
		//Must be a in leaf list
		NBASSERT(NBArray_indexOfValue(&obj->parser.tree.leaves.open, node) >= 0)
	} else {
		//Must not-be in leaf list
		NBASSERT(NBArray_indexOfValue(&obj->parser.tree.leaves.open, node) < 0)
	}
	//Validate path of leaf
	if(mustBePathOfLeaf){
		//NBASSERT(isPosibCompleted || node->parent != NULL) //Parent must be set
	} else {
		//NBASSERT(isPosibCompleted || node->parent == NULL) //Parent must be NULL
	}
	//Validate if repeats (leaves cannot be used mutiple times)
	if(arrNodes != NULL){
		const BOOL mustNotBeFound	= mustBeLeaf;
		const BOOL addIfNecesary	= TRUE;
		NBSintaxParser_dbgValidateTreeNodeAtArray(arrNodes, node, mustNotBeFound, addIfNecesary);
	}
}*/
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
/*void NBSintaxParser_dbgValidateTreeNodePosibs(const STNBSintaxParser* obj, const BOOL isPosibCompleted, const STNBSintaxParserPartNode* posibStart, const STNBSintaxParserPartNode* parent, const BOOL partentIsLastNodeH, const UI32 strAccumSz, STNBArray* arrNodes / *(STNBSintaxParserPartNode*)* /){
	if(posibStart != NULL){
		const STNBSintaxParserPartNode* nodeH = posibStart;
		//
		NBASSERT(nodeH->partIdx.iElem >= 0 && nodeH->partIdx.iElem < obj->rules->elems.use)
		const STNBSintaxParserElem* eee = NBArray_itmPtrAtIndex(&rules->elems, STNBSintaxParserElem, nodeH->partIdx.iElem);
		{
			switch(eee->type) {
				case ENSintaxParserElemType_Literal:
					{
						NBASSERT(nodeH->partIdx.iDef == 0)
						NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart <= eee->nameLen)
						NBASSERT(nodeH->posibs.use == 0)
						NBASSERT(nodeH->next == NULL)
						//Validate node
						NBSintaxParser_dbgValidateTreeNode(obj, arrNodes, nodeH, isPosibCompleted, parent, partentIsLastNodeH, strAccumSz);
					}
					break;
				case ENSintaxParserElemType_Callback:
					{
						NBASSERT(nodeH->partIdx.iDef == 0)
						NBASSERT(nodeH->partIdx.iPart >= 0)
						NBASSERT(nodeH->posibs.use == 0)
						NBASSERT(nodeH->next == NULL)
						//Validate node
						NBSintaxParser_dbgValidateTreeNode(obj, arrNodes, nodeH, isPosibCompleted, parent, partentIsLastNodeH, strAccumSz);
					}
					break;
				case ENSintaxParserElemType_Elem:
					{
						NBASSERT(nodeH->partIdx.iDef >= 0 && nodeH->partIdx.iDef < eee->defsSz)
						const STNBSintaxParserElemDef* def = &eee->defs[nodeH->partIdx.iDef];
						UI32 iPartLast = nodeH->partIdx.iPart;
						while(nodeH != NULL){
							const BOOL isLastNodeH = (nodeH->next == NULL);
							//Validate sequence
							NBASSERT(nodeH->partIdx.iElem == posibStart->partIdx.iElem)	//Must be same element
							NBASSERT(nodeH->partIdx.iDef == posibStart->partIdx.iDef)	//Must be same definition
							NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart <= def->partsSz)
							NBASSERT(iPartLast < nodeH->partIdx.iPart || nodeH == posibStart)	//Must be increase part sequence
							//Validate node
							NBSintaxParser_dbgValidateTreeNode(obj, arrNodes, nodeH, isPosibCompleted, parent, partentIsLastNodeH, strAccumSz);
							//Validate children
							{
								NBASSERT(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart < def->partsSz)
								if(nodeH->partIdx.iPart >= 0 && nodeH->partIdx.iPart < def->partsSz){
									const STNBSintaxParserElemDefPart* part = &def->parts[nodeH->partIdx.iPart];
									if(part->iElem >= 0 && part->iElem < obj->rules->elems.use){
										//Validate children
										SI32 i; for(i = 0; i < nodeH->posibs.use; i++){
											STNBSintaxParserPartNode* nodeV = NBArray_itmValueAtIndex(&nodeH->posibs, STNBSintaxParserPartNode*, i);
											NBASSERT(nodeV->partIdx.iElem == part->iElem) //Must be a definition of parent
											NBSintaxParser_dbgValidateTreeNodePosibs(obj, isPosibCompleted, nodeV, nodeH, (partentIsLastNodeH && isLastNodeH), strAccumSz, arrNodes);
										}
									}
								}
							}
							//Keep last Idx
							iPartLast = nodeH->partIdx.iPart;
							//Next
							nodeH = nodeH->next;
						}
					}
					break;
				default:
					NBASSERT(FALSE)
					break;
			}
		}
	}
}*/
#endif

