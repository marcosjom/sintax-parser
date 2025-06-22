#include "nb/NBFrameworkPch.h"
#include "nb/core/NBSintaxParserResults.h"
//
#include "nb/core/NBMemory.h"
#include "nb/core/NBArray.h"
//

void NBSintaxParserResults_nodeRelease(STNBSintaxParserResultNode* node);
void NBSintaxParserResults_nodeConcatChildren(const char colChar, const STNBSintaxParserResultNode* parent, const char* curPath, const UI32 curPathSz, const char* childPath, const UI32 childPathSz, const BOOL includeRanges, const BOOL includePartIdxs, const char* strAccum, STNBString* dst);

//

void NBSintaxParserResults_init(STNBSintaxParserResults* obj){
	NBMemory_setZeroSt(*obj, STNBSintaxParserResults);
}

void NBSintaxParserResults_release(STNBSintaxParserResults* obj){
	if(obj->results != NULL){
		UI32 i; for(i = 0; i < obj->resultsSz; i++){
			STNBSintaxParserResultNode* nn = &obj->results[i];
			NBSintaxParserResults_nodeRelease(nn);
		}
		//Free
		NBMemory_free(obj->results);
		obj->results = NULL;
	}
	obj->resultsSz = 0;
}

//Node

BOOL NBSintaxParserResults_nodeClone(const STNBSintaxParserResultNode* obj, STNBSintaxParserResultNode* dst, const BOOL ignoreNames){
	BOOL r = FALSE;
	if(dst != NULL){
		r				= TRUE;
		//Set values
		dst->type		= obj->type;
		dst->iElem		= obj->iElem;
		dst->iByteStart	= obj->iByteStart;
		dst->iByteAfter	= obj->iByteAfter;
		//Clone name
		if(obj->name != NULL && !ignoreNames){
			dst->name = NBString_strNewBuffer(obj->name);
		} else {
			dst->name	= NULL;
		}
		//Clone parts
		if(obj->parts != NULL && obj->partsSz > 0){
			dst->parts		= NBMemory_allocTypes(STNBSintaxParserResultNode, obj->partsSz);
			dst->partsSz	= obj->partsSz;
			{
				UI32 i; for(i = 0; i < obj->partsSz; i++){
					if(!NBSintaxParserResults_nodeClone(&obj->parts[i], &dst->parts[i], ignoreNames)){
						r = FALSE;
						break;
					}
				}
			}
		} else {
			dst->parts		= NULL;
			dst->partsSz	= 0;
		}
	}
	return r;
}

UI32 NBSintaxParserResults_nodeCountPartsForFlattenRecursivity(const STNBSintaxParserResultNode* obj, const UI32 iElemRecursive){
	UI32 r = 0;
	if(obj->parts != NULL && obj->partsSz > 0){
		UI32 i = 0; const STNBSintaxParserResultNode* part = &obj->parts[i];
		if(part->iElem == iElemRecursive){
			r += NBSintaxParserResults_nodeCountPartsForFlattenRecursivity(part, iElemRecursive);
			i++;
		}
		r += (obj->partsSz - i);
	}
	return r;
}

UI32 NBSintaxParserResults_nodeClonePartsForFlattenRecursivity(const STNBSintaxParserResultNode* obj, const UI32 iElemRecursive, const BOOL ignoreNames, STNBSintaxParserResultNode* dst, const UI32 dstSize){
	UI32 r = 0;
	if(obj->parts != NULL && obj->partsSz > 0){
		UI32 i = 0;
		//Clone recursivity part
		const STNBSintaxParserResultNode* part = &obj->parts[i];
		if(part->iElem == iElemRecursive){
			r += NBSintaxParserResults_nodeClonePartsForFlattenRecursivity(part, iElemRecursive, ignoreNames, dst, dstSize);
			dst += r; i++;
			NBASSERT(r <= dstSize)
		}
		//Clone other parts
		for(; i < obj->partsSz; i++){
			const STNBSintaxParserResultNode* part = &obj->parts[i];
			if(!NBSintaxParserResults_nodeCloneFlattenRecursivity(part, dst, ignoreNames)){
				break;
			} else {
				dst++; r++;
				NBASSERT(r <= dstSize)
			}
		}
	}
	return r;
}
	
