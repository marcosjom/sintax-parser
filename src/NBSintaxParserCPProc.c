#include "nb/NBFrameworkPch.h"
#include "nb/core/NBSintaxParserCPProc.h"
//
#include "nb/core/NBMemory.h"
//

BOOL NBCompare_STNBSintaxParserCMacroPtr(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz);
BOOL NBSintaxParserCPProc_macrosFind_(void* userParam, const char* identifier, const UI32 identifierLen, STNBSintaxParserCMacro** dst);

const char* NBSintaxParserCPProcPredifMacroNames_[] = {
	"__DATE__",
	"__FILE__",
	"__LINE__",
	"__STDC__",
	"__STDC_HOSTED__",
	"__STDC_MB_MIGHT_NEQ_WC__",
	"__STDC_VERSION__",
	"__TIME__",
	//Conditionally defined
	"__STDC_IEC_559__"
	"__STDC_IEC_559_COMPLEX__"
	"__STDC_ISO_10646__"
	//Forbiden
	"__cplusplus"
	"defined"
};

//PProc

void NBSintaxParserCPProc_init(STNBSintaxParserCPProc* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserCPProc);
	//macros library
	{
		NBArraySorted_init(&obj->macros, sizeof(STNBSintaxParserCMacro*), NBCompare_STNBSintaxParserCMacroPtr);
	}
	//root translation unit
	{
		NBSintaxParserCPProcUnit_init(&obj->root.unit);
		{
			STNBSintaxParserCPProcUnitCallbacks callbacks;
			NBMemory_setZeroSt(callbacks, STNBSintaxParserCPProcUnitCallbacks);
			callbacks.param		= obj;
			callbacks.macroFind	= NBSintaxParserCPProc_macrosFind_;
			NBSintaxParserCPProcUnit_setCallbacks(&obj->root.unit, &callbacks);
		}
		NBSintaxParserCMacroStack_init(&obj->root.stack);
		//Tokens accumulation
		{
			//STNBSintaxParserResultNode	accum.tokens;
			NBString_initWithSz(&obj->root.accum.strs, 0, 1024, 0.10f);
		}
	}
}

void NBSintaxParserCPProc_release(STNBSintaxParserCPProc* obj){
	//root translation unit
	{
		NBSintaxParserCPProcUnit_release(&obj->root.unit);
		NBSintaxParserCMacroStack_release(&obj->root.stack);
		//Tokens accumulation
		{
			NBSintaxParserResults_nodeRelease(&obj->root.accum.tokens);
			NBString_release(&obj->root.accum.strs);
		}
	}
	//macros library
	{
		NBSintaxParserCPProc_macrosEmpty(obj);
		NBASSERT(obj->macros.use == 0)
		NBArraySorted_release(&obj->macros);
	}
}

//

typedef struct STNBSintaxParserCPProc_macrosBuildHintsNodeParams_ {
	BOOL								isFunction;
	const STNBSintaxParserResultNode*	params;
	UI32								paramsCount;
	BOOL								isVaArgs;
	const char*							strAccum;
	UI32								strAccumSz;
} STNBSintaxParserCPProc_macrosBuildHintsNodeParams;

typedef struct STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken_ {
	UI32		iLeaf;
	UI32		iByteStart;
	UI32		bytesLen;
	BOOL		isPopulated;	//First token found
	BOOL		isHashToken;	//is a '#' token
	BOOL		isDblHashToken;	//is a '##' token
	BOOL		isParamRef;		//is a parameter
} STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken;

BOOL NBSintaxParserCPProc_macrosBuildHintsNode_(const STNBSintaxParserCPProc_macrosBuildHintsNodeParams* bldDef, const STNBSintaxParserResultNode* replacementParent, UI32* iLeafRef, STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken* prevTokenRef, STNBArray* dst /*STNBSintaxParserCMacroReplaceHint*/);
BOOL NBSintaxParserCPProc_macrosBuildHintsNodeEval_(const STNBSintaxParserCPProc_macrosBuildHintsNodeParams* bldDef, const STNBSintaxParserResultNode* replacementParent, const UI32 iLeaf, STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken* prevTokenRef, STNBArray* dst /*STNBSintaxParserCMacroReplaceHint*/);

