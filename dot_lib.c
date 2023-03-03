#include"dot_lib.h"
#include"flash_lib.h"
#include"k2350.h"
//#include"eng_font.h"
//#include"kor_font.h"

	/*
	초성 : ㄱㄲㄴㄷㄸㄹㅁㅂㅃㅅㅆㅇㅈㅉㅊㅋㅌㅍㅎ (19가지)
	(r R s e E f a q Q t T d w W c z x v g)
	(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18)
	 
	중성 :  ㅏㅐㅑㅒㅓㅔㅕㅖㅗㅘㅙㅚㅛㅜㅝㅞㅟㅠㅡㅢㅣ (21가지)
	(k o i O j p u P h hk ho hl y n nj np nl b m ml l)
	(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20)
	 
	종성 : 없음 ㄱㄲㄳㄴㄵㄶㄷㄹㄺㄻㄼㄽㄾㄿㅀㅁㅂㅄㅅㅆㅇㅈㅊㅋㅌㅍㅎ (28가지)
	(  r R rt s sw sg e f fr fa fq ft fx fv fg a q qt t T d w c z x v g)
	(00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27)
	*/
	
	// 전체자수 :
	// 받침 없는 글자 : 초성(19) x 중성(21) = 399 자
	// 받침 있는 글자 : 초성(19) x 중성(21) x 종성(27) = 10,773 자 
	//---------------------------------------------------------------------------
	
	// 초성 : 자음(14) + 쌍자음(5) = 19 개
	//#pragma romdata flash_cho = 0x0e090
	u08 table_cho[21] =
		//{2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
		
		{1, 2,17,3, 4,5,18,6,  7, 8,9,19,10,  11,16,12  ,13,14,20,15, 
		
	//   0   ㄱㅋㄲ   ㄴㄷㅌㄸ  ㄹ   ㅁㅂㅍㅃ  ㅅㅊㅆ      ㅇㅈㅎㅉ 
  		};
		//{0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	
	// 중성 : 자음(10) + 쌍자음(11) = 21 개
	//#pragma romdata flash_joong = 0xE0A6
	u08 table_joong[] = //[30]
		//{3,4,5,6,7,0,0,10,11,12,13,14,15,0,0,18,19,20,21,22,23,0,0,26,27,28,29,0};
	//  0  , ㅏㅑ  ㅓ ㅕ ㅗㅛ   ㅜㅠ   ㅡ  ㅣ 
		{2,  3,5,  7,11,  13,19, 20,26 ,27, 29,		 
	
		  4,  6,10,12,14,15,18,21,22,23};//11~20
		//ㅐ ㅒ ㅔ ㅖ ㅘ ㅙ ㅚ ㅝ ㅟ  ㅢ
	// 종성 : 자음(14) + 쌍자음(11) + 쌍자음(2) = 27 개
	//#pragma romdata flash_jong = 0xE0D6
	u08 table_jong[30] =
		//{2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,0,17,18,19,20,21,22,23,24,25,26,27,28,29};
		{1, 2, 26 ,3 , 5,8,27,  9, 17,19,28,  21,25,22,  24,23,29,
      // 0  ㄱ ㅋ ㄲ  ㄴㄷㅌ   ㄹ  ㅁㅂㅍ    ㅅㅊㅆ     ㅈㅇㅎ
		 4,   6 , 7, 11, 12 ,15, 13, 14 ,16 ,20};
		//ㄳ  ㄵ ㄶ ㄻ ㄼ ㄿ ㄽ ㄾ ㅀ ㅄ 

u08 get_keyfunc()
{
	static key_index=0;
	static char key_flag =0;
	u08 get_key=0;

	u08 key_value = (PORTB&0x10)>>4|(PORTA&0x10)>>3|(PORTA&0x20)>>3|(PORTA&0x08);
	        
	if(key_value==0) 
	{			
		if(key_index++>3) key_index =0;
		key_read(key_index); //줄선택함수 
		key_flag=1;
	}
	else{
		
		if(key_flag)
		{									
			key_flag=0;
			get_key = 	key_value | key_index<<4;
		}
	}// end key routine
	return get_key;
}
void init_gpio(void)
{
	ADCON0 = 0;
	WDTCON |= 0x10;
	
	ANCON0 = 0xff;
	ANCON1 = 0xff;
	WDTCON &= ~(0x10);

	TRISA = 0x38;
	LATA =0;

	TRISB = 0x10;
	LATB = 0;

	TRISC = 0;
	LATC = 0;
	
	TRISD = 0;
	LATD = 0;
	
	TRISE = 0;
	LATE = 0;

	TRISF = 0;
	LATF = 0;

	TRISG = 0;
	LATG = 0;
	
	TRISH = 0;
	LATH = 0;	
	
	TRISJ = 0;
	LATJ = 0;
}

