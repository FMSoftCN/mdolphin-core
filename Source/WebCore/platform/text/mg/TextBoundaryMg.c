/*
** $Id: TextBoundaryMg.c 592 2011-04-22 07:16:58Z lijiangwei $
**
** TextboundaryMg.c: implements file for struct array type
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-4-6
*/

#include <string.h>
#include <stdio.h>

#include  "TextBoundaryMg.h"

#include  "CharBreakDataMg.c"
#include  "WordBreakDataMg.c"


/**********************************************************************************************************************/
/************************************private APIs for finding data in array **********************************************/

//find wether the UNICODE codepoint is in the CODEAREAMG struct array
//parameters:
//result: 1  exist  ;0   not exist
static int  FindInCodeareaArray(const CODEAREAMG*  codeareap, int  arraylength, unsigned int codepoint)
{
    int min,max,tmp;
    
    min =0;
    max =arraylength -1;
    
    if(codepoint >codeareap[max].endcode || codepoint <codeareap[min].startcode)
        return 0;
        
    while(1)
    {
        if(codepoint >codeareap[min].endcode)
        {
            if(max  >min +1)
            {
                tmp =min +(max -min)/2;
                if(codepoint <codeareap[tmp].startcode)
                {
                    max =tmp;
                }
                else if(codepoint >codeareap[tmp].endcode)
                {
                    min =tmp;
                }
                else
                {
                    if(codepoint >=codeareap[tmp].startcode && codepoint <=codeareap[tmp].endcode)
                    {
                        return 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                if((codepoint >=codeareap[min].startcode && codepoint <=codeareap[min].endcode) ||
                   (codepoint >=codeareap[max].startcode && codepoint <=codeareap[max].endcode))
                {
                    return 1;
                }
                else
                {
                    break;
                }
            }
        }
        else if(codepoint >=codeareap[min].startcode)
        {
            return 1;
        }
        else
        {
            break;
        }
    }
    
    return 0;
}


//find wether the UNICODE codepoint is in the UCHAR array
//parameters:
//result: 1  exist  ;0   not exist
static int  FindInNormalArray(const UChar* codeareap, int  arraylength, unsigned int codepoint)
{
    int min,max,tmp;
    
    min =0;
    max =arraylength -1;
    
    if(codepoint <codeareap[min] || codepoint >codeareap[max])
        return 0;
        
    while(1)
    {
        if(codepoint >codeareap[min])
        {
            if(max >min +1)
            {
                tmp =min +(max -min)/2;
                if(codepoint ==codeareap[tmp])
                {
                    return 1;
                }
                else if(codepoint >codeareap[tmp])
                {
                    min =tmp;
                }
                else
                {
                    max =tmp;
                }
            }
            else if(codepoint ==codeareap[min] || codepoint ==codeareap[max])
            {
                return 1;
            }
            else
            {
                break;
            }
        }
        else if(codepoint ==codeareap[min])
        {
            return 1;
        }
        else
        {
            break;
        }
    }
    
    return 0;
}



/**********************************************************************************************************************/
/************************************private APIs about grapheme cluster **********************************************/


//test if the codepoint is a control type grapheme cluster
static int Is_ControlType(unsigned int codepoint)
{
    if(FindInCodeareaArray(C_controlBoundary,CONTROLBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


//test if the codepoint is a extend type grapheme cluster
static int Is_ExtendType(unsigned int codepoint)
{
    if(FindInCodeareaArray(C_extendBoundary,EXTENDBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


//test if the codepoint is a LV type grapheme cluster
static int Is_LVType(unsigned int codepoint)
{
    if(FindInNormalArray(C_LVBoundary,LVBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}



//get the type of the specific codepoint
//parameter: codepoint  Uniocde code point.
static GC_TYPE  GetType_GraphemeCluster(unsigned int codepoint)
{
    if(codepoint >0x10FFFF)
        return gc_ERROR_TYPE;
        
    if(codepoint ==C_CRBoundary[0])
    {
        return CR;
    }
    else if(codepoint ==C_LFBoundary[0])
    {
        return LF;
    }
    else if(codepoint >=C_VBoundary[0].startcode && codepoint <=C_VBoundary[0].endcode)
    {
        return V;
    }
    else if(codepoint >=C_TBoundary[0].startcode && codepoint <=C_TBoundary[0].endcode)
    {
        return T;
    }
    else if((codepoint >=C_LBoundary[0].startcode && codepoint <=C_LBoundary[0].endcode) ||
            (codepoint >=C_LBoundary[1].startcode && codepoint <=C_LBoundary[1].endcode))
    {
        return L;
    }
    else if(Is_ControlType(codepoint))
    {
        return CONTROL;
    }
    else if(Is_ExtendType(codepoint))
    {
        return EXTEND;
    }
    else if(Is_LVType(codepoint))
    {
        return LV;
    }
    else if(codepoint >=C_LVTBoundary[0].startcode && codepoint <=C_LVTBoundary[0].endcode)
    {
        return LVT;
    }

    return gc_TYPE_ANY;
}


/*********************************************************************************************************************/
/**************************************public API  about grapheme cluster ********************************************/

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


int  firstGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index;
    GC_TYPE   ch_type;
    
    if(NULL ==text || NULL ==pGraphemeClusterinfo)
        return -1;
        
    pGraphemeClusterinfo->current =-1;
    pGraphemeClusterinfo->cur_type =gc_ERROR_TYPE;
    index =0;
    while(index <textlength)
    {
        ch_len =utf16_len_first_char(mchar+index,textlength - index);
        
        wc =mchar[index];
        if(ch_len >1)
        {
            wc =utf16_conv_to_uc32(mchar+index);
        }
        
        ch_type =GetType_GraphemeCluster(wc);
        /*if(EXTEND ==ch_type)
        {
            index +=ch_len;
        }
        else
        {*/
            pGraphemeClusterinfo->current =index;
            pGraphemeClusterinfo->cur_type =ch_type;
            return index;
        //}
    }
    
    return -1;
}


int  nextGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index;
    GC_TYPE   ch_type;
    
    if(NULL ==text || NULL ==pGraphemeClusterinfo)
        return -1;
        
    index =pGraphemeClusterinfo->current;
    if(0 ==pGraphemeClusterinfo->current && 0 ==pGraphemeClusterinfo->cur_type)
    {
        pGraphemeClusterinfo->current =-1;
        pGraphemeClusterinfo->cur_type =gc_ERROR_TYPE;

		return firstGraphemeClusterBoundary(mchar,textlength,pGraphemeClusterinfo);
    }
    
    while(index <textlength)
    {
        //to get the next grapheme cluster index
        ch_len =utf16_len_first_char(mchar+index,textlength - index);
        index += ch_len;
        if(index >=textlength)
        {
            return -1;   //there is no more grapheme cluster in the text
        }
        
        //get code value
        ch_len =utf16_len_first_char(mchar+index,textlength - index);
        
        wc =mchar[index];
        if(ch_len >1)
        {
            wc =utf16_conv_to_uc32(mchar+index);
        }
        
        //get Grapheme cluster type
        ch_type =GetType_GraphemeCluster(wc);
        
        //Do not break before extending characters
        if(EXTEND ==ch_type)
        {
            if(LF ==pGraphemeClusterinfo->cur_type || CR ==pGraphemeClusterinfo->cur_type || 
               (CONTROL ==pGraphemeClusterinfo->cur_type && 0x0001 ==mchar[index -1]))    //��֪���CONTROL���XTENDָ���������X0001�ԣ�ʱͶ�һ�Ժ�پ�
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                return index;
            }
            
            pGraphemeClusterinfo->current =index;
            pGraphemeClusterinfo->cur_type =ch_type;
            
            continue;
        }
        
        //if it is the beginning of the text then return the index of the first valid grapheme cluster
        if(gc_ERROR_TYPE ==pGraphemeClusterinfo->cur_type)
        {
            pGraphemeClusterinfo->current =index;
            pGraphemeClusterinfo->cur_type =ch_type;
            
            return index;
        }
        
        //just wether there is boundary
        switch(pGraphemeClusterinfo->cur_type)
        {
			case gc_ERROR_TYPE:
				break;
            case  CR:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                if(LF !=ch_type)
                {
                    return index;
                }
            }
            break;
            case  LF:
            case  CONTROL:
            case  EXTEND:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                return index;
            }
            case  L:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                if(L !=ch_type && V !=ch_type && LV !=ch_type && LVT !=ch_type)
                {
                    return index;
                }
            }
            break;
            case  V:
            case  LV:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                if(V !=ch_type && T !=ch_type)
                {
                    return index;
                }
            }
            break;
            case  T:
            case  LVT:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                if(T !=ch_type)
                {
                    return index;
                }
            }
            break;
            case  gc_TYPE_ANY:
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                return index;
            }
        }
    }
    
    return -1;
}


int  previousGraphemeClusterBoundary(const UChar* text, int textlength, GRAPHEMECLUSTERINFO* pGraphemeClusterinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int index;
    GC_TYPE   ch_type;
    
    if(NULL ==text || NULL ==pGraphemeClusterinfo)
        return -1;
        
    index =pGraphemeClusterinfo->current;
    if(0 ==pGraphemeClusterinfo->current && 0 ==pGraphemeClusterinfo->cur_type)
    {
        pGraphemeClusterinfo->current =-1;
        pGraphemeClusterinfo->cur_type =gc_ERROR_TYPE;
    }
    
    //û�Ը�ַ��߽�
    if(0 ==index && gc_ERROR_TYPE ==pGraphemeClusterinfo->cur_type)
    {
        memset(pGraphemeClusterinfo,0,sizeof(GRAPHEMECLUSTERINFO));
        return nextGraphemeClusterBoundary(text,textlength,pGraphemeClusterinfo);
    }
    else if(0 ==index && gc_ERROR_TYPE !=pGraphemeClusterinfo->cur_type)
    {
        //ǰȷı߽�Ѿ���ʼ
        return index;
    }
    else
    {
        while(index >0)
        {
            if(mchar[index -1] <0xD800 || mchar[index -1] >0xDFFF)
            {
                //get code value
//                if(!(mchar[0] <0xD800 || mchar[0] >0xDFFF))
//                {
//                    return -1;   //there is an error,the codepoint value is invalid
//                }
                index -=1;
                wc =mchar[index];
                
                //get Grapheme cluster type
                ch_type =GetType_GraphemeCluster(wc);
                if(EXTEND ==ch_type)
                {
                    if(0 ==index)
                    {
                        return -1;   //there first boundary is the current ,the index:0 is not the first boundary
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =EXTEND;
                        
                        //Do not break before extending characters,so we continue
                        continue;
                    }
                }
                else
                {
                    pGraphemeClusterinfo->current =index;
                    pGraphemeClusterinfo->cur_type =ch_type;
                    
                    return index;
                }
            }
            else if(index >1 && (mchar[index -1] >=0xDC00 && mchar[index -1] <=0xDFFF) && 
                    (mchar[index -2] >=0xD800 && mchar[index -2] <=0xDBFF))
            {
                //get code value
                index -=2;
                wc =utf16_conv_to_uc32(mchar+index);
                
                //get Grapheme cluster type
                ch_type =GetType_GraphemeCluster(wc);
                
                if(EXTEND ==ch_type)
                {
                    if(0 ==index)
                    {
                        return -1;   //there first boundary is the current ,the index:0 is not the first boundary
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =EXTEND;
                        
                        //Do not break before extending characters,so we continue
                        continue;
                    }
                }
                else
                {
                    pGraphemeClusterinfo->current =index;
                    pGraphemeClusterinfo->cur_type =ch_type;
                    
                    return index;
                }
            }
            else
            {
                return -1;    //there is an error,the codepoint value is invalid
            }
        }
    }
    
    return -1;
}


static GC_TYPE getpreviousgraphemeclustertype(const UChar* text, int textlength, int* index)
{
    const UChar* mchar = text;
    unsigned int wc;
    
    if(*index <1 || NULL ==text)
        return gc_ERROR_TYPE;
        
    (*index) -=1;
    if(mchar[*index] <0xD800 || mchar[*index] >0xDFFF)
    {
        wc =mchar[*index];
    }
    else if(mchar[*index] >=0xDC00 && mchar[*index] <=0xDFFF)
    {
        if(*index <1)
        {
            return gc_ERROR_TYPE;
        }
        *index -=1;
        wc =utf16_conv_to_uc32(mchar+*index);
    }
    else
    {
        return gc_ERROR_TYPE;
    }    
    
    return GetType_GraphemeCluster(wc);
}


int  precedingGraphemeClusterBoundary(const UChar* text, int textlength, int offset,GRAPHEMECLUSTERINFO* pGraphemeClusterinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int index,seindex;
    GC_TYPE   ch_type,previous_type;
    
    if(NULL ==text || NULL ==pGraphemeClusterinfo)
        return -1;
        
    index =offset -1;

	if(index <0)
		return -1;
	if(0 ==index)
		return 0;    //the beginning is a starter
    if(index >=textlength)
        index =textlength -1;
        
    if(0 ==pGraphemeClusterinfo->current && 0 ==pGraphemeClusterinfo->cur_type)
    {
        pGraphemeClusterinfo->current =-1;
        pGraphemeClusterinfo->cur_type =gc_ERROR_TYPE;
    }
        
    while(index >=0)
    {
        if(mchar[index] <0xD800 || mchar[index] >0xDFFF)
        {
            wc =mchar[index];
        }
        else if(mchar[index] >=0xDC00 && mchar[index] <=0xDFFF)
        {
            if(index <1)
            {
                return -1;    //error
            }
            index -=1;
            wc =utf16_conv_to_uc32(mchar+index);
        }
        else if((index+1) <textlength && mchar[index] >=0xD800 && mchar[index] <=0xDBFF &&
                mchar[index+1] >=0xDC00 && mchar[index+1] <=0xDFFF)
        {
            wc =utf16_conv_to_uc32(mchar+index);
        }
        else
        {
            return -1;
        }
        
        //get Grapheme cluster type
        ch_type =GetType_GraphemeCluster(wc);
        
        if(EXTEND ==ch_type)
        {
            if(index <1)
                return -1;        //error
                
            if(0x000a ==mchar[index-1] || 0x000d ==mchar[index -1] || 0x0001 ==mchar[index -1])
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                return index;
            }
            index -=1;
            
            continue;
        }
        else
        {
            switch(ch_type)
            {
				case  gc_ERROR_TYPE:
				case  EXTEND:
					break;
                case  CR:
                case  CONTROL:
                case  gc_TYPE_ANY:
                {
                    pGraphemeClusterinfo->current =index;
                    pGraphemeClusterinfo->cur_type =ch_type;
                    
                    return index;
                }
                case  LF:
                {
                    seindex =index;
					previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =LF;
							return 0;
						}
		
						return -1;
					}
                    else if(CR !=previous_type)
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =seindex;
                        pGraphemeClusterinfo->cur_type =CR;

						index =seindex -1;
						if(index <0)
							return 0;        //already reach the beginning of the text ,it is the start
						continue;
                    }
                }
                break;
                case  L:
                case  LV:
                case  LVT:
                {
                    seindex =index;
					previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;
						}

						return -1;
					}
                    else if(L !=previous_type)
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =seindex;
                        pGraphemeClusterinfo->cur_type =L;
                        
						index =seindex -1;
						if(index <0)        //the beginning is a starter
							return 0;
						continue;
                    }
                }
                break;
                case  V:
                {
                    seindex =index;
                    previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;
						}

						return -1;
					}
                    else if(L ==previous_type || V ==previous_type || LV ==previous_type)
                    {
                        pGraphemeClusterinfo->current =seindex;
                        pGraphemeClusterinfo->cur_type =previous_type;
                        
						index =seindex -1;
						if(index <0)        //the beginning is a starter
							return 0;
						continue;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                }
                break;
                case  T:
                {
                    seindex =index;
                    previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;
						}

						return -1;
					}
                    else if(T ==previous_type || V ==previous_type || LV ==previous_type || LVT ==previous_type)
                    {
                        pGraphemeClusterinfo->current =seindex;
                        pGraphemeClusterinfo->cur_type =previous_type;

						index =seindex -1;
						if(index <0)        //the beginning is a starter
							return 0;
						continue;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                }
            }
        }
    }
    
    return -1;
}


