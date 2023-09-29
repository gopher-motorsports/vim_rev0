// GopherCAN_devboard_example.c
//  TODO DOCS - when the project becomes more fleshed out add a quick comment
//  explaining the purpose of this file
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GopherCAN_Interface.h"
#include "ledstrip.h"
#include "main.h"


// the HAL_CAN struct. This example only works for a single CAN bus
CAN_HandleTypeDef* example_hcan;

// Use this to define what module this board will be
#define THIS_MODULE_ID PLM_ID

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
	if (init_can(2, example_hcan, THIS_MODULE_ID, BXTYPE_MASTER))
	{
		// an error has occurred, stay here
		while (1);
	}

	// enable all of the variables in GopherCAN for testing
	//set_all_params_state(TRUE);

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
	service_can_tx(example_hcan);
}


// main_loop
//  another loop. This includes logic for sending a CAN command. Designed to be
//  called every 10ms
void main_loop()
{
	uint32_t time = HAL_GetTick();
	if(time - lastUpdate >= 1)
	{

//		  request_parameter(PRIO_LOW, DLM_ID, rpm_ecu.param_id);
//		  uint32_t rpm = rpm_ecu.data;
//
//		  if(lowMode) {
//			  uint32_t percent = map(rpm, 0, 9000, 0, 100);
//			  rpmProgressBar(percent, 0, 255, 0, 255, 0, 0);
//
//			  if(rpm > 9000)
//			  {
//				  lowMode = false;
//			  }
//		  }
//		  else
//		  {

		// Rainbow

			  rainbow(i);
			  write();
			  i--;
			  lastUpdate = time;
			  if(i < 0 || i > NUM_PIXELS){
				  i = NUM_PIXELS-1;
			  }

		// Chasing

		#define MIN_RPM      10.0f
		#define MAX_RPM      5000.0f
		#define MIN_SPEED    1.0f   // Speed is measured in dots moved per second
		#define MAX_SPEED    230.0f
		#define SLOPE        ((MAX_SPEED - MIN_SPEED) / (MAX_RPM - MIN_RPM)) // Dots per second to move per rpm (increased from base)
		#define INTERCEPT    (MIN_SPEED) // We will always move at least this speed

		#define SPACING      13

		static float offset = 0;

//		if(motorSpeedLF_rpm.data < (uint16_t)MIN_RPM)
//		{
//			float packVoltage = bmsAveBrickVoltage_V.data*84;
//			if(dcBusVoltage_V.data < 0.05f * packVoltage) {
//				  float intensity = abs((HAL_GetTick() % 2000) - 1000) / 1000.0f;
//				  setStripColor(0, (uint8_t)(255.0f * intensity), 0);
//			} else if (dcBusVoltage_V.data > 0.95f * packVoltage) {
//				float intensity = abs((HAL_GetTick() % 300) - 150) / 150.0f;
//				  setStripColor((uint8_t)(255.0f * intensity), 0, 0);
//			} else {
//				float intensity = abs((HAL_GetTick() % 500) - 250) / 250.0f;
//				  setStripColor(0, 0, (uint8_t)(255.0f * intensity));
//			}
//		}
//		else
//		{
//		  static float lastOffset = 0;
//		  float motorSpeed = (float)motorSpeedLF_rpm.data;
//		  if(motorSpeed > MAX_RPM)
//		  {
//			motorSpeed = MAX_RPM;
//		  }
//
//		  // Offset = (base_speed + speed_per_rpm * rpm) * delta_t
//		  offset += (INTERCEPT + SLOPE * motorSpeed) * (float)(HAL_GetTick() - lastUpdate) / 1000.0f;
//		  // If offset has advanced to the next integer value
//		  if((uint32_t)lastOffset != (uint32_t)offset)
//		  {
//			// Update the dotted line
//			dottedLine(SPACING, (uint32_t)offset, 255, 0, 0);
//			write();
//			fmod(offset, SPACING);
//		  }
//		  // Update last offset
//		  lastOffset = offset;
//		}
//		write();




//			  if(rpm < 8500)
//			  {
//				  lowMode = true;
//			  }
//		  }

//		  write();
//		lastUpdate = time;
	}
}

// end of GopherCAN_example.c
