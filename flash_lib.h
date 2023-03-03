#ifndef _FLASH_LIB_
#define _FLASH_LIB_

	#include<p18cxxx.h>
	#include"global.h"


	void ReadFlash2(unsigned long startaddr, unsigned int num_bytes, unsigned char *flash_array);
	void WriteBlockFlash2(unsigned long startaddr, unsigned char num_blocks, unsigned char *flash_array);
	
	
	
	//void get_eng_fontdata(u08* buf,char index,char ch1,char flag);
	void get_engFontData(u08 line,u08* buf,u08 *dst,char index,char len); //라인에 따른 그 폰트의 바이트를 리턴
	
	void put_kor(u08 line,u08* buf,u08 *dst,char index,char len);
	//void get_kor_fontdata(u08* buf,char index,u16 ch2,char flag);
	
	char get_kor_fontdata(u08* buf,char index,u16 ch,char cho,char flag);
/*
6.16 test 한글출력팜수
*/
	char get_eng_fontdata(u08* buf,char index,char ch);
	void get_kor_test(u08* buf,char index,u16 *ch2,char len);
	void get_kor_test2(u08* buf,char index,Kor_1word_Def *ch,char len);
	void get_kor_test3(u08* buf,char index,rom u16 *ch,char len,char font_index);
	//void get_kor_test3(u08* buf,char index,rom u16 *ch,char len);
	void EraseFlash2(unsigned long startaddr, unsigned long endaddr);
	void WriteBlockFlash3(unsigned long startaddr, unsigned char num_blocks, unsigned char *flash_array,char len);
#endif