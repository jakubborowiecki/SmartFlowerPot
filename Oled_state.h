/*
 * Button.h
 *
 *  Created on: Sep 25, 2025
 *      Author: kubax
 */

#ifndef INC_OLED_STATE_H_
#define INC_OLED_STAT_H_

#include "GFX_OLED.h"
#include "OLED.h"

typedef enum
{
	Data_Temp_Pressure_Humidity_State = 0,
	Liquid_Height_State, // Visualization (rectangle drawing with water level) and value in %
	Temperature_Graph_State, //min/max value and linear graph
	Currently_Using_Pump_State //pouring time and amount of water

}OLED_State;


void Oled_Control(); //machine state to screen
void ChangeOledState();



#endif /* INC_OLED_STATE_H_ */
