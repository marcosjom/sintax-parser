#include "nb/NBFrameworkPch.h"
#include "nb/sintax/NBSintaxParserRules.h"
//
#include "nb/core/NBMemory.h"
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/core/NBEncoding.h"

//

//Start Hints

void NBSintaxParserElemHintStart_init(STNBSintaxParserElemHintStart* obj);
void NBSintaxParserElemHintStart_release(STNBSintaxParserElemHintStart* obj);

void NBSintaxParserElemHintStartNode_init(STNBSintaxParserElemHintStartNode* obj);
void NBSintaxParserElemHintStartNode_release(STNBSintaxParserElemHintStartNode* obj);
STNBSintaxParserElemHintStartNode* NBSintaxParserElemHintStartNode_getChild(STNBSintaxParserElemHintStartNode* obj, const UI32 iElem, const UI32 iDef, const UI32 iPart, const BOOL addIfNecessary);

BOOL NBCompare_NBSintaxParserElemHintStartByElemIdx(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz){
	const STNBSintaxParserElemHintStart* d1 = (STNBSintaxParserElemHintStart*)data1;
	const STNBSintaxParserElemHintStart* d2 = (STNBSintaxParserElemHintStart*)data2;
	NBASSERT(dataSz == sizeof(*d1))
	if(dataSz == sizeof(*d1)){
		NBASSERT(d1->type == ENSintaxParserElemType_Elem || d1->type == ENSintaxParserElemType_Callback)
		NBASSERT(d2->type == ENSintaxParserElemType_Elem || d2->type == ENSintaxParserElemType_Callback)
		switch (mode) {
			case ENCompareMode_Equal:
				return d1->iElem == d2->iElem;
			case ENCompareMode_Lower:
				return d1->iElem < d2->iElem;
			case ENCompareMode_LowerOrEqual:
				return d1->iElem <= d2->iElem;
			case ENCompareMode_Greater:
				return d1->iElem > d2->iElem;
			case ENCompareMode_GreaterOrEqual:
				return d1->iElem >= d2->iElem;
			default:
				NBASSERT(FALSE)
				break;
		}
	}
	NBASSERT(FALSE)
	return FALSE;
}

BOOL NBCompare_NBSintaxParserElemHintStartByUtfChar(const ENCompareMode mode, const void* data1, const void* data2, const UI32 dataSz){
	NBASSERT(dataSz == sizeof(STNBSintaxParserElemHintStart))
	if(dataSz == sizeof(STNBSintaxParserElemHintStart)){
		const STNBSintaxParserElemHintStart* d1 = (STNBSintaxParserElemHintStart*)data1;
		const STNBSintaxParserElemHintStart* d2 = (STNBSintaxParserElemHintStart*)data2;
		NBASSERT(d1->type == ENSintaxParserElemType_Literal)
		NBASSERT(d2->type == ENSintaxParserElemType_Literal)
		switch (mode) {
			case ENCompareMode_Equal:
				return NBString_strIsEqualBytes(d1->utf.val, d1->utf.valSz, d2->utf.val, d2->utf.valSz);
			case ENCompareMode_Lower:
				return NBString_strIsLowerBytes(d1->utf.val, d1->utf.valSz, d2->utf.val, d2->utf.valSz);
			case ENCompareMode_LowerOrEqual:
				return NBString_strIsLowerOrEqualBytes(d1->utf.val, d1->utf.valSz, d2->utf.val, d2->utf.valSz);
			case ENCompareMode_Greater:
				return NBString_strIsGreaterBytes(d1->utf.val, d1->utf.valSz, d2->utf.val, d2->utf.valSz);
			case ENCompareMode_GreaterOrEqual:
				return NBString_strIsGreaterOrEqualBytes(d1->utf.val, d1->utf.valSz, d2->utf.val, d2->utf.valSz);
			default:
				NBASSERT(FALSE)
				break;
		}
	}
	NBASSERT(FALSE)
	return FALSE;
}

//

void NBSintaxParserRules_init(STNBSintaxParserRules* obj){
	NBArray_init(&obj->elems, sizeof(STNBSintaxParserElem), NULL);
	//Parser
	{
		STNBSintaxParserRulesParserState* parser = &obj->parser;
		NBMemory_setZeroSt(*parser, STNBSintaxParserRulesParserState);
		NBString_init(&parser->errDescs);
		NBString_init(&parser->tknAccum);
	}
}

void NBSintaxParserRules_release(STNBSintaxParserRules* obj){
	{
		NBSintaxParserRules_empty(obj);
		NBArray_release(&obj->elems);
		//Parser
		{
			STNBSintaxParserRulesParserState* parser = &obj->parser;
			NBString_release(&parser->errDescs);
			NBString_release(&parser->tknAccum);
		}
	}
}

//-----------------
//Start hints
//-----------------

void NBSintaxParserElemHintStart_init(STNBSintaxParserElemHintStart* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserElemHintStart);
}

void NBSintaxParserElemHintStart_release(STNBSintaxParserElemHintStart* obj){
	if(obj->type == ENSintaxParserElemType_Literal){
		if(obj->utf.val != NULL){
			NBMemory_free(obj->utf.val);
			obj->utf.val	= NULL;
			obj->utf.valSz	= 0;
		}
	}
	NBSintaxParserElemHintStartNode_release(&obj->posibs);
}

void NBSintaxParserElemHintStartNode_init(STNBSintaxParserElemHintStartNode* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserElemHintStartNode);
}

void NBSintaxParserElemHintStartNode_release(STNBSintaxParserElemHintStartNode* obj){
	if(obj->childrn != NULL){
		UI32 i; for(i = 0; i < obj->childrnSz; i++){
			STNBSintaxParserElemHintStartNode* chld = &obj->childrn[i];
			NBASSERT(chld != NULL)
			if(chld != NULL){
				NBSintaxParserElemHintStartNode_release(chld);
				chld = NULL;
			}
		}
		//
		NBMemory_free(obj->childrn);
		obj->childrn = NULL;
		obj->childrnSz = 0;
	}
}

STNBSintaxParserElemHintStartNode* NBSintaxParserElemHintStartNode_getChild(STNBSintaxParserElemHintStartNode* obj, const UI32 iElem, const UI32 iDef, const UI32 iPart, const BOOL addIfNecessary){
	STNBSintaxParserElemHintStartNode* r = NULL;
	//Search
	if(obj->childrn != NULL && obj->childrnSz > 0){
		UI32 i; for(i = 0; i < obj->childrnSz; i++){
			STNBSintaxParserElemHintStartNode* chld = &obj->childrn[i];
			NBASSERT(chld != NULL)
			if(chld != NULL){
				if(chld->idx.iElem == iElem && chld->idx.iDef == iDef && chld->idx.iPart == iPart){
					r = chld;
					break;
				}
			}
		}
	}
	//Add
	if(r == NULL && addIfNecessary){
		STNBSintaxParserElemHintStartNode chld;
		NBSintaxParserElemHintStartNode_init(&chld);
		chld.idx.iElem	= iElem;
		chld.idx.iDef	= iDef;
		chld.idx.iPart	= iPart;
		//Add
		{
			STNBSintaxParserElemHintStartNode* arrN = NBMemory_allocTypes(STNBSintaxParserElemHintStartNode, obj->childrnSz + 1);
			//Copy current
			if(obj->childrn != NULL){
				if(obj->childrnSz > 0){
					NBMemory_copy(arrN, obj->childrn, sizeof(obj->childrn[0]) * obj->childrnSz);
				}
				NBMemory_free(obj->childrn);
			}
			r = &arrN[obj->childrnSz];
			arrN[obj->childrnSz++] = chld;
			obj->childrn = arrN;
		}
	}
	return r;
}

//--------------
//- Sintax rules
//--------------

