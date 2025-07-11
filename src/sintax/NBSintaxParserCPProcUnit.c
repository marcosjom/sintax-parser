#include "nb/NBFrameworkPch.h"
#include "nb/sintax/NBSintaxParserCPProcUnit.h"
//
#include "nb/core/NBMemory.h"
//

BOOL NBCompare_STNBSintaxParserCMacroPtr(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz);

//Translating unit

BOOL NBSintaxParserCPProcUnit_feedNodeChildren_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const STNBSintaxParserResultNode* parent, UI32* iLeafRef, const STNBSintaxParserCMacroReplaceHint** nextHintRef);
BOOL NBSintaxParserCPProcUnit_feedNodeChild_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint** nextHintRef);
BOOL NBSintaxParserCPProcUnit_feedNode_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint* nextHint, const STNBSintaxParserCMacroReplaceHint* myHint);

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParserCPProcUnit_dbgConcatNodeChildrenRng_(const STNBSintaxParserCPProcUnitTokens* tokens, const STNBSintaxParserResultNode* parent, const UI32 iChildStart, UI32* iLeafRef, const STNBSintaxParserCMacroReplaceHint** nextHintRef, BOOL* stopFlagRef, UI32* iAfterCharCopiedRef, const STNBSintaxParserCPProcUnitTknsRngLive* rng, STNBString* dst);
#endif
//

void NBSintaxParserCPProcUnit_init(STNBSintaxParserCPProcUnit* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserCPProcUnit);
	obj->feedState = ENNBSintaxParserCPProcUnitFeedState_StartPending;
	//Parsing macro
	NBArray_init(&obj->macroOpen.params, sizeof(STNBSintaxParserCPProcUnitTknsRngLive), NULL);
}

void NBSintaxParserCPProcUnit_release(STNBSintaxParserCPProcUnit* obj){
	//Parsing macro
	NBArray_release(&obj->macroOpen.params);
}

//Config

void NBSintaxParserCPProcUnit_configFromOther(STNBSintaxParserCPProcUnit* obj, const STNBSintaxParserCPProcUnit* other){
	if(other != NULL){
		obj->dst			= other->dst;
		obj->callbacks		= other->callbacks;
		obj->allowOpDefined	= other->allowOpDefined;
	}
}

void NBSintaxParserCPProcUnit_setAllowOperatorDefined(STNBSintaxParserCPProcUnit* obj, const BOOL allow){
	obj->allowOpDefined = allow;
}

void NBSintaxParserCPProcUnit_setCallbacks(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitCallbacks* callbacks){
	if(callbacks == NULL){
		NBMemory_setZeroSt(obj->callbacks, STNBSintaxParserCPProcUnitCallbacks);
	} else {
		obj->callbacks = *callbacks;
	}
}

//Feed