void delay (int del)
{
	while(del)
		del--;	
}

void set_Address(char line)
{
	/*
		열선택..		
	*/
	switch(line)
	{
		case 0:	case 1:case 2:case 3:case 4:
			LATC &= 0xC0;
			LATG = 1<<line;
		break;

		case 5:
			LATG &= 0xE0;
			LATA |= 0x04;
		break;

		case 6:
			LATA &= ~0x04;
			LATC |= 0x02;
		break;
		
		case 7:
			LATC &= ~0x02;
			LATC |= 0x01;
		break;	
		
		default:
			LATC &= 0xC0;
			LATC |= 1<<(line-6);
		break;		
	}
}
void dot_off()
{
	LATB |= 0x0F;
	LATJ = 0xff;
	LATE = 0xff;
	LATH = 0xff;
	LATD = 0xff;
}

void clr_def(Kor_1word_Def* key_data,char len)
{
	char i;
	for(i=0;i<len;i++)
	{		
		key_data[i]->kor_sung_array[0]=1;
		key_data[i]->kor_sung_array[1]=2;
		key_data[i]->kor_sung_array[2]=1;
		key_data[i]->kor_font_addr=0xffff;
		key_data[i]->only_cho_flag=1;
		key_data[i]->ascii_font=0;	
	}
}
void Dotmatrix_output(char line,u08* data1)
{
		
	u08 i;
	u08 mask=0x08;
	
	/*LATB &= 0xF0;
	LATJ = 0;
	LATE &= ~0xfc;
	LATH = 0;*/
	
	LATB |= 0x0F;
	LATJ = 0xff;
	LATE = 0xff;
	LATH = 0xff;
	LATD = 0xff;
	
	LATC &= 0xC0;
	LATC &= ~0x02;
	LATA &= ~0x04;
	LATG &= 0xE0;	
	
	LATB &= ~((data1[0]>>4)&0x0F);
	LATJ &= ~((data1[0])&0x0F);

	LATD &= ~data1[1];
	
	LATE &= ~(data1[2]&0xfc);	
	
	mask = 0x02;
	for(i=0;i<2;i++)
	{
		if(data1[2]&mask) LATH &= ~(1<<i);
		mask >>=1;	
	}	
	
	mask = 0x80;
	for(i=2;i<4;i++)
	{
		if(data1[3]&mask) LATH &= ~(1<<i);
		mask >>=1;	
	}	

	LATE &= ~((data1[3]>>4)&0x03);
	LATH &= ~((data1[3]<<4)&0xf0);
	
	mask = 0x80;
	
	for(i=4;i<8;i++)
	{
		if(data1[4]&mask) LATJ &= ~(1<<i);
		mask >>=1;	
	}	

	set_Address(line);	
}


void init_Timer()
{
 	T0CON=0xC7;	//8비트 모드, 프리스케일러 1:256
				//(12MHz/4)를 256분주 
	TMR0L=158; //256-117=158 -> 0.01초마다 한번씩 인터럽트 발생				 
	
	INTCON = 0xA0; //interrupt enable
}

void key_read(char key_index)
{
	unsigned char key_value = 0;
	
	LATF &= ~0xE4;

	switch(key_index)
	{
		case 0:
			LATF |= 0x80;
		break;
		case 1:
			LATF |= 0x40;
		break;
		case 2:
			LATF |= 0x20;
		break;
		case 3:
			LATF |= 0x04;
		break;
	}	
}

u08 get_num_font4Key(u08 key)
{
	char ctod[9]={0,		// 0
				  1,		// 1
				  2,2,		// 2
 				  3,3,3,3,	// 4
				  4};		// 8
				
	switch(key)
	{
		case KEY_1:	// 01		
		case KEY_2:	// 02 		
		case KEY_3: // 04
			return '0' + ctod[key]; //123
		
		case KEY_4:
		case KEY_5:
		case KEY_6:
			return '3'+ ctod[key&0x0F];//456

		case KEY_0:
			return '0'; // 0
		
		case KEY_7:
		case KEY_8:
		case KEY_9:
			return '6'+ ctod[key&0x0F];//789
		break;
		
		case KEY_A:
			return '#';
				
		case KEY_B:
			return '*';
		
		default:
			return 0xff;
		break;
	}
}

