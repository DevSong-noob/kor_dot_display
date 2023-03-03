#ifndef _GLOBAL_
#define _GLOBAL_

	#include<p18cxxx.h>
	
	typedef  unsigned short u16;
	typedef  unsigned char u08;
	typedef  unsigned short U16;
	typedef  unsigned char U8;
	typedef  unsigned long u32;
		#define byte	unsigned char
	#define word	unsigned short
	#define KfontLen	24
	typedef struct
	{
		char now_font;
		char kor_sung_array[3];
		char ascii_font;
		char before_kor;
		char only_cho_flag;	
		u16 kor_font_addr;
	}Kor_1word_Def; 


#endif
