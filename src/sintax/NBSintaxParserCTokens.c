#include "nb/NBFrameworkPch.h"
#include "nb/sintax/NBSintaxParserCTokens.h"
//
#include "nb/core/NBMemory.h"
//

#define NB_SINTAX_PARSER_C_IS_SPACE_CHAR(C)		(C == ' ' || C == '\t')

//C99_TC3: http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

//Callback elements

UI32 /*ENSintaxParserValidateElemResultBit*/ NBSintaxParserCTokenValidateElem_(void* userParam, const SI32 iElem, const char* name, const char* preContent, const UI32 preContentSz, const UI32 utfCharPos, const char* utfChar, const UI32 utfCharSz);
UI32 NBSintaxParserTokensConsumeResults_(void* userParam, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz);
BOOL NBSintaxParserTokensUnconsumedChar_(void* userParam, const char* utfChar, const UI32 utfCharSz);
void NBSintaxParserTokensErrorFound_(void* userParam, const char* errDesc);

//

void NBSintaxParserCTokens_init(STNBSintaxParserCTokens* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserCTokens);
	NBSintaxParser_init(&obj->parser);
	NBString_init(&obj->strAccum);
	{
		STNBSintaxParserCallbacks callbacks;
		NBMemory_setZeroSt(callbacks, STNBSintaxParserCallbacks);
		callbacks.userParam			= obj;
		callbacks.validateElem		= NBSintaxParserCTokenValidateElem_;
		callbacks.consumeResults	= NBSintaxParserTokensConsumeResults_;
		callbacks.unconsumedChar	= NBSintaxParserTokensUnconsumedChar_;
		callbacks.errorFound		= NBSintaxParserTokensErrorFound_;
		NBSintaxParser_setCallbacks(&obj->parser, &callbacks);
	}
}

void NBSintaxParserCTokens_release(STNBSintaxParserCTokens* obj){
	NBSintaxParser_release(&obj->parser);
	{
		NBSintaxParserResults_nodeRelease(&obj->found);
		obj->foundRsrvd = 0;
	}
	NBString_release(&obj->strAccum);
}

//-----------
//- Callbacks
//-----------
void NBSintaxParserCTokens_shareGenericParserCallbacks(STNBSintaxParserCTokens* obj, STNBSintaxParserCallbacks* dst){
	if(dst != NULL){
		dst->userParam		= obj;
		dst->validateElem	= NBSintaxParserCTokenValidateElem_;
	}
}

//----------
//- Rules
//----------

BOOL NBSintaxParserCTokens_rulesAreLoaded(const STNBSintaxParserCTokens* obj){
	return NBSintaxParser_rulesAreLoaded(&obj->parser);
}

