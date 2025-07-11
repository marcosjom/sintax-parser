#ifndef NB_SINTAX_PARSER_RESULTS_H
#define NB_SINTAX_PARSER_RESULTS_H

#include "nb/NBFrameworkDefs.h"
#include "nb/core/NBString.h"
#include "nb/sintax/NBSintaxParserDefs.h"
//
#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------
//- Preprocessed definition (interal)
//-----------------------------------

typedef struct STNBSintaxParserResultNodeRng_ {
	UI32 iStart;	//first node index
	UI32 iAfter;	//node after last		
} STNBSintaxParserResultNodeRng;

typedef struct STNBSintaxParserResultNode_ {
	ENSintaxParserElemType				type;
	UI32								iElem;
	char*								name;
	UI32								iByteStart;		//Byte start in 'relAccum'
	UI32								iByteAfter;		//Byte after in 'relAccum', size = (iByteAfter - iByteStart);
	struct STNBSintaxParserResultNode_*	parts;			//Parent node (NULL on completed posibs)
	UI32								partsSz;
} STNBSintaxParserResultNode;

typedef struct STNBSintaxParserResults_ {
	UI32								iByteAfterMin;		//Results str min-len
	UI32								iByteAfterMax;		//Results str max-len
	STNBSintaxParserResultNode*			results;
	UI32								resultsSz;
} STNBSintaxParserResults;

void NBSintaxParserResults_init(STNBSintaxParserResults* obj);
void NBSintaxParserResults_release(STNBSintaxParserResults* obj);

BOOL NBSintaxParserResults_nodeClone(const STNBSintaxParserResultNode* node, STNBSintaxParserResultNode* dst, const BOOL ignoreNames);
BOOL NBSintaxParserResults_nodeCloneFlattenRecursivity(const STNBSintaxParserResultNode* node, STNBSintaxParserResultNode* dst, const BOOL ignoreNames);
void NBSintaxParserResults_nodeRelease(STNBSintaxParserResultNode* node);
//
UI32 NBSintaxParserResults_nodeChildLeveasCount(const STNBSintaxParserResultNode* node);
UI32 NBSintaxParserResults_nodeChildCountByElemIdx(const STNBSintaxParserResultNode* node, const UI32 iElem);
SI32 NBSintaxParserResults_nodeChildElemIdxByValue(const STNBSintaxParserResultNode* node, const char* strAccum, const UI32 strAccumSz, const UI32 iElem, const char* val, const UI32 valSz);
//

void NBSintaxParserResults_concat(const STNBSintaxParserResults* src, const char* strAccum, const BOOL includeTree, STNBString* dst);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
