/*
	6.28 
	수정사항 : 초성만 누렀을때, 중성 안ㄴ눌린것처럼 처리하기
	아 발음 으로 초성만 출력처리하자
	남은 일은 플래쉬 저장 및 출력 
	얼마 안남았음
	플래쉬에 저장하면 글자가 아닌 바이트 단위의 도트..
	에햇
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

#define IDLE_MODE		0
#define SHIFT_MODE		1
#define KEY_INPUT_MODE	2
#define LINE_CH_MODE	3
#define EDIT_SAVE_MODE	4

/*
	전역변수 선언
*/
volatile char time_out = 0; // 시프트를 위한 변수

volatile char font_index = 0; // 시프트를 위한 변수, 
volatile u08 shift_index = 0; // 시프트를 위한 변수, 
volatile u08 shift_index2 = 0; // 시프트를 위한 변수, 
volatile char time_stop = 0;

volatile char Now_Mode = IDLE_MODE;
volatile char onesec_flag = 0;
//volatile u16 key_font_addr = 0x7800;
volatile char edit_index = 0;

volatile u16 kor_font = 0xb0a1;
volatile u16 key_font = 0x8861;


#define STR_FLASH_LEN	(100)



#pragma romdata flash_string = 0x8000
rom u08 Line1[STR_FLASH_LEN] = "Made:A&I(Lim,Kim,Song)";

// 0~4 : line len

u32 addr_a[]={Line1};
far rom u16 *rom_pointer ;

volatile char len = 0;
volatile char line_index = 0;


volatile char flash_flag  = 0;
volatile u08 get_key = 0;
volatile char flash_len = 0;

void INTCHK(void);

u16 tmp_font_data[32];
//u08 tmp_data[72];
u32 addr;

void main(void)
{
	char i = 0,j;
	u08 font = 0;
	u08 x;

	
	Kor_1word_Def key_data[3];

	u08 disp_data[10]={0,};
	
	
	
	char kor_len = 4;	
	

	u08 kor_sung_array[3]={1,2,1};	//fill

	u16 kor_array[4]={0,24,48,72};
	unsigned long startaddr;
	
	

	for(i=0;i<32;i++)
		tmp_font_data[i]=0xffff;

	for(i=0;i<3;i++){
		key_data[i].kor_sung_array[0]=1;
		key_data[i].kor_sung_array[1]=2;
		key_data[i].kor_sung_array[2]=1;
		key_data[i].kor_font_addr=0xffff;
		key_data[i].only_cho_flag=1;
	}	

	init_gpio();	
	init_Timer();
	rom_pointer = (rom u16 *)Line1;
	
	
	
	while(1){
		
		//INTCON &= ~0x80;
		for(i=0;i<12;i++)
		{
			if(flash_flag){
				get_kor_test3(disp_data,i,rom_pointer,flash_len,font_index);
				shitf_led_func2(disp_data,shift_index);				
			}
			else{				
					
				get_kor_test2(disp_data,i,&key_data,3);		
			}	

			Dotmatrix_output(i,disp_data);		
			Delay10TCYx(3);
		}// 디스플레이 루틴!!!!
		//INTCON |= 0x80;	

		if(time_out)
		{	
			shift_index++;			
			//shift_index2++;
			if(shift_index>=12)
			{
				shift_index=0;

				font_index++;
					
				if(font_index>flash_len)
					font_index = -4;
			}				
								
				time_out =0;
		}
		
			
		
		// 키입력 처리 루틴!			
		if(get_key !=0)
		{			
				dot_off();
				Delay1KTCYx(1);
				if(get_key>= KEY_1 && get_key<=KEY_B){
					 
					get_kor_font4Key(get_key,&key_data);
					flash_flag =0;
				}
				else if(get_key==KEY_OK)
				{						
					//현재한글이 없는 갑이면 널값으로 할것!!
					if(flash_flag==0){ 
						key_data[2].kor_sung_array[0] =1;
						key_data[2].kor_sung_array[1] =2;
						key_data[2].kor_sung_array[2] =1;
							
						key_data[0]= key_data[1];	
						key_data[1]= key_data[2];	
						
						tmp_font_data[edit_index] = key_data[2].kor_font_addr;
						key_data[2].kor_font_addr = 0xffff;	
						key_data[2].only_cho_flag=1;
						edit_index++;
						if(edit_index>=32)	edit_index=32;
						kor_len = edit_index;					
					}
				}
				else if(get_key == KEY_M0)
				{
					if(flash_flag==0)
					{						
						//key_data[2]= key_data[1];	
						if(key_data[2].kor_font_addr == 0xffff)
						{
							if(edit_index<=0)	edit_index=0;
							else 	edit_index--;								
							
							tmp_font_data[edit_index] =0xffff;
	
							key_data[1]= key_data[0];	
							
							if(edit_index<3)
								key_data[0].kor_font_addr = 0xffff;				
							else 
								key_data[0].kor_font_addr = tmp_font_data[edit_index-3];								
												
						}
							key_data[2].kor_sung_array[0] =1;
							key_data[2].kor_sung_array[1] =2;
							key_data[2].kor_sung_array[2] =1;
							key_data[2].kor_font_addr = 0xffff;	
							key_data[2].only_cho_flag=1;
						
						
					}
				}
				else if(get_key==KEY_M2)
				{
					INTCON &= ~0x80;
					addr = addr_a[0];
					EraseFlash2(addr,addr+1);
					addr = addr_a[0];	
					WriteBlockFlash2(addr,1,tmp_font_data);
					
					flash_len = edit_index+1;
					edit_index=0;
					flash_flag =1;		
					font_index  = -4;
					shift_index =0;

						for(i=0;i<3;i++){
							key_data[i].kor_sung_array[0]=1;
							key_data[i].kor_sung_array[1]=2;
							key_data[i].kor_sung_array[2]=1;
							key_data[i].kor_font_addr=0xffff;
							key_data[i].only_cho_flag=1;
						}	

					INTCON |= 0x80;		
				}
				
			get_key =0;
		}// end if
	}
	
	while(1);
	
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
	static char t_cnt = 0;
	static char cnt_1s = 0;
	static char key_flag = 1;
	static char key_index = 0;

	u08 key_value;

	if(INTCONbits.TMR0IF)
	{	
		/*
			키입력에 대한 새로운 루틴			
		*/
		key_value = (PORTB&0x10)>>4|(PORTA&0x10)>>3|(PORTA&0x20)>>3|(PORTA&0x08);
	        
		if(key_value==0) 
		{			
			if(key_index++>3) key_index =0;
			key_read(key_index);
			key_flag=1;
		}
		else{
			
			if(key_flag)
			{									
				key_flag=0;
				get_key = 	key_value | key_index<<4;
			}
		}// end key routine
			
		if(++t_cnt > 2) // x 10ms 
		{
			t_cnt = 0;
			time_out=1;		
		}
		
		TMR0L=256-117; //256-117=158 -> 0.01초마다 한번씩 인터럽트 발생				 	
		INTCONbits.TMR0IF=0;// 타이머0 인터럽트 요구 플래그 클리어 시킴
	}		
	
}
