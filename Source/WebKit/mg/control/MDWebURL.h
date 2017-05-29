/*
** $Id$
**
** MDWebURL.h: The head file of webURL
**
** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd.
**
** All rights reserved by Feynman Software.
**
** Current maintainer: 
**
** Create date: 2010-12-13
*/

#ifndef MDWebURL_h
#define MDWebURL_h

class MDWebURLImpl;

class MDWebURL
{
public:
    MDWebURL(const char*);
    ~MDWebURL();

    char* standardURL();
    bool isNull()  const;
    bool isEmpty() const;
    bool isValid() const;
    // We can implement others here in future ...

private:
    MDWebURLImpl* m_impl;
};

#endif
