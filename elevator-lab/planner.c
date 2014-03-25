/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Lab 1: the elevator control system
 */

/**
 * The planner module, which is responsible for consuming
 * pin/key events, and for deciding where the elevator
 * should go next
 */

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#include "global.h"
#include "planner.h"
#include "assert.h"

static void plannerTask(void *params) {
  PinEvent pvBuffer;
	
	for(;;){
		
  xQueueReceive( pinEventQueue , &pvBuffer, portMAX_DELAY );
	//printf("Buffer: %d\n",  pvBuffer);
		
  }
}

void setupPlanner(unsigned portBASE_TYPE uxPriority) {
  xTaskCreate(plannerTask, "planner", 100, NULL, uxPriority, NULL);
}
