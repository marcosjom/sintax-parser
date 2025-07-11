#ifndef NB_SINTAX_PARSER_C_TOKENS_H
#define NB_SINTAX_PARSER_C_TOKENS_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBArray.h"
#include "nb/sintax/NBSintaxParser.h"
//
#include "nb/sintax/NBSintaxParserCDefs.h"
#include "nb/sintax/NBSintaxParserResults.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct STNBSintaxParserCTokens_ {
	BOOL						errFnd;		//
	STNBSintaxParser			parser;		//Tokens parser
	STNBSintaxParserResultNode	found;		//Results
	UI32						foundRsrvd; //Optimization, size of buffer (to avoid growing one by one)
	STNBString					strAccum;	//String that forms the tokens found
} STNBSintaxParserCTokens;

//

void NBSintaxParserCTokens_init(STNBSintaxParserCTokens* obj);
void NBSintaxParserCTokens_release(STNBSintaxParserCTokens* obj);


//Callbacks
void NBSintaxParserCTokens_shareGenericParserCallbacks(STNBSintaxParserCTokens* obj, STNBSintaxParserCallbacks* dst);

//Rules

BOOL NBSintaxParserCTokens_rulesAreLoaded(const STNBSintaxParserCTokens* obj);
BOOL NBSintaxParserCTokens_rulesLoadDefault(STNBSintaxParserCTokens* obj);
BOOL NBSintaxParserCTokens_rulesShareWithOther(STNBSintaxParserCTokens* obj, STNBSintaxParserCTokens* other);
BOOL NBSintaxParserCTokens_rulesShareWithParser(STNBSintaxParserCTokens* obj, STNBSintaxParser* other);

//Feed

BOOL NBSintaxParserCTokens_isTokenInProgress(const STNBSintaxParserCTokens* obj);
BOOL NBSintaxParserCTokens_feedStart(STNBSintaxParserCTokens* obj, const char** rootElems, const UI32 rootElemsSz);
BOOL NBSintaxParserCTokens_feedByte(STNBSintaxParserCTokens* obj, const char c);
BOOL NBSintaxParserCTokens_feedFlush(STNBSintaxParserCTokens* obj);

//Results
UI32 NBSintaxParserCTokens_resultsTokensCount(const STNBSintaxParserCTokens* obj);
void NBSintaxParserCTokens_resultsGet(const STNBSintaxParserCTokens* obj, STNBSintaxParserResultNode** const dstTokens, UI32* dstTokenSz, const char** dstStrAccum, UI32* dstStrAccumSz);
void NBSintaxParserCTokens_resultsClear(STNBSintaxParserCTokens* obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
