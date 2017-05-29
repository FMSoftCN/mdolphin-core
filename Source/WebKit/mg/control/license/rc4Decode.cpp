/*
 ** $Id$
 **
 ** rc4Decode.cpp: 
 **
 ** Copyright (C) 2003 ~ 2010 Beijing Feynman Software Technology Co., Ltd. 
 ** 
 ** All rights reserved by Feynman Software.
 **   
 ** Current maintainer: lvlei 
 **  
 ** Create date: 2010-08-30 
 */
#include "rc4Decode.h"
#include <assert.h>

#define FRAGMENT_SIZE 512

static inline void swap(unsigned char *s, unsigned int i, unsigned int j) {
    unsigned char temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}


static int decrypted_len(int len) {
    return len - ((len-1) / FRAGMENT_SIZE + 1) * sizeof(unsigned char);
}

static unsigned char calc_sum(int len, const unsigned char *data) {
    unsigned char sum;
    int i;
    for (i=0, sum=0; i<len; ++i) {
        sum -= data[i];
    }
    return sum;
}


rc4Key:: rc4Key(const unsigned char* data, int len)  
{
    int i, j;

    for (i = 0; i < 256; i++)
        m_data[i] = i;

    for (i = j = 0; i < 256; i++) {
        j = (j + data[i % len] + m_data[i]) & 255;
        swap(m_data, i, j);
    }

    m_i = m_j = 0;
}

int rc4Decode::decode(rc4Key &key, unsigned char *in, int in_len)
{
    int out_len = 0;
    int i;
    unsigned char sum;

    for (i=0; i<in_len; /* NULL */) {
        int n;

        decode(key, sizeof(unsigned char), &in[i], (unsigned char *)&sum);
        i += sizeof(unsigned char);

        n = (in_len - i > FRAGMENT_SIZE - sizeof(unsigned char)) ? (FRAGMENT_SIZE - sizeof(unsigned char)) : (in_len - i);
        decode(key, n, &in[i], &in[out_len]);
        if (calc_sum(n, &in[out_len]) != sum) {
            assert(0);
            return -1;
        }
        i += n;
        out_len += n;
    }

    assert(out_len == decrypted_len(in_len));
    return out_len;
}

void rc4Decode::decode(rc4Key &key, unsigned long len, const unsigned char *indata, unsigned char *outdata)
{
    unsigned long n;

    for (n=0; n<len; ++n) {
        outdata[n] = indata[n] ^ rc4Output(key);
    }
}

unsigned char rc4Decode::rc4Output(rc4Key &key)
{
    key.m_i = (key.m_i + 1) & 255;
    key.m_j = (key.m_j + key.m_data[key.m_i]) & 255;

    swap(key.m_data, key.m_i, key.m_j);

    return key.m_data[(key.m_data[key.m_i] + key.m_data[key.m_j]) & 255];
}

#if 0

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if 0
#include "00_minigui.dat.c"
#include "key.c"
#else
#include "watermark1.dat.c"
#include "watermark2.dat.c"
#include "watermark_key.c"
#endif

void test(int key_len, const unsigned char *key, int data_len, const unsigned char *data) {
     rc4Key rc4_key(key, key_len);
    unsigned char out[20480];
    int i;

    assert(data_len < sizeof(out));
    memcpy(out, data, data_len);
    //RC4_set_key(&rc4_key, key_len, key);
    //rc4Decode::rc4DecodeInstance()->decode(rc4_key, data_len, data, out);
    data_len = rc4Decode::rc4DecodeInstance()->decode(rc4_key, out, data_len);

    FILE *fb = fopen("/tmp/data_2.png", "wb+");
    fwrite(out, sizeof(unsigned char), data_len, fb);
    fclose(fb);
#if 0
    for (i=0; i<decLen; ++i) {
        printf("%02X", out[i]);
    }
    printf("\n");
#endif
}

int main(int argc, const char *argv[]) {

#if 0
    test(sizeof(splash_crypto_key), splash_crypto_key, 18067, __mg_splash_dat_00_minigui_data);
#else
    test(sizeof(watermark_crypto_key), watermark_crypto_key, 5582, __md_watermark_dat_watermark2_data);
#endif
    //test(strlen(argv[1]),(const unsigned char *)argv[1], strlen(argv[2]), (const unsigned char *)argv[2]);
#if 0
    unsigned char key []= {
        0x80,0x04,0xd7,0xf4,0x40,0x2b,0xef,0xdf,0xd2,0xa3,0x36,0xfd,0xbd,0x13,0x35,0xd1
    };

    test(16, key, strlen(argv[1]), (const unsigned char *)argv[1]);
    return 0;

    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        perror("open file");

    size_t read;
    char * line = NULL;
    size_t len = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        printf("%s", line);
        test(16, key, read, (unsigned char *)line);
    }

    if (line)
        free(line);
    fclose(fp);

    return 0;
#endif
}
#endif