void NBSintaxParserRules_empty(STNBSintaxParserRules* obj){
	SI32 i; for(i = (SI32)obj->elems.use - 1; i >= 0; i--){
		STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
		if(ee->name != NULL) NBMemory_free(ee->name); ee->name = NULL;
		if(ee->defs != NULL && ee->defsSz > 0){
			SI32 i; for(i = (SI32)ee->defsSz - 1; i >= 0; i--){
				STNBSintaxParserElemDef* dd = &ee->defs[i];
				if(dd->parts != NULL) NBMemory_free(dd->parts); dd->parts = NULL;
				dd->partsSz	= 0;
			}
			NBMemory_free(ee->defs);
		}
		ee->defs	= NULL;
		ee->defsSz	= 0;
		//Hints
		{
			ee->hints.canBeEmpty		= FALSE;
			ee->hints.canBeRecursive	= FALSE;
			{
				SI32 i; for(i = 0; i < ENSintaxParserElemType_Count; i++){
					STNBArraySorted* arr = &ee->hints.starts[i];
					SI32 i2; for(i2 = (SI32)arr->use - 1; i2 >= 0; i2--){
						STNBSintaxParserElemHintStart* ss = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, i2);
						NBSintaxParserElemHintStart_release(ss);
					}
					NBArraySorted_empty(arr);
					NBArraySorted_release(arr);
				}
			}
		}
	}
	NBArray_empty(&obj->elems);
}

STNBSintaxParserElem* NBSintaxParserRules_elemByName(STNBSintaxParserRules* obj, const ENSintaxParserElemType type, const char* name, const BOOL addIfNecessary){
	STNBSintaxParserElem* r = NULL;
	NBASSERT(!NBString_strIsEmpty(name))
	{
		SI32 i; for(i = (SI32)obj->elems.use - 1; i >= 0; i--){
			STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
			if(type == ee->type){
				if(NBString_strIsEqual(ee->name, name)){
					r = ee;
					break;
				}
			}
		}
		//Add new
		if(i == -1 && addIfNecessary){
			STNBSintaxParserElem ee;
			NBMemory_setZeroSt(ee, STNBSintaxParserElem);
			ee.type			= type;
			ee.name			= NBString_strNewBuffer(name);
			ee.nameLen		= NBString_strLenBytes(name);
			{
				NBASSERT(ENSintaxParserElemType_Count == 3)
				NBArraySorted_init(&ee.hints.starts[ENSintaxParserElemType_Literal], sizeof(STNBSintaxParserElemHintStart), NBCompare_NBSintaxParserElemHintStartByUtfChar);
				NBArraySorted_init(&ee.hints.starts[ENSintaxParserElemType_Callback], sizeof(STNBSintaxParserElemHintStart), NBCompare_NBSintaxParserElemHintStartByElemIdx);
				NBArraySorted_init(&ee.hints.starts[ENSintaxParserElemType_Elem], sizeof(STNBSintaxParserElemHintStart), NBCompare_NBSintaxParserElemHintStartByElemIdx);
			}
			r = (STNBSintaxParserElem*)NBArray_addValue(&obj->elems, ee);
		}
	}
	return r;
}

void NBSintaxParserRules_feedStart(STNBSintaxParserRules* obj){
	//Empty rules
	NBSintaxParserRules_empty(obj);
	//Restarts parse state
	{
		STNBSintaxParserRulesParserState* parser = &obj->parser;
		parser->errFnd			= FALSE;
		parser->completed		= FALSE;
		parser->isLineComment	= FALSE;
		parser->utf8AccumExp	= 0;
		parser->utf8AccumCur	= 0;
		parser->iCurElem		= 0;
		parser->curRuleTknsCount = 0;
		NBString_empty(&parser->errDescs);
		NBString_empty(&parser->tknAccum);
	}
}
	
