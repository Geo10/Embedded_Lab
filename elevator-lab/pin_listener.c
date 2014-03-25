/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Lab 1: the elevator control system
 */

/**
 * Functions listening for changes of specified pins
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "pin_listener.h"
#include "assert.h"


static void pollPin(PinListener *listener,
                    xQueueHandle pinEventQueue) {

		u8 currentPinSignal ;
    
		currentPinSignal	= GPIO_ReadInputDataBit(listener->gpio, listener->pin);		
					
		if ((listener->status == 0) && (currentPinSignal == 1))
         listener->status = 1;	
    else if ((listener->status == 1) && (currentPinSignal == 0))
    		 listener->status = 0;
		else if ((listener->status == 1) && (listener->counter == 6))
			   listener->status = 2;
		else if (listener->status == 2) 
			   listener->status = 3;
		else if ((listener->status == 3) && (currentPinSignal == 0))
			   listener->status = 4;
		else if (listener->status == 4)
			   listener->status = 1;
			   
		
		if (listener->status == 0)
		     listener->counter = 0;
		else if (listener->status == 1)
			   listener->counter++;
		else if (listener->status == 2)
			   xQueueSend( pinEventQueue, &listener->risingEvent, portMAX_DELAY );		
		else if (listener->status == 4){
			    if (listener->fallingEvent != 0)			
             xQueueSend( pinEventQueue, &listener->fallingEvent, portMAX_DELAY );	
				}
}

static void pollPinsTask(void *params) {
  PinListenerSet listeners = *((PinListenerSet*)params);
  portTickType xLastWakeTime;
  int i;

  xLastWakeTime = xTaskGetTickCount();

  for (;;) {
		setCarTargetPosition(400);
    for (i = 0; i < listeners.num; ++i)
	  pollPin(listeners.listeners + i, listeners.pinEventQueue);
    
	vTaskDelayUntil(&xLastWakeTime, listeners.pollingPeriod);
  }
}

void setupPinListeners(PinListenerSet *listenerSet) {
  portBASE_TYPE res;

  res = xTaskCreate(pollPinsTask, "pin polling",
                    100, (void*)listenerSet,
					listenerSet->uxPriority, NULL);
  assert(res == pdTRUE);
}
