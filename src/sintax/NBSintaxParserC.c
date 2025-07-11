#include "nb/NBFrameworkPch.h"
#include "nb/sintax/NBSintaxParserC.h"
//
#include "nb/core/NBMemory.h"
#include "nb/core/NBThreadMutex.h"
//

#define NB_SINTAX_PARSER_C_IS_SPACE_CHAR(C)		(C == ' ' || C == '\t')

//C99_TC3: http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

void NBSintaxParserCFile_init(STNBSintaxParserCFile* obj);
void NBSintaxParserCFile_release(STNBSintaxParserCFile* obj);

//Phase1
void NBSintaxParserC_phase1FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c);
void NBSintaxParserC_phase1Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file);

//Phase2
void NBSintaxParserC_phase2FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c);
void NBSintaxParserC_phase2Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file);

//Phase3
void NBSintaxParserC_phase3FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c);
void NBSintaxParserC_phase3Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file);
void NBSintaxParserC_phase3FlushTokensQueue(STNBSintaxParserC* obj, STNBSintaxParserCFile* file);
void NBSintaxParserC_phase3ConsumeNonTokenBytes(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char* str, const UI32 strSz);
BOOL NBSintaxParserC_phase3FeedByteAsCommentChar(STNBSintaxParserCPhase3* phase3, const char c, char* dstExtraPreChar);

//Phase4
void NBSintaxParserC_phase4FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c);
void NBSintaxParserC_phase4FeedToken(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const UI32 iElem, const char* token, const UI32 tokenSz);
void NBSintaxParserC_phase4ConsumeResults(STNBSintaxParserC* obj, STNBSintaxParserCFile* file);
void NBSintaxParserC_phase4ConsumeResults_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz);
BOOL NBSintaxParserC_phase4ConsumeResultsInclude_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResultNode* rslt, const char* strAccum, const UI32 strAccumSz);
BOOL NBSintaxParserC_phase4ConsumeResultsEvalIfExpression_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResultNode* rslt, const char* strAccum, const UI32 strAccumSz, BOOL* dstResult);

//Filestack
STNBSintaxParserCFile* NBSintaxParserC_filePush(STNBSintaxParserC* obj, const char* filepath, const UI32 filepathSz);
void NBSintaxParserC_filePop(STNBSintaxParserC* obj);

//PProc conds
void NBSintaxParserC_pprocCondPush(STNBSintaxParserC* obj, const BOOL condIsTrue);		//if, ifdef, ifndef found
BOOL NBSintaxParserC_pprocCondPartAdd(STNBSintaxParserC* obj, const ENSintaxParserCPProcCondPartType type, const BOOL condIsTrue); //elif, else found
BOOL NBSintaxParserC_pprocCondPop(STNBSintaxParserC* obj); //endif found
#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParserC_pprocCondAssert(const STNBSintaxParserC* obj);
#endif

//

void NBSintaxParserC_init(STNBSintaxParserC* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserC);
	//Config
	{
		STNBSintaxParserCConfig* cfg = &obj->cfg;
		//include paths
		{
			NBArray_init(&cfg->includePaths.usr, sizeof(UI32), NULL);
			NBArray_init(&cfg->includePaths.sys, sizeof(UI32), NULL);
			NBString_init(&cfg->includePaths.str);
			NBString_concatByte(&cfg->includePaths.str, '\0');
		}
	}
	//Parser
	{
		STNBSintaxParserCState* parser = &obj->parser;
		//Rules
		{
			NBSintaxParserCTokens_init(&parser->rules.templates);
		}
		//Files
		{
			NBArray_init(&parser->files.arr, sizeof(STNBSintaxParserCFile*), NULL);
			NBArray_init(&parser->files.stack, sizeof(STNBSintaxParserCFile*), NULL);
		}
		//Pproc conds
		{
			NBArray_init(&parser->pprocConds.arr, sizeof(STNBSintaxParserCPProcCondPart*), NULL);
			NBArray_init(&parser->pprocConds.stack, sizeof(UI32), NULL);
		}
	}
	//Pre-proc
	{
		STNBSintaxParserCPProc* pproc = &obj->pproc;
		NBSintaxParserCPProc_init(pproc);
	}
}

void NBSintaxParserC_release(STNBSintaxParserC* obj){
	//Config
	{
		STNBSintaxParserCConfig* cfg = &obj->cfg;
		//include paths
		{
			NBArray_release(&cfg->includePaths.usr);
			NBArray_release(&cfg->includePaths.sys);
			NBString_release(&cfg->includePaths.str);
		}
	}
	//Parser
	{
		STNBSintaxParserCState* parser = &obj->parser;
		//Rules
		{
			NBSintaxParserCTokens_release(&parser->rules.templates);
		}
		//Files
		{
			SI32 i; for(i = 0; i < parser->files.arr.use; i++){
				STNBSintaxParserCFile* file = NBArray_itmValueAtIndex(&parser->files.arr, STNBSintaxParserCFile*, i);
				NBSintaxParserCFile_release(file);
				NBMemory_free(file);
			}
			NBArray_empty(&parser->files.arr);
			NBArray_release(&parser->files.arr);
			//
			NBArray_empty(&parser->files.stack);
			NBArray_release(&parser->files.stack);
		}
		//Pproc conds
		{
			SI32 i; for(i = 0; i < parser->pprocConds.arr.use; i++){
				STNBSintaxParserCPProcCondPart* cPart = NBArray_itmValueAtIndex(&parser->pprocConds.arr, STNBSintaxParserCPProcCondPart*, i);
				NBMemory_free(cPart);
			}
			NBArray_empty(&parser->pprocConds.arr);
			NBArray_release(&parser->pprocConds.arr);
			//
			NBArray_empty(&parser->pprocConds.stack);
			NBArray_release(&parser->pprocConds.stack);
		}
	}
	//Pre-proc
	{
		STNBSintaxParserCPProc* pproc = &obj->pproc;
		NBSintaxParserCPProc_release(pproc);
	}
}

//--------------
//- Config
//--------------

void NBSintaxParserC_cfgIncludePathsUsrAdd(STNBSintaxParserC* obj, const char* path){
	STNBSintaxParserCConfig* cfg = &obj->cfg;
	const UI32 iStart = cfg->includePaths.str.length;
	NBArray_addValue(&cfg->includePaths.usr, iStart);
	//
	NBString_concat(&cfg->includePaths.str, path);
	NBString_concatByte(&cfg->includePaths.str, '\0');
}

void NBSintaxParserC_cfgIncludePathsSysAdd(STNBSintaxParserC* obj, const char* path){
	STNBSintaxParserCConfig* cfg = &obj->cfg;
	const UI32 iStart = cfg->includePaths.str.length;
	NBArray_addValue(&cfg->includePaths.sys, iStart);
	//
	NBString_concat(&cfg->includePaths.str, path);
	NBString_concatByte(&cfg->includePaths.str, '\0');
}

//--------------
//- Feed content
//--------------

BOOL NBSintaxParserC_feedStart(STNBSintaxParserC* obj, const char* rootFilename){
	BOOL r = FALSE;
	//Restarts parse state
	{
		STNBSintaxParserCState* parser = &obj->parser;
		parser->errFnd			= FALSE;
		parser->completed		= FALSE;
		//Empty files
		{
			SI32 i; for(i = 0; i < parser->files.arr.use; i++){
				STNBSintaxParserCFile* file = NBArray_itmValueAtIndex(&parser->files.arr, STNBSintaxParserCFile*, i);
				NBSintaxParserCFile_release(file);
				NBMemory_free(file);
			}
			NBArray_empty(&parser->files.arr);
			NBArray_empty(&parser->files.stack);
		}
		//Empty pproc-conds
		{
			SI32 i; for(i = 0; i < parser->pprocConds.arr.use; i++){
				STNBSintaxParserCPProcCondPart* cPart = NBArray_itmValueAtIndex(&parser->pprocConds.arr, STNBSintaxParserCPProcCondPart*, i);
				NBMemory_free(cPart);
			}
			NBArray_empty(&parser->pprocConds.arr);
			NBArray_empty(&parser->pprocConds.stack);
			parser->pprocConds.curPartIsEnabled		= TRUE;
			parser->pprocConds.curActivePartFnd		= TRUE;
		}
		//Empty pproc
		{
			STNBSintaxParserCPProc* pproc = &obj->pproc;
			NBSintaxParserCPProc_macrosEmpty(pproc);
			NBASSERT(pproc->macros.use == 0)
		}
		//Load rules templates
		if(!NBSintaxParserCTokens_rulesAreLoaded(&parser->rules.templates)){
			if(!NBSintaxParserCTokens_rulesLoadDefault(&parser->rules.templates)){
				PRINTF_ERROR("NBSintaxParserC, rulesFeed failed.\n");
				parser->errFnd = TRUE;
			}
		}
		//Push root file
		{
			UI32 rootFilenameSz = 0;
			if(!NBString_strIsEmpty(rootFilename)){
				rootFilenameSz = NBString_strLenBytes(rootFilename);
			}
			NBSintaxParserC_filePush(obj, rootFilename, rootFilenameSz);
		}
		
		//Result
		r = (!parser->errFnd);
	}
	return r;
}
	