BOOL NBSintaxParserRules_feed(STNBSintaxParserRules* obj, const char* syntaxDefs){
	BOOL r = FALSE;
	STNBSintaxParserRulesParserState* parser = &obj->parser;
	if(!parser->completed){
		if(syntaxDefs != NULL && !parser->errFnd){
			const char* c = syntaxDefs;
			while(*c != '\0'){
				if(!NBSintaxParserRules_feedByte(obj, *c)){
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

BOOL NBSintaxParserRules_feedBytes(STNBSintaxParserRules* obj, const char* syntaxDefs, const SI32 syntaxDefsSz){
	BOOL r = FALSE;
	STNBSintaxParserRulesParserState* parser = &obj->parser;
	if(!parser->completed){
		if(syntaxDefs != NULL &&  syntaxDefsSz > 0 && !parser->errFnd){
			const char* c = syntaxDefs;
			const char* cAfterEnd = &syntaxDefs[syntaxDefsSz];
			while(c < cAfterEnd){
				if(!NBSintaxParserRules_feedByte(obj, *c)){
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

void NBSintaxParserRules_startNewDef(STNBSintaxParserElem* ee){
	STNBArray defs;
	NBArray_initWithSz(&defs, sizeof(STNBSintaxParserElemDef), NULL, (ee->defsSz + 1), 1, 0.10f);
	//Add current
	if(ee->defs != NULL && ee->defsSz > 0){
		NBArray_addItems(&defs, ee->defs, sizeof(ee->defs[0]), ee->defsSz);
	}
	//Add new
	{
		STNBSintaxParserElemDef dd;
		NBMemory_setZeroSt(dd, STNBSintaxParserElemDef);
		NBArray_addValue(&defs, dd);
	}
	//Swap buffers
	{
		if(ee->defs != NULL) NBMemory_free(ee->defs);
		ee->defs	= NBArray_dataPtr(&defs, STNBSintaxParserElemDef);
		ee->defsSz	= defs.use;
		NBArray_resignToBuffer(&defs);
	}
	NBArray_release(&defs);
}

BOOL NBSintaxParserRules_appendPartToLastDef(STNBSintaxParserElem* ee, const UI32 iElem, const BOOL isOptional){
	BOOL r = FALSE;
	NBASSERT(ee->defs != NULL && ee->defsSz > 0) //Internal error
	if(ee->defs != NULL && ee->defsSz > 0){
		STNBSintaxParserElemDef* dd = &ee->defs[ee->defsSz - 1];
		//Add callback
		STNBArray parts;
		NBArray_initWithSz(&parts, sizeof(STNBSintaxParserElemDefPart), NULL, (dd->partsSz + 1), 1, 0.10f);
		//Add current
		if(dd->parts != NULL && dd->partsSz > 0){
			NBArray_addItems(&parts, dd->parts, sizeof(dd->parts[0]), dd->partsSz);
		}
		//Add new
		{
			STNBSintaxParserElemDefPart pp;
			NBMemory_setZeroSt(pp, STNBSintaxParserElemDefPart);
			pp.iElem		= iElem;
			pp.isOptional	= isOptional;
			NBArray_addValue(&parts, pp);
		}
		//Swap buffers
		{
			if(dd->parts != NULL) NBMemory_free(dd->parts);
			dd->parts	= NBArray_dataPtr(&parts, STNBSintaxParserElemDefPart);
			dd->partsSz	= parts.use;
			NBArray_resignToBuffer(&parts);
		}
		NBArray_release(&parts);
		r = TRUE;
	}
	return r;
}

BOOL NBSintaxParserRules_feedByte(STNBSintaxParserRules* obj, const char c){
	BOOL r = FALSE;
	STNBSintaxParserRulesParserState* parser = &obj->parser;
	if(!parser->completed){
		//Detect the size of the utf char
		if(!parser->errFnd && parser->utf8AccumExp == 0){
			if((parser->utf8AccumExp = NBEncoding_utf8BytesExpected(c)) <= 0){
				PRINTF_ERROR("NBSintaxParserRules, malformed utf8 content.\n");
				{
					if(parser->errDescs.length > 0){
						NBString_concatByte(&parser->errDescs, '\n');
					}
					NBString_concat(&parser->errDescs, "Malformed utf8 content.");
				}
				parser->errFnd = TRUE;
			}
		}
		//Accumulate byte
		if(!parser->errFnd){
			parser->utf8Accum[parser->utf8AccumCur++] = c;
			//Flush character
			if(parser->utf8AccumCur == parser->utf8AccumExp){
				const BOOL isNewLine	= (parser->utf8Accum[0] == '\r' || parser->utf8Accum[0] == '\n');
				const BOOL isSeparator	= (isNewLine || parser->utf8Accum[0] == ' ' || parser->utf8Accum[0] == '\t');
				//Process utf
				if(!parser->isLineComment){
					if(!isSeparator){
						//Concat to token
						NBString_concatBytes(&parser->tknAccum, parser->utf8Accum, parser->utf8AccumCur);
						//Detect "//"
						if(parser->tknAccum.length == 2){
							const char* tkn		= parser->tknAccum.str;
							if(tkn[0] == '/' && tkn[1] == '/'){
								//Start next token
								parser->isLineComment = TRUE;
								NBString_empty(&parser->tknAccum);
							}
						}
					} else {
						//Process token
						if(parser->tknAccum.length > 0){
							const char* tkn				= parser->tknAccum.str;
							const SI32 tknSz			= parser->tknAccum.length;
							const BOOL literalStarts	= (tkn[0] == '[');
							const BOOL literalEnds		= (tkn[tknSz - 1] == ']');
							const BOOL isNewDef			= (tkn[tknSz - 1] == ':');
							//PRINTF_INFO("NBSintaxParserRules, token '%s'.\n", tkn);
							if(literalStarts != literalEnds){
								PRINTF_ERROR("NBSintaxParserRules, token '%s' start and end missmatch.\n", tkn);
								{
									if(parser->errDescs.length > 0){
										NBString_concatByte(&parser->errDescs, '\n');
									}
									NBString_concat(&parser->errDescs, "Token '");
									NBString_concat(&parser->errDescs, tkn);
									NBString_concat(&parser->errDescs, "' start and end missmatch.");
								}
								parser->errFnd = TRUE;
							} else if(isNewDef){
								//Remove the ":"
								NBString_removeLastBytes(&parser->tknAccum, 1);
								//Add and set current elem
								{
									const STNBSintaxParserElem* ee = NBSintaxParserRules_elemByName(obj, ENSintaxParserElemType_Elem, parser->tknAccum.str, TRUE);
									parser->iCurElem = (UI32)(ee - NBArray_dataPtr(&obj->elems, STNBSintaxParserElem));
									NBASSERT(parser->iCurElem >= 0 && parser->iCurElem < obj->elems.use)
								}
							} else {
								//Add new element to current rule
								if(parser->iCurElem >= obj->elems.use){
									PRINTF_ERROR("NBSintaxParserRules, definition without parent element defined (at '%s').\n", tkn);
									{
										if(parser->errDescs.length > 0){
											NBString_concatByte(&parser->errDescs, '\n');
										}
										NBString_concat(&parser->errDescs, "Definition without parent element defined (at '");
										NBString_concat(&parser->errDescs, tkn);
										NBString_concat(&parser->errDescs, "').");
									}
									parser->errFnd = TRUE;
								} else {
									BOOL isOptional = FALSE;
									//Find and remove "(opt)"
									if(parser->tknAccum.length > 5){
										if(NBString_strIsEqual(&parser->tknAccum.str[parser->tknAccum.length - 5], "(opt)")){
											NBString_removeLastBytes(&parser->tknAccum, 5);
											isOptional = TRUE;
										}
									}
									//Start new definition at element
									if(parser->curRuleTknsCount == 0){
										STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, parser->iCurElem);
										NBSintaxParserRules_startNewDef(ee);
									}
									//Add part to last definition
									if(NBString_strIsEqual(parser->tknAccum.str, "(callback)")){
										//Add callback part
										const STNBSintaxParserElem* ee = NBSintaxParserRules_elemByName(obj, ENSintaxParserElemType_Callback, parser->tknAccum.str, TRUE);
										const UI32 eeIdx = (UI32)(ee - NBArray_dataPtr(&obj->elems, STNBSintaxParserElem)); 
										NBASSERT(eeIdx >= 0 && eeIdx < obj->elems.use)
										{
											STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, parser->iCurElem);
											if(!NBSintaxParserRules_appendPartToLastDef(ee, eeIdx, isOptional)){
												parser->errFnd = TRUE;
											}
										}
									} else if(literalStarts && literalEnds){
										//Remove the "]"
										NBString_removeLastBytes(&parser->tknAccum, 1);
										{
											//Add literal part
											const STNBSintaxParserElem* ee = NBSintaxParserRules_elemByName(obj, ENSintaxParserElemType_Literal, &parser->tknAccum.str[1], TRUE);
											const UI32 eeIdx = (UI32)(ee - NBArray_dataPtr(&obj->elems, STNBSintaxParserElem)); 
											NBASSERT(eeIdx >= 0 && eeIdx < obj->elems.use)
											{
												STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, parser->iCurElem);
												if(!NBSintaxParserRules_appendPartToLastDef(ee, eeIdx, isOptional)){
													parser->errFnd = TRUE;
												}
											}
										}
									} else {
										//Add reference part
										const STNBSintaxParserElem* ee = NBSintaxParserRules_elemByName(obj, ENSintaxParserElemType_Elem, parser->tknAccum.str, TRUE);
										const UI32 eeIdx = (UI32)(ee - NBArray_dataPtr(&obj->elems, STNBSintaxParserElem)); 
										NBASSERT(eeIdx >= 0 && eeIdx < obj->elems.use)
										{
											STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, parser->iCurElem);
											if(!NBSintaxParserRules_appendPartToLastDef(ee, eeIdx, isOptional)){
												parser->errFnd = TRUE;
											}
										}
									}
									parser->curRuleTknsCount++;
								}
							}
							//Start next token
							NBString_empty(&parser->tknAccum);
						}
					}
				}
				//End rule
				if(isNewLine){
					parser->isLineComment = FALSE;
					parser->curRuleTknsCount = 0;
				}
				//Start next char
				parser->utf8AccumExp = 0;
				parser->utf8AccumCur = 0;
			}
		}
		r = (!parser->errFnd);
	}
	return r;
}

STNBSintaxParserElemHintStart* NBSintaxParserRules_getHintStart(STNBSintaxParserElem* hintsOwner, const ENSintaxParserElemType type, const UI32 iElem, const char* utfChar, const UI32 utfCharSz, const BOOL addIfNecesary){
	STNBSintaxParserElemHintStart* r = NULL;
	STNBArraySorted* arr = &hintsOwner->hints.starts[type];
	//Find current
	if(arr->use > 0){
		STNBSintaxParserElemHintStart srch;
		srch.type			= type; 
		if(type == ENSintaxParserElemType_Literal){
			srch.utf.val	= (char*)utfChar;
			srch.utf.valSz	= utfCharSz;
		} else {
			srch.iElem		= iElem;
		}
		{
			const SI32 iFnd	= NBArraySorted_indexOf(arr, &srch, sizeof(srch), NULL);
			if(iFnd >= 0){
				r = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, iFnd);
#				ifdef NB_CONFIG_INCLUDE_ASSERTS
				NBASSERT(srch.type == r->type)
				if(type == ENSintaxParserElemType_Literal){
					NBASSERT(srch.utf.valSz == r->utf.valSz)
					NBASSERT(NBString_strIsEqualBytes(srch.utf.val, srch.utf.valSz, r->utf.val, r->utf.valSz))
				} else {
					NBASSERT(srch.iElem == r->iElem)
				}
#				endif
			}
		}
	}
	//Create (if necesary)
	if(r == NULL && addIfNecesary){
		STNBSintaxParserElemHintStart ss;
		NBSintaxParserElemHintStart_init(&ss);
		ss.type				= type; 
		if(type == ENSintaxParserElemType_Literal){
			ss.utf.val		= NBString_strNewBufferBytes(utfChar, utfCharSz);
			ss.utf.valSz	= utfCharSz;
		} else {
			ss.iElem		= iElem;
		}
		NBSintaxParserElemHintStartNode_init(&ss.posibs);
		r = (STNBSintaxParserElemHintStart*)NBArraySorted_addValue(arr, ss);
#		ifdef NB_CONFIG_INCLUDE_ASSERTS
		NBASSERT(ss.type == r->type)
		if(type == ENSintaxParserElemType_Literal){
			NBASSERT(ss.utf.valSz == r->utf.valSz)
			NBASSERT(NBString_strIsEqualBytes(ss.utf.val, ss.utf.valSz, r->utf.val, r->utf.valSz))
		} else {
			NBASSERT(ss.iElem == r->iElem)
		}
#		endif
	}
	return r;
}
								  
void NBSintaxParserRules_hintsAdd(STNBSintaxParserRules* obj, STNBSintaxParserElem* hintsOwner, const STNBSintaxParserPartIdx* curPath, const UI32 curPathSz, const UI32 curPathElemIdx, const STNBSintaxParserElem* curPathElem, BOOL* dstFullOptionalDefFound){
	BOOL isRedundant = FALSE, fullOptionalDefFound = FALSE;
	//Detect redundancy
	if(curPath != NULL && curPathSz > 0){
		UI32 i; for(i = 0; i < curPathSz; i++){
			const STNBSintaxParserPartIdx* pElem = &curPath[i];
			if(pElem->iElem == curPathElemIdx){
				/*{
					STNBString str;
					NBString_init(&str);
					NBString_concat(&str, "Rule '");
					NBString_concat(&str, hintsOwner->name);
					NBString_concat(&str, "' is redundant at path: ");
					{
						UI32 i; for(i = 0; i < curPathSz; i++){
							const STNBSintaxParserElemHintStartPart* pElem = &curPath[i];
							STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, pElem->iElem);
							if(i != 0) NBString_concat(&str, " -> ");
							//Step
							NBString_concatByte(&str, '\'');
							NBString_concat(&str, ee2->name);
							NBString_concatByte(&str, '\'');
							if(pElem->iElem == curPathElemIdx){
								NBString_concat(&str, " (redundant) ");
							}
						}
						//Repeat
						NBString_concat(&str, " -> ");
						NBString_concatByte(&str, '\'');
						NBString_concat(&str, curPathElem->name);
						NBString_concatByte(&str, '\'');
						NBString_concat(&str, " (again).");
					}
					NBString_release(&str);
				}*/
				isRedundant = TRUE;
				break;
			}
		}
	}
	//Process (if not redundant)
	if(!isRedundant){
		const STNBSintaxParserElem* ee = curPathElem;
		if(ee->defs != NULL && ee->defsSz > 0){
			UI32 iDef; for(iDef = 0; iDef < ee->defsSz; iDef++){
				STNBSintaxParserElemDef* dd = &ee->defs[iDef];
				if(dd->parts != NULL && dd->partsSz > 0){
					BOOL stopped = FALSE;
					UI32 iPart; for(iPart = 0; iPart < dd->partsSz && !stopped; iPart++){
						STNBSintaxParserElemDefPart* pp = &dd->parts[iPart];
						NBASSERT(pp->iElem >= 0 && pp->iElem < obj->elems.use)
						STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, pp->iElem);
						BOOL ee2IsOptional = TRUE;
						switch(ee2->type) {
							case ENSintaxParserElemType_Literal:
								//Add literal
								if(!NBString_strIsEmpty(ee2->name)){
									const UI32 firstCharLen = NBEncoding_utf8BytesExpected(*ee2->name);
									const UI32 nameLen = NBString_strLenBytes(ee2->name);
									//Use original 'isOptional' flag
									{
										ee2IsOptional = pp->isOptional;
									}
									//Add posib of starting with first-char
									{
										STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(hintsOwner, ee2->type, 0, ee2->name, firstCharLen, TRUE);
										//Create literal hint
										NBASSERT(start != NULL)
										NBASSERT(start->type == ENSintaxParserElemType_Literal)
										NBASSERT(NBString_strIsEqualBytes(start->utf.val, start->utf.valSz, ee2->name, firstCharLen))
										if(start != NULL){
											//Add previous path elements
											STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
											if(curPath != NULL && curPathSz > 0){
												UI32 i; for(i = 0; i < curPathSz; i++){
													const STNBSintaxParserPartIdx* idx = &curPath[i];
													pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, idx->iElem, idx->iDef, idx->iPart, TRUE);
													NBASSERT(pathNode != NULL);
												}
											}
											//Add element
											{
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, curPathElemIdx, iDef, iPart, TRUE);
											}
											//Add leaf node
											{
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, pp->iElem, 0, firstCharLen, TRUE);
												pathNode->isLeaf		= TRUE;
												pathNode->resultMask	= (firstCharLen == nameLen ? ENSintaxParserValidateElemResultBit_Complete : ENSintaxParserValidateElemResultBit_Partial);
											}
										}
									}
									//Add posib as iElem
									{
										STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(hintsOwner, ENSintaxParserElemType_Elem, pp->iElem, NULL, 0, TRUE);
										//Create literal hint
										NBASSERT(start != NULL)
										NBASSERT(start->type == ENSintaxParserElemType_Elem)
										NBASSERT(start->iElem == pp->iElem)
										if(start != NULL){
											//Add previous path elements
											STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
											if(curPath != NULL && curPathSz > 0){
												UI32 i; for(i = 0; i < curPathSz; i++){
													const STNBSintaxParserPartIdx* idx = &curPath[i];
													pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, idx->iElem, idx->iDef, idx->iPart, TRUE);
													NBASSERT(pathNode != NULL);
												}
											}
											//Add element
											{
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, curPathElemIdx, iDef, iPart, TRUE);
											}
											//Add leaf node
											{
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, pp->iElem, 0, 0, TRUE);
												pathNode->isLeaf		= TRUE;
												pathNode->resultMask	= ENSintaxParserValidateElemResultBit_Complete;
											}
										}
									}
								}
								break;
							case ENSintaxParserElemType_Callback:
								//Use original 'isOptional' flag
								{
									ee2IsOptional = pp->isOptional;
								}
								//Add to callback posibs
								{
									STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(hintsOwner, ee2->type, curPathElemIdx, NULL, 0, TRUE);
									//Create callback hint
									NBASSERT(start != NULL)
									NBASSERT(start->type == ENSintaxParserElemType_Callback)
									if(start != NULL){
										//Add previous path elements
										STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
										if(curPath != NULL && curPathSz > 0){
											UI32 i; for(i = 0; i < curPathSz; i++){
												const STNBSintaxParserPartIdx* idx = &curPath[i];
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, idx->iElem, idx->iDef, idx->iPart, TRUE);
												NBASSERT(pathNode != NULL);
											}
										}
										//Add element
										{
											pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, curPathElemIdx, iDef, iPart, TRUE);
										}
										//Add leaf node
										{
											pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, pp->iElem, 0, 0, TRUE);
											pathNode->isLeaf		= TRUE;
											pathNode->resultMask	= ENSintaxParserValidateElemResultBit_None;
										}
									}
								}
								break;
							case ENSintaxParserElemType_Elem:
								//Use original 'isOptional' flag
								{
									ee2IsOptional = pp->isOptional;
								}
								//Add to elem posiblilties
								{
									STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(hintsOwner, ee2->type, pp->iElem, NULL, 0, TRUE);
									//Create elem hint
									NBASSERT(start != NULL)
									NBASSERT(start->type == ENSintaxParserElemType_Elem)
									NBASSERT(start->iElem == pp->iElem)
									if(start != NULL){
										//Add previous path elements
										STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
										if(curPath != NULL && curPathSz > 0){
											UI32 i; for(i = 0; i < curPathSz; i++){
												const STNBSintaxParserPartIdx* idx = &curPath[i];
												pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, idx->iElem, idx->iDef, idx->iPart, TRUE);
												NBASSERT(pathNode != NULL);
											}
										}
										//Add leaf node
										{
											pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, curPathElemIdx, iDef, iPart, TRUE);
											pathNode->isLeaf		= TRUE;
											pathNode->resultMask	= ENSintaxParserValidateElemResultBit_Complete;
										}
										//Recursive
										if(curPathSz == 0 && pp->iElem == curPathElemIdx){
											hintsOwner->hints.canBeRecursive = TRUE;
										}
									}
								}
								//Go deeper
								{
									STNBSintaxParserPartIdx* newPath = NBMemory_allocTypes(STNBSintaxParserPartIdx, curPathSz + 1);
									//Add current path
									if(curPath != NULL && curPathSz > 0){
										NBMemory_copy(newPath, curPath, sizeof(curPath[0]) * curPathSz);
									}
									//Add this element
									{
										STNBSintaxParserPartIdx part;
										NBMemory_setZeroSt(part, STNBSintaxParserPartIdx);
										part.iElem			= curPathElemIdx;
										part.iDef			= iDef;
										part.iPart			= iPart;
										//part.iElemChild	= pp->iElem;
										//
										newPath[curPathSz]	= part;
									}
									//Call
									{
										BOOL fullOptionalDefFound = FALSE;
										NBSintaxParserRules_hintsAdd(obj, hintsOwner, newPath, (curPathSz + 1), pp->iElem, ee2, &fullOptionalDefFound);
										if(fullOptionalDefFound){
											//Set an implicit 'isOptional'
											ee2IsOptional = TRUE;
										}
									}
									NBMemory_free(newPath);
								}
								break;
							default:
								break;
						}
						//Continue posibilities
						if(!ee2IsOptional){
							stopped = TRUE;
						}
					}
					if(!stopped){
						fullOptionalDefFound = TRUE;
					}
				}
			}
		}
	}
	if(dstFullOptionalDefFound != NULL) *dstFullOptionalDefFound = fullOptionalDefFound;
}

