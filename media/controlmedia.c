#include "controlmedia.h"


extern FIL Fil; 
extern uint8_t acNoData[];
//extern WAVE_DATA SoundNoData;
 FRESULT res;         /* FatFs function common result code */
extern unsigned int br;         /* File read/write count */
extern UINT bw;
media_music mediawav;

//UINT bw;
//FRESULT res;
extern FATFS FatFs;
 BYTE buff_media[8000];
 BYTE buff_media2[8000];
extern DIR Dir;					/* Directory object */
//FATFS FatFs;		/* FatFs work area9 needed for each volume */
extern FIL Fil;	
  uint32_t  size_media,format_media,tocdo,count_media;
	
	
void TIM6_Config(void)
{
	   DAC_InitTypeDef            DAC_InitStructure;
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 0xF;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0xF;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* DAC channel2 Configuration */
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude =  DAC_TriangleAmplitude_2047;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is 
     automatically connected to the DAC converter. */
  DAC_Cmd(DAC_Channel_2, ENABLE);
  DAC_SetDualChannelData(DAC_Align_12b_R, 0x100, 0x100);

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
}
void khoitaodac(void){
 GPIO_InitTypeDef GPIO_InitStructure;

  /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically 
     connected to the DAC converter. In order to avoid parasitic consumption, 
     the GPIO pin should be configured in analog */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  /* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  TIM6_Config();  
  

}


uint8_t hex_to(uint8_t a){
	
switch (a){
case 0:  return '0';
case 1:  return '1';
case 2:  return '2';
case 3:  return '3';
case 4:  return '4';
case 5:  return '5';
case 6:  return '6';
case 7:  return '7';
case 8:  return '8';
case 9:  return '9';
case 10:  return 'A';
case 11:  return 'B';
case 12:  return 'C';
case 13:  return 'D';
case 14:  return 'E';
case 15: return 'F';
default: return 0;
}
}
void prinf_acii(uint16_t x,uint16_t y,uint32_t giatri){
char a[9],b,c;
int i;
for(i=0;i<9;i++) a[i]='\0';
a[0]=(giatri/0x01000000);
giatri=giatri*0x0100;
a[1]=(giatri/0x01000000);
giatri=giatri*0x0100;	
a[2]=(giatri/0x01000000);
giatri=giatri*0x0100;		
a[3]=(giatri/0x01000000);
giatri=giatri*0x0100;
//print1(a,x,y,0);
}



void prinfhex(uint16_t x,uint16_t y,uint32_t giatri){
char a[9],b,c;
int i;
for(i=0;i<9;i++) a[i]='\0';
a[0]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;
a[1]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;	
a[2]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;		
a[3]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;
a[4]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;
a[5]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;	
a[6]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;		
a[7]=hex_to(giatri/0x10000000);
giatri=giatri*0x10;	
//print1(a,x,y,0);
}

void load_infofile(void){		
mediawav.IDmedia=buff_media[0]*0x01000000+buff_media[1]*0x010000+buff_media[2]*0x0100+buff_media[3];
mediawav.SIZEmedia=buff_media[7]*0x01000000+buff_media[6]*0x010000+buff_media[5]*0x0100+buff_media[4];
mediawav.Formatmedia=buff_media[8]*0x01000000+buff_media[9]*0x010000+buff_media[10]*0x0100+buff_media[11];
mediawav.Subchunk1ID=buff_media[12]*0x01000000+buff_media[13]*0x010000+buff_media[14]*0x0100+buff_media[15];
mediawav.Subchunk1Size=buff_media[16]*0x01000000+buff_media[17]*0x010000+buff_media[18]*0x0100+buff_media[19];
mediawav.AudioFormat=buff_media[21]*0x0100+buff_media[20];
mediawav.NumChannels=buff_media[23]*0x0100+buff_media[22];
mediawav.SampleRate=buff_media[27]*0x01000000+buff_media[26]*0x010000+buff_media[25]*0x0100+buff_media[24];
mediawav.ByteRate=buff_media[31]*0x01000000+buff_media[30]*0x010000+buff_media[29]*0x0100+buff_media[28];
mediawav.BlockAlign=buff_media[32]*0x0100+buff_media[33];
mediawav.BitsPerSample=buff_media[35]*0x0100+buff_media[34];
mediawav.Subchunk2ID=buff_media[36]*0x01000000+buff_media[37]*0x010000+buff_media[38]*0x0100+buff_media[39];
mediawav.Subchunk2Size=buff_media[40]*0x01000000+buff_media[41]*0x010000+buff_media[42]*0x0100+buff_media[43];
	
/*
	setColor2(VGA_BLUE);
	fillRect(0,0,320,240);
	setColor2(VGA_RED);
prinfhex(10,210,mediawav.IDmedia);
prinfhex(10,190,mediawav.SIZEmedia);
prinfhex(10,170,mediawav.Formatmedia);
prinfhex(10,150,mediawav.Subchunk1ID);
prinfhex(10,130,mediawav.Subchunk1Size);
prinfhex(10,110,(0x00000000+mediawav.AudioFormat));
prinfhex(10,90,(0x00000000+mediawav.NumChannels));
prinfhex(10,70,mediawav.SampleRate);
prinfhex(10,60,mediawav.ByteRate);
prinfhex(10,40,mediawav.BlockAlign);
prinfhex(10,30,mediawav.BitsPerSample);
prinfhex(10,20,mediawav.Subchunk2ID);
prinfhex(10,10,mediawav.Subchunk2Size);	


prinf_acii(110,210,mediawav.IDmedia);
prinf_acii(110,190,mediawav.SIZEmedia);
prinf_acii(110,170,mediawav.Formatmedia);
prinf_acii(110,150,mediawav.Subchunk1ID);
prinf_acii(110,130,mediawav.Subchunk1Size);
prinf_acii(110,110,mediawav.AudioFormat);
prinf_acii(110,90,mediawav.NumChannels);
prinf_acii(110,70,mediawav.SampleRate);
prinf_acii(110,60,mediawav.ByteRate);
prinf_acii(110,40,mediawav.BlockAlign);
prinf_acii(110,30,mediawav.BitsPerSample);
prinf_acii(110,20,mediawav.Subchunk2ID);
prinf_acii(110,10,mediawav.Subchunk2Size);	
	*/
}