BOOL NBSintaxParserC_feed(STNBSintaxParserC* obj, const char* syntaxDefs){
	BOOL r = FALSE;
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		if(syntaxDefs != NULL && !parser->errFnd){
			const char* c = syntaxDefs;
			while(*c != '\0'){
				if(!NBSintaxParserC_feedByte(obj, *c)){
					parser->errFnd = TRUE;
					break;
				}
				c++;
			}
		}
		r = (!parser->errFnd);
	}
	return r;
}

BOOL NBSintaxParserC_feedBytes(STNBSintaxParserC* obj, const char* syntaxDefs, const SI32 syntaxDefsSz){
	BOOL r = FALSE;
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		if(syntaxDefs != NULL &&  syntaxDefsSz > 0 && !parser->errFnd){
			const char* c = syntaxDefs;
			const char* cAfterEnd = &syntaxDefs[syntaxDefsSz];
			while(c < cAfterEnd){
				if(!NBSintaxParserC_feedByte(obj, *c)){
					parser->errFnd = TRUE;
					break;
				}
				c++;
			}
		}
		r = (!parser->errFnd);
	}
	return r;
}
	
BOOL NBSintaxParserC_feedByte(STNBSintaxParserC* obj, const char c){
	BOOL r = FALSE;
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		if(parser->files.stack.use != 1){
			PRINTF_ERROR("NBSintaxParserC, expected root file at stack.\n");
			parser->errFnd = TRUE;
		} else {
			STNBSintaxParserCFile* file = NBArray_itmValueAtIndex(&parser->files.stack, STNBSintaxParserCFile*, 0);
			NBSintaxParserC_phase1FeedByte(obj, file, c);
		}
		r = (!parser->errFnd);
	}
	return r;
}

BOOL NBSintaxParserC_feedEnd(STNBSintaxParserC* obj){
	BOOL r = FALSE;
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		//Flush file
		{
			if(parser->files.stack.use != 1){
				PRINTF_ERROR("NBSintaxParserC, expected root file at stack.\n");
				parser->errFnd = TRUE;
			} else {
				//Pop root file (it will flush and validate the state)
				NBSintaxParserC_filePop(obj);
			}
		}
		//ToDo: implement
		//Post-process
		if(!parser->errFnd){
			//ToDo: implement
		}
		//
		r = (!parser->errFnd);
	}
	return r;
}

//Files

void NBSintaxParserCFile_init(STNBSintaxParserCFile* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserCFile);
	//Filename
	NBString_init(&obj->filename);
	//Phase 1
	//Phase 2
	//Phase 3
	NBSintaxParserCTokens_init(&obj->phase3.pptokens.queue);
	//Phase 4
	NBSintaxParser_init(&obj->phase4.pproc.parser);
}

void NBSintaxParserCFile_release(STNBSintaxParserCFile* obj){
	//Filename
	NBString_release(&obj->filename);
	//Phase 1
	NBMemory_setZero(obj->phase1);
	//Phase 2
	NBMemory_setZero(obj->phase2);
	//Phase 3
	NBSintaxParserCTokens_release(&obj->phase3.pptokens.queue);
	NBMemory_setZero(obj->phase3);
	//Phase 4
	NBSintaxParser_release(&obj->phase4.pproc.parser);
	NBMemory_setZero(obj->phase4);
}

//Filestack

STNBSintaxParserCFile* NBSintaxParserC_filePush(STNBSintaxParserC* obj, const char* filepath, const UI32 filepathSz){
	STNBSintaxParserCState* parser = &obj->parser;
	STNBSintaxParserCFile* file = NBMemory_allocType(STNBSintaxParserCFile);
	NBSintaxParserCFile_init(file);
	//Filename
	if(!NBString_strIsEmpty(filepath) && filepathSz > 0){
		NBString_setBytes(&file->filename, filepath, filepathSz);
	}
	//Load rules
	{
		//Phase 1
		//Phase 2
		//Phase 3
		{
			STNBSintaxParserCPhase3* phase3 = &file->phase3;
			//Comments
			NBMemory_setZero(phase3->comment); //Already zero-filled
			//Load rules
			if(!NBSintaxParserCTokens_rulesAreLoaded(&file->phase3.pptokens.queue)){
				if(!NBSintaxParserCTokens_rulesLoadDefault(&file->phase3.pptokens.queue)){
					PRINTF_ERROR("NBSintaxParserC, rulesFeed failed.\n");
					parser->errFnd = TRUE;
				} else {
					//Apply rules to other phases
					{
						STNBSintaxParserCPhase4* phase4 = &file->phase4;
						NBSintaxParserCTokens_rulesShareWithParser(&file->phase3.pptokens.queue, &phase4->pproc.parser);
						//Set callback
						{
							STNBSintaxParserCallbacks callbacks;
							NBMemory_setZeroSt(callbacks, STNBSintaxParserCallbacks);
							NBSintaxParserCTokens_shareGenericParserCallbacks(&file->phase3.pptokens.queue, &callbacks);
							NBSintaxParser_setCallbacks(&phase4->pproc.parser, &callbacks);
						}
					}
				}
			}
			//Start feed phase3
			{
				const char* rootElems[]	= { "preprocessing-token" };
				if(!NBSintaxParserCTokens_feedStart(&file->phase3.pptokens.queue, rootElems, (sizeof(rootElems) / sizeof(rootElems[0])))){
					PRINTF_ERROR("NBSintaxParserC, NBSintaxParserCTokens_feedStart failed.\n");
					parser->errFnd = TRUE;
				}
			}
			//Start feed phase4
			{
				STNBSintaxParserCPhase4* phase4 = &file->phase4;
				phase4->pproc.lastFeedWasSpace	= FALSE;
				phase4->pproc.curLineStarted	= FALSE;
				phase4->pproc.linesCount		= 0;
				phase4->pproc.isOpen			= FALSE;
				//pproc line parser
				{
					const char* rootElems[]		= { "if-group", "elif-group", "else-group", "endif-line", "control-line" };
					STNBSintaxParserConfig config;
					NBMemory_setZeroSt(config, STNBSintaxParserConfig);
					config.rootElems	= rootElems;
					config.rootElemsSz	= (sizeof(rootElems) / sizeof(rootElems[0]));
					config.resultsMode	= ENSintaxParserResultsMode_LongestsPerRootChildElem;
					config.resultsKeepHistory = TRUE; //Necesary when reading results without callback
					NBSintaxParser_feedStart(&phase4->pproc.parser, &config);
				}
			}
		}
	}
	//Add to files array and stack
	NBArray_addValue(&obj->parser.files.arr, file);
	NBArray_addValue(&obj->parser.files.stack, file);
	//
	return file;
}

void NBSintaxParserC_filePop(STNBSintaxParserC* obj){
	if(obj->parser.files.stack.use > 0){
		STNBSintaxParserCFile* file = NBArray_itmValueAtIndex(&obj->parser.files.stack, STNBSintaxParserCFile*, obj->parser.files.stack.use - 1);
		//Flush
		{
			//Flush phase 1
			if(!obj->parser.errFnd){
				NBSintaxParserC_phase1Flush(obj, file);
			}
			//Flush phase 2
			if(!obj->parser.errFnd){
				NBSintaxParserC_phase2Flush(obj, file);
			}
			//Flush phase 3
			if(!obj->parser.errFnd){
				NBSintaxParserC_phase3Flush(obj, file);
			}
		}
		//Remove from stack
		NBArray_removeItemAtIndex(&obj->parser.files.stack, obj->parser.files.stack.use - 1);
	}
}

