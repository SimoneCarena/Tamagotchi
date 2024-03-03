#ifndef _IMG_HANDLER_H
#define _IMG_HANDLER_H

#include <stdint.h>

#define BACCARANCIA_ROWS 31
#define BACCARANCIA_COLUMNS 34
#define BIDOOF_1_ROWS 78
#define BIDOOF_1_COLUMNS 90
#define BIDOOF_2_ROWS 90
#define BIDOOF_2_COLUMNS 72
#define RARE_CANDY_ROWS 44
#define RARE_CANDY_COLUMNS 44
#define HUNGER_ROWS 10
#define HUNGER_COLUMNS 11
#define HAPPINESS_ROWS 10
#define HAPPINESS_COLUMNS 12
#define VOLUME_ROWS 28
#define VOLUME_COLUMNS 20

#define SEC_1 0x17D7840
#define SEC_5 0x7735940
#define POLL_TIM 5000000
#define SEC_2 500000000

#define ALIVE 0
#define DEAD 1

void tamagotchi_setup(void);
void update_age(void);
int remove_happiness(void);
int remove_hunger(void);
void add_happiness(void);
void add_hunger(void);
void swap_sprite(void);
void eat_food(void);
void eat_snack(void);
void death_animation(void);
void outline_selection_food(uint16_t color);
void outline_selection_snack(uint16_t color);
void draw_reset_button(uint16_t color);
uint8_t check_point(uint8_t x, uint8_t y);
void cuddle_animation_1(void);
void cuddle_animation_2(void);
void cuddle_animation_3(void);
void add_volume(void);
void remove_volume(void);

#endif