BOOL NBSintaxParserCTokens_rulesLoadDefault(STNBSintaxParserCTokens* obj){
	BOOL r = FALSE;
	NBSintaxParser_rulesFeedStart(&obj->parser);
	if(!NBSintaxParser_rulesFeed(&obj->parser, NBSintaxParserCDefs_getDefaultSintaxRulesStr())){
		PRINTF_ERROR("NBSintaxParserCTokens, rulesFeed failed.\n");
		obj->errFnd = TRUE;
	} else if(!NBSintaxParser_rulesFeedEnd(&obj->parser)){
		PRINTF_ERROR("NBSintaxParserCTokens, rulesFeedEnd failed.\n");
		obj->errFnd = TRUE;
	} else {
		//Print enum
		/*{
			STNBString str;
			NBString_initWithSz(&str, 1024 * 4, 1024 * 16, 0.10f);
			if(!NBSintaxParser_rulesConcatAsRulesInC(&obj->parser, "SintaxParserC99TC3", &str)){
				PRINTF_ERROR("NBSintaxParserCTokens, NBSintaxParser_rulesConcatAsRulesInC failed.\n");
			} else {
				PRINTF_INFO("NBSintaxParserCTokens, NBSintaxParser_rulesConcatAsRulesInC returned:\n%s\n", str.str);
			}
			NBString_release(&str);
		}*/
		//Print rules
		/*{
			STNBString str;
			NBString_initWithSz(&str, 1024 * 4, 1024 * 16, 0.10f);
			if(!NBSintaxParser_concatByRefs(&obj->parser, &str)){
				PRINTF_ERROR("NBSintaxParserCTokens, NBSintaxParser_concatByRefs failed.\n");
			} else {
				PRINTF_INFO("NBSintaxParserCTokens, NBSintaxParser_concatByRefs returned:\n%s\n", str.str);
			}
			NBString_release(&str);
		}*/
		//Validate pregenerated-rules
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		{
			const SI32 countElems = NBSintaxParser_rulesGetElemsCount(&obj->parser);
			NBASSERT(countElems == ENSintaxParserC99TC3RulesElem_Count)
			if(countElems > 0){
				ENSintaxParserElemType type;
				STNBString name;
				NBString_init(&name);
				{
					SI32 i; for(i = 0; i < countElems; i++){
						NBString_empty(&name);
						if(!NBSintaxParser_rulesGetElemHeaderByIdx(&obj->parser, i, &type, &name)){
							NBASSERT(FALSE)
						} else {
							const ENSintaxParserC99TC3Rule iRule = NBSintaxParserCDefs_getRuleIdxByElemIdx(i);
							if(type != ENSintaxParserElemType_Elem){
								NBASSERT(iRule == ENSintaxParserC99TC3Rule_Count)
							} else {
								NBASSERT(iRule < ENSintaxParserC99TC3Rule_Count)
								const char* name2 = NBSintaxParserCDefs_getRuleNameByIdx(iRule);
								NBASSERT(NBString_strIsEqualStrBytes(name2, name.str, name.length));
							}
						}
					}
				}
				NBString_release(&name);
			}
		}
#		endif
		//Result
		r = TRUE;
	}
	return r;
}

BOOL NBSintaxParserCTokens_rulesShareWithOther(STNBSintaxParserCTokens* obj, STNBSintaxParserCTokens* other){
	BOOL r = FALSE;
	if(other != NULL){
		r = NBSintaxParser_rulesFeedAsRefOf(&other->parser, &obj->parser);
	}
	return r;
}

BOOL NBSintaxParserCTokens_rulesShareWithParser(STNBSintaxParserCTokens* obj, STNBSintaxParser* other){
	BOOL r = FALSE;
	if(other != NULL){
		r = NBSintaxParser_rulesFeedAsRefOf(other, &obj->parser);
	}
	return r;
}

//Feed

BOOL NBSintaxParserCTokens_isTokenInProgress(const STNBSintaxParserCTokens* obj){
	return (!NBSintaxParser_isEmpty(&obj->parser));
}

BOOL NBSintaxParserCTokens_feedStart(STNBSintaxParserCTokens* obj, const char** rootElems, const UI32 rootElemsSz){
	STNBSintaxParserConfig config;
	NBMemory_setZeroSt(config, STNBSintaxParserConfig);
	config.rootElems			= rootElems;
	config.rootElemsSz			= rootElemsSz;
	config.resultsMode			= ENSintaxParserResultsMode_LongestsPerRootChildElem;
	return NBSintaxParser_feedStart(&obj->parser, &config);
}

BOOL NBSintaxParserCTokens_feedByte(STNBSintaxParserCTokens* obj, const char c){
	return NBSintaxParser_feedByte(&obj->parser, c);
}

BOOL NBSintaxParserCTokens_feedFlush(STNBSintaxParserCTokens* obj){
	return NBSintaxParser_feedFlush(&obj->parser);
}

//Results

UI32 NBSintaxParserCTokens_resultsTokensCount(const STNBSintaxParserCTokens* obj){
	return obj->found.partsSz;
}

