/*
** $Id: TextBreakIteratorMg.cpp 590 2011-04-22 05:29:20Z lijiangwei $
**
** TextBreakIteratorMg.cpp: unicode break iterator.
**
** Copyright(C) 2007 Feynman Software.
**
** All rights reserved by Feynman Software.
**
** Create date: 2007-6-16
*/

#include <string.h>

#include "config.h"
#include "TextBreakIterator.h"

#include "LineBreakMg.h"
#include "TextBoundaryMg.h"

#include "NotImplemented.h"
namespace WebCore {

typedef enum {
    BRK_CHAR,
    BRK_WORD,
    BRK_LINE,
}BREAK_TYPE;


class PlatformBreakData
{
public:
    PlatformBreakData()
    {
        memset(&lbkInfo, 0, sizeof(LINEBREAKMG));
	    memset(&cbkInfo, 0, sizeof(GRAPHEMECLUSTERINFO));
        memset(&wbkInfo, 0, sizeof(WORDBOUNDARY));
    }

    ~PlatformBreakData()
    {
    }

    void clear(void)
    {
        memset(&lbkInfo, 0, sizeof(LINEBREAKMG));
	    memset(&cbkInfo, 0, sizeof(GRAPHEMECLUSTERINFO));
        memset(&wbkInfo, 0, sizeof(WORDBOUNDARY));
    }

    LINEBREAKMG lbkInfo;
    GRAPHEMECLUSTERINFO cbkInfo;
    WORDBOUNDARY wbkInfo;
};

class TextBreakIterator
{
public:
    TextBreakIterator(BREAK_TYPE type);
    ~TextBreakIterator();

    bool setText(const UChar * str, int len)
    {
        clear();

        if (!str || (len < 0))
            return false;
        
        string = str;
        string_len = len;
        return true;
    }
    
    const UChar * getText(void)
    {
        return string;
    }
    
    int getTextLen(void) 
    {
        return string_len;
    }
    
    BREAK_TYPE getType(void)
    {
        return btype;
    }
public:

    int break_first(void);
    int break_last(void);
    int break_next(void);
    int break_prev(void);
    int break_preceding(int pos);
    int break_following(int pos);
    int break_current(void);
    bool is_break(int pos);

private:
    BREAK_TYPE btype;
    const UChar * string;
    int string_len;
    PlatformBreakData * data;
    void clear(void);
private:
    TextBreakIterator(const TextBreakIterator &);
    TextBreakIterator &operator=(const TextBreakIterator &);
};

// text break iterator
TextBreakIterator::TextBreakIterator(BREAK_TYPE type)
    :btype(type), string(NULL), string_len(0)
{
    data = new PlatformBreakData;
}

TextBreakIterator::~TextBreakIterator()
{
    delete data;
}

void TextBreakIterator::clear(void)
{
    string = NULL;
    string_len = 0;
    data->clear();
}

int TextBreakIterator::break_first()
{
	switch (btype)
	{
		case BRK_LINE:
			return FirstLineBreak(string, string_len, &(data->lbkInfo));
		case BRK_CHAR:
            return firstGraphemeClusterBoundary(string, string_len, &(data->cbkInfo));
		case BRK_WORD:
            return firstwordBoundary(string, string_len, &(data->wbkInfo));
		default:
			break;
	}
    return -1;
}

static bool is_space(int c)
{
	switch (c){
		case ' ':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			return true;
		default:
			return false;
	}
}

int TextBreakIterator::break_last()
{
	// please reference gtk 
    // TextBreakLast is not meant to find just any break according to bi->m_type 
    // but really the one near the last character.
    // (cmp ICU documentation for ubrk_first and ubrk_last)
    // From ICU docs for ubrk_last:
    // "Determine the index immediately beyond the last character in the text being scanned." 

    // So we should advance or traverse back based on isspace(bi->getText()[pos]
    // If last character position in the original string is a whitespace,
    // traverse to the left until the first non-white character position is found
    // and return the position of the first white-space char after this one.
    // Otherwise return m_length, as "the first character beyond the last" is outside our string.
    int nextWhiteSpacePos = getTextLen();
    int pos = nextWhiteSpacePos;
    while (pos >= 0 ) {
		if(!(is_space(getText()[pos]))){
			nextWhiteSpacePos = pos;
			break;
		}
		--pos;
    }
    return nextWhiteSpacePos;

}

int TextBreakIterator::break_next()
{
	switch (btype)
	{
		case BRK_LINE:
			return NextLineBreak(string, string_len, &(data->lbkInfo));
		case BRK_CHAR:
            return nextGraphemeClusterBoundary(string, string_len, &(data->cbkInfo));
		case BRK_WORD:
            return nextwordBoundary(string, string_len, &(data->wbkInfo));
		default:
			break;
	}
	return -1;
}

int TextBreakIterator::break_prev()
{
	switch (btype)
	{
		case BRK_LINE:
			return PreviousLineBreak(string, string_len, &(data->lbkInfo));
		case BRK_CHAR:
            return previousGraphemeClusterBoundary(string, string_len, &(data->cbkInfo));
		case BRK_WORD:
			//fixed me ,it's not a good idea to just return 0,we should find a right way to break previous word.
			//if we just return 0,we can fixed the bug 5135: http://bugzilla.rd.minigui.com/show_bug.cgi?id=5135
            notImplemented();
			return 0;
			break;
		default:
			break;
	}
	return -1;
}

int TextBreakIterator::break_preceding(int pos)
{
	switch (btype)
	{
		case BRK_LINE:
			return PrecedingLineBreak(string, string_len, &(data->lbkInfo), pos);
		case BRK_CHAR:
            return precedingGraphemeClusterBoundary(string, string_len, pos, &(data->cbkInfo));
		case BRK_WORD:
    notImplemented();
			break;
		default:
			break;
	}
	return -1;
}

int TextBreakIterator::break_following(int pos)
{
	switch (btype)
	{
		case BRK_LINE:
			return FollowingLineBreak(string, string_len, &(data->lbkInfo), pos);
		case BRK_CHAR:
            return followingGraphemeClusterBoundary(string, string_len, pos, &(data->cbkInfo));
		case BRK_WORD:
    notImplemented();
			break;
		default:
			break;
	}

	return -1;     //error
}

int TextBreakIterator::break_current()
{
	switch (btype)
	{
		case BRK_LINE:
			return data->lbkInfo.pos;
		case BRK_CHAR:
            return data->cbkInfo.current; 
		case BRK_WORD:
            return data->wbkInfo.current; 
		default:
			break;
	}
   return -1;
}

bool TextBreakIterator::is_break(int pos)
{
	switch (btype)
	{
		case BRK_LINE:
            {
                int ret = IsLineBreak (string, string_len, pos);  
                if (ret < 0)
                    return false;
                return ret ? true : false;  
            }
            break;
		case BRK_CHAR:
		case BRK_WORD:
    notImplemented();
			break;
		default:
			break;
	}

	return false;     //error
}

// breaking functions

TextBreakIterator* characterBreakIterator(const UChar* string, int length)
{
	static TextBreakIterator TextBreakmg(BRK_CHAR);

	TextBreakmg.setText(string, length);

	return &TextBreakmg;
}

TextBreakIterator* wordBreakIterator(const UChar* string, int length)
{
	static TextBreakIterator TextBreakmg(BRK_WORD);

	TextBreakmg.setText(string, length);

	return &TextBreakmg;
}

TextBreakIterator* acquireLineBreakIterator(const UChar* string, int length)
{
	static TextBreakIterator TextBreakmg(BRK_LINE);

	TextBreakmg.setText(string, length);

	return &TextBreakmg;
}

void releaseLineBreakIterator(TextBreakIterator* iterator)
{
    //TODO?
}

TextBreakIterator* sentenceBreakIterator(const UChar*, int length)
{
    notImplemented();
    return 0;
}

int textBreakFirst(TextBreakIterator* bi)
{
	if (!bi)
		return -1;     //error

   return bi->break_first();
}

int textBreakLast(TextBreakIterator* bi)
{
	if (!bi)
		return -1;     //error

   return bi->break_last();
}

int textBreakPrevious(TextBreakIterator* bi)
{
	if (!bi)
		return -1;     //error

    return bi->break_prev();
}

int textBreakNext(TextBreakIterator* bi)
{
	if (!bi)
		return -1;     //error

    return bi->break_next();
}

int textBreakPreceding(TextBreakIterator* bi, int pos)
{
	if (!bi)
		return -1;     //error

    return bi->break_preceding(pos);
}

int textBreakFollowing(TextBreakIterator* bi, int pos)
{
	if(!bi)
		return -1;    //error

    return bi->break_following(pos);
}

int textBreakCurrent(TextBreakIterator* bi)
{
	if(!bi)
		return -1;    //error

    return bi->break_current();
}

bool isTextBreak(TextBreakIterator* bi, int pos)
{
	if(!bi)
		return false;    //error

    return bi->is_break(pos);
}

TextBreakIterator* cursorMovementIterator(const UChar* string, int length)
{
    return characterBreakIterator(string, length);
}

}
