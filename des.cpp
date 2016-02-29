#include "stdafx.h"
#include <stdio.h>
#include <memory.h>
#include "esb_des.h"

#define DEBUG 1 

const static int IP_K1[56] = {
	56,48,40,32,24,16,8,0,
	57,49,41,33,25,17,9,1,
	58,50,42,34,26,18,10,2,
	59,51,43,35,62,54,46,38,
	30,22,14,6,61,53,45,37,
	29,21,13,5,60,52,44,36,
	28,20,12,4,27,19,11,3
	};
const static int IP_K2[48] = {
	13,16,10,23,0,4,2,27,
	14,5,20,9,22,18,11,3,
	25,7,15,6,26,19,12,1,
	40,51,30,36,46,54,29,39,
	50,44,32,47,43,48,38,55,
	33,52,45,41,49,35,28,31
	};    
const static int IP_KR[16] = {
	1,1,2,2,2,2,2,2,
	1,2,2,2,2,2,2,1
	};   
const static int IP_1[64] = {
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7,
	56,48,40,32,24,16,8,0,
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6	
	};
const static int IP_2[64] = {
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25,
	32,0,40,8,48,16,56,24
	};
const static int IP_E[48] = {
	31,0,1,2,3,4,3,4,
	5,6,7,8,7,8,9,10,
	11,12,11,12,13,14,15,16,
	15,16,17,18,19,20,19,20,
	21,22,23,24,23,24,25,26,
	27,28,27,28,29,30,31,0
	};
const static int IP_P[32] = {
	15,6,19,20,28,11,27,16,
	0,14,22,25,4,17,30,9,
	1,7,23,13,31,26,2,8,
	18,12,29,5,21,10,3,24
	};
const static int IP_S[64] = {
	-1,-1,0,5,1,2,3,4,
	-1,-1,6,11,7,8,9,10,
	-1,-1,12,17,13,14,15,16,
	-1,-1,18,23,19,20,21,22,
	-1,-1,24,29,25,26,27,28,
	-1,-1,30,35,31,32,33,34,
	-1,-1,36,41,37,38,39,40,
	-1,-1,42,47,43,44,45,46
	};
const static int S[8][64] ={
	{
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
	},  
  {
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,  
  3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,  
  0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,  
  13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
	},  
  {
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,  
  13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,  
  13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,  
  1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
	},  
  {
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,  
  13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,  
  10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,  
  3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
	},  
  {
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,  
  14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,  
  4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,  
  11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
	},  
  {
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,  
  10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,  
  9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,  
  4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
	},  
  {
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,  
  13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,  
  1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,  
  6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
	},  
  {
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,  
  1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,  
  7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,  
  2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
	}
	};  

int encrypt3des(char * outdata, long * outlen, const char *indata , const long inlen, const char * key)
{
	int ret = 0;
        int modelen;
	long i=0,j=0; /*test 20100126*/
        char lastdata [8];
        char key1[8],key2[8],key3[8];
	
	if( !( outdata && indata && key ) )
	{
		printf("outdata,indata and key could not be null\n");
		ret = -1;  IF_ERR;
	}
	
	/* appending value to last 8 data */
	modelen = inlen%8;
	i=8-modelen;
	memset(lastdata,i,sizeof(lastdata));
	if(modelen!=0)
	{
		memcpy(lastdata,indata+(inlen-modelen),modelen);
	}

	memset(key1,0x00,sizeof(key1));
	memcpy(key1,key,8);
	memset(key2,0x00,sizeof(key2));
	memcpy(key2,key+8,8);
	memset(key3,0x00,sizeof(key3));
	memcpy(key3,key+16,8);
	
	*outlen=0;

  for(i=0,j=inlen>>3; i<j; ++i,outdata+=8,indata+=8,*outlen+=8)
	{
		ret = encrypt(outdata, indata,  key1);  IF_ERR;
		ret = decrypt(outdata, outdata, key2);  IF_ERR;
		ret = encrypt(outdata, outdata, key3);  IF_ERR;
	}
	
	ret = encrypt(outdata, lastdata, key1);  IF_ERR;
	ret = decrypt(outdata, outdata, key2);  IF_ERR;
	ret = encrypt(outdata, outdata, key3);  IF_ERR;
	*outlen+=8;

	return ret;
}

int decrypt3des(char * outdata, long * outlen, const char *indata,const long inlen, const char * key)
{
	char key1[8],key2[8],key3[8];
	int ret = 0;
	long i=0,j=0;
	if( !( outdata && indata && key ) )
	{
		printf("outdata,indata and key could not be null\n");
		return -1;
	}

	if(inlen%8!=0)
	{
		printf("datalen is error:inlen%8!=0\n");
		return -1;
	}
	

	memset(key1,0x00,sizeof(key3));
	memcpy(key1,key,8);
	memset(key2,0x00,sizeof(key2));
	memcpy(key2,key+8,8);
	memset(key3,0x00,sizeof(key1));
	memcpy(key3,key+16,8);
	
	*outlen = 0;
	for(i=0,j=inlen>>3; i<j; ++i,outdata+=8,indata+=8,*outlen+=8)
	{
		ret = decrypt(outdata, indata,  key3);  IF_ERR;
		ret = encrypt(outdata, outdata, key2);  IF_ERR;
		ret = decrypt(outdata, outdata, key1);  IF_ERR;
	}
	
	outdata-=8;
	i=outdata[7];
	memset(outdata+(8-i),0x00,i);	
	*outlen-=i;

	return ret;
}


