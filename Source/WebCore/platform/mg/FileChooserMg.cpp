/*
 ** $Id: FileChooserMg.cpp 590 2011-04-22 05:29:20Z lijiangwei $
 **
 ** FileChooserMg.cpp: The platform implementation for FileChooser.
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: Yan Xiaowei 
 **  
 ** Create date: 2010-06-04 
 */

#include "config.h"
#include "FileChooser.h"

#include "TextRun.h"
#include "Font.h"

namespace WebCore {

String FileChooser::basenameForWidth(const Font& font, int width) const
{
    if (width <= 0 || !m_filenames.size())
        return String();

    String output = m_filenames[0].threadsafeCopy();
    while (font.width(TextRun(output.impl())) > width && output.length() > 4)
        output = output.replace(0, 4, String("..."));
    return output;
}

} // namespace WebCore
