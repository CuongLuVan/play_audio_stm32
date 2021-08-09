/**
  @page DAC_SignalsGen DAC Signals generation example
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    DAC/DAC_SignalsGeneration/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Description of the DAC Signals generation example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Example Description 

This example provides a short description of how to use the DAC peripheral to 
generate several signals using DMA controller.
When the user presses the KEY push-button, DMA transfers the two selected 
waveforms to the DAC.
For each press on KEY button, 2 signals has been selected and can be monitored on  
the two DAC channels:
    - Escalator waveform (Channel 1) and Sine waveForm (Channel 2).
    - Noise waveform (Channel 1) and Triangle waveform (Channel 2).


@par Directory contents 

  - DAC/DAC_SignalsGeneration/stm32f2xx_conf.h    Library Configuration file
  - DAC/DAC_SignalsGeneration/stm32f2xx_it.c      Interrupt handlers
  - DAC/DAC_SignalsGeneration/stm32f2xx_it.h      Interrupt handlers header file
  - DAC/DAC_SignalsGeneration/main.c              Main program
  - DAC/DAC_SignalsGeneration/system_stm32f2xx.c  STM32F2xx system source file
  
@note The "system_stm32f2xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F2xx_Clock_Configuration_V1.0.0.xls" 
      provided with the AN3362 package available on <a href="http://www.st.com/internet/mcu/family/141.jsp">  ST Microcontrollers </a> 


@par Hardware and Software environment

  - This example runs on STM32F2xx Devices.
  
  - This example has been tested with STM322xG-EVAL RevB and can be easily tailored
    to any other development board.

  - STM322xG-EVAL Set-up 	
     - Use KEY push-button connected to PG15.
     - Connect PA4 (DAC Channel1) and PA5 (DAC Channel2) pins to an oscilloscope.
     - Make sure that JP31 is removed.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F2xx_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
  
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */

#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"
//#include "stm32f2xx.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_syscfg.h"
#include "integer.h"	/* Basic integer types */
#include "ffconf.h"		/* FatFs configuration options */
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
//#include "stm32f2xx_dac.h"
typedef struct{
uint32_t IDmedia;
uint32_t	SIZEmedia;
uint32_t Formatmedia;
uint32_t	Subchunk1ID;
uint32_t	Subchunk1Size;
uint16_t	AudioFormat;
uint16_t	NumChannels;
uint32_t	SampleRate;
uint32_t	ByteRate;
uint16_t	BlockAlign;
uint16_t	 BitsPerSample;
uint32_t	Subchunk2ID;
uint32_t	Subchunk2Size;
} media_music;

#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R1_ADDRESS     0x40007410
// set up timer6

void khoitaodac(void);


void DAC_Ch1_NoiseConfig(void);

void DAC_Ch2_TriangleConfig(void);
// playmedia on buff
void openmedia_play(char *filename);

	