//if, ifdef, ifndef found
void NBSintaxParserC_pprocCondPush(STNBSintaxParserC* obj, const BOOL condIsTrue){
	const UI32 iPos = obj->parser.pprocConds.arr.use;
	STNBSintaxParserCPProcCondPart* part = NBMemory_allocType(STNBSintaxParserCPProcCondPart);
	NBMemory_setZeroSt(*part, STNBSintaxParserCPProcCondPart);
	part->isEnabled = part->activePartFound = condIsTrue;
	part->type		= ENSintaxParserCPProcCondPartType_If;
	part->iRoot		= part->iPrev = part->iNext = iPos;
	part->iAfterEnd = iPos + 1;
	//Set flag
	{
		BOOL parentIsEnabled = TRUE;
		if(obj->parser.pprocConds.stack.use <= 0){
			part->iParent	= -1;
		} else {
			const UI32 iParent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 1);
			STNBSintaxParserCPProcCondPart* parent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iParent);
			part->iParent	= iParent; 
			parentIsEnabled = parent->isEnabled;
		}
		obj->parser.pprocConds.curPartIsEnabled		= (parentIsEnabled && part->isEnabled);
		obj->parser.pprocConds.curActivePartFnd		= part->isEnabled;
	}
	//Add
	NBArray_addValue(&obj->parser.pprocConds.arr, part);
	NBArray_addValue(&obj->parser.pprocConds.stack, iPos);
	//Validate
	IF_NBASSERT(NBSintaxParserC_pprocCondAssert(obj);)
}

//elif, else found
BOOL NBSintaxParserC_pprocCondPartAdd(STNBSintaxParserC* obj, const ENSintaxParserCPProcCondPartType type, const BOOL condIsTrue){
	BOOL r = FALSE;
	NBASSERT(type == ENSintaxParserCPProcCondPartType_Elif || type == ENSintaxParserCPProcCondPartType_Else)
	NBASSERT(obj->parser.pprocConds.arr.use > 0 && obj->parser.pprocConds.stack.use > 0)
	if(type == ENSintaxParserCPProcCondPartType_Elif || type == ENSintaxParserCPProcCondPartType_Else){
		if(obj->parser.pprocConds.arr.use > 0 && obj->parser.pprocConds.stack.use > 0){
			const UI32 iCur = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 1);
			STNBSintaxParserCPProcCondPart* cur = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iCur);
			STNBSintaxParserCPProcCondPart* root = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, cur->iRoot);
			if(cur->type != ENSintaxParserCPProcCondPartType_Else){
				BOOL parentIsEnabled = TRUE;
				const UI32 iPos = obj->parser.pprocConds.arr.use;
				STNBSintaxParserCPProcCondPart* part = NBMemory_allocType(STNBSintaxParserCPProcCondPart);
				NBMemory_setZeroSt(*part, STNBSintaxParserCPProcCondPart);
				//Check parent
				if(obj->parser.pprocConds.stack.use > 1){
					const UI32 iParent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 2);
					STNBSintaxParserCPProcCondPart* parent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iParent);
					NBASSERT(cur->iParent == iParent)
					parentIsEnabled = parent->isEnabled;
				}
				//Set values
				if(parentIsEnabled && !root->activePartFound){
					if(type == ENSintaxParserCPProcCondPartType_Elif){
						if(condIsTrue){
							part->isEnabled = root->activePartFound = TRUE;
						}
					} else {
						NBASSERT(type == ENSintaxParserCPProcCondPartType_Else)
						part->isEnabled = root->activePartFound = TRUE;
					}
				}
				part->type		= type;
				//Set parent
				part->iParent	= cur->iParent; 
				part->iRoot		= cur->iRoot; 
				part->iPrev		= iCur;
				part->iNext		= cur->iNext = iPos;
				part->iAfterEnd = cur->iAfterEnd = root->iAfterEnd = iPos + 1;
				//Update all mid-parts after-end
				{
					STNBSintaxParserCPProcCondPart* pp = root;
					while(pp < cur){
						pp->iAfterEnd = iPos + 1;
						pp++;
					}
				}
				//Set flag
				obj->parser.pprocConds.curPartIsEnabled		= part->isEnabled;
				obj->parser.pprocConds.curActivePartFnd		= root->activePartFound;
				//Add
				NBArray_addValue(&obj->parser.pprocConds.arr, part);
				//Update stack
				NBArray_setItemAt(&obj->parser.pprocConds.stack, obj->parser.pprocConds.stack.use - 1, &iPos, sizeof(iPos));
				//Validate
				IF_NBASSERT(NBSintaxParserC_pprocCondAssert(obj);)
				//result
				r = TRUE;

			}
		}
	}
	return r;
}

//endif found
BOOL NBSintaxParserC_pprocCondPop(STNBSintaxParserC* obj){
	BOOL r = FALSE;
	NBASSERT(obj->parser.pprocConds.arr.use > 0 && obj->parser.pprocConds.stack.use > 0)
	if(obj->parser.pprocConds.arr.use > 0 && obj->parser.pprocConds.stack.use > 0){
		const UI32 iCur = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 1);
		STNBSintaxParserCPProcCondPart* cur = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iCur);
		STNBSintaxParserCPProcCondPart* root = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, cur->iRoot);
		BOOL parentIsEnabled = TRUE;
		const UI32 iPos = obj->parser.pprocConds.arr.use;
		STNBSintaxParserCPProcCondPart* part = NBMemory_allocType(STNBSintaxParserCPProcCondPart);
		NBMemory_setZeroSt(*part, STNBSintaxParserCPProcCondPart);
		//Check parent
		if(obj->parser.pprocConds.stack.use > 1){
			const UI32 iParent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 2);
			STNBSintaxParserCPProcCondPart* parent = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iParent);
			NBASSERT(cur->iParent == iParent)
			parentIsEnabled = parent->isEnabled;
		}
		//Set values
		part->type		= ENSintaxParserCPProcCondPartType_Endif;
		//Set parent
		part->iParent	= cur->iParent; 
		part->iRoot		= cur->iRoot; 
		part->iPrev		= iCur;
		part->iNext		= cur->iNext = iPos;
		part->iAfterEnd = cur->iAfterEnd = root->iAfterEnd = iPos + 1;
		//Update all mid-parts after-end
		{
			STNBSintaxParserCPProcCondPart* pp = root;
			while(pp < cur){
				pp->iAfterEnd = iPos + 1;
				pp++;
			}
		}
		//Add
		NBArray_addValue(&obj->parser.pprocConds.arr, part);
		//Remove from stack
		NBArray_removeItemAtIndex(&obj->parser.pprocConds.stack, obj->parser.pprocConds.stack.use - 1);
		//Set flag
		if(obj->parser.pprocConds.stack.use == 0){
			obj->parser.pprocConds.curPartIsEnabled		= TRUE;
			obj->parser.pprocConds.curActivePartFnd		= TRUE;
		} else {
			const UI32 iCur = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, obj->parser.pprocConds.stack.use - 1);
			STNBSintaxParserCPProcCondPart* cur			= NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, iCur);
			STNBSintaxParserCPProcCondPart* root		= NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, cur->iRoot);
			obj->parser.pprocConds.curPartIsEnabled		= cur->isEnabled;
			obj->parser.pprocConds.curActivePartFnd		= root->activePartFound;
		}
		//Validate
		IF_NBASSERT(NBSintaxParserC_pprocCondAssert(obj);)
		//result
		r = TRUE;
	}
	return r;
}

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParserC_pprocCondAssertBranch_(const STNBSintaxParserC* obj, const SI32 iParent, const BOOL parentIsEnabled, UI32* idxRef){
	const STNBSintaxParserCState* state = &obj->parser;
	//
	const UI32 iFirst = *idxRef;
	const STNBSintaxParserCPProcCondPart* first = NBArray_itmValueAtIndex(&state->pprocConds.arr, STNBSintaxParserCPProcCondPart*, iFirst);
	const STNBSintaxParserCPProcCondPart* prevPart = first;
	UI32 iPrevPart = iFirst;
	BOOL lastIsEnabled = first->isEnabled;
	BOOL activePartFound = FALSE, haveExplicitEnd = FALSE;
	while(*idxRef < state->pprocConds.arr.use){
		const UI32 iPart = *idxRef; 
		const STNBSintaxParserCPProcCondPart* part = NBArray_itmValueAtIndex(&state->pprocConds.arr, STNBSintaxParserCPProcCondPart*, iPart);
		if(iPart == iFirst || part->iParent == iParent){
			NBASSERT((iPart == iFirst && part->type == ENSintaxParserCPProcCondPartType_If) || (iPart != iFirst && (part->type == ENSintaxParserCPProcCondPartType_Elif || part->type == ENSintaxParserCPProcCondPartType_Else || part->type == ENSintaxParserCPProcCondPartType_Endif)))
			NBASSERT(part->iParent == iParent)	//Parent must match
			NBASSERT(part->iRoot == iFirst)		//Root must batch
			NBASSERT(part->iPrev == iPrevPart)	//Prev must batch
			NBASSERT(prevPart->iNext == iPart || prevPart == part) //Next must match
			NBASSERT(part->iAfterEnd == first->iAfterEnd);
			NBASSERT(!(activePartFound && part->activePartFound))
			NBASSERT(!(!parentIsEnabled && part->isEnabled)) //No child can be enabled if parent is not
			prevPart		= part;
			iPrevPart		= iPart;
			lastIsEnabled	= part->isEnabled;
			if(part->isEnabled){
				activePartFound = TRUE;
			}
			(*idxRef)++;
			//Stop
			if(part->type == ENSintaxParserCPProcCondPartType_Endif){
				haveExplicitEnd = TRUE;
				break;
			}
		} else {
			NBASSERT(part->iParent == iPrevPart) //Parent must be this part
			NBSintaxParserC_pprocCondAssertBranch_(obj, iPrevPart, lastIsEnabled, idxRef);
			NBASSERT(iPart < *idxRef) //Must advance
		}
	}
	NBASSERT(*idxRef == first->iAfterEnd || !haveExplicitEnd) //Must match end
	NBASSERT(activePartFound == first->activePartFound) //Must match
}
#endif

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParserC_pprocCondAssert(const STNBSintaxParserC* obj){
	const STNBSintaxParserCState* state = &obj->parser;
	//Validate stack
	{
		SI32 iParent = -1; BOOL lastIsEnabled = TRUE, lastActivePartFound = TRUE;
		SI32 i; for(i = 0; i < state->pprocConds.stack.use; i++){
			const UI32 idx = NBArray_itmValueAtIndex(&obj->parser.pprocConds.stack, UI32, i);
			const STNBSintaxParserCPProcCondPart* part = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, idx);
			const STNBSintaxParserCPProcCondPart* root = NBArray_itmValueAtIndex(&obj->parser.pprocConds.arr, STNBSintaxParserCPProcCondPart*, part->iRoot);
			NBASSERT(iParent == part->iParent)
			NBASSERT(!(!lastIsEnabled && part->isEnabled)) //No child can be enabled if parent is not
			lastIsEnabled = part->isEnabled;
			lastActivePartFound = root->activePartFound;
			iParent = idx;
		}
		NBASSERT(state->pprocConds.curPartIsEnabled == lastIsEnabled)
		NBASSERT(state->pprocConds.curActivePartFnd == lastActivePartFound)
	}
	//Validate sequence
	if(state->pprocConds.arr.use > 0){
		UI32 idx = 0;
		NBSintaxParserC_pprocCondAssertBranch_(obj, -1, TRUE, &idx);
		NBASSERT(idx == state->pprocConds.arr.use)
	}
}
#endif
//Phase 1

