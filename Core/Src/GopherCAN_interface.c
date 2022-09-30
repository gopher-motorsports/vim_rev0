// GopherCAN_devboard_example.c
//  TODO DOCS - when the project becomes more fleshed out add a quick comment
//  explaining the purpose of this file
#include <stdbool.h>
#include "GopherCAN_Interface.h"
#include "ledstrip.h"
#include "main.h"


// the HAL_CAN struct. This example only works for a single CAN bus
CAN_HandleTypeDef* example_hcan;

// Use this to define what module this board will be
#define THIS_MODULE_ID DAM_R_ID

uint32_t lastUpdate = 0;
uint32_t rainbowArr[NUM_PIXELS][3];
uint16_t i = NUM_PIXELS-1;
bool lowMode = true;

// init
//  What needs to happen on startup in order to run GopherCAN
void init(CAN_HandleTypeDef* hcan_ptr)
{
	example_hcan = hcan_ptr;

	// initialize CAN
	// NOTE: CAN will also need to be added in CubeMX and code must be generated
	// Check the STM_CAN repo for the file "F0xx CAN Config Settings.pptx" for the correct settings
	if (init_can(example_hcan, THIS_MODULE_ID, BXTYPE_MASTER))
	{
		// an error has occurred, stay here
		while (1);
	}

	// enable all of the variables in GopherCAN for testing
	set_all_params_state(TRUE);

	clear();
	setStripBrightness(100);
	generateRainbow(rainbowArr);
}


// can_buffer_handling_loop
//  This loop will handle CAN RX software task and CAN TX hardware task. Should be
//  called every 1ms or as often as received messages should be handled
void can_buffer_handling_loop()
{
	// handle each RX message in the buffer
	if (service_can_rx_buffer())
	{
		// an error has occurred
	}

	// handle the transmission hardware for each CAN bus
	service_can_tx_hardware(example_hcan);
}


// main_loop
//  another loop. This includes logic for sending a CAN command. Designed to be
//  called every 10ms
void main_loop()
{
	uint32_t time = HAL_GetTick();
	if(time - lastUpdate >= 10)
	{
		lastUpdate = time;

		  request_parameter(PRIO_LOW, DLM_ID, rpm_ecu.param_id);
		  uint32_t rpm = rpm_ecu.data;

		  if(lowMode) {
			  uint32_t percent = map(rpm, 0, 9000, 0, 100);
			  rpmProgressBar(percent, 0, 255, 0, 255, 0, 0);

			  if(rpm > 9000)
			  {
				  lowMode = false;
			  }
		  }
		  else
		  {
			  rainbow(i);
			  write();
			  i--;
			  lastUpdate = time;
			  if(i < 0){
				  i = NUM_PIXELS-1;
			  }
			  if(rpm < 8500)
			  {
				  lowMode = true;
			  }
		  }

		  write();
	}
}

// end of GopherCAN_example.c