BOOL NBSintaxParserCPProcUnit_feedTokens(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack){
	BOOL r = FALSE;
	const STNBSintaxParserCPProcUnitTokens* tokens = obj->tokens.def;
	if(obj != NULL && callstack != NULL && tokens != NULL){
		if(tokens->range.iStart >= tokens->range.iAfter){
			//Nothing to run
			r = TRUE;
		} else if(tokens->parent != NULL){
			//Run
			UI32 iLeaf = 0;
			const STNBSintaxParserCMacroReplaceHint* nextHint = tokens->replaceHints;
			NBASSERT(obj->feedState == ENNBSintaxParserCPProcUnitFeedState_StartPending) //Must be pending to start
			obj->feedState = ENNBSintaxParserCPProcUnitFeedState_StartPending;
			if(tokens->parent->parts != NULL && tokens->parent->partsSz > 0){
				r = NBSintaxParserCPProcUnit_feedNodeChildren_(obj, callstack, tokens->parent, &iLeaf, &nextHint);
			}
			//Parent node is ignored
		}
	}
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedNodeChildren_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const STNBSintaxParserResultNode* parent, UI32* iLeafRef, const STNBSintaxParserCMacroReplaceHint** nextHintRef){
	BOOL r = TRUE;
	NBASSERT(obj != NULL)
	NBASSERT(obj->tokens.def != NULL)
	NBASSERT(parent != NULL)
	NBASSERT(iLeafRef != NULL)
	NBASSERT(nextHintRef != NULL)
	NBASSERT(parent->parts != NULL && parent->partsSz > 0) //Avoid unecesary calls
	//Process children
	if(parent->parts != NULL && parent->partsSz > 0){
		const STNBSintaxParserCPProcUnitTokens* tokens = obj->tokens.def;
		UI32 i; for(i = 0; i < parent->partsSz && obj->feedState != ENNBSintaxParserCPProcUnitFeedState_EndFound; i++){
			const STNBSintaxParserResultNode* child = &parent->parts[i];
			const STNBSintaxParserCMacroReplaceHint* childNextHint = NULL;
			const UI32 childLeafIdx = *iLeafRef;
			//Save current hint
			if(nextHintRef != NULL){
				childNextHint = *nextHintRef;
			}
			//Eval
			if(child->parts != NULL && child->partsSz > 0){
				//Eval as parent
				NBSintaxParserCPProcUnit_feedNodeChildren_(obj, callstack, child, iLeafRef, nextHintRef);
			} else if(NBSintaxParserCPProcUnit_feedNodeChild_(obj, callstack, i, child, *iLeafRef, nextHintRef)){
				(*iLeafRef)++;
			} else {
				r = FALSE;
				break;
			}
			//Apply state to param current reading
			if(obj->macroOpen.paramCur != NULL){
				//Apply to start
				if(obj->macroOpen.paramCur->start.iLeaf > childLeafIdx){
					obj->macroOpen.paramCur->start.iLeaf	= childLeafIdx;
					obj->macroOpen.paramCur->start.iChild	= i;
					obj->macroOpen.paramCur->start.node		= parent;
					obj->macroOpen.paramCur->start.hint		= childNextHint;
				}
			}
			//Move to next hint (if necesary)
			if(nextHintRef != NULL){
				if(*nextHintRef != NULL){
					if((*nextHintRef)->iLeafReplace == *iLeafRef){
						(*nextHintRef)++;
						if(*nextHintRef >= tokens->replaceHintsAfter){
							*nextHintRef = NULL;
						}
					}
				}
			}
		}
	}
	//
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedNodeChild_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint** nextHintRef){
	BOOL r = TRUE;
	const STNBSintaxParserCPProcUnitTokens* tokens = obj->tokens.def;
	//Detect start
	if(obj->feedState == ENNBSintaxParserCPProcUnitFeedState_StartPending){
		if(tokens->range.iStart == iLeaf){
			obj->feedState = ENNBSintaxParserCPProcUnitFeedState_StartFound;
			//Set start pos
			obj->tokens.iPosCur = child->iByteStart; 
		}
	}
	//Detect end
	if(obj->feedState == ENNBSintaxParserCPProcUnitFeedState_StartFound){
		if(tokens->range.iAfter == iLeaf){
			obj->feedState = ENNBSintaxParserCPProcUnitFeedState_EndFound;
		}
	}
	//Eval as leaf node
	if(obj->feedState == ENNBSintaxParserCPProcUnitFeedState_StartFound){
		//Determine if current hint is for this node
		const STNBSintaxParserCMacroReplaceHint* nextHint = NULL;
		const STNBSintaxParserCMacroReplaceHint* myHint = NULL;
		if(nextHintRef != NULL){
			//Next hint
			nextHint = *nextHintRef;
			//My hint
			if(*nextHintRef != NULL){
				if((*nextHintRef)->iLeafReplace == iLeaf){
					myHint = *nextHintRef; 
				}
			}
		}
		//Feed leaf node
		if(!NBSintaxParserCPProcUnit_feedNode_(obj, callstack, iChild, child, iLeaf, nextHint, myHint)){
			r = FALSE;
		}
	}
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedNodeOpenedDefined_(STNBSintaxParserCPProcUnit* obj, const ENSintaxParserC99TC3Rule rule, const char* tknStr, const UI32 tknStrSz);
BOOL NBSintaxParserCPProcUnit_feedNodeOpenedMacroFunction_(STNBSintaxParserCPProcUnit* obj, const ENSintaxParserC99TC3Rule rule, const char* tknStr, const UI32 tknStrSz, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint* nextHint);
BOOL NBSintaxParserCPProcUnit_feedMacroInvocation_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack);