int  followingGraphemeClusterBoundary(const UChar* text, int textlength, int offset,GRAPHEMECLUSTERINFO* pGraphemeClusterinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int index,seindex,ch_len;
    GC_TYPE   ch_type, previous_type;
    
    if(NULL ==text || NULL ==pGraphemeClusterinfo)
        return -1;
        
    index =offset <0?0:offset +1;
    if(index >=textlength)
        //index =textlength -1;
		return -1;
        
    if(0 ==pGraphemeClusterinfo->current && 0 ==pGraphemeClusterinfo->cur_type)
    {
        pGraphemeClusterinfo->current =-1;
        pGraphemeClusterinfo->cur_type =gc_ERROR_TYPE;
    }
    
    while(index <textlength)
    {
        if(mchar[index] <0xD800 || mchar[index] >0xDFFF)
        {
            wc =mchar[index];
            ch_len =1;
        }
        else if(mchar[index] >=0xDC00 && mchar[index] <=0xDFFF)
        {
            if(index +1<textlength)
            {
                index +=1;
                continue;
            }
            
            return -1;    //this is the end of text, here is no boundary after the offset
        }
        else if((index +1)<textlength && mchar[index] >=0xD800 && mchar[index] <=0xDBFF &&
                mchar[index+1] >=0xDC00 && mchar[index+1] <=0xDFFF)
        {
            wc =utf16_conv_to_uc32(mchar+index);
            ch_len =2;
        }
        else
        {
            return -1;     //error
        }
        
        //get Grapheme cluster type
        ch_type =GetType_GraphemeCluster(wc);
        
        if(EXTEND ==ch_type)
        {
            if(0x000a ==mchar[index -1] || 0x000d ==mchar[index -1] || 0x0001 ==mchar[index -1])
            {
                pGraphemeClusterinfo->current =index;
                pGraphemeClusterinfo->cur_type =ch_type;
                
                return index;
            }
            if(index +ch_len>=textlength)
                return -1;        //
            index +=ch_len;
            
            continue;
        }
        else
        {
            switch(ch_type)
            {
				case gc_ERROR_TYPE:
				case EXTEND:
					break;
                case  CR:
                case  CONTROL:
                case  gc_TYPE_ANY:
                {
                    pGraphemeClusterinfo->current =index;
                    pGraphemeClusterinfo->cur_type =ch_type;
                    
                    return index;
                }
                case  LF:
                {
                    seindex =index;
					previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =LF;
							return 0;         
						}
						return -1;       //error
					}
                    else if(CR !=previous_type)
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                    else
                    {
                        if(index +ch_len>=textlength)
                            return -1;        //there is no boundary after the offset
                        index +=ch_len;
                        
                        continue;
                    }
                }
                break;
                case  L:
                case  LV:
                case  LVT:
                {
                    seindex =index;
					previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;         
						}
						return -1;       //error
					}
                    else if(L !=previous_type)
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                    else
                    {
                        if(index +ch_len>=textlength)
                            return -1;        //there is no boundary after the offset
                        index +=ch_len;
                        
                        continue;
                    }
                }
                break;
                case  V:
                {
                    seindex =index;
                    previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;         
						}
						return -1;       //error
					}
                    else if(L ==previous_type || V ==previous_type || LV ==previous_type)
                    {
                        if(index +ch_len>=textlength)
                            return -1;        //there is no boundary after the offset
                        index +=ch_len;
                        
                        continue;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                }
                break;
                case  T:
                {
                    seindex =index;
                    previous_type =getpreviousgraphemeclustertype(mchar,textlength,&seindex);
					if(gc_ERROR_TYPE ==previous_type)
					{
						if(0 ==index)
						{
							pGraphemeClusterinfo->current =index;
                        	pGraphemeClusterinfo->cur_type =ch_type;
							return 0;         
						}
						return -1;       //error
					}
                    else if(T ==previous_type || V ==previous_type || LV ==previous_type || LVT ==previous_type)
                    {
                        if(index +ch_len>=textlength)
                            return -1;        //there is no boundary after the offset
                        index +=ch_len;
                        
                        continue;
                    }
                    else
                    {
                        pGraphemeClusterinfo->current =index;
                        pGraphemeClusterinfo->cur_type =ch_type;

						return index;
                    }
                }
            }
        }
    }
    
    return -1;
}