int encryptdes(char * outdata, int * outlen, const char *indata , const int inlen, const char * key)
{
	int ret = 0;
  int modelen;
	int i=0,j=0;
  char lastdata [8];
  char key1[8];
	
	if( !( outdata && indata && key ) )
	{
		printf("outdata,indata and key could not be null\n");
		ret = -1;  IF_ERR;
	}

	modelen = inlen%8;
	i=8-modelen;
	memset(lastdata,i,sizeof(lastdata));
	if(modelen!=0)
	{
		memcpy(lastdata,indata+(inlen-modelen),modelen);
	}

	memset(key1,0x00,sizeof(key1));
	memcpy(key1,key,8);
	
	*outlen=0;
  for(i=0,j=inlen>>3; i<j; ++i,outdata+=8,indata+=8,*outlen+=8)
	{
		ret = encrypt(outdata, indata,  key1);  IF_ERR;
	}
	
	ret = encrypt(outdata, lastdata, key1);  IF_ERR;
	*outlen+=8;

	return ret;
}

int decryptdes(char * outdata, long * outlen, const char *indata,const long inlen, const char * key)
{
	char key1[8];
	int ret = 0;
	long i=0,j=0;
	if( !( outdata && indata && key ) )
	{
		printf("outdata,indata and key could not be null\n");
		return -1;
	}

	if(inlen%8!=0)
	{
		printf("datalen is error:inlen%8!=0\n");
		return -1;
	}
	

	memset(key1,0x00,sizeof(key1));
	memcpy(key1,key,8);
	
	*outlen = 0;
	for(i=0,j=inlen>>3; i<j; ++i,outdata+=8,indata+=8,*outlen+=8)
	{
		ret = decrypt(outdata, indata,  key1);  IF_ERR;
	}
	
	outdata-=8;
	i=outdata[7];
	memset(outdata+(8-i),0x00,i);
	*outlen-=i;
	return ret;
}

int encrypt(char *des,const char *src,const char *key){
	int ret=0;
	char subkeys[16][6];
	
	memset(subkeys,0x00,sizeof(subkeys));
	
	ret=opr_key(subkeys,key,0);						IF_ERR; 	
	ret=opr_data(des,src,subkeys,0,0);		IF_ERR; 
	
	return ret;
}

int decrypt(char *des,const char *src,const char *key){
	int ret=0;
	char subkeys[16][6];
	
	memset(subkeys,0x00,sizeof(subkeys));
	
	ret=opr_key(subkeys,key,0);						IF_ERR; 
	ret=opr_data(des,src,subkeys,0,1);		IF_ERR; 
	
	return ret;
}


int opr_data(char *des,const char *src,char subkeys[16][6],const int seq,const int mode){
	int ret=0;
	short l=0,r=0;
	char IP1[8];
	char IPE[6];
	char IPE1[6];
	char IPB[8];
	char IPS[4];
	char IPP[4];
	char tmp;
	int i=0;/*test 20100126*/
	memset(IP1,0x00,sizeof(IP1));
	memset(IPE,0x00,sizeof(IPE));
	memset(IPB,0x00,sizeof(IPB));
	memset(IPS,0x00,sizeof(IPS));
	memset(IPP,0x00,sizeof(IPP));
	
	if(seq==0){
		/**IP_1：第一次换位运算**/	
		ret=bit_shift(IP1,src,IP_1,LEN_IP_1);						IF_ERR;
	}else{
		memcpy(IP1,src,8);
	}

	/**IP_E1：IP_1运算结果后边部分的扩展换位运算**/ 
	ret=bit_shift(IPE,IP1+4,IP_E,LEN_IP_E);						IF_ERR;
				
	/**IP_E2: IP_E1运算结果与对应的子密钥进行异或运算**/ 
	/**判断是加密还是解密,mode=0 加密；mode!=0 解密**/	
	if(mode==0){
		/*for(int i=0;i<6;i++){ */   /*test 20100126*/
		for(i=0;i<6;i++){
			IPE1[i]=IPE[i]^subkeys[seq][i];
		}
	}else{
		/*for(int i=0;i<6;i++){ */ /*test 20100126*/
		for(i=0;i<6;i++){ 
			IPE1[i]=IPE[i]^subkeys[15-seq][i];
		}
	}
		
	/**IP_B: 根据IP_E2运算结果计算S盒坐标**/
	ret=bit_shift(IPB,IPE1,IP_S,LEN_IP_S);						IF_ERR;	

	/**IP_S: 根据S盒坐标进行S盒运算**/
	/*for(int i=0;i<4;i++){*/   /*test 20100126*/
	for(i=0;i<4;i++){
		l=2*i;r=l+1;
		IPS[i]=(S[l][IPB[l]]<<4)|(S[r][IPB[r]]);
	}
		
	/**IP_P: 对S盒运算结果进行换位运算**/ 
	ret=bit_shift(IPP,IPS,IP_P,LEN_IP_P);							IF_ERR;
		
	/**递归调用**/ 
	if(seq==15){/**递归出口**/		
		/*for(int i=0;i<4;i++){*/  /*test 20100126*/
		for(i=0;i<4;i++){
			IP1[i]^=IPP[i];
		}						
		ret=bit_shift(des,IP1,IP_2,LEN_IP_2);						IF_ERR;
		return ret;
	}else{
		/**IP_K: 对IP_P运算结果与IP_1运算结果前边部分进行异或运算得到新的IP1后递归运算**/ 
		/*for(int i=0;i<4;i++){*/ /*test 20100126*/
		for(i=0;i<4;i++){
			tmp=IPP[i]^IP1[i];
			IP1[i]=IP1[i+4];
			IP1[i+4]=tmp;
		}	
		ret=opr_data(des,IP1,subkeys,seq+1,mode);				IF_ERR;
	}
	return ret;
} 