void NBSintaxParserCTokens_resultsGet(const STNBSintaxParserCTokens* obj, STNBSintaxParserResultNode** const dstTokens, UI32* dstTokenSz, const char** dstStrAccum, UI32* dstStrAccumSz){
	if(dstTokens != NULL) *dstTokens = obj->found.parts;
	if(dstTokenSz != NULL) *dstTokenSz = obj->found.partsSz;
	if(dstStrAccum != NULL) *dstStrAccum = obj->strAccum.str;
	if(dstStrAccumSz != NULL) *dstStrAccumSz = obj->strAccum.length;
}

void NBSintaxParserCTokens_resultsClear(STNBSintaxParserCTokens* obj){
	//Release current
	NBSintaxParserResults_nodeRelease(&obj->found);
	//Empty
	NBMemory_setZeroSt(obj->found, STNBSintaxParserResultNode);
	obj->foundRsrvd = 0;
	NBString_empty(&obj->strAccum);
}

//Callbacks

UI32 /*ENSintaxParserValidateElemResultBit*/ NBSintaxParserCTokenValidateElem_(void* userParam, const SI32 iElem, const char* name, const char* preContent, const UI32 preContentSz, const UI32 utfCharPos, const char* utfChar, const UI32 utfCharSz){
	UI32 r = ENSintaxParserValidateElemResultBit_None;
	IF_NBASSERT(STNBSintaxParserCTokens* obj = (STNBSintaxParserCTokens*)userParam;) NBASSERT(obj != NULL)
	if(utfChar != NULL && utfCharSz > 0){
		const char c = *utfChar;
		NBASSERT(iElem >= 0 && iElem < ENSintaxParserC99TC3RulesElem_Count)
		const ENSintaxParserC99TC3Rule elemId = NBSintaxParserCDefs_getRuleIdxByElemIdx(iElem);
		NBASSERT(NBString_strIsEqual(NBSintaxParserCDefs_getRuleNameByIdx(elemId), name))
		switch (elemId) {
			case ENSintaxParserC99TC3Rule_digit_sequence:
				if(c >= '0' && c <= '9'){
					r = ENSintaxParserValidateElemResultBit_Partial | ENSintaxParserValidateElemResultBit_Complete;
				}
				break;
			case ENSintaxParserC99TC3Rule_hexadecimal_digit_sequence:
				if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')){
					r = ENSintaxParserValidateElemResultBit_Partial | ENSintaxParserValidateElemResultBit_Complete;
				}
				break;
			case ENSintaxParserC99TC3Rule_c_char: //c-char
				/*
				 c-char:
				 (callback)
				 //any member of the source character set except
				 //the single-quote ', backslash \, or new-line character
				 escape-sequence
				 */
				if(utfCharPos == 0){
					r = ENSintaxParserValidateElemResultBit_Complete;
					if(c == '\'' || c == '\\' || c == '\r' || c == '\n'){
						r = ENSintaxParserValidateElemResultBit_None;
					}
				}
				break;
			case ENSintaxParserC99TC3Rule_s_char: //s-char
				/*
				 s-char:
				 (callback)
				 //any member of the source character set except
				 //the double-quote ", backslash \, or new-line character
				 escape-sequence
				 */
				if(utfCharPos == 0){
					r = ENSintaxParserValidateElemResultBit_Complete;
					if(c == '\"' || c == '\\' || c == '\r' || c == '\n'){
						r = ENSintaxParserValidateElemResultBit_None;
					}
				}
				break;
			case ENSintaxParserC99TC3Rule_h_char: //h-char
				/*
				 h-char:
				 (callback)
				 //any member of the source character set except
				 //the new-line character and >
				 */
				if(utfCharPos == 0){
					r = ENSintaxParserValidateElemResultBit_Complete;
					if(c == '\r' || c == '\n' || c == '>'){
						r = ENSintaxParserValidateElemResultBit_None;
					}
				}
				break;
			case ENSintaxParserC99TC3Rule_q_char: //q-char
				/*
				 q-char:
				 (callback)
				 //any member of the source character set except
				 //the new-line character and "
				 */
				if(utfCharPos == 0){
					r = ENSintaxParserValidateElemResultBit_Complete;
					if(c == '\r' || c == '\n' || c == '\"'){
						r = ENSintaxParserValidateElemResultBit_None;
					}
				}
				break;
			case ENSintaxParserC99TC3Rule_lparen: //lparen
				/*
				 lparen:
				 (callback)
				 //a ( character not immediately preceded by white-space
				 */
				if(utfCharPos == 0){
					if(c == '('){
						r = ENSintaxParserValidateElemResultBit_Complete;
						if(preContentSz > 0){
							const char prevChar = preContent[preContentSz - 1]; 
							if(prevChar == ' ' || prevChar == '\t' || prevChar == '\r' || prevChar == '\n'){
								r = ENSintaxParserValidateElemResultBit_None;
							}
						}
					}
				}
				break;
			case ENSintaxParserC99TC3Rule_new_line: //new-line:
				/*
				 new-line:
				 (callback)
				 //the new-line character
				 */
				if(c == '\r'){
					if(utfCharPos == 0){
						r = ENSintaxParserValidateElemResultBit_Partial;
					}
				} else if(c == '\n'){
					NBASSERT(utfCharPos == 0 || utfCharPos == 1)
					r = ENSintaxParserValidateElemResultBit_Complete;
				}
				break;
			default:
				NBASSERT(FALSE)
				break;
		}
	}
	return r;
}