void NBSintaxParserRules_hintsBuild(STNBSintaxParserRules* obj){
	UI32 iElem; for(iElem = 0; iElem < obj->elems.use; iElem++){
		STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, iElem);
		switch(ee->type) {
			case ENSintaxParserElemType_Literal:
				//Add literal
				if(!NBString_strIsEmpty(ee->name)){
					const UI32 firstCharLen = NBEncoding_utf8BytesExpected(*ee->name);
					const UI32 nameLen = NBString_strLenBytes(ee->name);
					//Add posib of starting with first-char
					{
						STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(ee, ee->type, 0, ee->name, firstCharLen, TRUE);
						//Create literal hint
						NBASSERT(start != NULL)
						NBASSERT(start->type == ENSintaxParserElemType_Literal)
						NBASSERT(NBString_strIsEqualBytes(start->utf.val, start->utf.valSz, ee->name, firstCharLen))
						if(start != NULL){
							STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
							//Add leaf node
							{
								pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, iElem, 0, firstCharLen, TRUE);
								pathNode->isLeaf		= TRUE;
								pathNode->resultMask	= (firstCharLen == nameLen ? ENSintaxParserValidateElemResultBit_Complete : ENSintaxParserValidateElemResultBit_Partial);
							}
						}
					}
				}
				break;
			case ENSintaxParserElemType_Callback:
				//Add to callback posibs
				{
					STNBSintaxParserElemHintStart* start = NBSintaxParserRules_getHintStart(ee, ee->type, iElem, NULL, 0, TRUE);
					//Create callback hint
					NBASSERT(start != NULL)
					NBASSERT(start->type == ENSintaxParserElemType_Callback)
					if(start != NULL){
						STNBSintaxParserElemHintStartNode* pathNode = &start->posibs;
						//Add leaf node
						{
							pathNode = NBSintaxParserElemHintStartNode_getChild(pathNode, iElem, 0, 0, TRUE);
							pathNode->isLeaf		= TRUE;
							pathNode->resultMask	= ENSintaxParserValidateElemResultBit_None;
						}
					}
				}
				break;
			case ENSintaxParserElemType_Elem:
				{
					BOOL fullOptionalDefFound = FALSE;
					NBSintaxParserRules_hintsAdd(obj, ee, NULL, 0, iElem, ee, &fullOptionalDefFound);
					if(fullOptionalDefFound){
						ee->hints.canBeEmpty = TRUE;
					}
				}
				break;
			default:
				NBASSERT(FALSE)
				break;
		}
	}
}

