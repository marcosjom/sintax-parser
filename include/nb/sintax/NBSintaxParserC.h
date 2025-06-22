#ifndef NB_SINTAX_PARSER_C_H
#define NB_SINTAX_PARSER_C_H

#include "nb/NBFrameworkDefs.h"
//
#include "nb/core/NBArray.h"
#include "nb/core/NBArraySorted.h"
#include "nb/core/NBSintaxParser.h"
//
#include "nb/core/NBSintaxParserCTokens.h"
#include "nb/core/NBSintaxParserCDefs.h"
#include "nb/core/NBSintaxParserCPProc.h"

#ifdef __cplusplus
extern "C" {
#endif

//--------
//- Parser
//--------

//Phase 1: characters map and trigraph sequances replacements.
typedef struct STNBSintaxParserCPhase1_ {
	//Trigraph sequences
	//??= #
	//??( [
	//??/ \
	//??) ]
	//??' ^
	//??< {
	//??! |
	//??> }
	//??- ~
	struct {
		UI32 partsAccum;	//'?' accumulated
	} trigraph;
} STNBSintaxParserCPhase1;

//Phase 2: remove '\\new-line' sequences.
typedef struct STNBSintaxParserCPhase2_ {
	struct {
		//1, means '\' was found
		//2, means '\' + '\r' were found
		UI32 partsAccum;
	} newline;
} STNBSintaxParserCPhase2;

//Phase 3: replace comments for white spaces and decompose in preproc-tokens
//Each file can't end with partial-token nor partial-comment.
typedef struct STNBSintaxParserCPhase3_ {
	//Comments
	struct {
		//1, means '/' was found when inactive, or '*' when active and multiline.
		UI8 partsAccum;
		BOOL isActive;
		BOOL isMultiline;
	} comment;
	//pp-tokens parser
	struct {
		STNBSintaxParserCTokens	queue;
	} pptokens;
} STNBSintaxParserCPhase3;

//Phase 4: preproc directives are executes and macro are expanded
typedef struct STNBSintaxParserCPhase4_ {
	//pproc parser
	struct {
		//if-group
		//elif-group
		//else-group
		//endif-line
		//control-line
		BOOL				lastFeedWasSpace;
		BOOL				curLineStarted;	//current line is empty or only contains spaces
		BOOL				isOpen;	//'#' found, and waiting for '\n'
		UI32				linesCount;
		STNBSintaxParser	parser;	//one line of directive (starts with '#', end with '\n')
	} pproc;
} STNBSintaxParserCPhase4;

//Pre-proccessor conditional

typedef enum ENSintaxParserCPProcCondPartType_ {
	ENSintaxParserCPProcCondPartType_If = 0,
	ENSintaxParserCPProcCondPartType_Elif,
	ENSintaxParserCPProcCondPartType_Else,
	ENSintaxParserCPProcCondPartType_Endif,
	//Count
	ENSintaxParserCPProcCondPartType_Count
} ENSintaxParserCPProcCondPartType;

typedef struct STNBSintaxParserCPProcCondPart_ {
	BOOL								activePartFound; //In current-level root, determines if an "true" condition was already found.
	BOOL								isEnabled;	//Condition is "true" for this part.
	ENSintaxParserCPProcCondPartType	type;		//Type of conditional-part
	SI32								iParent;	//Parent-level, part that contains this
	UI32								iRoot;		//Current-level, root part
	UI32								iPrev;		//Current-level, previous part
	UI32								iNext;		//Current-level, next part
	UI32								iAfterEnd;	//Next part after 'endif'
} STNBSintaxParserCPProcCondPart;
	
//File

typedef struct STNBSintaxParserCFile_ {
	STNBString						filename;
	//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
	//Phase 1: map characters and replace trigraph sequences.
	STNBSintaxParserCPhase1			phase1;
	//Phase 2: remove '\\new-line' sequences.
	STNBSintaxParserCPhase2			phase2;
	//Phase 3: replace comments for white spaces and decompose in preproc-tokens
	STNBSintaxParserCPhase3			phase3;
	//Phase 4: preproc directives are executes and macro are expanded
	STNBSintaxParserCPhase4			phase4;
} STNBSintaxParserCFile;

//Phase 1: characters map and trigraph sequances replacements.
typedef struct STNBSintaxParserCState_ {
	//Rules
	struct {
		STNBSintaxParserCTokens		templates;
	} rules;
	//Files
	struct {
		STNBArray					arr;	//STNBSintaxParserCFile*
		STNBArray					stack;	//STNBSintaxParserCFile*
	} files;
	//Pproc conds
	struct {
		STNBArray					arr;	//STNBSintaxParserCPProcCondPart*
		STNBArray					stack;	//UI32
		BOOL						curPartIsEnabled;	//current cond branch code is enable (if not, must be ignored)
		BOOL						curActivePartFnd;	//current cond branch active code already found (ignore any other)
	} pprocConds;
	//
	BOOL		errFnd;
	BOOL		completed;
} STNBSintaxParserCState;

typedef struct STNBSintaxParserCConfig_ {
	struct {
		STNBArray	usr;	//UI32, user include-paths
		STNBArray	sys;	//UI32, system include-paths
		STNBString	str;	//values
	} includePaths;
} STNBSintaxParserCConfig;
	
typedef struct STNBSintaxParserC_ {
	STNBSintaxParserCConfig	cfg;
	STNBSintaxParserCState	parser;
	STNBSintaxParserCPProc	pproc;
} STNBSintaxParserC;

void NBSintaxParserC_init(STNBSintaxParserC* obj);
void NBSintaxParserC_release(STNBSintaxParserC* obj);

//--------------
//- Config
//--------------
void NBSintaxParserC_cfgIncludePathsUsrAdd(STNBSintaxParserC* obj, const char* path);
void NBSintaxParserC_cfgIncludePathsSysAdd(STNBSintaxParserC* obj, const char* path);

//--------------
//- Feed content
//--------------
BOOL NBSintaxParserC_feedStart(STNBSintaxParserC* obj, const char* rootFilename);
BOOL NBSintaxParserC_feed(STNBSintaxParserC* obj, const char* data);
BOOL NBSintaxParserC_feedByte(STNBSintaxParserC* obj, const char c);
BOOL NBSintaxParserC_feedBytes(STNBSintaxParserC* obj, const char* data, const SI32 dataSz);
BOOL NBSintaxParserC_feedEnd(STNBSintaxParserC* obj);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