u08 get_small_eng_font4Key(u08 key)
{
	static char roll_next = 0;
	static u08 old_key=0xff;
	char spec_text[8]=	{ '!','?','@','.',
						  ',',':','-','/',};

	if(old_key!=key)
	{
		old_key = key;
		roll_next = 0;
	}
	else
		if(++roll_next>=4) roll_next = 0;

	switch(key)
	{
		case KEY_1:	return 'a'+roll_next;
		case KEY_2:	return 'e'+roll_next;
		case KEY_3: return 'i'+roll_next;
		case KEY_4:	return 'm'+roll_next;		
		case KEY_5:	return 'q'+roll_next;
		case KEY_6:	
			if(roll_next>=3) roll_next = 0;
			return 'u'+roll_next;
		case KEY_7:	
			if(roll_next>=3) roll_next = 0;
			return 'x'+roll_next;
			
		case KEY_8:	return spec_text[roll_next];				
		case KEY_9:	return spec_text[4+roll_next];
		
		case KEY_0:	return ' '; // 0		
		
		default:
			return 0xff;		
	}//end switch
}

u08 get_big_eng_font4Key(u08 key)
{
	static char roll_next = 0;
	static u08 old_key=0xff;
	char spec_text[8]=	{ '!','?','@','.',
						  ',',':','-','/',};

	if(old_key!=key)
	{
		old_key = key;
		roll_next = 0;
	}
	else
		if(++roll_next>=4) roll_next = 0;

	switch(key)
	{
		case KEY_1:	return 'A'+roll_next;
		case KEY_2:	return 'E'+roll_next;
		case KEY_3: return 'I'+roll_next;
		case KEY_4:	return 'M'+roll_next;		
		case KEY_5:	return 'Q'+roll_next;
		case KEY_6:	
			if(roll_next>=3) roll_next = 0;
			return 'U'+roll_next;
		case KEY_7:	
			if(roll_next>=3) roll_next = 0;
			return 'X'+roll_next;
			
		case KEY_8:	return spec_text[roll_next];				
		case KEY_9:	return spec_text[4+roll_next];
		
		case KEY_0:	return ' '; // 0		
		
		default:
			return 0xff;		
	}//end switch
}