BOOL NBSintaxParserRules_feedEnd(STNBSintaxParserRules* obj){
	BOOL r = FALSE;
	STNBSintaxParserRulesParserState* parser = &obj->parser;
	if(!parser->completed){
		//Flush
		if(!parser->errFnd){
			NBSintaxParserRules_feed(obj, "\n");
		}
		//Post-process
		if(!parser->errFnd){
			//Validate all elems have definitions
			{
				SI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					if(ee->type == ENSintaxParserElemType_Elem){
						if(ee->defs == NULL || ee->defsSz <= 0){
							PRINTF_ERROR("NBSintaxParserRules, #%d '%s' has no definitions:\n", (i + 1), ee->name);
							{
								if(parser->errDescs.length > 0){
									NBString_concatByte(&parser->errDescs, '\n');
								}
								NBString_concat(&parser->errDescs, "No definitions for '");
								NBString_concat(&parser->errDescs, ee->name);
								NBString_concat(&parser->errDescs, "'.");
							}
							parser->errFnd = TRUE;
						}
					}
				}
			}
			//Build
			if(!parser->errFnd){
				//Build hints
				NBSintaxParserRules_hintsBuild(obj);
				//Complet
				parser->completed = TRUE;
			}
		}
		//
		r = (!parser->errFnd);
	}
	//Print rules and hints
#	ifdef NB_CONFIG_INCLUDE_ASSERTS
	/*if(r){
		STNBString str;
		NBString_init(&str);
		NBSintaxParserRules_concat(obj, &str);
		PRINTF_INFO("NBSintaxParserRules, rules:\n%s\n", str.str);
		NBString_release(&str);
	}*/
#	endif
	return r;
}

//

SI32 NBSintaxParserRules_getElemIdx(const STNBSintaxParserRules* obj, const char* name){
	SI32 r = -1;
	SI32 i; for(i = (SI32)obj->elems.use - 1; i >= 0; i--){
		STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
		if(ee->type == ENSintaxParserElemType_Elem){
			if(NBString_strIsEqual(ee->name, name)){
				r = i;
				break;
			}
		}
	}
	return r;
}

BOOL NBSintaxParserRules_getElemHeaderByIdx(const STNBSintaxParserRules* obj, const UI32 iElem, ENSintaxParserElemType* dstype, STNBString* dstName){
	BOOL r = FALSE;
	if(iElem < obj->elems.use){
		STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, iElem);
		if(dstype != NULL) *dstype = ee->type;
		if(dstName != NULL) NBString_concatBytes(dstName, ee->name, ee->nameLen);
		r = TRUE;
	}
	return r;
}

SI32 NBSintaxParserRules_getElemsCount(const STNBSintaxParserRules* obj){
	return obj->elems.use;
}

void NBSintaxParserRules_concatHintStartPosibPaths(const STNBSintaxParserRules* obj, const STNBSintaxParserElemHintStartNode* posibs, const char* parentPath, STNBString* dst){
	if(posibs != NULL){
		if(posibs->childrn != NULL && posibs->childrnSz > 0){
			STNBString tmp;
			NBString_init(&tmp);
			{
				UI32 i; for(i = 0; i < posibs->childrnSz; i++){
					const STNBSintaxParserElemHintStartNode* chld = &posibs->childrn[i];
					//Empty
					NBString_empty(&tmp);
					//Add parent path
					if(!NBString_strIsEmpty(parentPath)){
						if(i == 0){
							//Add path
							NBString_concat(&tmp, parentPath);
							if(tmp.length > 0){
								NBString_concat(&tmp, ", ");
							}
						} else {
							//Add spaces
							NBString_concatRepeatedByte(&tmp, '-', NBString_strLenBytes(parentPath));
							if(tmp.length > 0){
								NBString_concat(&tmp, "|-");
							}
						}
					}
					//Add current child
					{
						const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, chld->idx.iElem);
						/*if(!chld->isLeaf){
							NBString_concat(&tmp, "(");
							NBString_concatUI32(&tmp, chld->idx.iDef);
							NBString_concat(&tmp, ", ");
							NBString_concatUI32(&tmp, chld->idx.iPart);
							NBString_concat(&tmp, ")");
						}*/
						if(ee2->type == ENSintaxParserElemType_Literal){
							NBString_concatByte(&tmp, '[');
							NBString_concat(&tmp, ee2->name);
							NBString_concatByte(&tmp, ']');
						} else {
							NBString_concat(&tmp, ee2->name);
						}
					}
					//Print
					if(chld->isLeaf){
						if((chld->resultMask & ENSintaxParserValidateElemResultBit_All) != 0){
							NBString_concat(&tmp, " (");
							if((chld->resultMask & (ENSintaxParserValidateElemResultBit_Partial & ENSintaxParserValidateElemResultBit_Complete)) != 0){
								NBString_concat(&tmp, "partial|complete");
							} else if((chld->resultMask & ENSintaxParserValidateElemResultBit_Partial) != 0){
								NBString_concat(&tmp, "partial");
							} else if((chld->resultMask & ENSintaxParserValidateElemResultBit_Complete) != 0){
								NBString_concat(&tmp, "complete");
							} else {
								NBASSERT(FALSE) //unexpected
							}
							NBString_concat(&tmp, ")");
						}
						if(dst != NULL){
							NBString_concat(dst, "//      ");
							NBString_concatBytes(dst, tmp.str, tmp.length);
							NBString_concat(dst, "\n");
						}
					}
					//Process child
					NBSintaxParserRules_concatHintStartPosibPaths(obj, chld, tmp.str, dst);
				}
			}
			NBString_release(&tmp);
		}
	}
}

