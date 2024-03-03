/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/img_handler.h"
#include "../timer/timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../music/music.h"

extern uint8_t dead;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

#define NONE 0
#define FOOD 1
#define SNACK 2

#define UPTICKS 1

#define CLICK_NOTES_N 1
#define EAT_NOTES_N 3
#define DEATH_NOTES_N 5
#define CUDDLE_NOTES_N 8

#define OFF 0
#define CLICK 1
#define EAT 2
#define CUDDLE 3
#define DEATH 4

extern uint8_t active_swap;
extern Matrix matrix;
extern Coordinate display;
extern uint8_t active_cuddle;
uint8_t music;

const NOTE click_notes[CLICK_NOTES_N] = {

	{d3, time_semicroma}
	
};

const NOTE eat_notes[EAT_NOTES_N] ={

	{d3, time_semicroma},
	{c4, time_semicroma},
	{d4, time_semicroma}

};

const NOTE death_notes[DEATH_NOTES_N] = {

	{f4, time_semicroma},
	{g4, time_semicroma},
	{a4, time_semicroma},
	{g4, time_semicroma},
	{f4, time_semicroma}

};

const NOTE cuddle_notes[CUDDLE_NOTES_N] = {

	{b2, time_semicroma},
	{c3b, time_semicroma},
	{a3, time_semicroma},
	{b3, time_semicroma},
	{b3, time_semicroma},
	{a3, time_semicroma},
	{c3b, time_semicroma},
	{b2, time_semicroma}
	
};

volatile uint8_t currentNote = 0;

void RIT_IRQHandler (void)
{					
		
	static uint32_t select;
	static uint32_t left;
	static uint32_t right;
	static uint8_t selected = NONE;
	static int ticks = 0;
	
	switch(dead){
		case ALIVE:
			if(((LPC_GPIO1->FIOPIN)&(1<<25))==0 && active_cuddle==0){
				select++;
				switch(select){
					case 1:
						if(selected==NONE){
							break;
						}
						else if(selected==FOOD){
							disable_timer(1);
							music = EAT;
							active_swap = 0;
							eat_food();
							outline_selection_food(Black);
							selected=NONE;
							active_swap = 1;
							enable_timer(1);
						}
						else if(selected==SNACK){
							disable_timer(1);
							music = EAT;
							active_swap = 0;
							eat_snack();
							outline_selection_snack(Black);
							selected=NONE;
							active_swap = 1;
							enable_timer(1);
						}
						break;
					default:
						break;
				}
			}
			else if(((LPC_GPIO1->FIOPIN)&(1<<27))==0 && active_cuddle==0){
				left++;
				switch(left){
					case 1:
						music = CLICK;
						selected = SNACK;
						outline_selection_food(Black);
						outline_selection_snack(Red);
						break;
					default:
						break;
				}
			}
			else if(((LPC_GPIO1->FIOPIN)&(1<<28))==0 && active_cuddle==0){
				right++;
				switch(right){
					case 1:
						music = CLICK;
						selected = FOOD;
						outline_selection_snack(Black);
						outline_selection_food(Red);
						break;
					default:
						break;
				}
			}
			else{
				select = 0;
				left = 0;
				right = 0;
				if(getDisplayPoint(&display, Read_Ads7846(), &matrix ) && active_cuddle==0){
					if(check_point(display.x,display.y)){
						disable_timer(1);
						music = CUDDLE;
						active_swap = 0;
						active_cuddle = 0;
						active_cuddle++;
						LPC_RIT->RICTRL |= 0x1;
						enable_timer(1);
						return;
					}
				}
			}
			break;
		case DEAD:
			if(((LPC_GPIO1->FIOPIN)&(1<<25))==0){
				tamagotchi_setup();
				return;
			}
			else{
				select = 0;
				reset_RIT();
				enable_RIT();
			}
			break;
		default:
			break;
	}
	
	switch(music){
		case OFF:
			break;
		case CLICK:
			if(!isNotePlaying())
			{
				++ticks;
				if(ticks == UPTICKS)
				{
				ticks = 0;
				playNote(click_notes[currentNote++]);
				}
			}
	
			if(currentNote==CLICK_NOTES_N)
			{
				currentNote = 0;
				music = OFF;
				break;
			}
			break;
		case EAT:
			if(!isNotePlaying())
			{
				++ticks;
				if(ticks == UPTICKS)
				{
				ticks = 0;
				playNote(eat_notes[currentNote++]);
				}
			}
	
			if(currentNote==EAT_NOTES_N)
			{
				currentNote = 0;
				music = OFF;
				break;
			}
			break;
		case DEATH:
			if(!isNotePlaying())
			{
				++ticks;
				if(ticks == UPTICKS)
				{
				ticks = 0;
				playNote(death_notes[currentNote++]);
				}
			}
	
			if(currentNote==DEATH_NOTES_N)
			{
				currentNote = 0;
				music = OFF;
				break;
			}
			break;
		case CUDDLE:
			if(!isNotePlaying())
			{
				++ticks;
				if(ticks == UPTICKS)
				{
				ticks = 0;
				playNote(cuddle_notes[currentNote++]);
				}
			}
	
			if(currentNote==CUDDLE_NOTES_N)
			{
				currentNote = 0;
				music = OFF;
				break;
			}
			break;
		default:
			break;
	}
	
	ADC_start_conversion();
	
  LPC_RIT->RICTRL |= 0x1;			/* clear interrupt flag */
  return;
	
}

/******************************************************************************
**                            End Of File
******************************************************************************/
