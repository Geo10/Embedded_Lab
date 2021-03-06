/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Lab 1: the elevator control system
 */

/**
 * This file defines the safety module, which observes the running
 * elevator system and is able to stop the elevator in critical
 * situations
 */

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>

#include "global.h"
#include "assert.h"

#define POLL_TIME (10 / portTICK_RATE_MS)

#define MOTOR_UPWARD   (TIM3->CCR1)
#define MOTOR_DOWNWARD (TIM3->CCR2)
#define MOTOR_STOPPED  (!MOTOR_UPWARD && !MOTOR_DOWNWARD)

#define STOP_PRESSED  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)
#define AT_FLOOR      GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define DOORS_CLOSED  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)

static portTickType xLastWakeTime;

static void check(u8 assertion, char *name) {
  if (!assertion) {
    printf("SAFETY REQUIREMENT %s VIOLATED: STOPPING ELEVATOR\n", name);
    for (;;) {
	  setCarMotorStopped(1);
  	  vTaskDelayUntil(&xLastWakeTime, POLL_TIME);
    }
  }
}

static void safetyTask(void *params) {
	signed long previousPosition, currentPosition;
	int delayCounter = 0 , arrivedFloor1 = 0 , arrivedFloor2 = 0, arrivedFloor3 = 0, assert_flag = 1;
  s16 timeSinceStopPressed = -1;

  xLastWakeTime = xTaskGetTickCount();
  currentPosition = getCarPosition();
  for (;;) {
    // Environment assumption 1: the doors can only be opened if
	//                           the elevator is at a floor and
    //                           the motor is not active

//	check((AT_FLOOR && MOTOR_STOPPED) || DOORS_CLOSED,
//	      "env1");

	// The elevator moves at a maximum speed of 50cm/s
		
		
		//printf("currentPosition: %ld\n", currentPosition);
		
		
	
		if ( delayCounter == 8 ){
			currentPosition = getCarPosition();
			delayCounter = 1;
		  if (currentPosition - previousPosition > 4){			 
			   assert_flag = 0;	
			}				
		}else if ( delayCounter < 10 )
		      delayCounter++;
		
	   
		  previousPosition = currentPosition;
	 check(assert_flag, "env2");

	// fill in environment assumption 3
		
//		arrivedFloor1 = 1;
//		arrivedFloor2 = 1;
//		arrivedFloor3 = 1;	 
	if (AT_FLOOR)	{
		if (!(getCarPosition() > -1) && (getCarPosition() < 1 )) 
			 arrivedFloor1 = 0;
		else if ((getCarPosition() > 399) && (getCarPosition() < 401))
			 arrivedFloor2 = 1;
		else if ((getCarPosition() > 799) && (getCarPosition() < 801))
		   arrivedFloor3 = 1;	 
  }
	
	check(!AT_FLOOR || (arrivedFloor1 || arrivedFloor2 || arrivedFloor3), "env3");
	
	
	// fill in your own environment assumption 4
	check(1, "env4");

    // System requirement 1: if the stop button is pressed, the motor is
	//                       stopped within 1s

	if (STOP_PRESSED) {
	  if (timeSinceStopPressed < 0)
	    timeSinceStopPressed = 0;
      else
	    timeSinceStopPressed += POLL_TIME;

      check(timeSinceStopPressed * portTICK_RATE_MS <= 1000 || MOTOR_STOPPED,
	        "req1");
	} else {
	  timeSinceStopPressed = -1;
	}

    // System requirement 2: the motor signals for upwards and downwards
	//                       movement are not active at the same time

    check(!MOTOR_UPWARD || !MOTOR_DOWNWARD,
          "req2");

	// fill in safety requirement 3
	check(1, "req3");

	// fill in safety requirement 4
	check(1, "req4");

	// fill in safety requirement 5
	check(1, "req5");

	// fill in safety requirement 6
	check(1, "req6");

	// fill in safety requirement 7
	check(1, "req7");

	vTaskDelayUntil(&xLastWakeTime, POLL_TIME);
  }

}

void setupSafety(unsigned portBASE_TYPE uxPriority) {
  xTaskCreate(safetyTask, "safety", 100, NULL, uxPriority, NULL);
}