int opr_key(char subkeys[16][6],const char *key,const int seq){
	int ret=0;
	char shiftkey[7];
  char cycshiftkey[7];
    
  memset(shiftkey,0x00,sizeof(shiftkey));
  memset(cycshiftkey,0x00,sizeof(cycshiftkey));

	if(seq==0){
		/**密钥的第一次置换**/	 
    ret=bit_shift(shiftkey,key,IP_K1,LEN_IP_K1);						IF_ERR;
    /**循环左移**/
    ret=bit_cyc_shift(cycshiftkey,shiftkey,IP_KR[seq],7); 	IF_ERR;  
	}else{
		ret=bit_cyc_shift(cycshiftkey,key,IP_KR[seq],7); 				IF_ERR;  	
	}
	/**第二次变位**/
  ret=bit_shift(subkeys[seq],cycshiftkey,IP_K2,LEN_IP_K2);	IF_ERR;
      
	/**递归调用**/ 
	if(seq==15) return ret;/**递归出口**/ 
	ret=opr_key(subkeys,cycshiftkey,seq+1);										IF_ERR;
	
	return ret;
}

int char2bits(char *bits,const char argc){
	int i=0; /*test 20100126*/
    /*for(int i=0;i<8;i++){*/ /*test 20100126*/
    for(i=0;i<8;i++){
		*(bits+i)=(argc>>(7-i))&0x00000001;
		if(!DEBUG) printf("%d",*(bits+i));
	}
	if(!DEBUG) printf(" ");
	return 0;
}

int str2bits(char *bits,const char *argv,const int len){
    	int i=0; /*test 20100126*/
	/*for(int i=0;i<len;i++){*/  /*test 20100126*/
	for(i=0;i<len;i++){
		char2bits(bits+(i*8),*(argv+i));
	}
	if(!DEBUG) printf("\n");
	return 0;
}

int bit_cyc_shift(char *des,const char *src,const int offset,const int len){
    
    	int i=0; /*test 20100126*/
	char mark[8]={0x01,0x03,0x07,0x0f,0x10,0x30,0x70,0xf0};
	if(!(0<offset<4)) return -1;

	
	/*for(int i=0;i<len;i++){*/ /*test 20100126*/
    for(i=0;i<len;i++){
		des[i]=0x00;
		
		if(i*2+1==len){		
			des[i] = ((src[i]&0xf0)<<offset) | ((src[0]>>(4-offset))&mark[offset+3])
						 | ((src[i]<<offset)&0x0f) | ((src[i+1]>>(8-offset))&mark[offset-1]);
		}else if(len==(i+1)){		
			des[i] = (src[i]<<offset) | ((src[i/2]&0x0f)>>(4-offset));
		}else{	
			des[i] = (src[i]<<offset) | ((src[i+1]>>(8-offset))&mark[offset-1]);
		}
	}
	return 0;	
}

int bit_shift(char *des,const char *src,const int *rule,const int des_len){
	char mark[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	int x=0,y=0,indx=-1;
		int i=0,j=0; /*test 20100126*/
	/*for(int i=0;i<des_len;i++){*/ /*test 20100126*/
	for(i=0;i<des_len;i++){
		des[i]=0x00;
		/*for(int j=0;j<8;j++){*/  /*test 20100126*/
		  for(j=0;j<8;j++){
			indx++;
			if(rule[indx]<0) continue;
			
			x=rule[indx]/8;
			y=rule[indx]%8;
			
			if(src[x]&mark[y]) des[i]|=mark[j];
		}
	}
	return 0;
}