//u16 get_kor_font4Key(u08 key,u16 now_kor)
//u16 get_kor_font4Key(u08 key,u08* array_kor_sung)
u16 get_kor_font4Key(u08 key,Kor_1word_Def* ch)
{
	static char roll_next = 0;
	static u08 old_key=0xff;	
	static char is_last_jong = 0;
	//i = 0x8000 | 2<<10 | 3<<5|1;
	//char copy_len = 5,i;
	/*u16 cho=array_kor_sung[0]?array_kor_sung[0]:1;
	u16 joong=array_kor_sung[1]?array_kor_sung[1]:2;
	u16 jong=array_kor_sung[2]?array_kor_sung[2]:1;*/
	
	u16 ch_jong = 0;
	u16 test_cho = ch[2]->kor_sung_array[0];

	u16 cho= ch[2]->kor_sung_array[0];
	u16 joong=ch[2]->kor_sung_array[1];
	u16 jong=ch[2]->kor_sung_array[2];

	u16 korfont = 0;
	
	if(old_key!=key)
	{
		old_key = key;
		roll_next = 0;
	}
	else
		if(roll_next++>=3) roll_next = 0;
	
	switch(key)
	{
		case KEY_1:	
			if(roll_next>2) roll_next =0;			

			if(joong == 2) //초성 
			{
				if(is_last_jong){
					 roll_next =0;
					 is_last_jong =0;		
				}
				cho = table_cho[1+roll_next];
				//ch_jong = ;
			}
			else{//종성 
				is_last_jong = 1;				
				jong = table_jong[1+roll_next];				
			}	
			
		break;

		case KEY_2:				

			if(joong == 2) //초성 
			{	
				if(is_last_jong){
					 roll_next =0;
					 is_last_jong =0;		
				}			
				cho = table_cho[4+roll_next];
				
			}
			else{//종성 
				if(roll_next>2) roll_next =0;	
					is_last_jong = 1;				
					
				if(table_jong[4+roll_next]==27 && jong==9)//ㄾ   
					jong = 14;									
				else 
					jong = table_jong[4+roll_next];					
			}				
			
		break;

		case KEY_4:	//ch_jong = table_cho[8];	break;
			;
			if(joong == 2) //초성 								
				cho = table_cho[8];				
			else
				jong = table_jong[7];			
		break;		
	
		case KEY_5:	//ch_jong = table_cho[9+roll_next];	break;
				//ch_jong = 9+roll_next;	break;
			if(joong == 2) //초성 
			{	
				if(is_last_jong){
					 roll_next =0;
					 is_last_jong =0;		
				}				
				cho = table_cho[9+roll_next];				
			}
			else{//종성 
				if(roll_next>2) roll_next =0;	
				is_last_jong = 1;								
			
				if(jong==9 || (jong >= 11 && jong<=16))		
					jong = table_jong[20+roll_next];
				else
					jong = table_jong[8+roll_next];			
			}				
		break;
		case KEY_7:	
			if(roll_next>2) roll_next =0;			
			if(joong == 2) //초성 
			{		
				if(is_last_jong){
					 roll_next =0;
					 is_last_jong =0;		
				}		
				cho = table_cho[13+roll_next];				
			}
			else{//종성 
				is_last_jong = 1;	
				if(table_jong[11+roll_next]==21 && jong==2)//ㅅ ㄳ		
					jong = 4;									
				else if(table_jong[11+roll_next]==21 && jong==9)//ㅅ  ㄽ	
					jong = 13;									
				else if(table_jong[11+roll_next]==21 && jong==19)//ㅅ ㅄ	
					jong = 20;									
				else
					jong = table_jong[11+roll_next];	
			}				
		break;

		case KEY_8:	//ch_jong = table_cho[16+roll_next];	break;
					//ch_jong = 16+roll_next;	break;	
			if(joong == 2) //초성 
			{	
				if(is_last_jong){
					 roll_next =0;
					 is_last_jong =0;		
				}				
				cho = table_cho[16+roll_next];
				
			}
			else{//종성 
				if(roll_next>2) roll_next =0;
				is_last_jong = 1;
				
				if(jong==5 && (jong>=6 && jong <=7))//ㄵ			
					jong = table_jong[18+(roll_next%2)];
				else if(jong==9)//ㅀ 
					jong = 16;								
				/*if(table_jong[14+roll_next]==24 && jong==5)//ㄵ			
					jong = 6;								
				else if(table_jong[14+roll_next]==29 && jong==5)//ㄶ			
					jong = 7;								
				else if(table_jong[14+roll_next]==29 && jong==9)//ㅀ 
					jong = 16;								*/
				else	
					jong = table_jong[14+roll_next];				
			}	
		break;			
		//
		case KEY_3:	
			if(roll_next>1) roll_next =0;	
			
			if(table_joong[1+roll_next]==3 && joong==13)			
				joong = 14;								
			else	
				joong = table_joong[1+roll_next]; 
		break;	

		case KEY_A:	
			if(roll_next>1) roll_next =0;
			
			if(table_joong[3+roll_next]==7 && joong==20)//ㅜ ㅓ 			
				joong = 21;								
			else	
				joong = table_joong[3+roll_next]; 
			//joong = table_joong[3+roll_next]; 
		break;	

		case KEY_6:	
			if(roll_next>1) roll_next =0;
			
			joong = table_joong[5+roll_next]; 
		break;

		case KEY_0:	
			if(roll_next>1) roll_next =0;
			joong = table_joong[7+roll_next]; 
		break;
		
		case KEY_9:
			/*if(joong==29)			
				joong = 23; 
			else*/
				joong = table_joong[9]; 
		break;

		case KEY_B:
			switch(joong)
			{
				case 3: joong = 4 ;break;//ㅏ+ㅣ=ㅐ
				case 5: joong = 6 ;break;//ㅑ+ㅣ=ㅒ
				case 7: joong = 10 ;break;//ㅓ+ㅣ=ㅔ
				case 11: joong = 12 ;break;	// ㅕ + ㅣ = ㅖ
				case 13: joong = 18 ;break;//ㅗ + ㅣ = ㅚ
				case 14: joong = 15 ;break;//ㅘ + ㅣ = ㅙ
				case 21: joong = 22 ;break;//ㅝ + ㅣ = ㅞ
				case 20: joong = 23 ;break;//ㅜ + ㅣ = n
				case 27: joong = 28 ;break;//ㅡ + ㅣ = ㅢ
				default: joong = 29; 	break;	// ㅣ 
			}			
			
		break; 		
		
		default:	break;	
	}//end switch
	
	/*if(joong != 2 && ch_jong!=0)
		jong = table_cho[ch_jong];		
	else if(joong == 2 && ch_jong!=0)
		cho = table_cho[ch_jong];
	*/	
	korfont = cho << 10 | 0x8000; 	 
	korfont |= jong;	
	
	if(joong == 2 && ch[2]->only_cho_flag) //fille code
	{		
		korfont |= 0x280;//ㅜ 발음	//10<<5
	}
	else{		
		korfont |= joong <<5;				
		ch[2]->only_cho_flag =0;
	}	
	
	
	korfont = inkor2outkor(korfont);

	if(korfont)
	{
		ch[2]->kor_sung_array[0] = cho;
		ch[2]->kor_sung_array[1] = joong;
		ch[2]->kor_sung_array[2] = jong;
		ch[2]->kor_font_addr = KStoDATA(korfont);
		//return 	korfont;
	}
	else
	{
		cho= ch[2]->kor_sung_array[0];
		joong = ch[2]->kor_sung_array[1];
		jong = ch[2]->kor_sung_array[2] ;
		korfont = cho << 10 | 0x8000; 	 
		korfont |= joong <<5;
		korfont |= jong;		
		korfont = inkor2outkor(korfont);
		ch[2]->kor_font_addr = KStoDATA(korfont);
	}
	
	//return korfont;
	
}