/*********************************************************************************************************************/
/**************************************private API  about word ********************************************************/

static int Is_FormatType(unsigned int codepoint)
{
    if(FindInCodeareaArray(W_FormatBoundary,FORMATBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_KatakanaType(unsigned int codepoint)
{
    if(FindInCodeareaArray(W_KatakanaBoundary,KATAKANABOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_AletterType(unsigned int codepoint)
{
    if(FindInCodeareaArray(W_ALetterBoundary,ALETTERBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_MidletterType(unsigned int codepoint)
{
    if(FindInNormalArray(W_MidLetterBoundary,MIDLETTERBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_MidnumType(unsigned int codepoint)
{
    if(FindInNormalArray(W_MidNumBoundary,MIDNUMBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_NumericType(unsigned int codepoint)
{
    if(FindInCodeareaArray(W_NumericBoundary,NUMERICBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}


static int Is_ExtendnumletType(unsigned int codepoint)
{
    if(FindInNormalArray(W_ExtendNumLetBoundary,EXTENDNUMLETBOUNDARY_NUM,codepoint))
    {
        return 1;
    }
    
    return 0;
}



//get the type of the specific codepoint, which used for word boundary
//parameter: codepoint  Uniocde code point.
static WORD_TYPE  GetType_word(unsigned int codepoint)
{
    if(codepoint >0x10FFFF)
        return w_ERROR_TYPE;
        
    if(Is_AletterType(codepoint))
    {
        return ALETTER;
    }
    else if(Is_MidletterType(codepoint))
    {
        return MIDLETTER;
    }
    else if(Is_NumericType(codepoint))
    {
        return NUMERIC;
    }
    else if(Is_ExtendnumletType(codepoint))
    {
        return EXTENDNUMLET;
    }
    else if(Is_MidnumType(codepoint))
    {
        return MIDNUM;
    }
    else if(Is_KatakanaType(codepoint))
    {
        return KATAKANA;
    }
    else if(Is_FormatType(codepoint))
    {
        return FORMAT;
    }
    
    return w_TYPE_ANY;
}


/*********************************************************************************************************************/
/**************************************public API  about word ********************************************************/

int  firstwordBoundary(const UChar* text, int textlength, WORDBOUNDARY* pwordinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index;
    WORD_TYPE   ch_type;
    GRAPHEMECLUSTERINFO   graphemecluster;
    
    if(NULL ==text || NULL ==pwordinfo)
        return -1;
        
//    pGraphemeClusterinfo->current =-1;
//    pGraphemeClusterinfo->cur_type =w_ERROR_TYPE;
//    index =0;
    
    index =firstGraphemeClusterBoundary(mchar,textlength,&graphemecluster);
    if(-1 ==index)
        return -1;  //error
    ch_len =utf16_len_first_char(mchar+index,textlength - index);
    
    wc =mchar[index];
    if(ch_len >1)
    {
        wc =utf16_conv_to_uc32(mchar+index);
    }
    
    ch_type =GetType_word(wc);
    
    pwordinfo->current =index;
    pwordinfo->cur_type = ch_type;
    
    return index;
}


static WORD_TYPE  getnextchar_wordtype(const UChar* text, int textlength, int offset, int* index_back)
{
    const UChar* mchar = text;
    unsigned int wc;
    int sech_len,seindex;
    GRAPHEMECLUSTERINFO   graphemecluster;
    
    seindex =followingGraphemeClusterBoundary(mchar,textlength,offset,&graphemecluster);
    if(-1 == seindex)
    {
        return w_ERROR_TYPE;
    }
    else
    {
        //get the char type behind the index
        sech_len =utf16_len_first_char(mchar+seindex,textlength - seindex);

        wc =mchar[seindex];
        if(sech_len >1)
        {
            wc =utf16_conv_to_uc32(mchar+seindex);
        }
        
        *index_back =seindex;
        return GetType_word(wc);
    }
    
    return w_ERROR_TYPE;
}


static WORD_TYPE  getpreviouschar_wordtype(const UChar* text, int textlength, int offset, int* index_back)
{
    const UChar* mchar = text;
    unsigned int wc;
    int sech_len,seindex;
    GRAPHEMECLUSTERINFO   graphemecluster;
    
    seindex =precedingGraphemeClusterBoundary(mchar,textlength,offset,&graphemecluster);
    if(-1 == seindex)
    {
        return w_ERROR_TYPE;
    }
    else
    {
        //get the char type behind the index
        sech_len =utf16_len_first_char(mchar+seindex,textlength - seindex);

        wc =mchar[seindex];
        if(sech_len >1)
        {
            wc =utf16_conv_to_uc32(mchar+seindex);
        }

        *index_back =seindex;
        return GetType_word(wc);
    }
    
    return w_ERROR_TYPE;
}


int  nextwordBoundary(const UChar* text, int textlength, WORDBOUNDARY* pwordinfo)
{
    const UChar* mchar = text;
    unsigned int wc;
    int ch_len,index,seindex,thindex;
    WORD_TYPE   ch_type,tmp_type;
    GRAPHEMECLUSTERINFO   graphemecluster;
    
    if(NULL ==text || NULL ==pwordinfo)
        return -1;
        

    index =pwordinfo->current;

	if(index+1 ==textlength)
	{
		pwordinfo->current = textlength;
		return textlength;    //will reach the end of text so return the textlength
	}

    if(0 ==pwordinfo->current && 0 ==pwordinfo->cur_type)
    {
        pwordinfo->current =-1;
        pwordinfo->cur_type =w_ERROR_TYPE;
        
        return firstwordBoundary(mchar,textlength,pwordinfo);
    }
    else if(0 !=pwordinfo->current && (pwordinfo->cur_type <FORMAT || pwordinfo->cur_type >w_TYPE_ANY))
    {
        return -1;       //error
    }
    if(index <0)
    {
        return firstwordBoundary(mchar,textlength,pwordinfo);
    }

    if(index+1 > textlength)
        return -1;    //already reach the end of text
        
    while(index <textlength)
    {
		if((index+1) == textlength) 
		{
			pwordinfo->current =textlength;
			return textlength;
		}
		if(0x0085 ==mchar[index] && 0x0300 ==mchar[index +1])
		{
		    //Ŀǰֻ�ȷ�}�ַ֮�ڽ�ʱ߽�ʱɷָ������û�ȷ����XTEND��0x0085�涼ɷָ�
		    //��Ҫ�����
		    pwordinfo->current =index +1;
            pwordinfo->cur_type =w_TYPE_ANY;
            
            return index +1;
		}
		memset(&graphemecluster,0,sizeof(GRAPHEMECLUSTERINFO));
        index =followingGraphemeClusterBoundary(mchar,textlength,index,&graphemecluster);
        if(-1 ==index)
        {
            return -1;        //error
        }
            
        ch_len =utf16_len_first_char(mchar+index,textlength - index);
    
        wc =mchar[index];
        if(ch_len >1)
        {
            wc =utf16_conv_to_uc32(mchar+index);
        }
        
        ch_type =GetType_word(wc);
        
        if(FORMAT ==ch_type)
        {
            if(0x000a ==mchar[index  -1] || 0x000d ==mchar[index -1] || 0x0085 ==mchar[index -1])
            {
                pwordinfo->current =index;
                pwordinfo->cur_type =ch_type;
                
                return index;
            }
            if(index +ch_len<textlength)
            {
                //index +=ch_len;
                continue;
            }
            else
            {
                return -1;   //there is no more boundary
            }
        }
        else
        {
            switch(pwordinfo->cur_type)
            {
				case w_ERROR_TYPE:
				{
					return -1;
				}
				case FORMAT:
				{
					pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;

					return index;
				}
                case  ALETTER:
                {
                    pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;
                    
                    if(ALETTER ==ch_type || NUMERIC ==ch_type || EXTENDNUMLET ==ch_type)
                    {
                        continue;
                    }
                    else if(MIDLETTER ==ch_type)
                    {
                        tmp_type =getnextchar_wordtype(mchar,textlength,index,&seindex);
                        if(ALETTER ==tmp_type)
                        {
                            pwordinfo->current =seindex;
                            pwordinfo->cur_type =ALETTER;
                            index =seindex;
                            
                            continue;
                        }
                        else if(FORMAT ==tmp_type)
                        {
                            thindex =seindex;
                            if(ALETTER ==getnextchar_wordtype(mchar,textlength,thindex,&seindex))
                            {
                                pwordinfo->current =seindex;
                                pwordinfo->cur_type =ALETTER;
                                index =seindex;
                                
                                continue;
                            }
                            else
                            {
                                return index;
                            }
                        }
                        else
                        {
                            return index;
                        }
                    }
                    else
                    {
                        return index;
                    }
                }
                break;
                case  NUMERIC:
                {
                    pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;
                    
                    if(NUMERIC ==ch_type || ALETTER ==ch_type || EXTENDNUMLET ==ch_type)
                    {
                        continue;
                    }
                    else if(MIDNUM ==ch_type)
                    {
                        tmp_type =getnextchar_wordtype(mchar,textlength,index,&seindex);
                        if(NUMERIC ==tmp_type)
                        {
                            pwordinfo->current =seindex;
                            pwordinfo->cur_type =NUMERIC;
                            index =seindex;
                            
                            continue;
                        }
                        else if(FORMAT ==tmp_type)
                        {
                            thindex =seindex;
                            if(NUMERIC ==getnextchar_wordtype(mchar,textlength,thindex,&seindex))
                            {
                                pwordinfo->current =seindex;
                                pwordinfo->cur_type =NUMERIC;
                                index =seindex;
                                
                                continue;
                            }
                            else
                            {
                                return index;
                            }
                        }
                        else
                        {
                            return index;
                        }
                    }
                    else
                    {
                        return index;
                    }
                }
                break;
                case  MIDLETTER:
                {
                    if(ALETTER ==ch_type)
                    {
                        tmp_type =getpreviouschar_wordtype(mchar,textlength,pwordinfo->current,&seindex);
                        if(ALETTER ==tmp_type)
                        {
							pwordinfo->current =index;
                    		pwordinfo->cur_type =ch_type;

                            continue;
                        }
                        else if(FORMAT ==tmp_type)
                        {
                            thindex =seindex;
                            if(ALETTER ==getpreviouschar_wordtype(mchar,textlength,thindex,&seindex))
                            {
                                pwordinfo->current =index;
                        		pwordinfo->cur_type =ch_type;
    
                                continue;
                            }
                            else
                            {
                                pwordinfo->current =index;
                        		pwordinfo->cur_type =ch_type;
    
    							return index;
                            }
                        }
						else
						{
							pwordinfo->current =index;
                    		pwordinfo->cur_type =ch_type;

							return index;
						}
                    }
                    else
                    {
						pwordinfo->current =index;
                    	pwordinfo->cur_type =ch_type;

                        return index;
                    }
                }
                break;
                case  MIDNUM:
                { 
                    if(NUMERIC ==ch_type)
                    {
                        tmp_type =getpreviouschar_wordtype(mchar,textlength,pwordinfo->current,&seindex);
                        if(NUMERIC ==tmp_type)
                        {
							pwordinfo->current =index;
                    		pwordinfo->cur_type =ch_type;

                            continue;
                        }
                        else if(FORMAT ==tmp_type)
                        {
                            thindex =seindex;
                            if(NUMERIC ==getpreviouschar_wordtype(mchar,textlength,thindex,&seindex))
                            {
                                pwordinfo->current =index;
                        		pwordinfo->cur_type =ch_type;
    
                                continue;
                            }
                            else
                            {
                                pwordinfo->current =index;
                        		pwordinfo->cur_type =ch_type;
    
                                return index;
                            }
                        }
						else
						{
							pwordinfo->current =index;
                    		pwordinfo->cur_type =ch_type;

							return index;
						}
                    }
                    else
                    {
						pwordinfo->current =index;
                    	pwordinfo->cur_type =ch_type;

                        return index;
                    }
                }
                break;
                case  KATAKANA:
                {
                    pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;
                    
                    if(KATAKANA ==ch_type || EXTENDNUMLET ==ch_type)
                    {
                        continue;
                    }
                    else
                    {
                        return index;
                    }
                }
                break;
                case  EXTENDNUMLET:
                {
                    pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;
                    
                    if(KATAKANA ==ch_type || EXTENDNUMLET ==ch_type || ALETTER ==ch_type || NUMERIC ==ch_type)
                    {
                        continue;
                    }
                    else
                    {
                        return index;
                    }
                }
                break;
                case  w_TYPE_ANY:
                {
                    pwordinfo->current =index;
                    pwordinfo->cur_type =ch_type;
                    
                    return index;
                }
            }
        }
    }
    return -1;    
}





