/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Lab 1: the elevator control system
 */

/**
 * Class for keeping track of the car position.
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "position_tracker.h"

#include "assert.h"

static void positionTrackerTask(void *params) {
	portTickType xLastWakeTime;
	int counter = 0;
	u8 currentPinSignal = 0, previousPinSignal = 0;
  PositionTracker *positionTracker = (PositionTracker *)params;
	xLastWakeTime = xTaskGetTickCount();
  for (;;){
		
    currentPinSignal = GPIO_ReadInputDataBit(positionTracker->gpio, positionTracker->pin);
	  

		if (positionTracker->direction == Up){
		   if ((currentPinSignal == 0) && (previousPinSignal == 1))
		      positionTracker->position++;
	  }
		else if (positionTracker->direction == Down){
       if ((currentPinSignal == 1) && (previousPinSignal == 0))			
			      positionTracker->position--;
		}	  
		previousPinSignal = currentPinSignal;
	  
		printf("positionTracker->position: %ld\n", positionTracker->position);
  vTaskDelayUntil(&xLastWakeTime, positionTracker->pollingPeriod);
  }
}

void setupPositionTracker(PositionTracker *tracker,
                          GPIO_TypeDef * gpio, u16 pin,
						  portTickType pollingPeriod,
						  unsigned portBASE_TYPE uxPriority) {
  portBASE_TYPE res;

  tracker->position = 0;
  tracker->lock = xSemaphoreCreateMutex();
  assert(tracker->lock != NULL);
  tracker->direction = Unknown;
  tracker->gpio = gpio;
  tracker->pin = pin;
  tracker->pollingPeriod = pollingPeriod;

  res = xTaskCreate(positionTrackerTask, "position tracker",
                    80, (void*)tracker, uxPriority, NULL);
  assert(res == pdTRUE);
}

void setDirection(PositionTracker *tracker, Direction dir) {
  tracker->direction = dir;

}

s32 getPosition(PositionTracker *tracker) {
  return tracker->position;
}