BOOL NBSintaxParserCPProcUnit_feedNode_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint* nextHint, const STNBSintaxParserCMacroReplaceHint* myHint){
	BOOL r = TRUE, consumed = FALSE;
	const STNBSintaxParserCPProcUnitTokens* tokens = obj->tokens.def;
	const char* tknStr		= &tokens->strAccum[child->iByteStart];
	const UI32 tknStrSz		= (child->iByteAfter - child->iByteStart);
	const ENSintaxParserC99TC3Rule rule = NBSintaxParserCDefs_getRuleIdxByElemIdx(child->iElem);
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	{
		STNBString str;
		NBString_init(&str);
		NBString_concatBytes(&str, tknStr, tknStrSz);
		PRINTF_INFO("NBSintaxParserCPProcUnit, feeding token '%s' (iLeaf%d, iChild%d).\n", str.str, iLeaf, iChild);
		NBString_release(&str);
	}
#	endif
	//Analyze state
	switch (obj->macroOpen.type) {
		case ENNBSintaxParserCPProcUnitMacroType_None:
			//Analyze start of macro or 'define' operator
			NBASSERT(obj->macroOpen.def == NULL)
			NBASSERT(obj->macroOpen.paramCur == NULL)
			NBASSERT(obj->macroOpen.params.use == 0)
			NBASSERT(!obj->macroOpen.paramMustStart)
			NBASSERT(!obj->macroOpen.paramMustClose)
			NBASSERT(obj->macroOpen.parentesisDepth == 0)
			NBASSERT(obj->callbacks.macroFind != NULL)
			//Feed prevStr
			NBASSERT(obj->tokens.iPosCur <= child->iByteStart)
			if(obj->tokens.iPosCur < child->iByteStart){
				const char* prevStr		= &tokens->strAccum[obj->tokens.iPosCur];
				const UI32 prevStrSz	= (child->iByteStart - obj->tokens.iPosCur);
				if(obj->dst != NULL){
					NBSintaxParser_feedUnanalyzableBytes(obj->dst, prevStr, prevStrSz);
				}
				obj->tokens.iPosCur		= child->iByteStart;
			}
			//Analyze
			if(rule == ENSintaxParserC99TC3Rule_identifier){
				//Is 'define' operator?
				if(!consumed && obj->allowOpDefined){
					if(NBString_strIsEqualStrBytes("defined", tknStr, tknStrSz)){
						obj->macroOpen.type = ENNBSintaxParserCPProcUnitMacroType_Defined;
						consumed = TRUE;
					}
				}
				//Is macro identifier?
				if(!consumed && obj->callbacks.macroFind != NULL){
					STNBSintaxParserCMacro* macroDef = NULL;
					if((*obj->callbacks.macroFind)(obj->callbacks.param, tknStr, tknStrSz, &macroDef)){
						NBASSERT(macroDef != NULL)
						BOOL isRedundant = FALSE;
						//Analyze if macro is at current stack
						{
							SI32 i; for(i = (SI32)callstack->defs.use - 1; i >= 0; i--){
								if(macroDef == NBArray_itmValueAtIndex(&callstack->defs, STNBSintaxParserCMacro*, i)){
									isRedundant = TRUE;
	#								ifdef NB_CONFIG_INCLUDE_ASSERTS
									{
										STNBString str;
										NBString_init(&str);
										NBString_concatBytes(&str, tknStr, tknStrSz);
										PRINTF_INFO("NBSintaxParserCPProcUnit,  macro '%s' ignored due to redundancy.\n", str.str);
										NBString_release(&str);
									}
	#								endif
									break;
								}
							}
						}
						//Start macro invocation
						if(!isRedundant){
							obj->macroOpen.type	= ENNBSintaxParserCPProcUnitMacroType_Open;
							obj->macroOpen.def	= macroDef;
							if(!macroDef->def.refs.isFunction){
								//Object-like macros does not require params, flush inmediatly.
								if(!NBSintaxParserCPProcUnit_feedMacroInvocation_(obj, callstack)){
									PRINTF_ERROR("NBSintaxParserCPProcUnit,  'feedCompletedMacro' failed.\n");
									r = FALSE;
								}
							}
							consumed = TRUE;
						}
					}
				}
			}
			break;
		case ENNBSintaxParserCPProcUnitMacroType_Defined:
			//Analyze continuation of 'define' operator
			if(!NBSintaxParserCPProcUnit_feedNodeOpenedDefined_(obj, rule, tknStr, tknStrSz)){
				r = FALSE;
			} else {
				consumed = TRUE;
			}
			break;
		case ENNBSintaxParserCPProcUnitMacroType_Open:
			//Analyze continuation of macro call
			NBASSERT(obj->macroOpen.def != NULL)
			NBASSERT(obj->macroOpen.def->def.refs.isFunction)
			if(obj->macroOpen.def->def.refs.isFunction){
				if(NBSintaxParserCPProcUnit_feedNodeOpenedMacroFunction_(obj, rule, tknStr, tknStrSz, iChild, child, iLeaf, nextHint)){
					//Execute if macro invocation was completed
					if(obj->macroOpen.parentesisDepth == 0 && obj->macroOpen.paramMustClose){
						if(!NBSintaxParserCPProcUnit_feedMacroInvocation_(obj, callstack)){
							PRINTF_ERROR("NBSintaxParserCPProcUnit,  'feedCompletedMacro' failed.\n");
							r = FALSE;
						}
					}
					consumed = TRUE;
				} else {
					if(obj->macroOpen.paramMustClose){
						PRINTF_ERROR("NBSintaxParserCPProcUnit, macro-function-like invocation is not well defined.\n");
						r = FALSE;
					} else {
						//Macro-identifier was not followed by '(',
						//send identifier as pure-code
						//and re-feed the current token.
						//ToDo
						NBASSERT(FALSE)
					}
				}
			}
			break;
		default:
			NBASSERT(FALSE)
			break;
	}
	//Add to destination
	if(!consumed){
		//Feed token
		if(obj->dst != NULL){
			NBSintaxParser_feedBytes(obj->dst, tknStr, tknStrSz);
		}
		consumed = TRUE;
	}
	//Advance current position
	if(consumed){
		obj->tokens.iPosCur = child->iByteAfter;
	}
	/*if(myHint != NULL){
		if(myHint->type == ENNBSintaxParserCMacroReplaceHintType_Ignore || myHint->isConcatStart || myHint->isConcatContinue || myHint->isParamToLiteral){
			
		}
	}*/
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedNodeOpenedDefined_(STNBSintaxParserCPProcUnit* obj, const ENSintaxParserC99TC3Rule rule, const char* tknStr, const UI32 tknStrSz){
	BOOL r = TRUE;
	//Analyze continuation of 'define' operator
	NBASSERT(obj->macroOpen.type == ENNBSintaxParserCPProcUnitMacroType_Defined)
	NBASSERT(obj->macroOpen.def == NULL)
	NBASSERT(obj->macroOpen.paramCur == NULL)
	NBASSERT(obj->macroOpen.params.use == 0)
	NBASSERT(obj->macroOpen.parentesisDepth <= 1)
	if(rule == ENSintaxParserC99TC3Rule_identifier){
		if(obj->macroOpen.paramMustClose){
			PRINTF_ERROR("NBSintaxParserCPProcUnit, 'define' operator with mutiple identifiers.\n");
			r = FALSE;
		} else {
			BOOL isDefined = FALSE;
			if(obj->callbacks.macroFind != NULL){
				isDefined = (*obj->callbacks.macroFind)(obj->callbacks.param, tknStr, tknStrSz, NULL);
			}
			//Feed token
			if(obj->dst != NULL){
				NBSintaxParser_feedByte(obj->dst, isDefined ? '1' : '0');
			}
			//Close operator
			if(obj->macroOpen.parentesisDepth == 0){
				NBASSERT(!obj->macroOpen.paramMustStart)
				NBASSERT(!obj->macroOpen.paramMustClose)
				obj->macroOpen.type = ENNBSintaxParserCPProcUnitMacroType_None;
			} else {
				obj->macroOpen.paramMustClose = TRUE;
			}
		}
	} else if(tknStrSz == 1){
		switch(*tknStr) {
			case '(':
				if(obj->macroOpen.parentesisDepth == 0){
					obj->macroOpen.parentesisDepth++;
				} else {
					PRINTF_ERROR("NBSintaxParserCPProcUnit, 'define' expected identifier after '('.\n");
					r = FALSE;
				}
				break;
			case ')':
				if(obj->macroOpen.paramMustClose){
					obj->macroOpen.parentesisDepth--; NBASSERT(obj->macroOpen.parentesisDepth == 0)
					obj->macroOpen.type = ENNBSintaxParserCPProcUnitMacroType_None;
					obj->macroOpen.paramMustClose = FALSE;
				} else {
					PRINTF_ERROR("NBSintaxParserCPProcUnit, 'define' expected identifier before ')'.\n");
					r = FALSE;
				}
				break;
			default:
				PRINTF_ERROR("NBSintaxParserCPProcUnit, 'define' expected identifier, '(' or ')' tokens.\n");
				r = FALSE;
				break;
		}
	} else {
		PRINTF_ERROR("NBSintaxParserCPProcUnit, 'define' expected identifier, '(' or ')' tokens.\n");
		r = FALSE;
	}
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedNodeOpenedMacroFunction_(STNBSintaxParserCPProcUnit* obj, const ENSintaxParserC99TC3Rule rule, const char* tknStr, const UI32 tknStrSz, const UI32 iChild, const STNBSintaxParserResultNode* child, const UI32 iLeaf, const STNBSintaxParserCMacroReplaceHint* nextHint){
	BOOL r = TRUE;
	NBASSERT(obj->macroOpen.type == ENNBSintaxParserCPProcUnitMacroType_Open)
	NBASSERT(obj->macroOpen.def != NULL)
	NBASSERT(obj->macroOpen.def->def.refs.isFunction)
	//Analyze continuation of macro call
	if(obj->macroOpen.parentesisDepth == 0){
		NBASSERT(obj->macroOpen.paramCur == NULL)
		NBASSERT(obj->macroOpen.params.use == 0)
		NBASSERT(!obj->macroOpen.paramMustStart)
		NBASSERT(!obj->macroOpen.paramMustClose)
		//Opening of parameters
		r = FALSE;
		if(tknStrSz == 1){
			if(*tknStr == '('){
				obj->macroOpen.parentesisDepth++;
				obj->macroOpen.paramMustStart = TRUE;
				obj->macroOpen.paramMustClose = TRUE;
				r = TRUE;
			}
		}
	} else {
		//Analyze
		NBASSERT(obj->macroOpen.parentesisDepth > 0)
		NBASSERT(obj->macroOpen.paramMustClose)
		BOOL openNewParam = obj->macroOpen.paramMustStart;
		obj->macroOpen.paramMustStart = FALSE;
		if(tknStrSz == 1){
			switch(*tknStr) {
				case '(':
					obj->macroOpen.parentesisDepth++;
					break;
				case ')':
					obj->macroOpen.parentesisDepth--;
					if(obj->macroOpen.parentesisDepth == 0 && obj->macroOpen.params.use == 0){
						//Avoid creating param whe empty
						openNewParam = FALSE;
					}
					break;
				case ',':
					if(obj->macroOpen.parentesisDepth == 1){
						obj->macroOpen.paramMustStart = TRUE;
					}
					break;
			}
		}
		//Accumulate token
		if(obj->macroOpen.paramCur != NULL){
			NBASSERT(obj->macroOpen.paramCur == NBArray_itmPtrAtIndex(&obj->macroOpen.params, STNBSintaxParserCPProcUnitTknsRngLive, obj->macroOpen.params.use - 1))
			obj->macroOpen.paramCur->after.node		= child;
			obj->macroOpen.paramCur->after.iLeaf	= iLeaf;
			obj->macroOpen.paramCur->after.iChild	= iChild;
			if(nextHint != NULL){
				obj->macroOpen.paramCur->after.hint = nextHint;
			}
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			{
				STNBString str;
				NBString_initWithStrBytes(&str, tknStr, tknStrSz);
				PRINTF_INFO("NBSintaxParserCPProcUnit, param%d adding token '%s' (iLeaf%d, iChild%d).\n", obj->macroOpen.params.use - 1, str.str, iLeaf, iChild);
				NBString_release(&str);
			}
#			endif
			//Close prev param
			if(obj->macroOpen.paramMustStart){
				obj->macroOpen.paramCur = NULL;
				IF_NBASSERT(PRINTF_INFO("NBSintaxParserCPProcUnit, param%d closed.\n", obj->macroOpen.params.use - 1);)
			}
		}
		//Open new param
		if(openNewParam){
			STNBSintaxParserCPProcUnitTknsRngLive param;
			NBMemory_setZeroSt(param, STNBSintaxParserCPProcUnitTknsRngLive);
			//
			param.start.node	= child;
			param.start.iLeaf	= iLeaf;
			param.start.iChild	= iChild;
			param.start.hint	= nextHint;
			//
			param.after.node	= child;
			param.after.iLeaf	= iLeaf;
			param.after.iChild	= iChild;
			param.after.hint	= nextHint;
			//
			obj->macroOpen.paramCur = (STNBSintaxParserCPProcUnitTknsRngLive*)NBArray_addValue(&obj->macroOpen.params, param);
#			ifdef NB_CONFIG_INCLUDE_ASSERTS
			{
				STNBString str;
				NBString_initWithStrBytes(&str, tknStr, tknStrSz);
				PRINTF_INFO("NBSintaxParserCPProcUnit, param%d opened with token '%s' (iLeaf%d, iChild%d).\n", obj->macroOpen.params.use - 1, str.str, iLeaf, iChild);
				NBString_release(&str);
			}
#			endif
			//Close as empty param
			if(obj->macroOpen.paramMustStart){
				obj->macroOpen.paramCur = NULL;
				IF_NBASSERT(PRINTF_INFO("NBSintaxParserCPProcUnit, param%d closed as empty.\n", obj->macroOpen.params.use - 1);)
			}
		}
		r = TRUE;
	}
	return r;
}

BOOL NBSintaxParserCPProcUnit_feedMacroInvocation_(STNBSintaxParserCPProcUnit* obj, STNBSintaxParserCPProcUnitStack* callstack){
	BOOL r = TRUE;
	const STNBSintaxParserCPProcUnitTokens* tokens = obj->tokens.def;
	//Print
	NBASSERT(obj->macroOpen.def != NULL)
	NBASSERT(callstack != NULL)
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	{
		STNBString str;
		NBString_init(&str);
		NBString_concat(&str, "macro: '"); NBString_concatBytes(&str, obj->macroOpen.def->def.refs.name, obj->macroOpen.def->def.refs.nameLen); NBString_concat(&str, "'");
		if(obj->macroOpen.def->def.refs.isFunction || obj->macroOpen.params.use > 0){
			SI32 i;
			NBString_concat(&str, ", ");
			NBString_concatSI32(&str, obj->macroOpen.params.use);
			NBString_concat(&str, obj->macroOpen.params.use > 0 ? " params:" : "params");
			for(i = 0; i < obj->macroOpen.params.use; i++){
				const STNBSintaxParserCPProcUnitTknsRngLive* p = NBArray_itmPtrAtIndex(&obj->macroOpen.params, STNBSintaxParserCPProcUnitTknsRngLive, i);
				NBString_concat(&str, "\nparam");
				NBString_concatSI32(&str, i);
				NBString_concat(&str, ": ");
				NBASSERT(p->start.node != NULL)
				NBASSERT(p->start.iChild < p->start.node->partsSz)
				if(p->start.iChild < p->start.node->partsSz){
					const STNBSintaxParserResultNode* child = &p->start.node->parts[p->start.iChild];
					const STNBSintaxParserCMacroReplaceHint* nextHintRef = NULL;
					BOOL stopFlag = FALSE; UI32 iLeaf = p->start.iLeaf, iAfterCharCopied = child->iByteStart;
					NBSintaxParserCPProcUnit_dbgConcatNodeChildrenRng_(tokens, p->start.node, p->start.iChild, &iLeaf, &nextHintRef, &stopFlag, &iAfterCharCopied, p, &str);
				}
			}
		}
		PRINTF_INFO("NBSintaxParserCPProcUnit, invoking %s\n", str.str);
		NBString_release(&str);
	}
#	endif
	//New unit
	{
		const SI32 stackSzBefore = callstack->defs.use;
		STNBSintaxParserCPProcUnitTokens subTokens;
		STNBSintaxParserCPProcUnit subUnit;
		NBSintaxParserCPProcUnit_init(&subUnit);
		//Set config
		{
			NBSintaxParserCPProcUnit_configFromOther(&subUnit, obj);
		}
		//Set tokens
		{
			NBMemory_setZeroSt(subTokens, STNBSintaxParserCPProcUnitTokens);
			subTokens.parent				= obj->macroOpen.def->def.refs.replacement;
			subTokens.range.iStart			= 0;
			subTokens.range.iAfter			= obj->macroOpen.def->def.refs.replacementLeavesCount;
			if(obj->macroOpen.def->def.refs.replacementHints.use > 0){
				subTokens.replaceHints		= NBArray_dataPtr(&obj->macroOpen.def->def.refs.replacementHints, STNBSintaxParserCMacroReplaceHint);
				subTokens.replaceHintsAfter	= subTokens.replaceHints + obj->macroOpen.def->def.refs.replacementHints.use;  
			}
			subTokens.strAccum				= obj->macroOpen.def->def.str.str;
			subTokens.strAccumSz			= obj->macroOpen.def->def.str.length;
			//Init tokens position
			subUnit.tokens.def				= &subTokens;
			subUnit.tokens.iPosCur			= 0; //It will be automatically inited at "feedTokens"
		}
		//Set params
		if(obj->macroOpen.params.use > 0){
			obj->params.tokens	= tokens;
			obj->params.arr		= NBArray_dataPtr(&obj->macroOpen.params, STNBSintaxParserCPProcUnitTknsRngLive);
			obj->params.arrSz	= obj->macroOpen.params.use;
		}
		//Push at stack
		{
			NBArray_addValue(&callstack->defs, obj->macroOpen.def);
		}
		//Call
		if(!NBSintaxParserCPProcUnit_feedTokens(&subUnit, callstack)){
			PRINTF_ERROR("NBSintaxParserCPProcUnit, macro invocation failed.\n");
			r = FALSE;
		} else {
			//Pop from stack
			NBASSERT(callstack->defs.use == (stackSzBefore + 1)) //The stack must end at the same state after invocation
			if(callstack->defs.use != (stackSzBefore + 1)){
				PRINTF_ERROR("NBSintaxParserCPProcUnit, callstack differs in size after macro invocation.\n");
				r = FALSE;
			} else {
				const STNBSintaxParserCMacro* topDef = NBArray_itmValueAtIndex(&callstack->defs, STNBSintaxParserCMacro*, callstack->defs.use - 1); 
				NBASSERT(topDef == obj->macroOpen.def) //The stack must have this invocation at the top
				if(topDef != obj->macroOpen.def){
					PRINTF_ERROR("NBSintaxParserCPProcUnit, callstack differs in top-element after macro invocation.\n");
					r = FALSE;
				} else {
					//Pop element
					NBArray_removeItemAtIndex(&callstack->defs, callstack->defs.use - 1);
				}
			}
		}
		NBSintaxParserCPProcUnit_release(&subUnit);
	}
	return r;
}

#ifdef NB_CONFIG_INCLUDE_ASSERTS
void NBSintaxParserCPProcUnit_dbgConcatNodeChildrenRng_(const STNBSintaxParserCPProcUnitTokens* tokens, const STNBSintaxParserResultNode* parent, const UI32 iChildStart, UI32* iLeafRef, const STNBSintaxParserCMacroReplaceHint** nextHintRef, BOOL* stopFlagRef, UI32* iAfterCharCopiedRef, const STNBSintaxParserCPProcUnitTknsRngLive* rng, STNBString* dst){
	NBASSERT(tokens != NULL)
	NBASSERT(parent != NULL)
	NBASSERT(iLeafRef != NULL)
	NBASSERT(nextHintRef != NULL)
	NBASSERT(stopFlagRef != NULL)
	NBASSERT(iAfterCharCopiedRef != NULL)
	NBASSERT(rng != NULL)
	NBASSERT(dst != NULL)
	//Process children
	if(parent->parts != NULL && parent->partsSz > 0){
		UI32 i; for(i = iChildStart; i < parent->partsSz && !(*stopFlagRef); i++){
			const STNBSintaxParserResultNode* child = &parent->parts[i];
			//Eval
			if(child->parts != NULL && child->partsSz > 0){
				//Eval as parent
				NBSintaxParserCPProcUnit_dbgConcatNodeChildrenRng_(tokens, child, 0, iLeafRef, nextHintRef, stopFlagRef, iAfterCharCopiedRef, rng, dst);
			} else {
				(*iLeafRef)++;
				//Detect end
				if(*iLeafRef == rng->after.iLeaf){
					//End detected
					PRINTF_INFO("NBSintaxParserCPProcUnit, stopping rng at iLeaf-%d, child #%d of %d.\n", *iLeafRef, (i + 1), parent->partsSz);
					*stopFlagRef = TRUE;
				} else {
					//Process
					const STNBSintaxParserCMacroReplaceHint* myHint = NULL;
					if(nextHintRef != NULL){
						//My hint
						if(*nextHintRef != NULL){
							if((*nextHintRef)->iLeafReplace == *iLeafRef){
								myHint = *nextHintRef; 
							}
						}
					}
					//Concat pending bytes
					if(*iAfterCharCopiedRef < child->iByteStart){
						const char* str		= &tokens->strAccum[*iAfterCharCopiedRef];
						const UI32 strSz	= (child->iByteStart - *iAfterCharCopiedRef);
						NBString_concatBytes(dst, str, strSz);
					}
					//Concat token
					if(child->iByteStart < child->iByteAfter){
						const char* str		= &tokens->strAccum[child->iByteStart];
						const UI32 strSz	= (child->iByteAfter - child->iByteStart);
						NBString_concatBytes(dst, str, strSz);
					}
					if(myHint != NULL){
						NBString_concat(dst, "[hint:");
						{
							NBString_concat(dst, "type");
							NBString_concatSI32(dst, myHint->type);
							NBString_concat(dst, ":");
						}
						if(myHint->type == ENNBSintaxParserCMacroReplaceHintType_ParamRef){
							NBString_concat(dst, "param");
							NBString_concatSI32(dst, myHint->iParam);
							NBString_concat(dst, ":");
						}
						if(myHint->isConcatStart){
							NBString_concat(dst, "isConcatStart:");
						}
						if(myHint->isConcatContinue){
							NBString_concat(dst, "isConcatContinue:");
						}
						if(myHint->isParamToLiteral){
							NBString_concat(dst, "isParamToLiteral:");
						}
						NBString_concat(dst, "]");
					}
					//Keep track of last copied
					*iAfterCharCopiedRef = child->iByteAfter;
				}
			}
			//Move to next hint (if necesary)
			if(nextHintRef != NULL){
				if(*nextHintRef != NULL){
					if((*nextHintRef)->iLeafReplace == *iLeafRef){
						if(*nextHintRef == rng->after.hint){
							*nextHintRef = NULL;
						} else {
							(*nextHintRef)++;
						}
					}
				}
			}
		}
	}
}
#endif

//Macro run stack

void NBSintaxParserCMacroStack_init(STNBSintaxParserCPProcUnitStack* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserCPProcUnitStack);
	NBArray_init(&obj->defs, sizeof(STNBSintaxParserCMacro*), NULL);
}

void NBSintaxParserCMacroStack_release(STNBSintaxParserCPProcUnitStack* obj){
	NBArray_release(&obj->defs);
}

