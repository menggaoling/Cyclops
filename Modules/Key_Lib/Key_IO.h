#ifndef __KEY_IO_H
#define __KEY_IO_H

#include "HAL_Interface.h"
//---------------------------KEY Port--------------------//
#define KEYBOARD_SCAN0_PORT		GPIOA 
#define KEYBOARD_SCAN0_PIN		GPIO_Pin_11
#define KEYBOARD_SCAN1_PORT		GPIOD
#define KEYBOARD_SCAN1_PIN		GPIO_Pin_14
#define KEYBOARD_SCAN2_PORT		GPIOD
#define KEYBOARD_SCAN2_PIN		GPIO_Pin_13
#define KEYBOARD_SCAN3_PORT		GPIOD
#define KEYBOARD_SCAN3_PIN		GPIO_Pin_12
#define KEYBOARD_SCAN4_PORT		GPIOD
#define KEYBOARD_SCAN4_PIN		GPIO_Pin_11
#define KEYBOARD_SCAN5_PORT		GPIOD
#define KEYBOARD_SCAN5_PIN		GPIO_Pin_10

#define KEYBOARD_KeyIn0_PORT	        GPIOC
#define KEYBOARD_KeyIn0_PIN		GPIO_Pin_6
#define KEYBOARD_KeyIn1_PORT	        GPIOC
#define KEYBOARD_KeyIn1_PIN		GPIO_Pin_7
#define KEYBOARD_KeyIn2_PORT	        GPIOC
#define KEYBOARD_KeyIn2_PIN		GPIO_Pin_9

#endif