void NBSintaxParserC_phase1FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c){
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		//Phase 1: map characters and replace trigraph sequences.
		STNBSintaxParserCPhase1* phase1 = &file->phase1;
		BOOL isTriglaphPart = FALSE;
		if(c == '?'){
			phase1->trigraph.partsAccum++;
			//Flush extra '?'
			while(phase1->trigraph.partsAccum > 2){
				NBSintaxParserC_phase2FeedByte(obj, file, '?');
				phase1->trigraph.partsAccum--;
			}
			isTriglaphPart = TRUE;
		} else if(phase1->trigraph.partsAccum == 2){
			char replaceChar = '\0';
			switch (c) {
				case '=': replaceChar = '#'; break; //Triglaph: ??= #
				case '(': replaceChar = '['; break; //Triglaph: ??( [
				case '/': replaceChar = '\\'; break; //Triglaph: ??/ \...  
				case ')':  replaceChar = ']'; break; //Triglaph: ??) ]
				case '\'':  replaceChar = '^'; break; //Triglaph: ??' ^
				case '<':  replaceChar = '{'; break; //Triglaph: ??< {
				case '!':  replaceChar = '|'; break; //Triglaph: ??! |
				case '>':  replaceChar = '}'; break; //Triglaph: ??> }
				case '-':  replaceChar = '~'; break; //Triglaph: ??- ~
			}
			if(replaceChar != '\0'){
				isTriglaphPart = TRUE;
				phase1->trigraph.partsAccum = 0;
				//
				NBSintaxParserC_phase2FeedByte(obj, file, replaceChar);
			}
		}
		//Flush triglaph
		if(!isTriglaphPart){
			//Flush parts
			NBSintaxParserC_phase1Flush(obj, file);
			//Feed cur char
			NBSintaxParserC_phase2FeedByte(obj, file, c);
		}
	}
}

void NBSintaxParserC_phase1Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file){
	STNBSintaxParserCPhase1* phase1 = &file->phase1;
	//Flush all '?'
	while(phase1->trigraph.partsAccum > 0){
		NBSintaxParserC_phase2FeedByte(obj, file, '?');
		phase1->trigraph.partsAccum--;
	}
}

//Phase 2

void NBSintaxParserC_phase2FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c){
	STNBSintaxParserCState* parser = &obj->parser;
	if(!parser->completed){
		STNBSintaxParserCPhase2* phase2 = &file->phase2;
		BOOL isIgnorePart = FALSE;
		//Analyze
		NBASSERT(phase2->newline.partsAccum <= 2)
		switch (phase2->newline.partsAccum) {
			case 0:
				if(c == '\\'){
					phase2->newline.partsAccum++;
					isIgnorePart = TRUE;
				}
				break;
			case 1:
				if(c == '\r'){
					phase2->newline.partsAccum++;
					isIgnorePart = TRUE;
				} else if(c == '\n'){
					//Ignore the '\' + '\n' seq
					phase2->newline.partsAccum = 0;
					isIgnorePart = TRUE;
				}
			case 2:
				if(c == '\n'){
					//Ignore the '\' + '\r' + '\n' seq
					phase2->newline.partsAccum = 0;
					isIgnorePart = TRUE;
				}
				break;
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			default:
				NBASSERT(FALSE) //Program logic error
				break;
#			endif
		}
		//Flush
		if(!isIgnorePart){
			//Flush parts
			NBSintaxParserC_phase2Flush(obj, file);
			//Feed to phase 3
			NBSintaxParserC_phase3FeedByte(obj, file, c);
		}
	}
}

void NBSintaxParserC_phase2Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file){
	STNBSintaxParserCPhase2* phase2 = &file->phase2;
	//Flush parts
	{
		NBASSERT(phase2->newline.partsAccum <= 2)
		//Flush '\\'
		if(phase2->newline.partsAccum > 0){
			NBSintaxParserC_phase3FeedByte(obj, file, '\\');
		}
		//Flush '\r'
		if(phase2->newline.partsAccum > 1){
			NBSintaxParserC_phase3FeedByte(obj, file, '\r');
		}
		//
		phase2->newline.partsAccum = 0;
	}
}

//Phase 3

BOOL NBSintaxParserC_phase3FeedByteAsCommentChar(STNBSintaxParserCPhase3* phase3, const char c, char* dstExtraPreChar){
	BOOL r = FALSE;
	if(phase3->comment.isActive){
		//Comment is open
		//Detect end-of-comment
		if(phase3->comment.isMultiline){
			if(c == '*'){
				phase3->comment.partsAccum	= 1;
			} else if(phase3->comment.partsAccum == 1){
				r = (c == '/');
				phase3->comment.isActive	= !r;
				phase3->comment.partsAccum	= 0;
			}
		} else {
			if(c == '\n'){
				phase3->comment.isActive	= FALSE;
				phase3->comment.partsAccum	= 0;
			}
		}
		//Detect if comment just ended
		if(!phase3->comment.isActive){
			//Comment just ended
			//Feed ' ' as comment replacement
			if(dstExtraPreChar != NULL){
				*dstExtraPreChar = ' ';
			}
		}
		r = TRUE;
	} else {
		//Detect start of comment
		const UI8 partsAccumBefore = phase3->comment.partsAccum;
		switch(partsAccumBefore) {
			case 0:
				if(c == '/'){
					phase3->comment.partsAccum = 1;
					r = TRUE;
				}
				break;
			case 1:
				if(c == '/'){
					phase3->comment.isActive	= TRUE;
					phase3->comment.isMultiline	= FALSE;
					r = TRUE;
				} else if(c == '*'){
					phase3->comment.isActive	= TRUE;
					phase3->comment.isMultiline	= TRUE;
					r = TRUE;
				} else if(dstExtraPreChar != NULL){
					*dstExtraPreChar = '/';
				}
				phase3->comment.partsAccum		= 0;
				break;
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			default:
				NBASSERT(FALSE);
				break;
#			endif
		}
	} 
	return r;
}

