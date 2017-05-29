/*
 ** $Id$
 **
 ** rc4Decode.h: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-30 
 */

#ifndef rc4Decode_h
#define rc4Decode_h

struct rc4Key{
public:
    rc4Key(const unsigned char* data, int len);

    unsigned char m_data[256];
    unsigned int m_i;
    unsigned int m_j;
};

class rc4Decode {
public:
    static rc4Decode* rc4DecodeInstance() 
    {
       static rc4Decode decode;
       return &decode;
    }
    int decode(rc4Key &key, unsigned char *data, int len);
    void decode(rc4Key &key, unsigned long len, const unsigned char *indata, unsigned char *outdata);
private:
    unsigned char rc4Output(rc4Key &key);

    rc4Decode() {};
    ~rc4Decode() {};
};


#endif
