/*
** $Id: LineBreakMg.c 286 2010-09-16 08:41:59Z xwyan $
**
** linebreakMg.c: Line break implements file.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-5-23
*/

#include <stdio.h>
#include <string.h>

#include  "LineBreakMg.h"
#include  "LineBreakDataMg.c"

#define   MAX_CCH     1

// Define some short-cuts for the table
#define oo DIRECT_BRK				// '_' break allowed
#define SS INDIRECT_BRK				// '%' only break across space (aka 'indirect break' below)
#define cc COMBINING_INDIRECT_BRK	// '#' indirect break for combining marks
#define CC COMBINING_PROHIBITED_BRK	// '@' indirect break for combining marks
#define XX PROHIBITED_BRK			// '^' no break allowed_BRK
#define xS HANGUL_SPACE_BRK			// break allowed, except when spaces are used with Hangul (not used)

// Line Break Pair Table corresponding to Table 2 of UAX#14, Version 5.0.0 
// plus Korean Syllable Block extensions - for details see that document

static enum break_action brkPairs[][JT+1]=
{   //                ---     'after'  class  ------
	//		1	2	3	4	5	6	7	8	9  10  11  12  13  14  15  16  17  18  19  20  21   22  23  24  25  26  
	//     OP, CL, QU, GL, NS, EX, SY, IS, PR, PO, NU, AL, ID, IN, HY, BA, BB, B2, ZW, CM, WJ,  H2, H3, JL, JV, JT, = after class
	/*OP*/ {XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, CC, XX,  XX, XX, XX, XX, XX}, // OP open
	/*CL*/ {oo, XX, SS, SS, XX, XX, XX, XX, SS, SS, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // CL close
	/*QU*/ {XX, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS}, // QU quotation
	/*GL*/ {SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS}, // GL glue
	/*NS*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // NS no-start
	/*EX*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // EX exclamation/interrogation
	/*SY*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // SY Syntax (slash)
	/*IS*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // IS infix (numeric) separator
	/*PR*/ {SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, SS, oo, SS, SS, oo, oo, XX, cc, XX,  SS, SS, SS, SS, SS}, // PR prefix
	/*PO*/ {SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // NU numeric
	/*NU*/ {SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // AL alphabetic
	/*AL*/ {SS, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // AL alphabetic
	/*ID*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // ID ideograph (atomic)
	/*IN*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // IN inseparable
#ifdef v500
// Version 5.0.0
	/*HY*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // HY hyphens and spaces
	/*BA*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // BA break after 
#else
// Version 5.0.1
	/*HY*/ {oo, XX, SS, oo, SS, XX, XX, XX, oo, oo, SS, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // HY hyphens and spaces
	/*BA*/ {oo, XX, SS, oo, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // BA break after 
#endif
	/*BB*/ {SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS}, // BB break before 
	/*B2*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, oo, oo, oo, oo, SS, SS, oo, XX, XX, cc, XX,  oo, oo, oo, oo, oo}, // B2 break either side, but not pair
	/*ZW*/ {oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, oo, XX, oo, oo,  oo, oo, oo, oo, oo}, // ZW zero width space
	/*CM*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, oo, SS, SS, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, oo}, // CM combining mark
	/*WJ*/ {SS, XX, SS, SS, SS, XX, XX, XX, SS, SS, SS, SS, SS, SS, SS, SS, SS, SS, XX, cc, XX,  SS, SS, SS, SS, SS}, // WJ word joiner
																							    
	/*H2*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, SS, SS}, // Hangul 2 Jamo syllable
	/*H3*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, SS}, // Hangul 3 Jamo syllable
	/*JL*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  SS, SS, SS, SS, oo}, // Jamo Leading Consonant
	/*JV*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, SS, SS}, // Jamo Vowel
	/*JT*/ {oo, XX, SS, SS, SS, XX, XX, XX, oo, SS, oo, oo, oo, SS, SS, SS, oo, oo, XX, cc, XX,  oo, oo, oo, oo, SS}, // Jamo Trailing Consonant
	
};


/*
find the line break property of the UNICODE point specified by parameter codepoint in the area between the 
startpos parameter end endpos parameter.
*/
static BREAKCLASS FindinDB(int startpos, int endpos, unsigned int codepoint)
{
    int min,max,tmp;
    
    if(startpos ==endpos)
    {
        if(codepoint >=class_data[startpos].startcode && 
           codepoint <=class_data[startpos].endcode)
        {
            return class_data[startpos].LBKclass;
        }
        else
        {
            return XX;
        }
    }
    
    min =startpos;
    max =endpos;
    if(codepoint <class_data[min].startcode || codepoint >class_data[max].endcode)
        return XX;
        
    while(1)
    {
        if(codepoint >class_data[min].endcode)
        {
            if(max >min +1)
            {
                tmp =min + (max -min)/2;
                if(codepoint <class_data[tmp].startcode)
                {
                    max =tmp;
                }
                else if(codepoint >class_data[tmp].endcode)
                {
                    min =tmp;
                }
                else
                {
                    if(codepoint >=class_data[tmp].startcode && codepoint <=class_data[tmp].endcode)
                    {
                        return class_data[tmp].LBKclass;
                    }
                    else
                    {
                        return XX;
                    }
                }
            }
            else
            {
                if(codepoint >=class_data[max].startcode && codepoint <=class_data[max].endcode)
                {
                    return class_data[max].LBKclass;
                }
                else
                {
                    return XX;
                }
            }
        }
        else if(codepoint >=class_data[min].startcode)
        {
            return class_data[min].LBKclass;
        }
        else
        {
            return XX;
        }
    }
}


/*
return the line break property of the UNICODE code specified by codepoint.
this function can find four languages's UNICODE code's line break property.the four language is:chinese, english, korean,
japanese.For default we look up chinese first ,if in all chinese language Unicode code return XX, than the function will 
to look up in the four languages area, namely , the all database.
*/
static  BREAKCLASS  classifyofCodePoint(unsigned int codepoint)
{
    BREAKCLASS  bkClass;
    
    bkClass =FindinDB(CHINESE_START1,CHINESE_END1,codepoint);    //first look up chinese area
    
    if(XX ==bkClass)
    {
        bkClass =FindinDB(CHINESE_START2,CHINESE_END2,codepoint);    //second look up high chinese area
        if(XX ==bkClass)
        {
            bkClass =FindinDB(0,BREAKDATA_NUM,codepoint);      //look up all the database
        }
    }
    
    return bkClass;
}



static int utf16_len_first_char (const UChar* mstr, int len)
{
    if (mstr[0] < 0xD800 || mstr[0] > 0xDFFF)
        return 1;

    if (mstr[0] >= 0xD800 && mstr[0] <= 0xDBFF) 
    {
        if (len < 2)
            return 0;

        //kept unpaired surrogate
        if (mstr[1] < 0xDC00 || mstr[1] > 0xDFFF)
            return 1;
    }

    return 2;
}


static unsigned int utf16_conv_to_uc32 (const UChar* mchar)
{
    unsigned int wc;

    wc = mchar[0];
    wc <<= 10;
    wc |= (mchar[1] & 0x03FF);
    wc += 0x10000;

    return wc;
}



// placeholder function for complex break analysis
// cls - last resolved line break class (this is !SA)
// pcls - pointer to array of line breaking classes with pcls[0] == SA (input)
// pbrk - pointer to array of line breaking opportunities (output)
//
static int findComplexBreak(enum break_class cls, enum break_class *pcls, enum break_action *pbrk, int cch)
{
    int ich;
    
    if (!cch)
        return 0;

    for (ich = 1; ich < cch; ich++) {

        // .. do complex break analysis here
        // and report any break opportunities in pbrk ..

        pbrk[ich-1] = PROHIBITED_BRK; // by default: no break

        if (pcls[ich] != SA)
                break;
    }
    return ich;
}


/* Line break actions
  * these are already declared above as they are needed for some of the UI functions
  * repeated here for ease of reference (the symbols used in the table in UAX#14 as 
  * well as the constants used in the brkPairs array are shown as well)
  
enum break_action {
       DIRECT_BRK = 0,             	// _ in table, 	oo in array
       INDIRECT_BRK,               	// % in table, 	SS in array
       COMBINING_INDIRECT_BRK,		// # in table, 	cc in array
       COMBINING_PROHIBITED_BRK,  	// @ in table 	CC in array
       PROHIBITED_BRK,             	// ^ in table, 	XX in array
       EXPLICIT_BRK };				// ! in rules
*/

// handle spaces separately, all others by table
// pcls - pointer to array of line breaking classes (input)
// pbrk - pointer to array of line break opportunities (output)
// cch - number of elements in the arrays (count of characters) (input)
// ich - current index into the arrays (variable) (returned value)
// cls - current resolved line break class for 'before' character (variable)
// fLEGACY_CM  - whether Keep SP CM together, true: keep them together, false  no
static int findLineBrk(enum break_class *pcls, enum break_action *pbrk, int cch, int fLEGACY_CM)
{
    int ich;
    enum break_action brk;
    
    if (cch <= 0) 
        return 0;

    enum break_class cls = pcls[0];

    // handle case where input starts with an LB_LF
    if (cls == LB_LF)
        cls = BK;

    // treat NL like BK
    if (cls == NL)
         cls = BK;

    // treat SP at start of input as if it followed WJ
    if (cls == SP)
         cls = WJ;

    // loop over all pairs in the string up to a hard break or CRLF pair
    for (ich = 1; (ich < cch) && (cls != BK) && (cls != LB_CR || pcls[ich] == LB_LF); ich++) {

        // handle spaces explicitly
        if (pcls[ich] == SP) {
            pbrk[ich-1] = PROHIBITED_BRK;   // apply rule LB 7: �SP
            continue;                       // do not update cls
        }

        if (pcls[ich] == BK || pcls[ich] == NL || pcls[ich] == LB_LF) {
            pbrk[ich-1] = PROHIBITED_BRK;
            cls = BK;
            continue;
        }

        if (pcls[ich] == LB_CR) {
            pbrk[ich-1] = PROHIBITED_BRK;
            cls = LB_CR;
            continue;
        }

        // handle complex scripts in a separate function
        if (cls == SA || pcls[ich] == SA) {
            ich += findComplexBreak(cls, &pcls[ich-1], &pbrk[ich-1], cch - (ich-1));
            if (ich < cch)
                cls = pcls[ich];
            continue;
        }

        // lookup pair table information in brkPairs[before, after];
        brk = brkPairs[cls][pcls[ich]];

        pbrk[ich-1] = brk;                              // save break action in output array

        if (brk == INDIRECT_BRK) {                      // resolve indirect break
            if (pcls[ich - 1] == SP)                    // if context is A SP * B
                pbrk[ich-1] = INDIRECT_BRK;             //       break opportunity
            else                                        // else
                pbrk[ich-1] = PROHIBITED_BRK;           //       no break opportunity
        } 
        else if (brk == COMBINING_PROHIBITED_BRK) {   // this is the case OP SP* CM
            pbrk[ich-1] = COMBINING_PROHIBITED_BRK;     // no break allowed
            if (pcls[ich-1] != SP)
                continue;                               // apply rule 9: X CM* -> X
        } 
        else if (brk == COMBINING_INDIRECT_BRK) {     // resolve combining mark break
            pbrk[ich-1] = PROHIBITED_BRK;               // don't break before CM
            if (pcls[ich-1] == SP) {
                if (!fLEGACY_CM)                        // new: SP is not a base
                    pbrk[ich-1] = COMBINING_INDIRECT_BRK;    // apply rule SP �
                else {
                    pbrk[ich-1] = PROHIBITED_BRK;		// legacy: keep SP CM together
                    if (ich > 1)
                        pbrk[ich-2] = ((pcls[ich - 2] == SP) ? INDIRECT_BRK : DIRECT_BRK);
                }
            }
            else                                      // apply rule 9: X CM * -> X
                continue;                               // don't update cls
        }
        cls = pcls[ich];                                // save cls of current character
    }
    // always break at the end
    pbrk[ich-1] = EXPLICIT_BRK;
    return ich;
}

/*********************************************************************************************************/
/*******************************************below: public function***************************************/
/*
To find the first line break of the text,this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   LINEBREAKMG* pLBKinfo:   return the property of the line break
   
BE CAREFUL:  should break before the position returned by this function
             return -1 for error
*/
int FirstLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index,len,i,breakpos,processed,j;
    LINEBREAKMG   lbclsmg[MAX_CCH];
    BREAKACTION   lbrks[MAX_CCH];
    BREAKCLASS    lbcls[MAX_CCH];

    if (NULL ==text || NULL ==pLBKinfo)
        return -1;

    processed = 0;
    while (1) {
        memset(lbclsmg,0,sizeof(LINEBREAKMG)*MAX_CCH);
        memset(lbcls,0,sizeof(BREAKCLASS)*MAX_CCH);
        memset(lbrks,0,sizeof(BREAKACTION)*MAX_CCH);

        len = MAX_CCH > (textlength -processed)?(textlength -processed):MAX_CCH;
        for (i=0,index=processed; i<len && index<textlength; i++) {
            ch_len = utf16_len_first_char(mchar+index,textlength - index);
            wc = mchar[index];
            if (ch_len >1) {
                /* if the character is expressed by surrogate, we combine the two surrogate
                 * into original UNICODE codepoint. before getting the character's line break
                 * property.This is different from the UAX #14,which get the line break property
                 * of the two surrogates respectively.The reason we get the UNICODE codepoint's line
                 * break property other than the surrogate's line break property is some chinese is
                 * expressed by surrogate, and chinese can make line break between randomly two of it.
                 * If according to the UAX #14,you will find there is no line break between two chinese.
                 * So, i change it,although it may be wrong.
                 */
                wc = utf16_conv_to_uc32(mchar+index);
            }
            lbclsmg[i].mlbkClass = classifyofCodePoint(wc);
            lbclsmg[i].pos = index;
            lbclsmg[i].length = ch_len;
            index += ch_len;
        }

        for(j=0;j<MAX_CCH;j++)
            lbcls[j] = lbclsmg[j].mlbkClass;

        //get line breaks of four characters
        breakpos = findLineBrk(lbcls, lbrks, i, 0);
        for (i =0;i<breakpos;i++) {
            if (lbrks[i] !=COMBINING_PROHIBITED_BRK && lbrks[i] !=PROHIBITED_BRK && lbclsmg[i].pos >0) {
                pLBKinfo->mlbkClass =lbclsmg[i].mlbkClass;
                pLBKinfo->pos =lbclsmg[i].pos;
                pLBKinfo->length =lbclsmg[i].length;
                return (lbclsmg[i].pos+lbclsmg[i].length); //be careful: should break before the position returned by this function
            }
        }

        processed = lbclsmg[1].pos;
        if (processed +lbclsmg[1].length >=textlength)
            break;
    }
    return textlength;
}

/*
To find the next line break of the text,this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   LINEBREAKMG* pLBKinfo:   send in the current line break property and return the next property of the line break
   
BE CAREFUL:  should break before the position returned by this function
             return -1 for error
*/
int NextLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index,len,i,breakpos,processed,j;
    LINEBREAKMG   lbclsmg[MAX_CCH];
    BREAKACTION   lbrks[MAX_CCH];
    BREAKCLASS    lbcls[MAX_CCH];

    if(NULL ==text || NULL ==pLBKinfo)
        return -1;

   // processed =pLBKinfo->pos +pLBKinfo->length;
    processed = pLBKinfo->pos ;
    while(1) {
        memset(lbclsmg,0,sizeof(LINEBREAKMG)*MAX_CCH);
        memset(lbcls,0,sizeof(BREAKCLASS)*MAX_CCH);
        memset(lbrks,0,sizeof(BREAKACTION)*MAX_CCH);
        len =MAX_CCH >(textlength -processed)?(textlength -processed):MAX_CCH;
//        len =MAX_CCH <(textlength -processed)?(textlength -processed):MAX_CCH;
        for(i=0,index = processed;i <len && index < textlength;i++) {
            ch_len = utf16_len_first_char(mchar+index,textlength - index);
            wc = mchar[index];
            if(ch_len >1) {
                //if the character is expressed by surrogate, we combine the two surrogate into original UNICODE codepoint
                // before getting the character's line break property.This is different from the UAX #14,which get the line
                //break property of the two surrogates respectively.The reason we get the UNICODE codepoint's line break 
                //property other than the surrogate's line break property is some chinese is expressed by surrogate, and 
                //chinese can make line break between randomly two of it.If according to the UAX #14,you will find there is 
                //no line break between two chinese.So, i change it,although it may be wrong.
                wc = utf16_conv_to_uc32(mchar+index);
            }
            lbclsmg[i].mlbkClass = classifyofCodePoint(wc);
            lbclsmg[i].pos = index;
            lbclsmg[i].length = ch_len;
            index += ch_len;
        }

        for(j=0;j<MAX_CCH;j++)
            lbcls[j] = lbclsmg[j].mlbkClass;

        //get line breaks of four characters
        breakpos = findLineBrk(lbcls, lbrks, i, 0);
        for(i =0;i<breakpos;i++) {
            if(lbrks[i] !=COMBINING_PROHIBITED_BRK && lbrks[i] !=PROHIBITED_BRK) {
                pLBKinfo->mlbkClass =lbclsmg[i].mlbkClass;
                pLBKinfo->pos =lbclsmg[i].pos;
                pLBKinfo->length =lbclsmg[i].length;
                return (lbclsmg[i].pos+lbclsmg[i].length);    //be careful: should break before the position returned by this function
            }
        }
        
        processed = lbclsmg[1].pos;
        if(processed +lbclsmg[1].length >=textlength)
            break;
    }

    return textlength;
}


static int previousPosition(const UChar* mchar, int startpos)
{
    int previouspos = 0;
    if(startpos <= 0) {
        previouspos =0;
    }
    else {
        if(startpos >1 && 
           (mchar[startpos-1] >=0xDC00 && mchar[startpos-1] <=0xDFFF && 
            mchar[startpos-2] >=0xD800 && mchar[startpos-2] <=0xDBFF)) {
            previouspos =startpos -2;
        }
        else {
            previouspos =startpos -1;
        }
    }

    return previouspos;
}


/*
To find the previous line break of the text,this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   LINEBREAKMG* pLBKinfo:   send in the current line break property and return the previous property of the line break
   
BE CAREFUL:  should break before the position returned by this function
             return -1 for error or no previous line break
*/
int PreviousLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index,len,i,breakpos,processed,j;
    LINEBREAKMG   lbclsmg[MAX_CCH];
    BREAKACTION   lbrks[MAX_CCH];
    BREAKCLASS    lbcls[MAX_CCH];

    if(NULL ==text || NULL ==pLBKinfo)
        return -1;

    processed = previousPosition(mchar, pLBKinfo->pos);
    while(1) {
        memset(lbclsmg,0,sizeof(LINEBREAKMG)*MAX_CCH);
        memset(lbcls,0,sizeof(BREAKCLASS)*MAX_CCH);
        memset(lbrks,0,sizeof(BREAKACTION)*MAX_CCH);
        len =MAX_CCH >(textlength -processed)?(textlength -processed):MAX_CCH;
        for(i=0,index =processed;i <len && index <textlength;i++) {
            ch_len = utf16_len_first_char(mchar+index,textlength - index);
            wc = mchar[index];
            if(ch_len >1) {
                /* if the character is expressed by surrogate, we combine the two surrogate into
                 * original UNICODE codepoint before getting the character's line break property.
                 * This is different from the UAX #14,which get the line break property of the two
                 * surrogates respectively.The reason we get the UNICODE codepoint's line break 
                 * property other than the surrogate's line break property is some chinese is expressed
                 * by surrogate, and chinese can make line break between randomly two of it.If according
                 * to the UAX #14,you will find there is no line break between two chinese. So, i change
                 * it,although it may be wrong.
                 */
                wc = utf16_conv_to_uc32(mchar+index);
            }
            lbclsmg[i].mlbkClass = classifyofCodePoint(wc);
            lbclsmg[i].pos = index;
            lbclsmg[i].length = ch_len;
            index += ch_len;
        }

        for(j=0;j<MAX_CCH;j++)
            lbcls[j] =lbclsmg[j].mlbkClass;

        //get line breaks of four characters
        breakpos = findLineBrk(lbcls,lbrks,i,0);

        if (lbrks[0] !=COMBINING_PROHIBITED_BRK && lbrks[0] !=PROHIBITED_BRK) {
            pLBKinfo->mlbkClass =lbclsmg[0].mlbkClass;
            pLBKinfo->pos =lbclsmg[0].pos;
            pLBKinfo->length =lbclsmg[0].length;
            return (lbclsmg[0].pos+lbclsmg[0].length);    //be careful: should break before the position returned by this function
        }

        processed = previousPosition(mchar, lbclsmg[0].pos);
        if (processed <=0)
            break;
    }
    return -1;
}


/*
To find the preceding line break of the text before the position specified by parameter Precedingpos ,
this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   LINEBREAKMG* pLBKinfo:   send in the current line break property and return the preceding property of the line break
   int  Precedingpos:   the position the user want to look  a line break before it
   
BE CAREFUL:  should break before the position returned by this function
             return -1 for error or no preceding line break
*/
int PrecedingLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo, int Precedingpos)
{
    int adjustedpos;

    if (NULL ==text || NULL ==pLBKinfo)
        return -1;

    if (Precedingpos <=0) {
        return -1;
    }
    else {
        if (Precedingpos >1 && text[Precedingpos -1] >=0xDC00 && text[Precedingpos -1] <=0xDFFF)
            adjustedpos = Precedingpos -2;
        else
            adjustedpos = Precedingpos -1;
    }

    if (adjustedpos <=0)
        return -1;

    pLBKinfo->pos =adjustedpos;
    return PreviousLineBreak(text, textlength,pLBKinfo);
}



/*
To find the Following line break of the text before the position specified by parameter Followingpos ,
this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   LINEBREAKMG* pLBKinfo:   send in the current line break property and return the Following property of the line break
   int  Followingpos:    the position the user want to look  a line break Following it
   
BE CAREFUL:  should break before the position returned by this function
             return -1 for error 
*/
int FollowingLineBreak(const UChar* text, int textlength, LINEBREAKMG* pLBKinfo, int Followingpos)
{
    int adjustedpos,len;
    
    if(NULL ==text || NULL ==pLBKinfo)
        return -1;
        
    if(Followingpos+1 >=textlength) {
        return textlength;
    }
    else {
		/*
        if(text[Followingpos +1] >=0xD800 && text[Followingpos +1] <=0xDBFF) {
			printf("adjustedpos =Followingpos +2;[%d]\n",Followingpos);
            adjustedpos =Followingpos +2;
        }
        else {
			printf("adjustedpos =Followingpos +1;[%d]\n",Followingpos);
            adjustedpos =Followingpos +1;
        }
		*/
		adjustedpos =Followingpos ;
    }
    
    if (adjustedpos +1 >=textlength)
        return textlength;
    
    if (text[adjustedpos] >=0xD800 && text[adjustedpos] >=0xDBFF)
        len =2;
    else
        len =1;

    pLBKinfo->pos =adjustedpos;
    pLBKinfo->length =len;
    return NextLineBreak(text,textlength,pLBKinfo);
}


/*
To determine whether can make a line break at the pos of the text ,
this function is default for chinese, not for korean and japanese.
if the text is korean or japanese ,the result may be an error result.
parameters:
   UChar* text :  the text bo be resolved
   int textlength:    text length ,the number of unit UChar
   int  pos:    the character index the user want to look  a line break Following it
   
BE CAREFUL:  By default, this function is to judge whether can make line break between character[pos -1] and character[pos]
             return -1 for error 
*/
int IsLineBreak(const UChar* text, int textlength,int pos)
{
    LINEBREAKMG   LBKinfo;
    int index =-1;
    
    if (NULL ==text)
        return -1;
        
    if (0 ==pos || (text[pos] >=0xDC00 && text[pos] <=0xDFFF))
        return 0;
        
    index = FirstLineBreak(text +pos-1,textlength -pos+1,&LBKinfo);
    if(index != 1)
        return 0;

    return 1;
}