void shitf_led_func(u08 *src,char index)
{
	char i;
	for(i=0;i<5;i++)
	{
		src[i]<<=index;
		src[i]|= src[i+1]>>(8-index);
	}
}

void shitf_led_func2(u08 *src,char index)
{
	char i;
	//char index2 = index>8 ? index-8:index;
	
	if(index>=8)
	{
		for(i=0;i<5;i++)
			src[i] = src[i+1];
		index = index-8;
	}
	for(i=0;i<5;i++)
	{
		src[i]<<=index;
		src[i]|= src[i+1]>>(8-index);
	}
}

//void get_engFontData(u08 line,u08* buf,u08 *dst,char index,char len)

word KStoDATA(word code)
{
	byte high,low;
	word index;

	high = (code>>8)&0xff;
	low  =  code    &0xff;
	if ((code>=0xb0a1)&&(code<=0xc8fe))
	{
		index = (high-0xb0);
		index *=94;
		index +=low-0xa1;
		return (index*KfontLen);
	} else return 0xffff;
}	


u16 inkor2outkor(u16 kor_data)
{
	u16 a,z,k=0,i,xx;
	a = 0;
	z=2350-1;
	i=(a+z)/2;
	
	while(1)
	{
		xx = KSC2KSSM[i][1];
		if(KSC2KSSM[i][1]>kor_data)
		{	
			z=i;	
			i=(a+z)/2;
				
		}
		else if(KSC2KSSM[i][1]<kor_data)
		{				
			a=i;
			i=(a+z)/2;						
		}		
		else
			break;

		if(a==i) 
		{
			k++;
			if(k>=2) {
				i=-1;
				break;	
			}
		}
		else	k=0;
	}
	
	return KSC2KSSM[i][0];
}

char get_kor_fontdata2(u08* buf,Kor_1word_Def *fofont)
{
	char j,i=0,k=0;
	char index =0;
	u08 kor_data[2];
	u16 k_i;

	//tmp = fofont[1]->ascii_font;	
	do{
		if(fofont[i]->ascii_font!=0)
		{
			index = get_eng_fontdata(buf,index,fofont[i]->ascii_font);
			k+=2;
		}
		else if(fofont[i]->kor_font_addr!=0xffff)
		{
			index = get_kor_fontdata(buf,index,fofont[i]->kor_font_addr,fofont[i]->kor_sung_array[0],fofont[i]->only_cho_flag);
			k+=3;
		}
		else{		
			index = get_eng_fontdata(buf,index,' ');
			k+=2;		
		}
		i++;
	}while(k<8);
	return k;

}