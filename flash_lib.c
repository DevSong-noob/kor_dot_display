#include"flash_lib.h"
#include"eng_font.h"
#include"kor_font.h"


void ReadFlash2(unsigned long startaddr, unsigned int num_bytes, unsigned char *flash_array)
{
		u08 len=0;
		TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
		TBLPTRH = (((unsigned int)startaddr)>>8);	
		TBLPTRL	= (startaddr);

		while(num_bytes--)
		{
			//*********** Table read sequence ******************************
			_asm	TBLRDPOSTINC _endasm
			*(flash_array + len) = TABLAT;
			len++;
		}	

}

void EraseFlash2(unsigned long startaddr, unsigned long endaddr)
{
		unsigned char flag=0;

		//startaddr =(unsigned long)((startaddr / 0x400)) *0x400;	//Calculate starting address of the block
		startaddr =((startaddr & 0x1fc00));	//Calculate starting address of the block
		while(startaddr<endaddr)
		{
			TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
			TBLPTRH = (startaddr)>>8;	
			TBLPTRL	= (startaddr);
			//*********Flash Erase sequence*****************
			EECON1bits.WREN = 1;
			EECON1bits.FREE = 1;
			if(INTCONbits.GIE)
			{
				INTCONbits.GIE = 0;
				flag=1;
			}
			EECON2 = 0x55;
			EECON2 = 0xAA;
			EECON1bits.WR = 1;
			if(flag)
				INTCONbits.GIE = 1;
			
			startaddr = startaddr +0x400;
		}
}

void WriteBlockFlash2(unsigned long startaddr, unsigned char num_blocks, unsigned char *flash_array)
{
		unsigned char write_byte=0,flag=0;

		startaddr = (startaddr & 0x1ffc0) ;	//Calculate starting address of the block
		
		while(num_blocks--)
		{
				TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
				TBLPTRH = (startaddr)>>8;	
				TBLPTRL	= (startaddr);
				
				write_byte = 64;
				while(write_byte--)
				{
					TABLAT = *(flash_array++);
					_asm  TBLWTPOSTINC 	_endasm
				
				}		

			  TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
			  TBLPTRH = ((startaddr)>>8);	
			  TBLPTRL = (startaddr);
			  //*********** Flash write sequence ***********************************
			  EECON1bits.WREN = 1;
			  if(INTCONbits.GIE)
			  {
				INTCONbits.GIE = 0;
				flag=1;
			  }		  
			  EECON2 = 0x55;
			  EECON2 = 0xAA;
			  EECON1bits.WR =1;
			  EECON1bits.WREN = 0 ; 
			  if(flag)
			  {
				INTCONbits.GIE = 1;	
				flag=0;
			  }
	  
			 startaddr = startaddr + 0x40;									//increment to one block of 64 bytes
		}
		
}

void WriteBlockFlash3(unsigned long startaddr, unsigned char num_blocks, unsigned char *flash_array,char len)
{
		unsigned char write_byte=0,flag=0;

		startaddr = (startaddr & 0x1ffc0) ;	//Calculate starting address of the block
		
		while(num_blocks--)
		{
				TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
				TBLPTRH = (startaddr)>>8;	
				TBLPTRL	= (startaddr);
				
				write_byte = len;
				while(write_byte--)
				{
					TABLAT = *(flash_array++);
					_asm  TBLWTPOSTINC 	_endasm
				
				}		
				write_byte = 64-len;
				while(write_byte--)
				{
					TABLAT = 0xff;
					_asm  TBLWTPOSTINC 	_endasm
				
				}	
			  TBLPTRU = (startaddr>>16);						//Load the address to Address pointer registers
			  TBLPTRH = (startaddr>>8);	
			  TBLPTRL = (startaddr);
			  //*********** Flash write sequence ***********************************
			  EECON1bits.WREN = 1;
			  if(INTCONbits.GIE)
			  {
				INTCONbits.GIE = 0;
				flag=1;
			  }		  
			  EECON2 = 0x55;
			  EECON2 = 0xAA;
			  EECON1bits.WR =1;
			  EECON1bits.WREN = 0 ; 
			  if(flag)
			  {
				INTCONbits.GIE = 1;	
				flag=0;
			  }
	  
			 startaddr = startaddr + 0x40;									//increment to one block of 64 bytes
		}
		
}
void get_engFontData(u08 line,u08* buf,u08 *dst,char index,char len) //라인에 따른 그 폰트의 바이트를 리턴
{
	//u08 _ret_data = 0;	
	char i;
		
	//if(buf[index]<0x20 || len<index || index<0) return 0;
	for(i=0;i<5;i++)
	{
		 if((buf[index+i]<0x20) || (buf[index+i]>0x80) || len<(index+i) || (index+i)<0) 
			dst[i] = 0;
		else
			dst[i]= ASCII_fontTable[(buf[index+i]-0x20)*12L+line];
	}
	
		
	//return _ret_data;
}	

void get_eng_fontdata(u08* buf,char index,char ch1,char flag)
{
	char j;
 	u08 tmp;
	
	if(flag)
	{
		for(j=0;j<12;j++)
		{
			tmp = ASCII_fontTable[(ch1-0x20)*12L+j];			
			buf[((index-1)*12)+j] |= (tmp>>4)&0x0f;
			buf[((index)*12)+j] = (tmp)&0x0f;
		}
	}
	else
	{
		for(j=0;j<12;j++)	
			buf[(index*12)+j]= ASCII_fontTable[(ch1-0x20)*12L+j];
	}
}

