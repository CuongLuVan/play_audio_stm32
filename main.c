
#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"

//#include "stm32f2xx.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "integer.h"	/* Basic integer types */
#include "ffconf.h"		/* FatFs configuration options */


//extern void disk_timerproc (void);


DWORD AccSize;				/* Work register for fs command */
WORD AccFiles, AccDirs;
FILINFO Finfo;
#if _USE_LFN
char Lfname[512];
#endif

char Line[256];				/* Console input buffer */
BYTE Buff[4096] __attribute__ ((aligned (4))) ;	/* Working buffer */

 FATFS FatFs;				/* File system object for each logical drive */
DIR Dir;					/* Directory object */
//FATFS FatFs;		/* FatFs work area needed for each volume */
 FIL Fil;			/* File object needed for each open file */
unsigned int br;
 UINT bw;
volatile UINT Timer;


static
FRESULT scan_files (
	char* path		/* Pointer to the path name working buffer */
)
{
	DIR dirs;
	FRESULT res;
	BYTE i;
	char *fn;


	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (_FS_RPATH && Finfo.fname[0] == '.') continue;
#if _USE_LFN
			fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;
#else
			fn = Finfo.fname;
#endif
			if (Finfo.fattrib & AM_DIR) {
				AccDirs++;
				*(path+i) = '/'; strcpy(path+i+1, fn);
				res = scan_files(path);
				*(path+i) = '\0';
				if (res != FR_OK) break;
			} else {
			/*	xprintf("%s/%s\n", path, fn); */
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}



void mx_sd_config(void) {
		/* Private typedef ---------------------------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStruct;


	/** Configure pins as GPIO
		 PA12	 ------> GPIO_Output
		 PA11	 ------> GPIO_Output
		 PA9	 ------> GPIO_Input
		 PA10	 ------> GPIO_Output
	*/


	/*Enable or disable the AHB1 peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

}


void RCC_Configuration(void)
{
 SystemCoreClockUpdate ();
GPIO_AFIODeInit();
	RCC_DeInit();
}

void GPIO_Configuration(void)
{   
	
	GPIO_InitTypeDef GPIO_InitStruct;


	/** DAC GPIO Configuration	
		 PA4	 ------> DAC_OUT1
		 PA5	 ------> DAC_OUT2
	*/

	
	
	
	/*Enable or disable APB2 peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*Configure GPIO pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	/** Configure pins as GPIO
		 PC0	 ------> GPIO_Output
		 PC1	 ------> GPIO_Output
		 PC2	 ------> GPIO_Output
		 PC3	 ------> GPIO_Output
		 PA0-WKUP	 ------> GPIO_Output
		 PB0	 ------> GPIO_Input
		 PB1	 ------> GPIO_Input
		 PB2	 ------> GPIO_Input
		 PB10	 ------> GPIO_Input
		 PB11	 ------> GPIO_Input
	*/

	/*Enable or disable APB2 peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);

	//!!! GPIO_Init is commented because some parameters are missing
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	//!!! GPIO_Init is commented because some parameters are missing
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	//!!! GPIO_Init is commented because some parameters are missing
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AIN;;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}


int main(void)
{uint16_t duara,men;
	uint32_t  giatri;
	char acao[]="fr chen them<thu1.txt>\n";
char vao[1024];
FRESULT res;
	
	GPIO_Configuration();

	//_delay_ms(100);
	mx_sd_config();
	 men=0;
	
	//	_delay_ms(1000);	
		f_mount(&FatFs, "", 0);		/* Give a work area to the default drive */
 f_open(&Fil,"thu2.txt", FA_OPEN_EXISTING | FA_READ);	
res = f_read(&Fil, Buff, 16, &bw);
	for(giatri=0;giatri<16;giatri++) acao[giatri]=Buff[giatri];
		f_close(&Fil);								/* Close the file */
	
	
	//openmedia_play("tuyet2.wav");
		
		
	while(1){
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
		GPIO_ResetBits(GPIOC,GPIO_Pin_2);
		GPIO_ResetBits(GPIOC,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)) {
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			openmedia_play("tuyet1.wav");
      }//phim1
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)) {
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			openmedia_play("tuyet2.wav");
      }// phim2
					if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)) {
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			openmedia_play("tuyet3.wav");
      }// phim3
					if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)) {
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			openmedia_play("tuyet4.wav");
      }// phim4
					if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)) {
			GPIO_SetBits(GPIOC,GPIO_Pin_0);
			openmedia_play("tuyet5.wav");
      }// phim5
		
		
	}


	
}



DWORD get_fattime (void)
{
	/* Returns current time packed into a DWORD variable */
	return	  ((DWORD)(2013 - 1980) << 25)	/* Year 2013 */
			| ((DWORD)7 << 21)				/* Month 7 */
			| ((DWORD)28 << 16)				/* Mday 28 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				/* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}



void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */

  while (1)
  {}
}