BOOL NBSintaxParserResults_nodeCloneFlattenRecursivity(const STNBSintaxParserResultNode* obj, STNBSintaxParserResultNode* dst, const BOOL ignoreNames){
	BOOL r = FALSE;
	if(dst != NULL){
		r				= TRUE;
		//Set values
		dst->type		= obj->type;
		dst->iElem		= obj->iElem;
		dst->iByteStart	= obj->iByteStart;
		dst->iByteAfter	= obj->iByteAfter;
		//Clone name
		if(obj->name != NULL && !ignoreNames){
			dst->name = NBString_strNewBuffer(obj->name);
		} else {
			dst->name	= NULL;
		}
		//Count parts
		{
			const UI32 partsSz = NBSintaxParserResults_nodeCountPartsForFlattenRecursivity(obj, obj->iElem);
			if(partsSz == 0){
				dst->parts		= NULL;
				dst->partsSz	= 0;
			} else {
				dst->parts		= NBMemory_allocTypes(STNBSintaxParserResultNode, partsSz);
				dst->partsSz	= NBSintaxParserResults_nodeClonePartsForFlattenRecursivity(obj, obj->iElem, ignoreNames, dst->parts, partsSz);
				NBASSERT(dst->partsSz == partsSz)
			}
		}
	}
	return r;
}

void NBSintaxParserResults_nodeRelease(STNBSintaxParserResultNode* obj){
	if(obj->name != NULL) NBMemory_free(obj->name); obj->name = NULL;
	if(obj->parts != NULL){
		UI32 i; for(i = 0; i < obj->partsSz; i++){
			STNBSintaxParserResultNode* nn = &obj->parts[i];
			NBSintaxParserResults_nodeRelease(nn);
		}
		//Free
		NBMemory_free(obj->parts);
		obj->parts = NULL;
	}
	obj->partsSz = 0;
}

//Count of leaves

UI32 NBSintaxParserResults_nodeChildLeveasCount_(const STNBSintaxParserResultNode* node){
	UI32 r = 0;
	NBASSERT(node != NULL)
	NBASSERT(node->parts != NULL && node->partsSz > 0) //Avoid unnecesary calls
	//Process children
	if(node->parts != NULL && node->partsSz > 0){
		UI32 i; for(i = 0; i < node->partsSz; i++){
			const STNBSintaxParserResultNode* child = &node->parts[i];
			if(child->parts != NULL && child->partsSz > 0){
				//Child is a parent
				r += NBSintaxParserResults_nodeChildLeveasCount_(child);
			} else {
				//Child is a leaf
				r++;
			}
		}
	}
	return r;
}

UI32 NBSintaxParserResults_nodeChildLeveasCount(const STNBSintaxParserResultNode* node){
	UI32 r = 0;
	if(node->parts != NULL && node->partsSz > 0){
		//Node is a parent
		r += NBSintaxParserResults_nodeChildLeveasCount_(node);
	} else {
		r++;
	}
	return r;
}

//Count by elem

UI32 NBSintaxParserResults_nodeChildCountByElemIdx_(const STNBSintaxParserResultNode* node, const UI32 iElem){
	UI32 r = 0;
	NBASSERT(node != NULL)
	NBASSERT(node->parts != NULL && node->partsSz > 0) //avoid inecesary calls
	//Process children
	if(node->parts != NULL && node->partsSz > 0){
		UI32 i; for(i = 0; i < node->partsSz; i++){
			const STNBSintaxParserResultNode* child = &node->parts[i];
			//Compare child
			if(child->iElem == iElem){
				r++;
			}
			//Compare children
			if(child->parts != NULL && child->partsSz > 0){
				//Child is a parent
				r += NBSintaxParserResults_nodeChildCountByElemIdx_(child, iElem);
			}
		}
	}
	return r;
}

UI32 NBSintaxParserResults_nodeChildCountByElemIdx(const STNBSintaxParserResultNode* node, const UI32 iElem){
	UI32 r = 0;
	//Compare root
	if(node->iElem == iElem){
		r++;
	}
	//Compare children
	if(node->parts != NULL && node->partsSz > 0){
		//Node is a parent
		r += NBSintaxParserResults_nodeChildCountByElemIdx_(node, iElem);
	}
	return r;
}

//Find leaf by elem and value