void get_kor_fontdata(u08* buf,char index,u16 *ch)
{
	char j,i;
	u08 tmp;
	u08 kor_data[2];
	u16 k_i;

	for(i=0;i<4;i++)
	{
		for(j=0;j<24;j+=2)
		{			
			k_i = ch[i]+j;			
			kor_data[0] = KS_fontTable[k_i];		
			kor_data[1] = KS_fontTable[k_i+1];		
			
			if(i==0)// 
			{						
				buf[j] = kor_data[0];
				buf[j+1] &= 0x0F;
				buf[j+1] |= kor_data[1]&0xF0;
			}
			else if(i==1)
			{
				buf[j+1] &= 0xF0;
				buf[j+1] |= (kor_data[0]>>4)&0x0F;
				buf[j+24] = (kor_data[0]<<4)|(kor_data[1]>>4);		
			}
			else if(i==2)
			{			
				buf[j+24+1] = kor_data[0];	
				buf[j+48] &= 0x0F;
				buf[j+48] |= kor_data[1] & 0xF0;						
			}
			else if(i==3)
			{			
				buf[48 +j] &= 0xF0;	
				buf[48 +j] |= (kor_data[0]>>4)&0x0F;
			
				buf[48+j+1] = ((kor_data[0]<<4)&0xf0)|((kor_data[1]>>4)&0x0F);							
			}
			//buf[(i*24)+j] = KS_fontTable[ch2[i]+(j)];
		}	
	}	
}

/*
6.16 test 한글출력팜수
*/

//void get_kor_test(u08* buf,char index,u16 ch2)
void get_kor_test(u08* buf,char index,u16 *ch2,char len)
{
	char j;	
	u16 k_i ;//= ch2+(index*2);
	
	for(j=0;j<len;j++)
	{		
		k_i = ch2[j]+(index*2);
		
		if(ch2[j]==0xffff)
		{
			buf[(j*2)] = 0;
			buf[(j*2)+1] = 0;
		}	
		else{	
			buf[(j*2)] = KS_fontTable[k_i];
			buf[(j*2)+1] = KS_fontTable[k_i+1];
		}
	}	

}
/*
	29일 테스트 함수 
*/
void get_kor_test2(u08* buf,char index,Kor_1word_Def *ch,char len)
{
	u16 k_i ;//= ch2+(index*2);
	int i,j=0,k=0;
	u08 kor_data[2];
	char x=8;

	for(j=0;j<len;j++)
	{			
		if(ch[j]->kor_font_addr==0xffff)
		{
			kor_data[0] = 0;
			kor_data[1] = 0;
			
		}
		else if(ch[j].only_cho_flag) 
		{
			if(ch[j].kor_sung_array[0]<=3)			
				x=7;		
			if(index>1 && index<x)
			{
				k_i = (ch[j]->kor_font_addr)+((index-2)*2);			
				kor_data[0] = KS_fontTable[k_i];		
				kor_data[1] = KS_fontTable[k_i+1];
			}
			else{
				kor_data[0] = 0;
				kor_data[1] = 0;
			}
		}
		else
		{
			k_i = (ch[j]->kor_font_addr)+(index*2);			
			kor_data[0] = KS_fontTable[k_i];		
			kor_data[1] = KS_fontTable[k_i+1];
			
		}
		k =  j&0x03;
		
			
		if(k==0)// 
		{
			buf[0] = kor_data[0];
			buf[1] &= 0x0F;
			buf[1] |= kor_data[1]&0xF0;
		}
		else if(k==1)// 
		{
			buf[1] &= 0xF0;
			buf[1] |= (kor_data[0]>>4)&0x0F;
			buf[2] = (kor_data[0]<<4)|(kor_data[1]>>4);		
		}
		else if(k==2)
		{	
			if(index==11){
				kor_data[0]=0xff;
				kor_data[1]=0xff;
			}
			buf[3] = kor_data[0];	
			buf[4] &= 0x0F;
			buf[4] |= kor_data[1] & 0xF0;						
		}
		
		
	}//end for
	
}

void get_kor_test3(u08* buf,char index,rom u16 *ch,char len,char font_index)
{
	u16 k_i ,x;//= ch2+(index*2);
	int i,j=0,k=0;
	u08 kor_data[2];
	//u16 xxx = font_index+4;

	for(j=0;j<4;j++)
	{
		if((font_index+j<0) || (len < font_index+j))
		{
		//	k_i = 0;		
		//	kor_data[0] = KS_fontTable[k_i];		
		//	kor_data[1] = KS_fontTable[k_i+1];

			kor_data[0] = 0;
			kor_data[1] = 0;			
		}//추후에는 문자열의 종류에 상관없이 스페이스는 8바이트만 사용 
				
		else
		{
			
			if(ch[j+font_index]==0xffff)
			{
				kor_data[0] = 0;		
				kor_data[1] = 0;
			}	
			else
			{
				k_i =  (ch[j+font_index])+(index*2);;
				kor_data[0] = KS_fontTable[k_i];		
				kor_data[1] = KS_fontTable[k_i+1];
			}
				
			
		}
		k =  j&0x03;		
			
		if(k==0)// 
		{
			buf[0] = kor_data[0];
			buf[1] &= 0x0F;
			buf[1] |= kor_data[1]&0xF0;
		}
		else if(k==1)// 
		{
			buf[1] &= 0xF0;
			buf[1] |= (kor_data[0]>>4)&0x0F;
			buf[2] = (kor_data[0]<<4)|(kor_data[1]>>4);		
		}
		else if(k==2)
		{			
			buf[3] = kor_data[0];	
			buf[4] &= 0x0F;
			buf[4] |= kor_data[1] & 0xF0;						
		}
		else if(k==3)
		{		
			buf[4] &= 0xF0;	
			buf[4] |= (kor_data[0]>>4)&0x0F;
			buf[5] = (kor_data[0]<<4) | (kor_data[1]>>4);
		}
		
	}//end for
	
}



