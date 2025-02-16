/*
 * sensor_handler.h
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include "vl53l8cx_platform.h"
#include "vl53l8cx_api.h"

void Sensor_Init(void);
uint16_t Sensor_GetDistance(void);

#endif /* SENSOR_HANDLER_H */