void NBSintaxParserC_phase3FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c){
	STNBSintaxParserCState* parser		= &obj->parser;
	if(!parser->completed){
		STNBSintaxParserCPhase3* phase3 = &file->phase3;
		const BOOL parserIsEmpty		= (!NBSintaxParserCTokens_isTokenInProgress(&phase3->pptokens.queue));
		BOOL isCommentChar				= FALSE;
		//Aanlyze if is comment
		if(parserIsEmpty){
			char extraPreChar	= '\0';
			isCommentChar		= NBSintaxParserC_phase3FeedByteAsCommentChar(phase3, c, &extraPreChar);
			if(extraPreChar != '\0'){
				//Feed 'extraPreChar' to next stage
				//IF_NBASSERT(PRINTF_INFO("NBSintaxParserC, uncomment-char: '%c' (un-ignoring)\n", extraPreChar);)
				if(!NBSintaxParserCTokens_feedByte(&phase3->pptokens.queue, extraPreChar)){
					parser->errFnd = TRUE;
					PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte('%c') failed at phase3\n", extraPreChar);
				}
			}
		}
		//Feed byte
		if(!isCommentChar){
			if(!NBSintaxParserCTokens_feedByte(&phase3->pptokens.queue, c)){
				parser->errFnd = TRUE;
				PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte('%c') failed at phase3\n", c);
			}
		}
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		else {
			//PRINTF_INFO("NBSintaxParserC, comment-char: '%c' (detected without feeding)\n", c);
		}
#		endif
		//Flush tokens queue
		if(NBSintaxParserCTokens_resultsTokensCount(&file->phase3.pptokens.queue) > 0){
			NBSintaxParserC_phase3FlushTokensQueue(obj, file);
		}
	}
}

void NBSintaxParserC_phase3Flush(STNBSintaxParserC* obj, STNBSintaxParserCFile* file){
	STNBSintaxParserCState* parser	= &obj->parser;
	STNBSintaxParserCPhase3* phase3 = &file->phase3;
	NBASSERT(phase3->comment.partsAccum <= 1)
	if(phase3->comment.partsAccum == 1){
		if(!NBSintaxParserCTokens_feedByte(&phase3->pptokens.queue, '/')){
			parser->errFnd = TRUE;
			PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte('/') failed at phase3 flush\n");
		} else if(!NBSintaxParserCTokens_feedFlush(&phase3->pptokens.queue)){
			parser->errFnd = TRUE;
			PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte('/') failed at phase3 flush\n");
		}
		phase3->comment.partsAccum = 0;
	}
	//Flush tokens queue
	NBSintaxParserC_phase3FlushTokensQueue(obj, file);
}

void NBSintaxParserC_phase3FlushTokensQueue(STNBSintaxParserC* obj, STNBSintaxParserCFile* file){
	STNBSintaxParserCPhase3* phase3 = &file->phase3;
	STNBSintaxParserResultNode* tokens = NULL;
	UI32 tokensSz			= 0;
	const char* strAccum	= NULL;
	UI32 strAccumSz			= 0;
	//Get queue
	NBSintaxParserCTokens_resultsGet(&file->phase3.pptokens.queue, &tokens, &tokensSz, &strAccum, &strAccumSz);
	if(tokens != NULL && strAccum != NULL && strAccumSz > 0){
		//Consume all strAccum
		UI32 i, iByteAfterLast = 0;
		for(i = 0; i < tokensSz; i++){
			const STNBSintaxParserResultNode* tkn = &tokens[i];
			NBASSERT(iByteAfterLast <= tkn->iByteStart)
			NBASSERT(tkn->iByteStart <= tkn->iByteAfter)
			NBASSERT(tkn->iByteStart <= strAccumSz && tkn->iByteAfter <= strAccumSz)
			//Feed unconsumed chars
			if(iByteAfterLast < tkn->iByteStart){
				NBSintaxParserC_phase3ConsumeNonTokenBytes(obj, file, &strAccum[iByteAfterLast], (tkn->iByteStart - iByteAfterLast));
			}
			//Feed token
			{
				const ENSintaxParserC99TC3Rule elemId = NBSintaxParserCDefs_getRuleIdxByElemIdx(tkn->iElem);
				if(elemId == ENSintaxParserC99TC3Rule_punctuator){
					//TMP, Feed as token
					/*if(strAccum[rslt->iByteStart] != ' ' && strAccum[rslt->iByteStart] != '\t' && strAccum[rslt->iByteStart] != '\r' && strAccum[rslt->iByteStart] != '\n'){
						file->phase4.pproc.isOpen = TRUE; //ToDo: remove.
						NBSintaxParserC_phase4FeedToken(obj, part->iElem, &strAccum[rslt->iByteStart], (rslt->iByteAfter - rslt->iByteStart));
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						{
							STNBString str;
							NBString_init(&str);
							NBString_concatBytes(&str, &strAccum[rslt->iByteStart], (rslt->iByteAfter - rslt->iByteStart));
							PRINTF_INFO("NBSintaxParserC, pp-token-%s: '%s'.\n", NBSintaxParserCDefs_getRuleNameByIdx(elemId), str.str);
							NBString_release(&str);
						}
#						endif
					} else*/ {
						//Analyze per char
						NBSintaxParserC_phase3ConsumeNonTokenBytes(obj, file, &strAccum[tkn->iByteStart], (tkn->iByteAfter - tkn->iByteStart));
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						/*{
							STNBString str;
							NBString_init(&str);
							NBString_concatBytes(&str, &strAccum[part->iByteStart], (part->iByteAfter - part->iByteStart));
							PRINTF_INFO("NBSintaxParserC, pp-lose-%s: '%s'.\n", NBSintaxParserCDefs_getRuleNameByIdx(elemId), str.str);
							NBString_release(&str);
						}*/
#						endif
					}
				} else {
					//Feed results as block (is any of: identifier, pp-number, character-constant, or string-literal)
					NBASSERT(elemId == ENSintaxParserC99TC3Rule_identifier || elemId == ENSintaxParserC99TC3Rule_pp_number || elemId == ENSintaxParserC99TC3Rule_character_constant || elemId == ENSintaxParserC99TC3Rule_string_literal)
					if(!phase3->comment.isActive){
						//Feed as char
						//UI32 i; for(i = rslt->iByteStart; i < rslt->iByteAfter; i++){
						//	NBSintaxParserC_phase4FeedByte(obj, strAccum[i]);
						//}
						//Flush comment parts
						NBASSERT(phase3->comment.partsAccum <= 1)
						if(phase3->comment.partsAccum == 1){
							NBSintaxParserC_phase4FeedByte(obj, file, '/');
							phase3->comment.partsAccum = 0;
						}
						//Feed as token
						NBSintaxParserC_phase4FeedToken(obj, file, tkn->iElem, &strAccum[tkn->iByteStart], (tkn->iByteAfter - tkn->iByteStart));
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						/*{
							STNBString str;
							NBString_init(&str);
							NBString_concatBytes(&str, &strAccum[rslt->iByteStart], (rslt->iByteAfter - rslt->iByteStart));
							PRINTF_INFO("NBSintaxParserC, pp-token-%s: '%s'.\n", NBSintaxParserCDefs_getRuleNameByIdx(elemId), str.str);
							NBString_release(&str);
						}*/
#						endif
					}
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					else { 
						/*{
							STNBString str;
							NBString_init(&str);
							NBString_concatBytes(&str, &strAccum[rslt->iByteStart], (rslt->iByteAfter - rslt->iByteStart));
							PRINTF_INFO("NBSintaxParserC, pp-comment-%s: '%s'.\n", NBSintaxParserCDefs_getRuleNameByIdx(elemId), str.str);
							NBString_release(&str);
						}*/
					}
#					endif
				}
			}
			//Next
			iByteAfterLast = tkn->iByteAfter; 
		}
		//Feed unconsumed chars
		if(iByteAfterLast < strAccumSz){
			NBSintaxParserC_phase3ConsumeNonTokenBytes(obj, file, &strAccum[iByteAfterLast], (strAccumSz - iByteAfterLast));
		}
		//Clear queue
		NBSintaxParserCTokens_resultsClear(&file->phase3.pptokens.queue);
	}
}