BOOL NBSintaxParserCPProc_macrosAdd(STNBSintaxParserCPProc* obj, const STNBSintaxParserResults* results, const char* strAccum, const UI32 strAccumSz){
	BOOL r = FALSE;
	NBASSERT(obj != NULL)
	NBASSERT(results != NULL)
	NBASSERT(strAccum != NULL && strAccumSz > 0)
	if(obj != NULL && results != NULL && strAccum != NULL){
		//[#] [define] identifier replacement-list new-line
		//[#] [define] identifier lparen identifier-list(opt) [)] replacement-list new-line
		//[#] [define] identifier lparen [...] [)] replacement-list new-line
		//[#] [define] identifier lparen identifier-list [,] [...] [)] replacement-list new-line
		const STNBSintaxParserResultNode* node = NULL;
		//Find best definition
		NBASSERT(results->results != NULL && results->resultsSz > 0)
		if(results->results != NULL && results->resultsSz > 0){
			UI32 i; for(i = 0; i < results->resultsSz; i++){
				const STNBSintaxParserResultNode* nn = &results->results[i];
				NBASSERT(nn->partsSz > 3) //At least: '#' 'define' identifier new-line
				if(nn->partsSz > 3){
					if(node == NULL){
						//Set first
						node = nn;
					} else {
						//Preference for function-like definitions ('lparen')
						const STNBSintaxParserResultNode* nAfterId = &nn->parts[3];
						if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nAfterId->iElem) == ENSintaxParserC99TC3Rule_lparen){
							node = nn;
						}
					}
				}
			}
		}
		//Process
		if(node != NULL){
			const STNBSintaxParserResultNode* nHash		= &node->parts[0]; 
			const STNBSintaxParserResultNode* nDefne	= &node->parts[1];
			const STNBSintaxParserResultNode* nIdentf	= &node->parts[2];
			NBASSERT(NBString_strIsEqualStrBytes("#", &strAccum[nHash->iByteStart], (nHash->iByteAfter - nHash->iByteStart)))
			NBASSERT(NBString_strIsEqualStrBytes("define", &strAccum[nDefne->iByteStart], (nDefne->iByteAfter - nDefne->iByteStart)))
			NBASSERT(NBSintaxParserCDefs_getRuleIdxByElemIdx(nIdentf->iElem) == ENSintaxParserC99TC3Rule_identifier)
			if(NBString_strIsEqualStrBytes("#", &strAccum[nHash->iByteStart], (nHash->iByteAfter - nHash->iByteStart))){
				if(NBString_strIsEqualStrBytes("define", &strAccum[nDefne->iByteStart], (nDefne->iByteAfter - nDefne->iByteStart))){
					if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nIdentf->iElem) == ENSintaxParserC99TC3Rule_identifier){
						SI32 iFnd = - 1;
						STNBSintaxParserCMacro* srchPtr = NULL;
						STNBSintaxParserCMacro srch;
						NBMemory_setZeroSt(srch, STNBSintaxParserCMacro);
						srch.def.refs.name		= &strAccum[nIdentf->iByteStart];
						srch.def.refs.nameLen	= (nIdentf->iByteAfter - nIdentf->iByteStart);
						srchPtr					= &srch;
						iFnd					= NBArraySorted_indexOf(&obj->macros, &srchPtr, sizeof(srchPtr), NULL);
						if(iFnd >= 0){
							STNBSintaxParserCMacro* mm = NBArraySorted_itmValueAtIndex(&obj->macros, STNBSintaxParserCMacro*, iFnd);
							const STNBSintaxParserResultNode* nIdentf2 = &mm->def.node.parts[2];
							const char* oldDef		= &mm->def.str.str[nIdentf2->iByteAfter];
							const UI32 oldDefSz		= (mm->def.node.iByteAfter - nIdentf2->iByteAfter);
							const char* newDef		= &strAccum[nIdentf->iByteAfter];
							const UI32 newDefSz		= (node->iByteAfter - nIdentf->iByteAfter);
							NBASSERT(NBString_strIsEqualBytes(srch.def.refs.name, srch.def.refs.nameLen, mm->def.refs.name, mm->def.refs.nameLen))
							if(NBString_strIsEqualBytes(oldDef, oldDefSz, newDef, newDefSz)){
								r = TRUE;
								PRINTF_INFO("NBSintaxParserCPProc, macro re-defined (same).\n");
							} else {
								PRINTF_ERROR("NBSintaxParserCPProc, macro re-defined (different).\n");
							}
						} else {
							UI32 iPart = 3; const STNBSintaxParserResultNode* nNext = &node->parts[iPart]; 
							BOOL isFunction = FALSE, isVaArgs = FALSE; SI32 iParams = -1, iReplacement = -1;
							//result
							r = TRUE;
							//Detect fucntion params
							if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nNext->iElem) == ENSintaxParserC99TC3Rule_lparen){
								isFunction = TRUE;
								iPart++; nNext = NULL;
								//Detect parameters
								if(iPart < node->partsSz){
									nNext = &node->parts[iPart];
									if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nNext->iElem) == ENSintaxParserC99TC3Rule_identifier_list){
										iParams = iPart++; nNext = NULL;
										//Ignore ','
										if(iPart < node->partsSz){
											nNext = &node->parts[iPart];
											if(NBString_strIsEqualStrBytes(",", &strAccum[nNext->iByteStart], (nNext->iByteAfter - nNext->iByteStart))){
												iPart++; nNext = NULL;
											}
										}
									}
								}
								//Detect ellipsis '...'
								if(iPart < node->partsSz){
									nNext = &node->parts[iPart];
									if(NBString_strIsEqualStrBytes("...", &strAccum[nNext->iByteStart], (nNext->iByteAfter - nNext->iByteStart))){
										isVaArgs = TRUE; iPart++; nNext = NULL;
									}
								}
								//Detect ')'
								if(iPart >= node->partsSz){
									PRINTF_ERROR("NBSintaxParserCPProc, expected ')' at macro definition.\n");
									r = FALSE;
								} else {
									nNext = &node->parts[iPart];
									if(!NBString_strIsEqualStrBytes(")", &strAccum[nNext->iByteStart], (nNext->iByteAfter - nNext->iByteStart))){
										PRINTF_ERROR("NBSintaxParserCPProc, expected ')' at macro definition.\n");
										r = FALSE;
									} else {
										iPart++; nNext = NULL;
									}
								}
							}
							//Detect replacement list
							if(iPart < node->partsSz){
								nNext = &node->parts[iPart];
								if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nNext->iElem) == ENSintaxParserC99TC3Rule_replacement_list){
									//Ignore if is empty
									if(nNext->parts != NULL && nNext->partsSz > 0){
										iReplacement = iPart; 
									}
									iPart++; nNext = NULL;
								}
							}
							//Detect 'new-line'
							if(iPart >= node->partsSz){
								PRINTF_ERROR("NBSintaxParserCPProc, expected 'new-line' at macro definition.\n");
								r = FALSE;
							} else {
								nNext = &node->parts[iPart];
								if(NBSintaxParserCDefs_getRuleIdxByElemIdx(nNext->iElem) != ENSintaxParserC99TC3Rule_new_line){
									PRINTF_ERROR("NBSintaxParserCPProc, expected 'new-line' at macro definition.\n");
									r = FALSE;
								} else {
									iPart++; nNext = NULL;
									if(iPart < node->partsSz){
										PRINTF_ERROR("NBSintaxParserCPProc, additional tokens after macro definition.\n");
										r = FALSE;
									} else {
										UI32 paramsCount = 0, replacementLeavesCount = 0;
										STNBArray replacementHints;
										NBArray_init(&replacementHints, sizeof(STNBSintaxParserCMacroReplaceHint), NULL);
										//Validate and build replacement hints
										if(iReplacement >= 0){
											STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken prevTokenRef2;
											STNBSintaxParserCPProc_macrosBuildHintsNodeParams params2;
											NBMemory_setZeroSt(prevTokenRef2, STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken);
											NBMemory_setZeroSt(params2, STNBSintaxParserCPProc_macrosBuildHintsNodeParams);
											params2.isFunction			= isFunction;
											if(iParams > 0){
												const ENSintaxParserC99TC3RulesElem iElemIdentifier = ENSintaxParserC99TC3RulesElem_2; //"identifier"
												NBASSERT(ENSintaxParserC99TC3Rule_identifier == NBSintaxParserCDefs_getRuleIdxByElemIdx(iElemIdentifier));
												params2.params			= &node->parts[iParams];
												params2.paramsCount		= paramsCount = NBSintaxParserResults_nodeChildCountByElemIdx(params2.params, iElemIdentifier);
											}
											params2.isVaArgs			= isVaArgs;
											params2.strAccum			= strAccum;
											params2.strAccumSz			= strAccumSz;
											//Validate macro
											if(!NBSintaxParserCPProc_macrosBuildHintsNode_(&params2, &node->parts[iReplacement], &replacementLeavesCount, &prevTokenRef2, &replacementHints)){
												PRINTF_ERROR("NBSintaxParserCPProc, could not build hints for function-like macro.\n");
												r = FALSE;
											} else if(prevTokenRef2.isDblHashToken){
												PRINTF_ERROR("NBSintaxParserCPProc, '##' cannot be the last token in the replacement-list.\n");
												r = FALSE;
											} else if(isFunction && prevTokenRef2.isHashToken){
												PRINTF_ERROR("NBSintaxParserCPProc, '#' must be followed by a parameter (found as last token).\n");
												r = FALSE;
											}
										}
										//Validate not forbiden
										if(r){
											const char* name = &strAccum[nIdentf->iByteStart];
											const UI32 nameSz= (nIdentf->iByteAfter - nIdentf->iByteStart);
											const SI32 count = sizeof(NBSintaxParserCPProcPredifMacroNames_) / sizeof(NBSintaxParserCPProcPredifMacroNames_[0]);
											SI32 i; for(i = 0; i < count; i++){
												if(NBString_strIsEqualStrBytes(NBSintaxParserCPProcPredifMacroNames_[i], name, nameSz)){
													PRINTF_ERROR("NBSintaxParserCPProc, '%s' is not allowed to be defined or undefined.\n", NBSintaxParserCPProcPredifMacroNames_[i]);
													r = FALSE;
													break;
												}
											}
										}
										//Process
										if(!r){
											NBArray_release(&replacementHints);
										} else {
											STNBSintaxParserCMacro* mm = NBMemory_allocType(STNBSintaxParserCMacro);
											NBMemory_setZeroSt(*mm, STNBSintaxParserCMacro);
											NBString_initWithStrBytes(&mm->def.str, strAccum, strAccumSz);
											NBSintaxParserResults_nodeClone(node, &mm->def.node, TRUE);
											mm->def.refs.name			= &mm->def.str.str[nIdentf->iByteStart];
											mm->def.refs.nameLen		= (nIdentf->iByteAfter - nIdentf->iByteStart);
											mm->def.refs.isFunction		= isFunction;
											mm->def.refs.isVaArgs		= isVaArgs;
											if(iParams >= 0){
												mm->def.refs.params		= &mm->def.node.parts[iParams];
												mm->def.refs.paramsCount = paramsCount;
											}
											if(iReplacement >= 0){
												mm->def.refs.replacement = &mm->def.node.parts[iReplacement];
												NBASSERT(replacementLeavesCount == NBSintaxParserResults_nodeChildLeveasCount(mm->def.refs.replacement))
											}
											mm->def.refs.replacementLeavesCount = replacementLeavesCount;
											mm->def.refs.replacementHints = replacementHints;
											//Add
											NBArraySorted_add(&obj->macros, &mm, sizeof(mm));
											//Print
#											ifdef NB_CONFIG_INCLUDE_ASSERTS
											{
												STNBString str;
												NBString_initWithStrBytes(&str, mm->def.refs.name, mm->def.refs.nameLen);
												PRINTF_INFO("NBSintaxParserCPProc, macro added '%s%s'%s%s%s.\n", str.str, (mm->def.refs.isFunction ? "()" : ""), (mm->def.refs.isVaArgs ? ", isVaArgs" : ""), (mm->def.refs.params != NULL ? ", with params": ""), (mm->def.refs.replacement != NULL ? ", with replacement-list" : ""));
												NBString_release(&str);
											}
#											endif
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return r;
}

BOOL NBSintaxParserCPProc_macrosBuildHintsNode_(const STNBSintaxParserCPProc_macrosBuildHintsNodeParams* bldDef, const STNBSintaxParserResultNode* replacementParent, UI32* iLeafRef, STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken* prevTokenRef, STNBArray* dst /*STNBSintaxParserCMacroReplaceHint*/){
	BOOL r = TRUE;
	NBASSERT(bldDef != NULL)
	NBASSERT(dst != NULL)
	NBASSERT(iLeafRef != NULL)
	NBASSERT(prevTokenRef != NULL)
	NBASSERT(replacementParent != NULL)
	//Process children
	if(replacementParent->parts != NULL && replacementParent->partsSz > 0){
		UI32 i; for(i = 0; i < replacementParent->partsSz; i++){
			const STNBSintaxParserResultNode* child = &replacementParent->parts[i];
			if(child->parts != NULL && child->partsSz > 0){
				//Eval as parent
				if(!NBSintaxParserCPProc_macrosBuildHintsNode_(bldDef, child, iLeafRef, prevTokenRef, dst)){
					r = FALSE;
					break;
				}
			} else if(!NBSintaxParserCPProc_macrosBuildHintsNodeEval_(bldDef, child, (*iLeafRef)++, prevTokenRef, dst)){
				r = FALSE;
				break;
			}
		}
	} else if(!NBSintaxParserCPProc_macrosBuildHintsNodeEval_(bldDef, replacementParent, (*iLeafRef)++, prevTokenRef, dst)){
		//Eval as node
		r = FALSE;
	}
	return r;
}

BOOL NBSintaxParserCPProc_macrosBuildHintsNodeEval_(const STNBSintaxParserCPProc_macrosBuildHintsNodeParams* bldDef, const STNBSintaxParserResultNode* replacementParent, const UI32 iLeaf, STNBSintaxParserCPProc_macrosBuildHintsNodeLastToken* prevTokenRef, STNBArray* dst /*STNBSintaxParserCMacroReplaceHint*/){
	BOOL r = TRUE;
	NBASSERT(bldDef != NULL)
	NBASSERT(dst != NULL)
	NBASSERT(prevTokenRef != NULL)
	NBASSERT(replacementParent != NULL)
	NBASSERT(replacementParent->parts == NULL && replacementParent->partsSz == 0)
	//Eval
	BOOL isCommaToken	= FALSE;
	BOOL isHashToken	= FALSE;
	BOOL isDblHashToken	= FALSE;
	BOOL isDblHashReplc	= FALSE;
	BOOL isParamRef		= FALSE;
	const char* val		= &bldDef->strAccum[replacementParent->iByteStart];
	const UI32 valSz	= (replacementParent->iByteAfter - replacementParent->iByteStart);
	switch(valSz){
		case 1:
			isCommaToken	= (*val == ',');
			isHashToken		= (*val == '#');
			break;
		case 2:
			isDblHashToken	= (*val == '#' && val[1] == '#');
			break;
	}
	//Add as 'ignore' hint for '#' or '##' tokens
	if((bldDef->isFunction && isHashToken) || isDblHashToken){
		STNBSintaxParserCMacroReplaceHint hint;
		NBMemory_setZeroSt(hint, STNBSintaxParserCMacroReplaceHint);
		hint.type				= ENNBSintaxParserCMacroReplaceHintType_Ignore;
		hint.iLeafReplace		= iLeaf;
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		{
			STNBString tmp;
			NBString_init(&tmp);
			NBString_concatBytes(&tmp, val, valSz);
			PRINTF_INFO("NBSintaxParserCPProc, HINT, leaf(%d, '%s') will be ignored.\n", hint.iLeafReplace, tmp.str);
			NBString_release(&tmp);
		}
#		endif
		NBArray_addValue(dst, hint);
	}
	//Analyze '##' states
	{
		//Mark previous parameter as start-of-concatenation
		if(isDblHashToken){
			if(!prevTokenRef->isPopulated){
				PRINTF_ERROR("NBSintaxParserCPProc, '##' canot be the first token in the replacement-list.\n");
				r = FALSE;
			} else {
				NBASSERT(dst->use > 0) //At least the "##" must be added at this point
				BOOL prevHintAdded = FALSE;
				if(dst->use > 0){
					STNBSintaxParserCMacroReplaceHint* prevHint = NBArray_itmPtrAtIndex(dst, STNBSintaxParserCMacroReplaceHint, dst->use - 1);
					if(prevHint->iLeafReplace == prevTokenRef->iLeaf){
						//Just update the hint
						prevHint->isConcatStart = TRUE; //is preceded by "##"
						prevHintAdded = TRUE;
#						ifdef NB_CONFIG_INCLUDE_ASSERTS
						{
							STNBString tmp;
							NBString_init(&tmp);
							NBString_concatBytes(&tmp, val, valSz);
							PRINTF_INFO("NBSintaxParserCPProc, HINT, leaf(%d, '%s'), will-start-concatenation.\n", prevHint->iLeafReplace, tmp.str);
							NBString_release(&tmp);
						}
#						endif
					}
				}
				//Add hint if necesary
				if(!prevHintAdded){
					STNBSintaxParserCMacroReplaceHint hint;
					NBMemory_setZeroSt(hint, STNBSintaxParserCMacroReplaceHint);
					hint.type				= ENNBSintaxParserCMacroReplaceHintType_Token;
					hint.iLeafReplace		= prevTokenRef->iLeaf;
					hint.isConcatStart		= TRUE;	//is preceded by "##"
#					ifdef NB_CONFIG_INCLUDE_ASSERTS
					{
						STNBString tmp;
						NBString_init(&tmp);
						NBString_concatBytes(&tmp, val, valSz);
						PRINTF_INFO("NBSintaxParserCPProc, HINT, leaf(%d, '%s'), will-start-concatenation.\n", hint.iLeafReplace, tmp.str);
						NBString_release(&tmp);
					}
#					endif
					NBArray_addValue(dst, hint);
				}
			}
		}
		//Flags next token as continue-concatenation
		if(prevTokenRef->isDblHashToken){
			isDblHashReplc = TRUE;
		}
	}
	//Analyze token
	if(!(isCommaToken || isHashToken || isDblHashToken)){
		BOOL hintAdded = FALSE;
		//Analyze param
		if(bldDef->isFunction){
			UI32 iParam = 0;
			SI32 iLeafParam = -1;
			//Find param
			if(NBString_strIsEqualStrBytes("__VA_ARGS__", val, valSz)){
				if(!bldDef->isVaArgs){
					PRINTF_ERROR("NBSintaxParserCPProc, __VA_ARGS__ shall occur only in function-like macro that uses the ellipsis notation in the parameters.\n");
					r = FALSE;
				} else {
					iParam		= bldDef->paramsCount;
					isParamRef	= TRUE;
				}
			} else if(bldDef->params != NULL){
				const ENSintaxParserC99TC3RulesElem iElemIdentifier = ENSintaxParserC99TC3RulesElem_2; //"identifier"
				NBASSERT(ENSintaxParserC99TC3Rule_identifier == NBSintaxParserCDefs_getRuleIdxByElemIdx(iElemIdentifier));
				const SI32 iFnd = NBSintaxParserResults_nodeChildElemIdxByValue(bldDef->params, bldDef->strAccum, bldDef->strAccumSz, iElemIdentifier, val, valSz); 
				if(iFnd >= 0){
					NBASSERT(iFnd < bldDef->paramsCount)
					if(iFnd >= bldDef->paramsCount){
						PRINTF_ERROR("NBSintaxParserCPProc, params index is out-of-range.\n");
						r = FALSE;
					} else {
						iParam		= (bldDef->paramsCount - iFnd - 1);
						isParamRef	= TRUE;
					}
				}
			}
			//Add
			if(isParamRef){
				STNBSintaxParserCMacroReplaceHint hint;
				NBMemory_setZeroSt(hint, STNBSintaxParserCMacroReplaceHint);
				hint.type				= ENNBSintaxParserCMacroReplaceHintType_ParamRef;
				hint.iLeafReplace		= iLeaf;
				hint.iLeafParam			= iLeafParam;
				hint.iParam				= iParam;
				hint.isParamToLiteral	= prevTokenRef->isHashToken;	//is preceded by "#"
				hint.isConcatContinue	= prevTokenRef->isDblHashToken;	//is preceded by "##"
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				{
					STNBString tmp;
					NBString_init(&tmp);
					NBString_concatBytes(&tmp, val, valSz);
					PRINTF_INFO("NBSintaxParserCPProc, HINT, leaf(%d, '%s') is argument(#%d of %d%s).\n", hint.iLeafReplace, tmp.str, (hint.iParam + 1), bldDef->paramsCount, (hint.isParamToLiteral ? ", is-string-literal-replace" : hint.isConcatContinue ? ", will-be-concatenated": ""));
					NBString_release(&tmp);
				}
#				endif
				NBArray_addValue(dst, hint);
				hintAdded = TRUE;
			}
		}
		//Add merge hint if necesary (preceded by "##")
		if(!hintAdded && prevTokenRef->isDblHashToken){
			STNBSintaxParserCMacroReplaceHint hint;
			NBMemory_setZeroSt(hint, STNBSintaxParserCMacroReplaceHint);
			hint.type				= ENNBSintaxParserCMacroReplaceHintType_Token;
			hint.iLeafReplace		= iLeaf;
			hint.isConcatContinue	= prevTokenRef->isDblHashToken;	//is preceded by "##"
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
			{
				STNBString tmp;
				NBString_init(&tmp);
				NBString_concatBytes(&tmp, val, valSz);
				PRINTF_INFO("NBSintaxParserCPProc, HINT, leaf(%d, '%s'), will-be-concatenated.\n", hint.iLeafReplace, tmp.str);
				NBString_release(&tmp);
			}
#				endif
			NBArray_addValue(dst, hint);
			hintAdded = TRUE;
		}
	}
	//Save as prev token
	if(prevTokenRef != NULL){
		//Validate 
		if(bldDef->isFunction && prevTokenRef->isHashToken && !isParamRef){
			PRINTF_ERROR("NBSintaxParserCPProc, '#' must be followed by a parameter.\n");
			r = FALSE;
		}
		prevTokenRef->iLeaf			= iLeaf;
		prevTokenRef->iByteStart	= replacementParent->iByteStart;
		prevTokenRef->bytesLen		= valSz;
		prevTokenRef->isPopulated	= TRUE;
		prevTokenRef->isHashToken	= isHashToken;
		prevTokenRef->isDblHashToken = isDblHashToken;
		prevTokenRef->isParamRef	= isParamRef;
	}
	return r;
}

BOOL NBSintaxParserCPProc_macrosFind_(void* userParam, const char* identifier, const UI32 identifierLen, STNBSintaxParserCMacro** dst){
	return NBSintaxParserCPProc_macrosFind((STNBSintaxParserCPProc*)userParam, identifier, identifierLen, dst);
}

BOOL NBSintaxParserCPProc_macrosFind(STNBSintaxParserCPProc* obj, const char* identifier, const UI32 identifierLen, STNBSintaxParserCMacro** dst){
	BOOL r = FALSE;
	NBASSERT(obj != NULL)
	if(obj != NULL){
		SI32 iFnd = - 1;
		STNBSintaxParserCMacro* srchPtr = NULL;
		STNBSintaxParserCMacro srch;
		NBMemory_setZeroSt(srch, STNBSintaxParserCMacro);
		srch.def.refs.name		= identifier;
		srch.def.refs.nameLen	= identifierLen;
		srchPtr					= &srch;
		iFnd					= NBArraySorted_indexOf(&obj->macros, &srchPtr, sizeof(srchPtr), NULL);
		if(iFnd >= 0){
			STNBSintaxParserCMacro* mm = NBArraySorted_itmValueAtIndex(&obj->macros, STNBSintaxParserCMacro*, iFnd);
			NBASSERT(NBString_strIsEqualBytes(srch.def.refs.name, srch.def.refs.nameLen, mm->def.refs.name, mm->def.refs.nameLen))
			if(dst != NULL) *dst = mm;
			r = TRUE;
		} 
	}
	return r;
}

void NBSintaxParserCPProc_macrosEmpty(STNBSintaxParserCPProc* obj){
	NBASSERT(obj != NULL)
	if(obj != NULL){
		STNBArraySorted* macros = &obj->macros; 
		SI32 i; for(i = (SI32)macros->use - 1; i >= 0 ; i--){
			STNBSintaxParserCMacro* mm = NBArraySorted_itmValueAtIndex(macros, STNBSintaxParserCMacro*, i);
			if(mm != NULL){
				//Def
				{
					NBSintaxParserResults_nodeRelease(&mm->def.node);
					NBString_release(&mm->def.str);
					NBMemory_setZero(mm->def.refs);
				}
				//Hints
				NBArray_release(&mm->def.refs.replacementHints);
				//Release
				NBMemory_free(mm);
				mm = NULL;
			}
		}
		NBArraySorted_empty(macros);
	}
}

//

BOOL NBCompare_STNBSintaxParserCMacroPtr(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz){
	NBASSERT(dataSz == sizeof(STNBSintaxParserCMacro*))
	if(dataSz == sizeof(STNBSintaxParserCMacro*)){
		const STNBSintaxParserCMacro* d1 = *((const STNBSintaxParserCMacro**)data1);
		const STNBSintaxParserCMacro* d2 = *((const STNBSintaxParserCMacro**)data2);
		switch (mode) {
			case ENCompareMode_Equal:
				return NBString_strIsEqualBytes(d1->def.refs.name, d1->def.refs.nameLen, d2->def.refs.name, d2->def.refs.nameLen);
			case ENCompareMode_Lower:
				return NBString_strIsLowerBytes(d1->def.refs.name, d1->def.refs.nameLen, d2->def.refs.name, d2->def.refs.nameLen);
			case ENCompareMode_LowerOrEqual:
				return NBString_strIsLowerOrEqualBytes(d1->def.refs.name, d1->def.refs.nameLen, d2->def.refs.name, d2->def.refs.nameLen);
			case ENCompareMode_Greater:
				return NBString_strIsGreaterBytes(d1->def.refs.name, d1->def.refs.nameLen, d2->def.refs.name, d2->def.refs.nameLen);
			case ENCompareMode_GreaterOrEqual:
				return NBString_strIsGreaterOrEqualBytes(d1->def.refs.name, d1->def.refs.nameLen, d2->def.refs.name, d2->def.refs.nameLen);
			default:
				NBASSERT(FALSE)
				break;
		}
	}
	return FALSE;
}

//Feed

BOOL NBSintaxParserCPProc_feedToken(STNBSintaxParserCPProc* obj, const ENSintaxParserC99TC3Rule type, const char* val, const UI32 valSz){
	BOOL r = FALSE;
	
	return r;
}

BOOL NBSintaxParserCPProc_feedUtf(STNBSintaxParserCPProc* obj, const char* utf, const UI32 utfSz){
	BOOL r = FALSE;
	
	return r;
}

BOOL NBSintaxParserCPProc_feedFlush(STNBSintaxParserCPProc* obj){
	BOOL r = FALSE;
	
	return r;
}
