/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The * 
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"
#include "system.h"
#include "altera_up_avalon_character_lcd.h"
#include "io.h"

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2

alt_up_character_lcd_dev *char_lcd;

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{

	int *sw0 = (int*)SWITCH_BASE;
	int *sw1 = (int*)SWITCH_0_BASE;
	char *p = SERVO_PWM_0_BASE;
  while (1)
  { 
	  if (*sw0 == SWITCH_HAS_IN && *sw1 == SWITCH_HAS_OUT){
		  IOWR_8DIRECT(SERVO_PWM_0_BASE, 0, 0x11);
//		  clear_lcd(char_lcd);
//		  alt_up_character_lcd_set_cursor_pos(char_lcd,0,0);
//		  alt_up_character_lcd_string(char_lcd, "CW");
		printf("CW\n");
	  	}



	  else if (*sw1 == SWITCH_HAS_IN && *sw0 == SWITCH_HAS_OUT){
		  IOWR_8DIRECT(SERVO_PWM_0_BASE, 0, 0xFF);
//		  clear_lcd(char_lcd);
//		  alt_up_character_lcd_set_cursor_pos(char_lcd,0,0);
//		  alt_up_character_lcd_string(char_lcd, "CCW");
		printf("CCW\n");
	  }

	  else{
		IOWR_8DIRECT(SERVO_PWM_0_BASE, 0, 0x00);
//		clear_lcd(char_lcd);
//		alt_up_character_lcd_set_cursor_pos(char_lcd,0,0);
//		alt_up_character_lcd_string(char_lcd, "NEUTRAL");
		printf("NEUTRAL\n");
	  }
	  printf("%x %x\n",p[0],p[1]);

	  OSTimeDlyHMSM(0, 0, 5, 0);
  }
}
/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{
  while (1)
  {
//    printf("Hello from task2\n");
    OSTimeDlyHMSM(0, 0, 3, 0);
  }
}


void clear_lcd(alt_up_character_lcd_dev *lcd){
	int x = 0;
	int y = 0;
	for(y = 0; y < 2; y++){
		for(x = 0; x < 16; x++){
			alt_up_character_lcd_set_cursor_pos(lcd,0,0);
			alt_up_character_lcd_erase_pos(lcd,x,y);
		}
	}
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
	char_lcd = alt_up_character_lcd_open_dev("/dev/character_lcd_0");
	if(char_lcd ==NULL){
	printf("Could not open lcd device\n");}

	alt_up_character_lcd_init(char_lcd);
//	alt_up_character_lcd_cursor_off(char_lcd);
	IOWR_8DIRECT(SERVO_PWM_0_BASE, 0, 0x00);

  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
              

  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSStart();
  return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