BOOL NBSintaxParserRules_concatAsRulesInC(const STNBSintaxParserRules* obj, const char* prefix, STNBString* dst){
	BOOL r = FALSE;
	const STNBSintaxParserRulesParserState* parser = &obj->parser;
	//Print rules
	if(parser->completed){
		STNBString prevComment; //Comment for prev line
		NBString_init(&prevComment);
		//Enum of rules
		{
			NBString_empty(&prevComment);
			NBString_concat(dst, "typedef enum EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "Rule_ {");
			{
				UI32 addedCount = 0;
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					if(ee->type == ENSintaxParserElemType_Elem){
						{
							if(addedCount != 0) NBString_concat(dst, ",");
							if(prevComment.length != 0){
								NBString_concat(dst, " // ");
								NBString_concatBytes(dst, prevComment.str, prevComment.length);
							}
							NBString_concat(dst, "\n\t");
							NBString_empty(&prevComment);
						}
						NBString_concat(dst, "EN");
						NBString_concat(dst, prefix);
						NBString_concat(dst, "Rule_");
						{
							STNBString str;
							NBString_initWithStrBytes(&str, ee->name, ee->nameLen);
							NBString_replaceByte(&str, '-', '_');
							NBString_concat(dst, str.str);
							NBString_release(&str);
						}
						if(addedCount == 0){
							NBString_concat(dst, " = 0");
						}
						addedCount++;
					}
				}
				//Count
				{
					{
						if(addedCount != 0) NBString_concat(dst, ",");
						if(prevComment.length != 0){
							NBString_concat(dst, " // ");
							NBString_concatBytes(dst, prevComment.str, prevComment.length);
						}
						NBString_concat(dst, "\n\t");
						NBString_empty(&prevComment);
					}
					NBString_concat(dst, "//Count\n\t");
					NBString_concat(dst, "EN");
					NBString_concat(dst, prefix);
					NBString_concat(dst, "Rule_Count");
					if(addedCount == 0){
						NBString_concat(dst, " = 0");
					}
					addedCount++;
				}
			}
			NBString_concat(dst, "\n} EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "Rule;\n\n");
		}
		//Enum of rules elems
		{
			NBString_empty(&prevComment);
			NBString_concat(dst, "typedef enum EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "RulesElem_ {");
			{
				UI32 addedCount = 0;
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					{
						if(addedCount != 0) NBString_concat(dst, ",");
						if(prevComment.length != 0){
							NBString_concat(dst, " // ");
							NBString_concatBytes(dst, prevComment.str, prevComment.length);
						}
						NBString_concat(dst, "\n\t");
						NBString_empty(&prevComment);
					}
					NBString_concat(dst, "EN");
					NBString_concat(dst, prefix);
					NBString_concat(dst, "RulesElem_");
					NBString_concatUI32(dst, i);
					//
					switch (ee->type) {
						case ENSintaxParserElemType_Literal:
							NBString_concat(&prevComment, "[");
							NBString_concatBytes(&prevComment, ee->name, ee->nameLen);
							NBString_concat(&prevComment, "]");
							break;
						case ENSintaxParserElemType_Callback:
							NBString_concatBytes(&prevComment, ee->name, ee->nameLen);
							break;
						case ENSintaxParserElemType_Elem:
							NBString_concatBytes(&prevComment, ee->name, ee->nameLen);
							break;
						default:
							NBASSERT(FALSE)
							break;
					}
					if(addedCount == 0){
						NBString_concat(dst, " = 0");
					}
					//
					addedCount++;
				}
				//Count
				{
					{
						if(addedCount != 0) NBString_concat(dst, ",");
						if(prevComment.length != 0){
							NBString_concat(dst, " // ");
							NBString_concatBytes(dst, prevComment.str, prevComment.length);
						}
						NBString_concat(dst, "\n\t");
						NBString_empty(&prevComment);
					}
					//
					NBString_concat(dst, "//Count\n\t");
					NBString_concat(dst, "EN");
					NBString_concat(dst, prefix);
					NBString_concat(dst, "RulesElem_Count");
					if(addedCount == 0){
						NBString_concat(dst, " = 0");
					}
					addedCount++;
				}
			}
			NBString_concat(dst, "\n} EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "RulesElem;\n\n");
		}
		//List of rules names
		{
			NBString_empty(&prevComment);
			NBString_concat(dst, "const char* NB");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "RulesNames_[EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "Rule_Count");
			NBString_concat(dst, "] = {");
			{
				UI32 addedCount = 0;
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					if(ee->type == ENSintaxParserElemType_Elem){
						{
							if(addedCount != 0) NBString_concat(dst, ",");
							if(prevComment.length != 0){
								NBString_concat(dst, " // ");
								NBString_concatBytes(dst, prevComment.str, prevComment.length);
							}
							NBString_concat(dst, "\n\t");
							NBString_empty(&prevComment);
						}
						NBString_concat(dst, "\"");
						NBString_concatBytes(dst, ee->name, ee->nameLen);
						NBString_concat(dst, "\"");
						addedCount++;
					}
				}
			}
			{
				if(prevComment.length != 0){
					NBString_concat(dst, " // ");
					NBString_concatBytes(dst, prevComment.str, prevComment.length);
				}
				NBString_concat(dst, "\n");
				NBString_empty(&prevComment);
			}
			NBString_concat(dst, "};\n\n");
		}
		//List of rulesElems's rules
		{
			NBString_empty(&prevComment);
			NBString_concat(dst, "const EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "Rule NB");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "RulesElemsRules_[EN");
			NBString_concat(dst, prefix);
			NBString_concat(dst, "RulesElem_Count");
			NBString_concat(dst, "] = {");
			{
				UI32 addedCount = 0;
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					{
						if(addedCount != 0) NBString_concat(dst, ",");
						if(prevComment.length != 0){
							NBString_concat(dst, " // ");
							NBString_concatBytes(dst, prevComment.str, prevComment.length);
						}
						NBString_concat(dst, "\n\t");
						NBString_empty(&prevComment);
					}
					NBString_concat(dst, "EN");
					NBString_concat(dst, prefix);
					NBString_concat(dst, "Rule_");
					if(ee->type != ENSintaxParserElemType_Elem){
						NBString_concat(dst, "Count");
						switch (ee->type) {
							case ENSintaxParserElemType_Literal:
								NBString_concat(&prevComment, "[");
								NBString_concatBytes(&prevComment, ee->name, ee->nameLen);
								NBString_concat(&prevComment, "]");
								break;
							case ENSintaxParserElemType_Callback:
								NBString_concatBytes(&prevComment, ee->name, ee->nameLen);
								break;
							default:
								NBASSERT(FALSE)
								break;
						}
					} else {
						STNBString str;
						NBString_initWithStrBytes(&str, ee->name, ee->nameLen);
						NBString_replaceByte(&str, '-', '_');
						NBString_concat(dst, str.str);
						NBString_release(&str);
					}
					addedCount++;
				}
			}
			{
				if(prevComment.length != 0){
					NBString_concat(dst, " // ");
					NBString_concatBytes(dst, prevComment.str, prevComment.length);
				}
				NBString_concat(dst, "\n");
				NBString_empty(&prevComment);
			}
			NBString_concat(dst, "};\n\n");
		}
		NBString_release(&prevComment);
		//Result
		r = TRUE;
	}
	return r;
}

BOOL NBSintaxParserRules_concat(const STNBSintaxParserRules* obj, STNBString* dst){
	BOOL r = FALSE;
	const STNBSintaxParserRulesParserState* parser = &obj->parser;
	//Print rules
	if(parser->completed){
		if(obj->elems.use > 0){
			STNBArraySorted* refsTo = NBMemory_allocTypes(STNBArraySorted, obj->elems.use);
			STNBArraySorted* refsBy = NBMemory_allocTypes(STNBArraySorted, obj->elems.use);
			//Init arrays
			{
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					NBArraySorted_init(&refsTo[i], sizeof(UI32), NBCompareUI32);
					NBArraySorted_init(&refsBy[i], sizeof(UI32), NBCompareUI32);
				}
			}
			//Build cross references
			{
				UI32 iBy; for(iBy = 0; iBy < obj->elems.use; iBy++){
					STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, iBy); 
					if(ee->type == ENSintaxParserElemType_Elem){
						if(ee->defs != NULL && ee->defsSz > 0){
							UI32 i; for(i = 0; i < ee->defsSz; i++){
								STNBSintaxParserElemDef* dd = &ee->defs[i];
								if(dd->parts != NULL && dd->partsSz > 0){
									UI32 i; for(i = 0; i < dd->partsSz; i++){
										STNBSintaxParserElemDefPart* pp = &dd->parts[i];
										NBASSERT(pp->iElem < obj->elems.use)
										if(pp->iElem < obj->elems.use){
											const UI32 iTo = pp->iElem; 
											//Add refTo
											if(NBArraySorted_indexOf(&refsTo[iBy], &iTo, sizeof(iTo), NULL) < 0){
												NBArraySorted_addValue(&refsTo[iBy], iTo);
											}
											//Add refFrom
											if(NBArraySorted_indexOf(&refsBy[pp->iElem], &iBy, sizeof(iBy), NULL) < 0){
												NBArraySorted_addValue(&refsBy[pp->iElem], iBy);
											}
										}
									}
								}
							}
						}
					}
				}
			}
			//Concat
			{
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
					if(ee->type == ENSintaxParserElemType_Elem /*|| ee->type == ENSintaxParserElemType_Literal*/){
						if(ee->type == ENSintaxParserElemType_Literal){
							NBString_concat(dst, "//");
						}
						NBString_concat(dst, ee->name);
						NBString_concat(dst, ":");
						NBString_concat(dst, " //");
						NBString_concat(dst, "refBy("); NBString_concatSI32(dst, refsBy[i].use); NBString_concat(dst, ")");
						NBString_concat(dst, " refTo("); NBString_concatSI32(dst, refsTo[i].use); NBString_concat(dst, ")");
						NBString_concat(dst, "\n");
						if(ee->defs != NULL && ee->defsSz > 0){
							BOOL countPosibs = 0;
							UI32 i; for(i = 0; i < ee->defsSz; i++){
								const STNBSintaxParserElemDef* dd = &ee->defs[i];
								if(dd->parts != NULL && dd->partsSz > 0){
									//if(countPosibs == 0){
									//	NBString_concat(dst, "//Posibs\n");
									//}
									{
										UI32 i; for(i = 0; i < dd->partsSz; i++){
											const STNBSintaxParserElemDefPart* pp = &dd->parts[i];
											//space
											if(i != 0){
												NBString_concat(dst, " ");
											}
											//value
											NBASSERT(pp->iElem < obj->elems.use)
											if(pp->iElem < obj->elems.use){
												const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, pp->iElem);
												if(ee->type == ENSintaxParserElemType_Literal){
													NBString_concat(dst, "//");
												}
												if(ee2->type == ENSintaxParserElemType_Literal){
													NBString_concatByte(dst, '[');
													NBString_concat(dst, ee2->name);
													NBString_concatByte(dst, ']');
												} else {
													NBString_concat(dst, ee2->name);
												}
												if(pp->isOptional){
													NBString_concat(dst, "(opt)");
												}
											}
										}
									}
									NBString_concat(dst, "\n");
									countPosibs++;
								}
							}
						}
						//Hints
						{
							BOOL hintsHeaderPrinted = FALSE;
							STNBString str;
							NBString_init(&str);
							//Can be empty
							{
								if(ee->hints.canBeEmpty){
									if(!hintsHeaderPrinted){
										NBString_concat(dst, "//Hints\n");
										hintsHeaderPrinted = TRUE;
									}
									NBString_concat(dst, "//  Can be empty\n");
								}
								if(ee->hints.canBeRecursive){
									if(!hintsHeaderPrinted){
										NBString_concat(dst, "//Hints\n");
										hintsHeaderPrinted = TRUE;
									}
									NBString_concat(dst, "//  Can be recursive\n");
								}
							}
							//Starts
							{
								SI32 iType; for(iType = 0; iType < ENSintaxParserElemType_Count; iType++){
									const STNBArraySorted* arr = &ee->hints.starts[iType];
									UI32 i; for(i = 0; i < arr->use; i++){
										const STNBSintaxParserElemHintStart* start = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, i);
										NBASSERT(start->type == iType)
										const STNBSintaxParserElemHintStartNode* posibs = &start->posibs;
										if(posibs->childrn != NULL && posibs->childrnSz > 0){
											STNBString tmp;
											NBString_initWithSz(&tmp, 256, 1024, 0.10f);
											{
												UI32 i; for(i = 0; i < posibs->childrnSz; i++){
													const STNBSintaxParserElemHintStartNode* chld = &posibs->childrn[i];
													NBString_empty(&tmp);
													NBSintaxParserRules_concatHintStartPosibPaths(obj, chld, NULL, &tmp);
													if(tmp.length > 0){
														if(!hintsHeaderPrinted){
															NBString_concat(dst, "//Hints\n");
															hintsHeaderPrinted = TRUE;
														}
														NBString_concat(dst, "//  Starts with ");
														{
															switch(start->type) {
																case ENSintaxParserElemType_Literal:
																	NBString_concat(dst, "'");
																	NBString_concat(dst, start->utf.val);
																	NBString_concat(dst, "'");
																	break;
																case ENSintaxParserElemType_Callback:
																	NBASSERT(start->iElem >= 0 && start->iElem < obj->elems.use)
																	{
																		const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, start->iElem);
																		NBString_concat(dst, "(callback)(");
																		NBString_concat(dst, ee2->name);
																		NBString_concat(dst, ")");
																	}
																	break;
																case ENSintaxParserElemType_Elem:
																	NBASSERT(start->iElem >= 0 && start->iElem < obj->elems.use)
																	{
																		const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, start->iElem);
																		switch(ee2->type) {
																			case ENSintaxParserElemType_Literal:
																				NBString_concatByte(dst, '[');
																				NBString_concat(dst, ee2->name);
																				NBString_concatByte(dst, ']');
																				break;
																			case ENSintaxParserElemType_Callback:
																				NBASSERT(FALSE)
																				break;
																			case ENSintaxParserElemType_Elem:
																				NBString_concat(dst, ee2->name);
																				break;
																			default:
																				NBASSERT(FALSE)
																				break;
																		}
																	}
																	break;
																default:
																	NBASSERT(FALSE)
																	break;
															}
														}
														//NBString_concat(dst, "\n");
														NBString_concat(dst, " when:\n");
														NBString_concatBytes(dst, tmp.str, tmp.length);
													}
												}
											}
											NBString_release(&tmp);
										}
									}
								}
							}
							NBString_release(&str);
						}
						NBString_concat(dst, "\n");
					}
				}
			}
			//Release arays
			{
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					NBArraySorted_empty(&refsTo[i]);
					NBArraySorted_release(&refsTo[i]);
					//
					NBArraySorted_empty(&refsBy[i]);
					NBArraySorted_release(&refsBy[i]);
				}
				NBMemory_free(refsTo); refsTo = NULL;
				NBMemory_free(refsBy); refsBy = NULL;
			}
		}
		r = TRUE;
	}
	return r;
}
	
