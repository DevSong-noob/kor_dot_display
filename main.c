/*

*/
#include<p18cxxx.h>
#include"dot_lib.h"
#include"flash_lib.h"
#include<delays.h>

#pragma config DEBUG =ON
#pragma config FOSC = HSPLL
#pragma config WDTEN = OFF
#pragma config IESO = OFF
#pragma config XINST = OFF
#pragma config FCMEN = OFF
#pragma config STVREN = OFF
#pragma config PLLDIV = 3
#pragma config CPUDIV = OSC1
//#pragma config BW = 16
#pragma config CP0 = OFF
#pragma config MODE = MM

//////////////=
// 프로그램 시작 

#define DISP_MODE			0
#define KOR_EDIT			1
#define ENG_BIG_EDIT		2
#define NUM_EDIT			3

u08 disp_data[10]={0,};
u16 tmp_font_data[32]={0,};
u08 tmp_data[80]={0,};


#pragma romdata flash_1 = 0x14000
rom u08 Line1_string[512];


//u32 addr_a[10]={&Line1,&Line2};

u16 font_save[32]={0,};

volatile char mode_flag = ENG_BIG_EDIT;

void INTCHK(void);

void main(void)
{
	char i,x,j=0;
	char key_shift = 0;
	char kor_len = 4;	

	u16 edit_index = 0;	
	Kor_1word_Def key_data[6];

	init_gpio();
	
//	EraseFlash2(Line1,Line1+1);

//	WriteBlockFlash2(addr_a[0],1,tmp_data);

	clr_def(&key_data,5);	
	init_Timer();
	
	while(1)
	{
		x = get_keyfunc();
		if(x!=0)
		{
			x = 0;
		}
		
		Delay1KTCYx(3);//1ms
	}
}


#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR (void) 
{
  _asm
    goto INTCHK
  _endasm
}
#pragma code 

#pragma interrupt INTCHK
void INTCHK(void)
{	
	static char i=0,j=0;
	static char cnt =0;
	if(INTCONbits.TMR0IF)
	{		
	/*	disp_data[0] = Line1[(j*5)+0];
		disp_data[1] = Line1[(j*5)+1];
		disp_data[2] = Line1[(j*5)+2];
		disp_data[3] = Line1[(j*5)+3];
		disp_data[4] = Line1[(j*5)+4];*/

		disp_data[0] = tmp_data[(j*5)+0];
		disp_data[1] = tmp_data[(j*5)+1];
		disp_data[2] = tmp_data[(j*5)+2];
		disp_data[3] = tmp_data[(j*5)+3];
		disp_data[4] = tmp_data[(j*5)+4];
		
	
		Dotmatrix_output(j,disp_data);	
		i+=2;j++;
		if(i>=24)
		{
			i=0;j=0;
		}
	
		TMR0L=256-11; //256-117=158 -> 0.001초
		INTCONbits.TMR0IF=0;// 타이머0 인터럽트 요구 플래그 클리어 시킴
	}		
	
}