void NBSintaxParserC_phase3ConsumeNonTokenBytes(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char* str, const UI32 strSz){
	STNBSintaxParserCPhase3* phase3 = &file->phase3;
	const char* strAfter = str + strSz;
	while(str < strAfter){
		char extraPreChar = '\0';
		const BOOL isCommentChar = NBSintaxParserC_phase3FeedByteAsCommentChar(phase3, *str, &extraPreChar);
		if(extraPreChar != '\0'){
			//Feed 'extraPreChar' to next stage
			//Any char is considerated a pptoken
			NBSintaxParserC_phase4FeedByte(obj, file, extraPreChar);
		}
		if(!isCommentChar){
			//Feed '*cByte' to next stage
			//Any char is considerated a pptoken
			NBSintaxParserC_phase4FeedByte(obj, file, *str);
		}
		str++;
	}
}

//

void NBSintaxParserC_phase4FeedByte(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const char c){
	STNBSintaxParserCState* parser	= &obj->parser;
	STNBSintaxParserCPhase4* phase4	= &file->phase4;
	STNBSintaxParser* stxParser		= &phase4->pproc.parser;
	const BOOL isSpaceChar			= NB_SINTAX_PARSER_C_IS_SPACE_CHAR(c);
	//Open pproc-line
	if(!phase4->pproc.curLineStarted && c == '#'){
		phase4->pproc.isOpen = TRUE;
	}
	//Feed
	if(phase4->pproc.isOpen){
		//Feed as PProcLine
		if(isSpaceChar){
			if(!phase4->pproc.lastFeedWasSpace){
				phase4->pproc.lastFeedWasSpace = TRUE;
				if(!NBSintaxParser_feedUnanalyzableByte(stxParser, ' ')){
					parser->errFnd = TRUE;
					PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte(' ') failed at phase4.\n");
				}
			}
		} else {
			phase4->pproc.lastFeedWasSpace	= FALSE;
			phase4->pproc.curLineStarted	= TRUE;
			if(!NBSintaxParser_feedByte(stxParser, c)){
				parser->errFnd = TRUE;
				PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedByte('%c') failed at phase4.\n", c);
			}
		}
	} else {
		//Feed as content
		/*if(isSpaceChar){
			if(!phase4->tokens.lastFeedWasSpace){
				phase4->tokens.lastFeedWasSpace = TRUE;
			}
		} else {
			phase4->tokens.lastFeedWasSpace = FALSE;
			
		}*/
	}
	//Detect new-line
	if(c == '\n'){
		if(phase4->pproc.isOpen){
			if(!NBSintaxParser_feedFlush(stxParser)){
				parser->errFnd = TRUE;
				PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedFlush('%c') failed at phase4.\n", c);
			} else {
				//Analyze available results
				NBSintaxParserC_phase4ConsumeResults(obj, file);
			}
		}
		phase4->pproc.lastFeedWasSpace	= FALSE;
		phase4->pproc.isOpen			= FALSE;
		phase4->pproc.curLineStarted	= FALSE;
		phase4->pproc.linesCount++;
	}
}

void NBSintaxParserC_phase4FeedToken(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const UI32 iElem, const char* token, const UI32 tokenSz){
	STNBSintaxParserCState* parser	= &obj->parser;
	STNBSintaxParserCPhase4* phase4 = &file->phase4;
	STNBSintaxParser* stxParser = &phase4->pproc.parser;
	//ToDo: enable asserts (commented to test token feed)
	//NBASSERT(*token != '#') //Should not be a '#' (to avoid missing detection of pproc lines)
	//NBASSERT(*token != '\r' & *token != '\n') //Should not be new-line (to avoid missing end of pproc lines)
	//NBASSERT(*token != ' ' && *token != '\t') //Should not be a ' ' (to avoid missing start of lines)
	//Feed
	if(phase4->pproc.isOpen){
		//Feed as PProcLine
		phase4->pproc.lastFeedWasSpace	= FALSE;
		phase4->pproc.curLineStarted	= TRUE;
		if(!NBSintaxParser_feedTokenBytes(stxParser, iElem, token, tokenSz)){
			parser->errFnd = TRUE;
			PRINTF_ERROR("NBSintaxParserC, NBSintaxParser_feedTokenBytes('%s') failed at phase4.\n", token);
		}
	} else {
		//Feed as content
	}
}

//PPLines

void NBSintaxParserC_phase4ConsumeResults(STNBSintaxParserC* obj, STNBSintaxParserCFile* file){
	STNBSintaxParserCState* parser	= &obj->parser;
	STNBSintaxParserCPhase4* phase4 = &file->phase4;
	STNBSintaxParser* stxParser		= &phase4->pproc.parser;
	const UI32 resultsHistCount		= NBSintaxParser_resultsHistoryCount(stxParser);
	NBASSERT(resultsHistCount >= 0 && resultsHistCount <= 1)
	if(resultsHistCount == 1){
		const STNBSintaxParserResults* results = NULL;
		const char* strAccum = NULL; UI32 strAccumSz = 0;
		if(!NBSintaxParser_resultsHistoryGet(stxParser, 0, &results, &strAccum, &strAccumSz)){
			parser->errFnd = TRUE;
			PRINTF_ERROR("NBSintaxParserC, could not retrieve the pp-line result.\n");
		} else if(results == NULL){
			parser->errFnd = TRUE;
			PRINTF_ERROR("NBSintaxParserC, could not retrieve the pp-line result.\n");
		} else {
			//Process
			NBSintaxParserC_phase4ConsumeResults_(obj, file, results, strAccum, strAccumSz);
			//Clear results
			NBSintaxParser_resultsHistoryEmpty(stxParser);
		}
	}
}

