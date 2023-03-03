#ifndef _DOT_LIB_
#define _DOT_LIB_

	#include<p18cxxx.h>
	#include"global.h"
	#include<delays.h>


	#define KEY_1		0x01
	#define KEY_2		0x02
	#define KEY_3		0x04

	#define KEY_A		0x08

	#define KEY_4		0x11
	#define KEY_5		0x12
	#define KEY_6		0x14

	#define KEY_0		0x18

	#define KEY_7		0x21
	#define KEY_8		0x22
	#define KEY_9		0x24
	
	#define KEY_B		0x28
	
	
	#define KEY_OK		0x31
	#define KEY_M0		0x32
	#define KEY_M1		0x34	
	#define KEY_M2		0x38
	
	#define KEY_ERROR		0xff
	
	#define KOR		0
	#define ASCII	1


	enum{DISP_MODE,ENG_MODE,NUM_MODE,SPEC_MODE};
	
	u08 get_keyfunc();

	void init_gpio(void);		
	void delay (int del);
	void set_Address(char line);
	void Dotmatrix_output(char line,u08* data1);
	void ch_array(unsigned char* src, rom unsigned char *dst,int index);
	
	void init_Timer(void);
	void key_read(char key_index);
	// 6.22 키입력을 10ms단위의 타이머에게 전달..
	void clr_def(Kor_1word_Def* key_data,char len);
	
	
	u08 get_small_eng_font4Key(u08 key);
	u08 get_big_eng_font4Key(u08 key);
	u08 get_num_font4Key(u08 key);	
	u16 get_kor_font4Key(u08 key,Kor_1word_Def* ch)	;

	void ch_array(unsigned char* src, rom unsigned char *dst,int index);

	void PutString(rom char *str,u08 line,char index,char len);
	u16 inkor2outkor(u16 kor_data);
	void dot_off();
	void shitf_led_func2(u08 *src,char index);
	word KStoDATA(word code);
	char get_kor_fontdata2(u08* buf,Kor_1word_Def *fofont);
#endif