SI32 NBSintaxParserResults_nodeChildElemIdxByValue_(const STNBSintaxParserResultNode* node, const char* strAccum, const UI32 strAccumSz, const UI32 iElem, const char* val, const UI32 valSz, UI32* iElemRef){
	SI32 r = -1;
	NBASSERT(node != NULL)
	NBASSERT(iElemRef != NULL)
	NBASSERT((strAccum == NULL && strAccumSz <= 0) || (strAccum != NULL && strAccumSz > 0))
	NBASSERT(node->parts != NULL && node->partsSz > 0) //Avoid unecesary calls
	NBASSERT(node->iElem != iElem) //Avoid unecesary calls
	//Process children
	if(node->parts != NULL && node->partsSz > 0){
		UI32 i; for(i = 0; i < node->partsSz && r == -1; i++){
			const STNBSintaxParserResultNode* child = &node->parts[i];
			//Analyze child
			if(child->iElem == iElem){
				const char* val2	= &strAccum[child->iByteStart];
				const UI32 valSz2	= (child->iByteAfter - child->iByteStart);
				if(NBString_strIsEqualBytes(val, valSz, val2, valSz2)){
					r = (SI32)(*iElemRef);
					break;
				}
				(*iElemRef)++;
			} else if(child->parts != NULL && child->partsSz > 0){
				//Analyze children
				r = NBSintaxParserResults_nodeChildElemIdxByValue_(child, strAccum, strAccumSz, iElem, val, valSz, iElemRef);
			}	
		}
	}
	return r;
}
	
SI32 NBSintaxParserResults_nodeChildElemIdxByValue(const STNBSintaxParserResultNode* node, const char* strAccum, const UI32 strAccumSz, const UI32 iElem, const char* val, const UI32 valSz){
	SI32 r = -1;
	//Analyze parent
	if(node->iElem == iElem){
		const char* val2	= &strAccum[node->iByteStart];
		const UI32 valSz2	= (node->iByteAfter - node->iByteStart);
		if(NBString_strIsEqualBytes(val, valSz, val2, valSz2)){
			r = 0;
		}
	} else if(node->parts != NULL && node->partsSz > 0){
		//Analyze children
		UI32 iElemCount = 0;
		r = NBSintaxParserResults_nodeChildElemIdxByValue_(node, strAccum, strAccumSz, iElem, val, valSz, &iElemCount);
	}
	return r;
}

//---------
//- Concat
//---------

void NBSintaxParserResults_concat(const STNBSintaxParserResults* src, const char* strAccum, const BOOL includeTree, STNBString* dst){
	if(src->results != NULL && src->resultsSz > 0 && dst != NULL){
		const BOOL includeRanges = TRUE;
		const BOOL includePartIdxs = FALSE;
		UI32 i; for(i = 0; i < src->resultsSz; i++){
			const STNBSintaxParserResultNode* nn = &src->results[i];
			//iElem
			if(includePartIdxs){
				NBString_concat(dst, "(");
				NBString_concatUI32(dst, nn->iElem);
				NBString_concat(dst, ")");
			}
			//name
			if(nn->type == ENSintaxParserElemType_Elem){
				if(!NBString_strIsEmpty(nn->name)){
					NBString_concat(dst, nn->name);
				}
			} else {
				if(!NBString_strIsEmpty(strAccum)){
					NBString_concatByte(dst, '[');
					NBString_concatBytes(dst, &strAccum[nn->iByteStart], (nn->iByteAfter - nn->iByteStart));
					NBString_concatByte(dst, ']');
				}
			}
			//Range
			if(includeRanges){
				NBString_concat(dst, "(");
				NBString_concatUI32(dst, nn->iByteStart);
				NBString_concat(dst, ",+");
				NBString_concatUI32(dst, (nn->iByteAfter - nn->iByteStart));
				NBString_concat(dst, ")");
			}
			//Value
			if(!NBString_strIsEmpty(strAccum) && nn->iByteStart < nn->iByteAfter){
				NBString_concat(dst, ": ");
				{
					UI32 i; for(i = nn->iByteStart; i < nn->iByteAfter; i++){
						const char c = strAccum[i];
						switch(c) {
							case '\t': NBString_concatBytes(dst, "\\t", 2); break;
							case '\r': NBString_concatBytes(dst, "\\r", 2); break;
							case '\n': NBString_concatBytes(dst, "\\n", 2); break;
							default: NBString_concatByte(dst, c); break;
						}
					}
				}
			}
			//
			NBString_concat(dst, "\n");
			// d
			if(includeTree){
				const char leftPathChar		= '|';
				const char leftChildChar	= (i == (src->resultsSz - 1) ? ' ' : leftPathChar);
				NBSintaxParserResults_nodeConcatChildren('|', nn, &leftPathChar, sizeof(leftPathChar), &leftChildChar, sizeof(leftChildChar), includeRanges, includePartIdxs, strAccum, dst);
			}
		}
	}
}

