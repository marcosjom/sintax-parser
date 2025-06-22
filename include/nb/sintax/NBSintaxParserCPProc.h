#ifndef NB_SINTAX_PARSER_C_PPROC_H
#define NB_SINTAX_PARSER_C_PPROC_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/core/NBSintaxParser.h"
#include "nb/core/NBRange.h"
//
#include "nb/core/NBSintaxParserCDefs.h"
#include "nb/core/NBSintaxParserCTokens.h"
#include "nb/core/NBSintaxParserCPProcDefs.h"
#include "nb/core/NBSintaxParserCPProcUnit.h"

#ifdef __cplusplus
extern "C" {
#endif

//Pre-proc

typedef struct STNBSintaxParserCPProc_ {
	STNBArraySorted						macros;		//STNBSintaxParserCMacro*
	//root translation unit
	struct {
		STNBSintaxParserCPProcUnit		unit;		//translation unit 
		STNBSintaxParserCPProcUnitStack	stack;		//stack (to avoid recursivity)
		struct {
			STNBSintaxParserResultNode	tokens;		//tokens accumulation (for function-like macro calls)
			STNBString					strs;		//str of tokens in accumulation
		} accum;
	} root;
} STNBSintaxParserCPProc;

//

void NBSintaxParserCPProc_init(STNBSintaxParserCPProc* obj);
void NBSintaxParserCPProc_release(STNBSintaxParserCPProc* obj);

//Feed

BOOL NBSintaxParserCPProc_feedToken(STNBSintaxParserCPProc* obj, const ENSintaxParserC99TC3Rule type, const char* val, const UI32 valSz);
BOOL NBSintaxParserCPProc_feedUtf(STNBSintaxParserCPProc* obj, const char* utf, const UI32 utfSz);
BOOL NBSintaxParserCPProc_feedFlush(STNBSintaxParserCPProc* obj);

//

BOOL NBSintaxParserCPProc_macrosAdd(STNBSintaxParserCPProc* obj, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz);
BOOL NBSintaxParserCPProc_macrosFind(STNBSintaxParserCPProc* obj, const char* identifier, const UI32 identifierLen, STNBSintaxParserCMacro** dst);
void NBSintaxParserCPProc_macrosEmpty(STNBSintaxParserCPProc* obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
