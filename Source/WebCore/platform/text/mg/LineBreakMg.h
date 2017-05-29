/*
** $Id: LineBreakMg.h 123 2010-06-22 09:55:14Z xwyan $
**
** LineBreakMg.h: Line break header file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-5-21
*/

#ifndef  LineBreakMg_h
#define  LineBreakMg_h

#include  "unicode/UcharMg.h"

// Line Break Character Types

// These correspond to the line break class values defined in UAX#14, Version 
// 5.0.0. In a real implementation, there would be a mapping from character
// code to line break class value. In this demo version, the mapping is from
// a pseudo alphabet to these line break classes. The actual line break algorithm
// takes as input only line break classes, so, by changing the mapping from
// pseudo alphabet to actual Unicode Characters, this demo could be adapted 
// for use in actual line breaking.

typedef enum break_class
{
	// input types
	OP = 0,	// open
	CL,	// close
	CP,	// closing parentheses (from 5.2.0) (before 5.2.0 treat like CL)
	QU,	// quotation
	GL,	// glue
	NS,	// no-start
	EX,	// exclamation/interrogation
	SY,	// Syntax (slash)
	IS,	// infix (numeric) separator
	PR,	// prefix
	PO,	// postfix
	NU,	// numeric
	AL,	// alphabetic
	ID,	// ideograph (atomic)
	IN,	// inseparable
	HY,	// hyphen
	BA,	// break after
	BB,	// break before
	B2,	// break both
	ZW,	// ZW space
	CM,	// combining mark
	WJ, // word joiner

	// used for Korean Syllable Block pair table
	H2, // Hamgul 2 Jamo Syllable
	H3, // Hangul 3 Jamo Syllable
	JL, // Jamo leading consonant
	JV, // Jamo vowel
	JT, // Jamo trailing consonant

	// these are not handled in the pair tables
	SA, // South (East) Asian
	SP,	// space
	PS,	// paragraph and line separators
	BK,	// hard break (newline)
	LB_CR, // carriage return
	LB_LF, // line feed
	NL, // next line
	CB, // contingent break opportunity
	SG, // surrogate
	AI, // ambiguous
	XX, // unknown
}BREAKCLASS;


// Break actions are the types of break opportunities that may occur at a particular
// point in the input. Values for these are also needed in the UI portion of the code
// so they are already defined here - for explanation see below in the line break
// section.
typedef enum break_action
{
	DIRECT_BRK,
	INDIRECT_BRK, 		
	COMBINING_INDIRECT_BRK, 	
	COMBINING_PROHIBITED_BRK, 	
	PROHIBITED_BRK,
	EXPLICIT_BRK,
	HANGUL_SPACE_BRK,
}BREAKACTION;


typedef struct tagLineBreakClass{
    unsigned int   startcode;
    unsigned int   endcode;
    BREAKCLASS     LBKclass;
}LINEBREAKCLASS_DB;


typedef  struct tagLineBreakMg{
    BREAKCLASS   mlbkClass;
    int          pos;
    int          length;
}LINEBREAKMG;

#ifdef  __cplusplus
extern "C" {
#endif

int FirstLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo);
int NextLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo);
int PreviousLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo);
int PrecedingLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo, int Precedingpos);
int FollowingLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo, int Followingpos);
int IsLineBreak(const UChar* text, int textlength,int pos);

#ifdef  __cplusplus
 }
#endif

#endif   /*end of head file*/



