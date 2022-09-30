/*
 * GopherCAN_Interface.h
 *
 *  Created on: Sep 29, 2022
 *      Author: Isaiah
 */

#ifndef INC_GOPHERCAN_INTERFACE_H_
#define INC_GOPHERCAN_INTERFACE_H_

#include "GopherCAN.h"

void init(CAN_HandleTypeDef* hcan_ptr);
void can_buffer_handling_loop();
void main_loop();

#endif /* INC_GOPHERCAN_INTERFACE_H_ */