BOOL NBSintaxParserRules_concatByRefs(const STNBSintaxParserRules* obj, STNBString* dst){
	BOOL r = FALSE;
	const STNBSintaxParserRulesParserState* parser = &obj->parser;
	//Print rules
	if(parser->completed){
		if(obj->elems.use > 0){
			STNBArraySorted* refsTo = NBMemory_allocTypes(STNBArraySorted, obj->elems.use);
			STNBArraySorted* refsBy = NBMemory_allocTypes(STNBArraySorted, obj->elems.use);
			//Init arrays
			{
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					NBArraySorted_init(&refsTo[i], sizeof(UI32), NBCompareUI32);
					NBArraySorted_init(&refsBy[i], sizeof(UI32), NBCompareUI32);
				}
			}
			//Build cross references
			{
				UI32 iBy; for(iBy = 0; iBy < obj->elems.use; iBy++){
					STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, iBy); 
					if(ee->type == ENSintaxParserElemType_Elem){
						if(ee->defs != NULL && ee->defsSz > 0){
							UI32 i; for(i = 0; i < ee->defsSz; i++){
								STNBSintaxParserElemDef* dd = &ee->defs[i];
								if(dd->parts != NULL && dd->partsSz > 0){
									UI32 i; for(i = 0; i < dd->partsSz; i++){
										STNBSintaxParserElemDefPart* pp = &dd->parts[i];
										NBASSERT(pp->iElem < obj->elems.use)
										if(pp->iElem < obj->elems.use){
											const UI32 iTo = pp->iElem; 
											//Add refTo
											if(NBArraySorted_indexOf(&refsTo[iBy], &iTo, sizeof(iTo), NULL) < 0){
												NBArraySorted_addValue(&refsTo[iBy], iTo);
											}
											//Add refFrom
											if(NBArraySorted_indexOf(&refsBy[pp->iElem], &iBy, sizeof(iBy), NULL) < 0){
												NBArraySorted_addValue(&refsBy[pp->iElem], iBy);
											}
										}
									}
								}
							}
						}
					}
				}
			}
			//Concat
			{
				SI32 minRefsBy = 0, maxRefsBy = 0;
				//Get the max and min ref numbers
				{
					UI32 countFnd = 0;
					UI32 i; for(i = 1; i < obj->elems.use; i++){
						const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
						if(ee->type == ENSintaxParserElemType_Elem){
							if(countFnd == 0){
								minRefsBy = maxRefsBy = refsBy[i].use;
							} else {
								if(minRefsBy > refsBy[i].use) minRefsBy = refsBy[i].use;
								if(maxRefsBy < refsBy[i].use) maxRefsBy = refsBy[i].use;
							}
							countFnd++;
						}
					}
				}
				//Print in order
				{
					SI32 iRefs; for(iRefs = minRefsBy; iRefs <= maxRefsBy; iRefs++){
						UI32 i; for(i = 0; i < obj->elems.use; i++){
							const STNBSintaxParserElem* ee = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, i);
							if(ee->type == ENSintaxParserElemType_Elem /*|| ee->type == ENSintaxParserElemType_Literal*/){
								if(iRefs == refsBy[i].use){
									if(ee->type == ENSintaxParserElemType_Literal){
										NBString_concat(dst, "//");
									}
									NBString_concat(dst, ee->name);
									NBString_concat(dst, ":");
									NBString_concat(dst, " //");
									NBString_concat(dst, "refBy("); NBString_concatSI32(dst, refsBy[i].use); NBString_concat(dst, ")");
									NBString_concat(dst, " refTo("); NBString_concatSI32(dst, refsTo[i].use); NBString_concat(dst, ")");
									NBString_concat(dst, "\n");
									if(ee->defs != NULL && ee->defsSz > 0){
										UI32 countPosibs = 0;
										UI32 i; for(i = 0; i < ee->defsSz; i++){
											const STNBSintaxParserElemDef* dd = &ee->defs[i];
											if(dd->parts != NULL && dd->partsSz > 0){
												//if(countPosibs == 0){
												//	NBString_concat(dst, "//Posibs\n");
												//}
												{
													UI32 i; for(i = 0; i < dd->partsSz; i++){
														const STNBSintaxParserElemDefPart* pp = &dd->parts[i];
														//space
														if(i != 0){
															NBString_concat(dst, " ");
														}
														//value
														NBASSERT(pp->iElem < obj->elems.use)
														if(pp->iElem < obj->elems.use){
															const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, pp->iElem);
															if(ee->type == ENSintaxParserElemType_Literal){
																NBString_concat(dst, "//");
															}
															if(ee2->type == ENSintaxParserElemType_Literal){
																NBString_concatByte(dst, '[');
																NBString_concat(dst, ee2->name);
																NBString_concatByte(dst, ']');
															} else {
																NBString_concat(dst, ee2->name);
															}
															if(pp->isOptional){
																NBString_concat(dst, "(opt)");
															}
														}
													}
												}
												NBString_concat(dst, "\n");
												countPosibs++;
											}
										}
									}
									//Hints
									{
										BOOL hintsHeaderPrinted = FALSE;
										STNBString str;
										NBString_init(&str);
										//Can be empty
										{
											if(ee->hints.canBeEmpty){
												if(!hintsHeaderPrinted){
													NBString_concat(dst, "//Hints\n");
													hintsHeaderPrinted = TRUE;
												}
												NBString_concat(dst, "//  Can be empty\n");
											}
											if(ee->hints.canBeRecursive){
												if(!hintsHeaderPrinted){
													NBString_concat(dst, "//Hints\n");
													hintsHeaderPrinted = TRUE;
												}
												NBString_concat(dst, "//  Can be recursive\n");
											}
										}
										//Starts
										{
											SI32 iType; for(iType = 0; iType < ENSintaxParserElemType_Count; iType++){
												const STNBArraySorted* arr = &ee->hints.starts[iType];
												UI32 i; for(i = 0; i < arr->use; i++){
													const STNBSintaxParserElemHintStart* start = NBArraySorted_itmPtrAtIndex(arr, STNBSintaxParserElemHintStart, i);
													NBASSERT(start->type == iType)
													const STNBSintaxParserElemHintStartNode* posibs = &start->posibs;
													if(posibs->childrn != NULL && posibs->childrnSz > 0){
														STNBString tmp;
														NBString_initWithSz(&tmp, 256, 1024, 0.10f);
														{
															UI32 i; for(i = 0; i < posibs->childrnSz; i++){
																const STNBSintaxParserElemHintStartNode* chld = &posibs->childrn[i];
																NBString_empty(&tmp);
																NBSintaxParserRules_concatHintStartPosibPaths(obj, chld, NULL, &tmp);
																if(tmp.length > 0){
																	if(!hintsHeaderPrinted){
																		NBString_concat(dst, "//Hints\n");
																		hintsHeaderPrinted = TRUE;
																	}
																	NBString_concat(dst, "//  Starts with ");
																	{
																		switch(start->type) {
																		case ENSintaxParserElemType_Literal:
																			NBString_concat(dst, "'");
																			NBString_concat(dst, start->utf.val);
																			NBString_concat(dst, "'");
																			break;
																		case ENSintaxParserElemType_Callback:
																			NBASSERT(start->iElem >= 0 && start->iElem < obj->elems.use)
																			{
																				const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, start->iElem);
																				NBString_concat(dst, "(callback)(");
																				NBString_concat(dst, ee2->name);
																				NBString_concat(dst, ")");
																			}
																			break;
																		case ENSintaxParserElemType_Elem:
																			NBASSERT(start->iElem >= 0 && start->iElem < obj->elems.use)
																			{
																				const STNBSintaxParserElem* ee2 = NBArray_itmPtrAtIndex(&obj->elems, STNBSintaxParserElem, start->iElem);
																				switch(ee2->type) {
																					case ENSintaxParserElemType_Literal:
																						NBString_concatByte(dst, '[');
																						NBString_concat(dst, ee2->name);
																						NBString_concatByte(dst, ']');
																						break;
																					case ENSintaxParserElemType_Callback:
																						NBASSERT(FALSE)
																						break;
																					case ENSintaxParserElemType_Elem:
																						NBString_concat(dst, ee2->name);
																						break;
																					default:
																						NBASSERT(FALSE)
																						break;
																				}
																			}
																			break;
																		default:
																			NBASSERT(FALSE)
																			break;
																		}
																	}
																	//NBString_concat(dst, "\n");
																	NBString_concat(dst, " when:\n");
																	NBString_concatBytes(dst, tmp.str, tmp.length);
																}
															}
														}
														NBString_release(&tmp);
													}
												}
											}
										}
										NBString_release(&str);
									}
									//
									NBString_concat(dst, "\n");
								}
							}
						}
					}
				}
			}
			//Release arays
			{
				UI32 i; for(i = 0; i < obj->elems.use; i++){
					NBArraySorted_empty(&refsTo[i]);
					NBArraySorted_release(&refsTo[i]);
					//
					NBArraySorted_empty(&refsBy[i]);
					NBArraySorted_release(&refsBy[i]);
				}
				NBMemory_free(refsTo); refsTo = NULL;
				NBMemory_free(refsBy); refsBy = NULL;
			}
		}
		r = TRUE;
	}
	return r;
}