void NBSintaxParserC_phase4ConsumeResults_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz){
	STNBSintaxParserCState* parser	= &obj->parser;
	NBASSERT(results->iByteAfterMin <= results->iByteAfterMax)
	NBASSERT(results->iByteAfterMin > 0 && results->iByteAfterMax > 0)
	const STNBSintaxParserResultNode* rslt = NULL;
	//Find largest result
	{
		UI32 rsltByteAfter = 0;
		UI32 i; for(i = 0; i < results->resultsSz; i++){
			const STNBSintaxParserResultNode* rslt2 = &results->results[i];
			IF_NBASSERT(const ENSintaxParserC99TC3Rule elemFnd =) NBSintaxParserCDefs_getRuleIdxByElemIdx(rslt2->iElem);
			//NBASSERT(rslt2->iByteStart == 0) //Allowed
			NBASSERT(rslt2->iByteStart <= rslt2->iByteAfter)
			NBASSERT(rslt2->iElem >= 0 && rslt2->iElem < ENSintaxParserC99TC3RulesElem_Count)
			NBASSERT(elemFnd == ENSintaxParserC99TC3Rule_if_group
					 || elemFnd == ENSintaxParserC99TC3Rule_elif_group
					 || elemFnd == ENSintaxParserC99TC3Rule_else_group
					 || elemFnd == ENSintaxParserC99TC3Rule_endif_line
					 || elemFnd == ENSintaxParserC99TC3Rule_control_line)
			NBASSERT(NBString_strIsEqual(NBSintaxParserCDefs_getRuleNameByIdx(elemFnd), rslt2->name))
			NBASSERT(rslt2->parts != NULL && rslt2->partsSz > 0)
			NBASSERT(rslt2->partsSz >= 2)
			if(rslt2->parts != NULL && rslt2->partsSz >= 2){
				IF_NBASSERT(const STNBSintaxParserResultNode* partType = &rslt2->parts[1];)
				IF_NBASSERT(const STNBSintaxParserResultNode* partLast = &rslt2->parts[rslt2->partsSz - 1];)
				NBASSERT(partType->type == ENSintaxParserElemType_Literal)
				NBASSERT(NBSintaxParserCDefs_getRuleIdxByElemIdx(partLast->iElem) == ENSintaxParserC99TC3Rule_new_line)
				//NBASSERT(part->iByteStart == 0)
				NBASSERT(partType->iByteStart <= partType->iByteAfter)
				NBASSERT(partType->iElem >= 0 && partType->iElem < ENSintaxParserC99TC3RulesElem_Count)
				NBASSERT(partLast->iByteStart <= partLast->iByteAfter)
				NBASSERT(partLast->iElem >= 0 && partLast->iElem < ENSintaxParserC99TC3RulesElem_Count)
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				{
					STNBString str, line;
					NBString_init(&str);
					NBString_init(&line);
					NBString_concatBytes(&str, &strAccum[partType->iByteStart], (partType->iByteAfter - partType->iByteStart));
					NBString_concatBytes(&line, &strAccum[rslt2->iByteStart], (partLast->iByteStart - rslt2->iByteStart));
					PRINTF_INFO("NBSintaxParserC, %d-pproc-%s: '%s'.\n", i, str.str, line.str);
					NBString_release(&str);
					NBString_release(&line);
				}
#				endif
				if(rsltByteAfter < rslt2->iByteAfter){
					rsltByteAfter = rslt2->iByteAfter;
					rslt = rslt2;
				}
			}
		}
	}
	//Print results
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	{
		const BOOL includeTree = FALSE;
		STNBString str;
		NBString_init(&str);
		NBSintaxParserResults_concat(results, strAccum, includeTree, &str);
		PRINTF_INFO("NBSintaxParserC, consume pp-line:\n%s.\n", str.str);
		NBString_release(&str);
	}
#	endif
	//Analyze results
	if(rslt != NULL){
		//Process
		if(rslt->partsSz > 1){ //'#' and something else
			const STNBSintaxParserResultNode* partType = &rslt->parts[1];
			const char* strType		= &strAccum[partType->iByteStart];
			const UI32 strTypeSz	= (partType->iByteAfter - partType->iByteStart);
			if(NBString_strIsEqualStrBytes("define", strType, strTypeSz)){
				//Register macro
				if(obj->parser.pprocConds.curPartIsEnabled){
					if(!NBSintaxParserCPProc_macrosAdd(&obj->pproc, results, strAccum, rslt->iByteAfter)){
						parser->errFnd = TRUE;
						PRINTF_ERROR("NBSintaxParserC, could not register macro.\n");
					}
				}
			} else if(NBString_strIsEqualStrBytes("include", strType, strTypeSz)){
				//Include file
				if(obj->parser.pprocConds.curPartIsEnabled){
					if(!NBSintaxParserC_phase4ConsumeResultsInclude_(obj, file, rslt, strAccum, strAccumSz)){
						parser->errFnd = TRUE;
						PRINTF_ERROR("NBSintaxParserC, 'include' directive failed.\n");
					}
				}
			} else if(NBString_strIsEqualStrBytes("ifdef", strType, strTypeSz) || NBString_strIsEqualStrBytes("ifndef", strType, strTypeSz)){
				if(!obj->parser.pprocConds.curPartIsEnabled){
					//Just push without validating
					NBSintaxParserC_pprocCondPush(obj, FALSE);
				} else {
					//Vaidate condition
					if(rslt->partsSz < 3){
						parser->errFnd = TRUE;
						PRINTF_ERROR("NBSintaxParserC, no macro-name at ifdef.\n");
					} else {
						const STNBSintaxParserResultNode* partIdtf = &rslt->parts[2];
						const char* strIdtf		= &strAccum[partIdtf->iByteStart];
						const UI32 strIdtfSz	= (partIdtf->iByteAfter - partIdtf->iByteStart);
						const ENSintaxParserC99TC3RulesElem iElemIdentifier = ENSintaxParserC99TC3RulesElem_2; //"identifier"
						NBASSERT(ENSintaxParserC99TC3Rule_identifier == NBSintaxParserCDefs_getRuleIdxByElemIdx(iElemIdentifier));
						if(partIdtf->iElem != iElemIdentifier){
							parser->errFnd = TRUE;
							PRINTF_ERROR("NBSintaxParserC, expected identifier at ifdef.\n");
						} else {
							const BOOL isDefined = NBSintaxParserCPProc_macrosFind(&obj->pproc, strIdtf, strIdtfSz, NULL);
							NBSintaxParserC_pprocCondPush(obj, NBString_strIsEqualStrBytes("ifndef", strType, strTypeSz) ? !isDefined : isDefined);
						}
					}
				}
			} else if(NBString_strIsEqualStrBytes("if", strType, strTypeSz)){
				BOOL isTrue = FALSE;
				if(obj->parser.pprocConds.curPartIsEnabled){
					if(!NBSintaxParserC_phase4ConsumeResultsEvalIfExpression_(obj, file, rslt, strAccum, strAccumSz, &isTrue)){
						parser->errFnd = TRUE;
						PRINTF_ERROR("NBSintaxParserC, #if constant-expression could not be evaluated.\n");
					}
				}
				NBSintaxParserC_pprocCondPush(obj, isTrue);
			} else if(NBString_strIsEqualStrBytes("elif", strType, strTypeSz)){
				BOOL isTrue = FALSE;
				if(obj->parser.pprocConds.curPartIsEnabled && !obj->parser.pprocConds.curActivePartFnd){
					if(!NBSintaxParserC_phase4ConsumeResultsEvalIfExpression_(obj, file, rslt, strAccum, strAccumSz, &isTrue)){
						parser->errFnd = TRUE;
						PRINTF_ERROR("NBSintaxParserC, #elif constant-expression could not be evaluated.\n");
					}
				}
				if(!NBSintaxParserC_pprocCondPartAdd(obj, ENSintaxParserCPProcCondPartType_Elif, isTrue)){
					parser->errFnd = TRUE;
					PRINTF_ERROR("NBSintaxParserC, #elif not valid.\n");
				}
			} else if(NBString_strIsEqualStrBytes("else", strType, strTypeSz)){
				if(!NBSintaxParserC_pprocCondPartAdd(obj, ENSintaxParserCPProcCondPartType_Else, FALSE)){
					parser->errFnd = TRUE;
					PRINTF_ERROR("NBSintaxParserC, #else not valid.\n");
				}
			} else if(NBString_strIsEqualStrBytes("endif", strType, strTypeSz)){
				if(!NBSintaxParserC_pprocCondPop(obj)){
					parser->errFnd = TRUE;
					PRINTF_ERROR("NBSintaxParserC, #endif not valid.\n");
				}
			}
		}
	}
}