//PPToken

UI32 NBSintaxParserTokensConsumeResults_(void* userParam, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz){
	UI32 r = 0;
	STNBSintaxParserCTokens* obj = (STNBSintaxParserCTokens*)userParam; NBASSERT(obj != NULL)
	if(results != NULL){
		//Print results
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		/*{
			const BOOL includeTree = FALSE;
			STNBString str;
			NBString_init(&str);
			NBSintaxParserResults_concat(results, strAccum, includeTree, &str);
			PRINTF_INFO("NBSintaxParserC, consume token:\n%s.\n", str.str);
			NBString_release(&str);
		}*/
		/*if(results->resultsSz > 0){
			//Flatten results
			const STNBSintaxParserResultNode* rslt = &results->results[0];
			STNBSintaxParserResultNode clone;
			NBMemory_setZeroSt(clone, STNBSintaxParserResultNode);
			if(!NBSintaxParserResults_nodeCloneFlattenRecursivity(rslt, &clone, FALSE)){
				PRINTF_ERROR("NBSintaxParserC, clone results failed.\n");
			} else {
				STNBSintaxParserResults rr2;
				NBMemory_setZeroSt(rr2, STNBSintaxParserResults);
				rr2.iByteAfterMin	= clone.iByteAfter;
				rr2.iByteAfterMax	= clone.iByteAfter;
				rr2.results			= &clone;
				rr2.resultsSz		= 1;
				{
					const BOOL includeTree = FALSE;
					STNBString str;
					NBString_init(&str);
					NBSintaxParserResults_concat(&rr2, strAccum, includeTree, &str);
					PRINTF_INFO("NBSintaxParserC, flatten token:\n%s.\n", str.str);
					NBString_release(&str);
				}
			}
			NBSintaxParserResults_nodeRelease(&clone);
		}*/
#		endif
		NBASSERT(results->iByteAfterMin == results->iByteAfterMax)
		NBASSERT(results->iByteAfterMin > 0 && results->iByteAfterMax > 0)
		NBASSERT(results->resultsSz == 1)
		//Analyze results
		if(results->resultsSz == 1){ //Must be one token
			const STNBSintaxParserResultNode* rslt = &results->results[0];
			NBASSERT(rslt->iByteStart == 0)
			NBASSERT(rslt->iByteStart <= rslt->iByteAfter)
			NBASSERT(rslt->iElem >= 0 && rslt->iElem < ENSintaxParserC99TC3RulesElem_Count)
			NBASSERT(NBSintaxParserCDefs_getRuleIdxByElemIdx(rslt->iElem) == ENSintaxParserC99TC3Rule_preprocessing_token)
			NBASSERT(NBString_strIsEqual(NBSintaxParserCDefs_getRuleNameByIdx(NBSintaxParserCDefs_getRuleIdxByElemIdx(rslt->iElem)), rslt->name))
			NBASSERT(rslt->parts != NULL && rslt->partsSz == 1)
			if(rslt->parts != NULL && rslt->partsSz == 1){
				const STNBSintaxParserResultNode* part = &rslt->parts[0];
				NBASSERT(part->iByteStart == 0) //Tokens must start at zero-index
				NBASSERT(part->iByteStart <= part->iByteAfter)
				NBASSERT(part->iElem >= 0 && part->iElem < ENSintaxParserC99TC3RulesElem_Count)
				NBASSERT(NBString_strIsEqual(NBSintaxParserCDefs_getRuleNameByIdx(NBSintaxParserCDefs_getRuleIdxByElemIdx(part->iElem)), part->name))
				IF_NBASSERT(const ENSintaxParserC99TC3Rule elemId =) NBSintaxParserCDefs_getRuleIdxByElemIdx(part->iElem);
				NBASSERT(elemId >= 0 && elemId < ENSintaxParserC99TC3Rule_Count)
				NBASSERT(elemId == ENSintaxParserC99TC3Rule_identifier || elemId == ENSintaxParserC99TC3Rule_pp_number || elemId == ENSintaxParserC99TC3Rule_character_constant || elemId == ENSintaxParserC99TC3Rule_string_literal || elemId == ENSintaxParserC99TC3Rule_punctuator)
				//
				const char* val = &strAccum[part->iByteStart];
				const UI32 valSz = (part->iByteAfter - part->iByteStart);
				STNBSintaxParserResultNode tkn;
				NBMemory_setZeroSt(tkn, STNBSintaxParserResultNode);
				tkn.type		= part->type;
				tkn.iElem		= part->iElem;
				tkn.iByteStart	= obj->strAccum.length;
				tkn.iByteAfter	= tkn.iByteStart + valSz;
				//Increase buffer
				if(obj->foundRsrvd >= obj->found.partsSz){
					const UI32 newArrSz = (obj->foundRsrvd + 8);
					STNBSintaxParserResultNode* newArr = NBMemory_allocTypes(STNBSintaxParserResultNode, newArrSz);
					if(obj->found.parts != NULL){
						if(obj->found.partsSz > 0){
							NBMemory_copy(newArr, obj->found.parts, sizeof(obj->found.parts[0]) * obj->found.partsSz);
						}
						NBMemory_free(obj->found.parts);
						obj->found.parts = NULL;
					}
					obj->found.parts	= newArr;
					obj->foundRsrvd		= newArrSz;
				}
				//Add to found
				NBASSERT(obj->found.partsSz < obj->foundRsrvd)
				obj->found.parts[obj->found.partsSz++] = tkn;
				//Add content
				NBString_concatBytes(&obj->strAccum, val, valSz);
				//Add as result
				r = rslt->iByteAfter;
			}
		}
	}
	return r;
}

BOOL NBSintaxParserTokensUnconsumedChar_(void* userParam, const char* utfChar, const UI32 utfCharSz){
	BOOL r = FALSE;
	STNBSintaxParserCTokens* obj = (STNBSintaxParserCTokens*)userParam; NBASSERT(obj != NULL)
	if(utfCharSz > 0){
		NBString_concatBytes(&obj->strAccum, utfChar, utfCharSz);
		r = TRUE;
	}
	//Print if not consumed
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	/*if(!r){
		STNBString str;
		NBString_init(&str);
		NBString_concatBytes(&str, utfChar, utfCharSz);
		PRINTF_INFO("NBSintaxParserC, unconsumed in token: '%s'.\n", str.str);
		NBString_release(&str);
	}*/
#	endif
	return r;
}

void NBSintaxParserTokensErrorFound_(void* userParam, const char* errDesc){
	//STNBSintaxParserCTokens* obj		= (STNBSintaxParserCTokens*)userParam; NBASSERT(obj != NULL)
	//STNBSintaxParserCPhase3* phase3	= &obj->parser.phase3;
	PRINTF_INFO("NBSintaxParserC, error token: '%s'.\n", errDesc);
}
