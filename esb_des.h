/********************************************************
  Name: des.h
  Copyright: xiaozw	
  Author: xiaozw
  Date: 07-11-09 16:44
  Description: DES算法的C语言实现
*********************************************************/ 
#ifndef _DES_H
#define _DES_H

#define LEN_IP_K1 7
#define LEN_IP_K2 6
#define LEN_IP_KR 16
#define LEN_IP_1 8
#define LEN_IP_2 8
#define LEN_IP_E 6
#define LEN_IP_S 8
#define LEN_IP_P 4

#define IF_ERR if(ret!=0){printf("INFO:File[%s]|Line[%d]|return=[%d]\n",__FILE__,__LINE__,ret);return ret;};

int opr_key(char subkeys[16][6],const char *key,const int seq);
int opr_data(char *des,const char *src,char subkeys[16][6],const int seq,const int mode);
int encrypt(char *des,const char *src,const char *key);
int decrypt(char *des,const char *src,const char *key);
int encrypt3des(char * outdata, long * outlen, const char * indata, const long inlen, const char * key);
int decrypt3des(char * outdata, long * outlen, const char * indata, const long inlen, const char * key);

int char2bits(char *bits,const char argc);
int str2bits(char *bits,const char *argv,const int len);
int bit_cyc_shift(char *des,const char *src,const int len,const int offset);
int bit_shift(char *des,const char *src,const int *rule,const int len);

#endif