void DAC_Ch2_TriangleConfig(void)
{ DAC_InitTypeDef  DAC_InitStructure;
 /* DAC channel2 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* Enable DAC Channel2 */
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* Set DAC channel2 DHR12RD register */
  DAC_SetChannel2Data(DAC_Align_12b_R,0x10);
}

/**
  * @brief  DAC  Channel1 Noise Configuration
  * @param  None
  * @retval None
  */
void DAC_Ch1_NoiseConfig(void)
{ DAC_InitTypeDef  DAC_InitStructure;
 /* DAC channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

  /* Set DAC Channel1 DHR12L register */
  DAC_SetChannel1Data(DAC_Align_12b_L,0x10);
}

	
	
	
	
	


void _delay_us (int t){
 uint32_t dem;
for(;t>0;t--) for(dem=0;dem<12;dem++);
}
void openmedia_play(char *filename){
	
char _filename[128];
int i;
	i=0;
	while(*filename) {_filename[i]=*filename;
		i++;
		*filename++;
	}
		while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)); 
	// cho khi nha phim so 1
	
		f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
 f_open(&Fil,_filename, FA_OPEN_EXISTING | FA_READ);	
  res = f_read(&Fil, buff_media, 44, &bw);
if ((res || bw) == 0) ; else {
load_infofile();  
	//wavePlayerInit();
	 khoitaodac();
 //print1("doc file_x",120,160,0);
	tocdo=240000000/mediawav.SampleRate;
	size_media=mediawav.SIZEmedia-8044;
	 res = f_read(&Fil, buff_media, 8000, &bw);
//	 print1("ok_lodad",120,160,0);
//	DAC_Ch2_TriangleConfig();
 //DAC_Ch1_NoiseConfig();
	// print1("ok_chay thu xem nao",120,160,0);
for(count_media=0;count_media<7998;count_media +=2){	
	DAC_SetChannel2Data(DAC_Align_12b_R,buff_media[count_media]);
	DAC_SetChannel1Data(DAC_Align_12b_R,buff_media[count_media+1]);
	_delay_us(100);
}
	// print1("dung thu",120,160,0);
		count_media=0;
format_media=1;
	if (SysTick_Config(SystemCoreClock / (mediawav.SampleRate*3)))
  { 
    /* Capture error */ 
		// print1("error_loi",180,110,0);
    while (1);
  }	
	// phim 1 duoc nhan thi ket thuc choi nhac
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))&&(size_media!=0)){
	if(size_media>8000)	{
if(format_media==1)
{size_media-=8000;
	format_media=3;
			res = f_read(&Fil,buff_media2, 8000, &bw); }		
if(format_media==2)
{size_media-=8000;
	format_media=4;
			res = f_read(&Fil,buff_media, 8000, &bw); }

} else{if(format_media==1)
{size_media=0;
	format_media=3;
			res = f_read(&Fil,buff_media2, count_media, &bw); }		
if(format_media==2)
{size_media=0;
	format_media=4;
			res = f_read(&Fil,buff_media,count_media, &bw); }}
	}
	
//print1("nghe",180,110,0);
	
	
	
}
	format_media=10;
f_close(&Fil);								/* Close the file */
//	print1("chay xong nhac",200,170,0);
}





/**
  * @brief  Stop wave playing
  * @param  None
  * @retval None
  */



