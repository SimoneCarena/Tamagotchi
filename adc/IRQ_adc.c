/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "adc.h"
#include "../timer/timer.h"
#include "../timer/img_handler.h"

extern uint8_t current_volume_value;

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;   
unsigned short AD_last = 0xFF;     /* Last converted value               */

void ADC_IRQHandler(void) {
	
	uint8_t val_vol;
  	
  AD_current = ((LPC_ADC->ADGDR>>4) & 0xFFF);/* Read Conversion Result             */
  if(AD_current != AD_last){
		if(AD_current<1024){
			val_vol = VOLUME_OFF;
		}
		else if(AD_current>=1024 && AD_current<2048){
			val_vol = VOLUME_1;		
		}
		else if(AD_current>=2048 && AD_current<3072){
			val_vol = VOLUME_2;	
		}
		else{
			val_vol = VOLUME_MAX;
		}
  }
	
	if(current_volume_value>val_vol){
		while(current_volume_value!=val_vol){
			remove_volume();
			current_volume_value--;
		}
	}
	else if(current_volume_value<val_vol){
		while(current_volume_value!=val_vol){
			add_volume();
			current_volume_value++;
		}
	}
	
}