void NBSintaxParserResults_nodeConcatChildren(const char colChar, const STNBSintaxParserResultNode* parent, const char* curPath, const UI32 curPathSz, const char* childPath, const UI32 childPathSz, const BOOL includeRanges, const BOOL includePartIdxs, const char* strAccum, STNBString* dst){
	if(parent != NULL){
		NBASSERT(parent->iByteStart <= parent->iByteAfter)
		if(parent->parts != NULL && parent->partsSz > 0){
			STNBString myPath, myChildPath, myChildPath2;
			NBString_init(&myPath);
			NBString_init(&myChildPath);
			NBString_init(&myChildPath2);
			{
				STNBArray subStrs;
				NBArray_init(&subStrs, sizeof(STNBString), NULL);
				{
					if(!NBString_strIsEmpty(curPath) && curPathSz > 0){
						NBString_setBytes(&myPath, curPath, curPathSz);
					}
					if(!NBString_strIsEmpty(childPath) && childPathSz > 0){
						NBString_concatBytes(&myChildPath, childPath, childPathSz);
					}
					{
						UI32 i; for(i = 0; i < parent->partsSz; i++){
							const STNBSintaxParserResultNode* nn = &parent->parts[i];
							const BOOL hasChildren = (nn->parts != NULL && nn->partsSz > 0);
							NBASSERT(nn->iByteStart <= nn->iByteAfter)
							NBASSERT(parent->iByteStart <= nn->iByteStart)
							NBASSERT(nn->iByteAfter <= parent->iByteAfter)
							//
							NBString_concat(&myPath, (i != 0 ? ", " : (myPath.length > 0 ? "-" : "")));
							//
							NBString_concatRepeatedByte(&myChildPath, ' ', (myPath.length - myChildPath.length));
							NBString_setBytes(&myChildPath2, myChildPath.str, myChildPath.length);
							NBString_concatByte(&myChildPath, (hasChildren ? colChar : ' '));
							NBString_concatByte(&myChildPath2, ' ');
							{
								//iElem
								if(includePartIdxs){
									NBString_concat(&myPath, "(");
									NBString_concatUI32(&myPath, nn->iElem);
									NBString_concat(&myPath, ")");
								}
								//name
								if(nn->type == ENSintaxParserElemType_Elem){
									if(!NBString_strIsEmpty(nn->name)){
										NBString_concat(&myPath, nn->name);
									}
								} else {
									if(!NBString_strIsEmpty(strAccum)){
										NBString_concatByte(&myPath, '[');
										{
											UI32 i; for(i = nn->iByteStart; i < nn->iByteAfter; i++){
												const char c = strAccum[i];
												switch(c) {
													case '\t': NBString_concatBytes(&myPath, "\\t", 2); break;
													case '\r': NBString_concatBytes(&myPath, "\\r", 2); break;
													case '\n': NBString_concatBytes(&myPath, "\\n", 2); break;
													default: NBString_concatByte(&myPath, c); break;
												}
											}
										}
										NBString_concatByte(&myPath, ']');
									}
								}
								//range
								if(!hasChildren){
									const BOOL showContent = (nn->iByteStart < nn->iByteAfter && nn->type == ENSintaxParserElemType_Elem); 
									if(showContent || includeRanges){
										NBString_concat(&myPath, "(");
										if(includeRanges){
											NBString_concatUI32(&myPath, nn->iByteStart);
											NBString_concat(&myPath, ",+");
											NBString_concatUI32(&myPath, (nn->iByteAfter - nn->iByteStart));
										}
										if(showContent){
											if(includeRanges){
												NBString_concat(&myPath, ",");
											}
											NBString_concat(&myPath, "'");
											NBString_concatBytes(&myPath, &strAccum[nn->iByteStart], (nn->iByteAfter - nn->iByteStart));
											NBString_concat(&myPath, "'");
										}
										NBString_concat(&myPath, ")");
									}
								}
								//Add children
								if(nn->parts != NULL && nn->partsSz > 0){
									STNBString subStr;
									NBString_init(&subStr);
									//Add substring
									NBSintaxParserResults_nodeConcatChildren(colChar, nn, myChildPath.str, myChildPath.length, myChildPath2.str, myChildPath2.length, includeRanges, includePartIdxs, strAccum, &subStr);
									NBArray_addValue(&subStrs, subStr);
								}
							}
						}
					}
					//Add my path
					if(dst != NULL){
						NBString_concatBytes(dst, myPath.str, myPath.length);
						NBString_concatByte(dst, '\n');
					}
					//Add sub paths
					{
						SI32 i; for(i = ((SI32)subStrs.use - 1); i >= 0; i--){
							STNBString* str = NBArray_itmPtrAtIndex(&subStrs, STNBString, i);
							NBString_concatBytes(dst, str->str, str->length);
						}
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
				NBArray_release(&subStrs);
			}
			NBString_release(&myPath);
			NBString_release(&myChildPath);
			NBString_release(&myChildPath2);
		}
	}
}