BOOL NBSintaxParserC_phase4ConsumeResultsInclude_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResultNode* rslt, const char* strAccum, const UI32 strAccumSz){
	BOOL r = FALSE;
	//process tokens to produce the header
	STNBSintaxParser subParser;
	STNBSintaxParserCPProcUnit			unit;
	STNBSintaxParserCPProcUnitStack		unitStack;
	STNBSintaxParserCPProcUnitTokens	unitTokens;
	NBSintaxParser_init(&subParser);
	NBSintaxParserCPProcUnit_init(&unit);
	NBSintaxParserCPProcUnit_configFromOther(&unit, &obj->pproc.root.unit);
	NBSintaxParserCMacroStack_init(&unitStack);
	NBMemory_setZeroSt(unitTokens, STNBSintaxParserCPProcUnitTokens);
	//Set destination
	{
		
		NBSintaxParserCTokens_rulesShareWithParser(&file->phase3.pptokens.queue, &subParser);
		//Set callback
		{
			STNBSintaxParserCallbacks callbacks;
			NBMemory_setZeroSt(callbacks, STNBSintaxParserCallbacks);
			NBSintaxParserCTokens_shareGenericParserCallbacks(&file->phase3.pptokens.queue, &callbacks);
			NBSintaxParser_setCallbacks(&subParser, &callbacks);
		}
		//Set root elemns
		{
			const char* rootElems[]		= { "header-name" };
			STNBSintaxParserConfig config;
			NBMemory_setZeroSt(config, STNBSintaxParserConfig);
			config.rootElems	= rootElems;
			config.rootElemsSz	= (sizeof(rootElems) / sizeof(rootElems[0]));
			config.resultsMode	= ENSintaxParserResultsMode_LongestsPerRootChildElem;
			config.resultsKeepHistory = TRUE; //Necesary when reading results without callback
			NBSintaxParser_feedStart(&subParser, &config);
		}
		unit.dst = &subParser;
	}
	//Set tokens
	{
		unitTokens.parent				= rslt;
		unitTokens.range.iStart			= 2; //start after '#' + 'include'
		unitTokens.range.iAfter			= NBSintaxParserResults_nodeChildLeveasCount(rslt); //End at new line
		unitTokens.strAccum				= strAccum;
		unitTokens.strAccumSz			= strAccumSz;
		//Init tokens position
		unit.tokens.def					= &unitTokens;
		unit.tokens.iPosCur				= 0; //It will be automatically inited at "feedTokens"
	}
	//Feed (ignore errors)
	NBSintaxParserCPProcUnit_feedTokens(&unit, &unitStack);
	//Analyze results
	{
		//Focus on the first result (extra tokens are allowed, but ignored)
		const UI32 resutlsCount = NBSintaxParser_resultsHistoryCount(&subParser);
		if(resutlsCount > 0){
			const STNBSintaxParserResults* results = NULL;
			const char* strAccum = NULL; UI32 strAccumSz = 0;
			if(NBSintaxParser_resultsHistoryGet(&subParser, 0, &results, &strAccum, &strAccumSz)){
				if(results != NULL){
					NBASSERT(results->results != NULL)
					NBASSERT(results->resultsSz == 1)
					if(results->results != NULL && results->resultsSz == 1){
						const STNBSintaxParserResultNode* rNode = &results->results[0];
						PRINTF_INFO("NBSintaxParserC, header-name: %s.\n", &strAccum[rNode->iByteStart]);
						if((rNode->iByteStart + 2) < rNode->iByteAfter){ //+2 = '<' + '>' = '\"' + '\"'
							const char cFirst	= strAccum[rNode->iByteStart];
							IF_NBASSERT(const char cLast = strAccum[rNode->iByteAfter - 1];)
							NBASSERT((cFirst == '\"' && cLast == '\"') || (cFirst == '<' && cLast == '>'))
							const STNBArray* paths[2];
							if(cFirst == '\"'){
								paths[0] = &obj->cfg.includePaths.usr;	//First
								paths[1] = &obj->cfg.includePaths.sys;	//Second
							} else {
								NBASSERT(cFirst == '<')
								paths[0] = &obj->cfg.includePaths.sys;	//First
								paths[1] = &obj->cfg.includePaths.usr;	//Second
							}
							//Search file
							{
								STNBString pathTmp;
								NBString_init(&pathTmp);
								{
									const char* strPaths = obj->cfg.includePaths.str.str;
									SI32 iPaths, i; const int count = (sizeof(paths) / sizeof(paths[0]));
									for(iPaths = 0; iPaths < count; iPaths++){
										const STNBArray* pp = paths[iPaths];
										for(i = 0; i < pp->use; i++){
											const char* path = &strPaths[NBArray_itmValueAtIndex(pp, UI32, i)];
											//Concat path
											NBString_set(&pathTmp, path);
											//Concat slash if necesary
											if(pathTmp.length > 0){
												const char cLast = pathTmp.str[pathTmp.length - 1];
												if(cLast != '\\' && cLast != '/'){
													NBString_concatByte(&pathTmp, '/');
												}
											}
											//Concat filename
											NBString_concatBytes(&pathTmp, &strAccum[rNode->iByteStart + 1], rNode->iByteAfter - rNode->iByteStart - 2);
											//Try to pen file
											{
												STNBFileRef stream = NBFile_alloc(NULL);
												if(!NBFile_open(stream, pathTmp.str, ENNBFileMode_Read)){
													PRINTF_INFO("NBSintaxParserC, file doesnt exists: %s.\n", pathTmp.str);
												} else {
													PRINTF_INFO("NBSintaxParserC, file exists: %s.\n", pathTmp.str);
													//Push root file
													STNBSintaxParserCFile* file = NBSintaxParserC_filePush(obj, pathTmp.str, pathTmp.length);
													//Feed
													{
														BYTE buff[4096]; BYTE* start; BYTE* afterEnd; SI32 rd;
														NBFile_lock(stream);
														while(!obj->parser.errFnd){
															rd = NBFile_read(stream, buff, sizeof(buff));
															if(rd <= 0) break;
															start = buff; afterEnd = buff + rd;
															while(start < afterEnd && !obj->parser.errFnd){
																NBSintaxParserC_phase1FeedByte(obj, file, *start);
																start++;
															}
														}
														NBFile_unlock(stream);
													}
													//Pop file from stack (it will flush and validate the state)
													NBASSERT(&obj->parser.files.stack.use > 0)
													NBASSERT(file == NBArray_itmValueAtIndex(&obj->parser.files.stack, STNBSintaxParserCFile*, obj->parser.files.stack.use - 1))
													NBSintaxParserC_filePop(obj);
													//End
													NBFile_close(stream);
													iPaths = count; //end-external-cycle
													i = pp->use;	//end-internal cycle
													r = TRUE;
												}
												NBFile_release(&stream);
											}
										}
									}
								}
								NBString_release(&pathTmp);
							}
						}
						//Ignore other tokens
					}
					//Ignore other results
				}
			}
		}
	}
	NBSintaxParserCPProcUnit_release(&unit);
	NBSintaxParserCMacroStack_release(&unitStack);
	NBSintaxParser_release(&subParser);
	return r;
}

BOOL NBSintaxParserC_phase4ConsumeResultsEvalIfExpression_(STNBSintaxParserC* obj, STNBSintaxParserCFile* file, const STNBSintaxParserResultNode* rslt, const char* strAccum, const UI32 strAccumSz, BOOL* dstResult){
	BOOL r = FALSE;
	//process tokens to produce the header
	STNBSintaxParser subParser;
	STNBSintaxParserCPProcUnit			unit;
	STNBSintaxParserCPProcUnitStack		unitStack;
	STNBSintaxParserCPProcUnitTokens	unitTokens;
	NBSintaxParser_init(&subParser);
	NBSintaxParserCPProcUnit_init(&unit);
	NBSintaxParserCPProcUnit_configFromOther(&unit, &obj->pproc.root.unit);
	NBSintaxParserCPProcUnit_setAllowOperatorDefined(&unit, TRUE); //allow 'defined(MACRO)'
	NBSintaxParserCMacroStack_init(&unitStack);
	NBMemory_setZeroSt(unitTokens, STNBSintaxParserCPProcUnitTokens);
	//Set destination
	{
		
		NBSintaxParserCTokens_rulesShareWithParser(&file->phase3.pptokens.queue, &subParser);
		//Set callback
		{
			STNBSintaxParserCallbacks callbacks;
			NBMemory_setZeroSt(callbacks, STNBSintaxParserCallbacks);
			NBSintaxParserCTokens_shareGenericParserCallbacks(&file->phase3.pptokens.queue, &callbacks);
			NBSintaxParser_setCallbacks(&subParser, &callbacks);
		}
		//Set root elemns
		{
			const char* rootElems[]		= { "constant-expression" };
			STNBSintaxParserConfig config;
			NBMemory_setZeroSt(config, STNBSintaxParserConfig);
			config.rootElems	= rootElems;
			config.rootElemsSz	= (sizeof(rootElems) / sizeof(rootElems[0]));
			config.resultsMode	= ENSintaxParserResultsMode_LongestsPerRootChildElem;
			config.resultsKeepHistory = TRUE; //Necesary when reading results without callback
			NBSintaxParser_feedStart(&subParser, &config);
		}
		unit.dst = &subParser;
	}
	//Set tokens
	{
		unitTokens.parent				= rslt;
		unitTokens.range.iStart			= 2; //start after '#' + 'if' / 'elif'
		unitTokens.range.iAfter			= NBSintaxParserResults_nodeChildLeveasCount(rslt); //End at new line
		unitTokens.strAccum				= strAccum;
		unitTokens.strAccumSz			= strAccumSz;
		//Init tokens position
		unit.tokens.def					= &unitTokens;
		unit.tokens.iPosCur				= 0; //It will be automatically inited at "feedTokens"
	}
	//Feed
	if(NBSintaxParserCPProcUnit_feedTokens(&unit, &unitStack)){
		//Analyze results
		const UI32 resutlsCount = NBSintaxParser_resultsHistoryCount(&subParser);
		if(resutlsCount == 1){
			const STNBSintaxParserResults* results = NULL;
			const char* strAccum = NULL; UI32 strAccumSz = 0;
			if(NBSintaxParser_resultsHistoryGet(&subParser, 0, &results, &strAccum, &strAccumSz)){
				if(results != NULL){
					if(results->results != NULL && results->resultsSz > 0){
						const STNBSintaxParserResultNode* rNode = &results->results[0];
						PRINTF_INFO("NBSintaxParserC, constant-expression: %s.\n", &strAccum[rNode->iByteStart]);
						if(rNode->iByteStart >= rNode->iByteAfter){
							r = FALSE;
						} else {
							//ToDo: eval expression
						}
					}
				}
			}
		}
	}
	NBSintaxParserCPProcUnit_release(&unit);
	NBSintaxParserCMacroStack_release(&unitStack);
	NBSintaxParser_release(&subParser);
	return r;
}
