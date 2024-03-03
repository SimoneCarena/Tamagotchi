/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "img_handler.h"
#include "../RIT/RIT.h"

#define OFF 0
#define CLICK 1
#define EAT 2
#define CUDDLE 3
#define DEATH 4

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint8_t current_volume_value = VOLUME_OFF;

extern uint8_t dead;
volatile uint8_t active_swap = 1;
volatile uint8_t active_cuddle = 0;
extern uint8_t music;
extern uint8_t currentNote;

uint16_t SinTable[45] =                                       
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

void TIMER0_IRQHandler (void)
{
	
	update_age();
	if(active_swap){
		disable_RIT();
		swap_sprite();
		enable_RIT();
	}
	if(active_cuddle == 1){
		cuddle_animation_1();
		active_cuddle++;
	}
	else if(active_cuddle == 2){
		cuddle_animation_2();
		active_cuddle++;
	}
	else if(active_cuddle == 3){
		cuddle_animation_3();
		active_cuddle = 0;
		reset_RIT();
		enable_RIT();
		enable_timer(1);
		active_swap = 1;
		add_happiness();
	}
  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	
	int hunger_value, happiness_value;
	
	hunger_value = remove_hunger();
	happiness_value = remove_happiness();
	if(happiness_value == 0 || hunger_value == 0){
		currentNote = 0;
		dead = DEAD;
		disable_timer(0);
		disable_timer(1);
		death_animation();
		music = DEATH;
		LPC_TIM1->IR = 1;
		return;
	}
	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	static int sineticks=0;
	/* DAC management */	
	static int currentValue; 
	currentValue = SinTable[sineticks];
	currentValue -= 410;
	currentValue /= 1;
	currentValue += 410;
	switch(current_volume_value){
		case VOLUME_OFF:
			currentValue = 0;
			break;
		case VOLUME_1:
			currentValue/=3;
			break;
		case VOLUME_2:
			currentValue = (currentValue*2)/3;
		default:
			break;
	}
	LPC_DAC->DACR = (currentValue) <<6;
	sineticks++;
	if(sineticks==45) sineticks=0;
	
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
	
	disable_timer(2);
	LPC_TIM3->IR = 1;
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
