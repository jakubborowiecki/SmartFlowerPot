/*
 * Oled_state.c
 *
 *  Created on: Sep 25, 2025
 *      Author: kubax
 */

#include "main.h"
#include "Oled_state.h"
#include "hw611.h"
#include "stdio.h"

//Temperature and Pressure from BMP280
extern float Temperature_main;
extern float Pressure_main;

//Data to fgraph
extern uint8_t status_temp_graph;
extern uint8_t temp_history[60];
extern uint8_t index_temp_history;


//ADC
extern uint32_t Potentiometr;
extern uint32_t Humidity_Soil;
extern uint32_t Water_Height;

char Message[32];

OLED_State Screen_State;


void Data_Temp_Pressure_Humidity_State_Function()
{
	sprintf(Message, "SMART FLOWER POT");
	DrawString(20, 5, Message, WHITE, BLACK);

	drawFastHLine(0, 14, 128, WHITE);

	sprintf(Message, "Temperature: %.2f C", Temperature_main);
	DrawString(2, 22, Message, WHITE, BLACK);

	sprintf(Message, "Pressure: %.2f hPa", Pressure_main);
	DrawString(2, 34, Message, WHITE, BLACK);

	sprintf(Message, "Soil moisture: %.2lu %%", 100 - Humidity_Soil/41);
	DrawString(2, 46, Message, WHITE, BLACK);


	OLED_Display();
}

const float SCALE_FACTOR_Water_Height = 0.010986578;
float result_water_scale;

void Liquid_Height_Function_State()
{
	sprintf(Message, "100 %%");
	DrawString(2, 10, Message, WHITE, BLACK);

	sprintf(Message, "75  %%");
	DrawString(2, 20, Message, WHITE, BLACK);

	sprintf(Message, "50  %%");
	DrawString(2, 30, Message, WHITE, BLACK);

	sprintf(Message, "25  %%");
	DrawString(2, 40, Message, WHITE, BLACK);

	sprintf(Message, "0   %%");
	DrawString(2, 50, Message, WHITE, BLACK);

	sprintf(Message, "WATER HEIGHT");
	DrawString(30, 0, Message, WHITE, BLACK);

	drawRect(55, 12, 60, 45, WHITE);
	drawFastHLine(55, 12, 60, BLACK);

	for(uint8_t i = 0; i < 40; i = i + 5)
	{
		result_water_scale = (float)Water_Height * SCALE_FACTOR_Water_Height;
		if (result_water_scale > i)
		{
			writeFillRect(60, 48 - i , 50, 5, WHITE);
		}
		else
		{
			writeFillRect(60, 48 - i , 50, 5, BLACK);
		}
	}

	OLED_Display();
}


uint8_t read_index;
float temp;
uint8_t current_x;
uint8_t current_y;


uint8_t x_start = 45; //25
uint8_t y_start = 15;
uint8_t graph_height = 38;
uint8_t graph_width = 60;


#define T_MIN       20
#define T_MAX       29
#define Y_PIXEL_MAX 53
#define Y_PIXEL_MIN 15
#define T_RANGE     (T_MAX - T_MIN)
#define Y_RANGE     (Y_PIXEL_MAX - Y_PIXEL_MIN)

float temp_diff;
float y_offset;

uint8_t map_temp_to_y(float temp) {
	temp = (uint8_t)temp;
    if (temp <= T_MIN) return Y_PIXEL_MAX;
    if (temp >= T_MAX) return Y_PIXEL_MIN;


    temp_diff = temp - T_MIN;

    y_offset = (temp_diff * Y_RANGE) / T_RANGE;

    return Y_PIXEL_MAX - y_offset;
}



void Temperature_Graph_State_Function()
{
	sprintf(Message, "TEMPERATURE GRAPH");
	DrawString(15, 0, Message, WHITE, BLACK);



	sprintf(Message, "29");
	DrawString(2, 12, Message, WHITE, BLACK);

	sprintf(Message, "26");
	DrawString(2, 25, Message, WHITE, BLACK);

	sprintf(Message, "23");
	DrawString(2, 38, Message, WHITE, BLACK);

	sprintf(Message, "20");
	DrawString(2, 50, Message, WHITE, BLACK);



	sprintf(Message, "NOW");
	DrawString(95, 55, Message, WHITE, BLACK);



	sprintf(Message, "-30s");
	DrawString(35, 55, Message, WHITE, BLACK);







	drawFastVLine(20, 15, 38, WHITE);


	if(status_temp_graph)
	{
		status_temp_graph = 0;

		writeFillRect(21, 15, 107, 40, BLACK);


		for(uint8_t i = 0; i < 60; i++)
		{
			read_index = (index_temp_history + i) % 60;
			temp = temp_history[read_index];

			current_x = x_start + i;
	        current_y = map_temp_to_y(temp);

	        if(temp == 0)
	        {
	        	continue;
	        }

	        drawPixel(current_x, current_y, WHITE);

		}
        OLED_Display();

	}

}

const uint8_t water_drop_32x32[] = {
		  0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0x3f, 0xff, 0xff,
		  0xff, 0xfe, 0x3f, 0xff, 0xff,
		  0xff, 0xfe, 0x0f, 0xff, 0xff,
		  0xff, 0xfe, 0x07, 0xff, 0xff,
		  0xff, 0xff, 0xa3, 0xff, 0xff,
		  0xff, 0xff, 0xb1, 0xff, 0xff,
		  0xff, 0xff, 0xb1, 0xff, 0xff,
		  0xff, 0xff, 0xb1, 0xff, 0xff,
		  0xff, 0xfe, 0x3c, 0x7f, 0xff,
		  0xff, 0xfe, 0x3c, 0x3f, 0xff,
		  0xff, 0xfe, 0x3c, 0x3f, 0xff,
		  0xff, 0xfe, 0x3e, 0x3f, 0xff,
		  0xff, 0xfc, 0x7f, 0x1f, 0xff,
		  0xff, 0xfc, 0x7f, 0x1f, 0xff,
		  0xff, 0xf8, 0xff, 0x0f, 0xff,
		  0xff, 0xf9, 0xff, 0x8f, 0xff,
		  0xff, 0xf3, 0xff, 0xc7, 0xff,
		  0xff, 0xf7, 0xff, 0xe3, 0xff,
		  0xff, 0xe7, 0xff, 0xe1, 0xff,
		  0xff, 0xc7, 0xff, 0xf1, 0xff,
		  0xff, 0x8f, 0xff, 0xf9, 0xff,
		  0xff, 0x9f, 0xff, 0xf9, 0xff,
		  0xff, 0x1f, 0xff, 0xf8, 0xff,
		  0xff, 0x1f, 0xff, 0xfc, 0xff,
		  0xff, 0x3f, 0xff, 0xf8, 0xff,
		  0xff, 0x1f, 0xff, 0xf8, 0xff,
		  0xff, 0x1f, 0xff, 0xf8, 0xff,
		  0xff, 0x8f, 0xff, 0xfc, 0xff,
		  0xff, 0x8f, 0xff, 0xfc, 0xff,
		  0xff, 0x8f, 0xff, 0xfc, 0xff,
		  0xff, 0xc7, 0xff, 0xfc, 0xff,
		  0xff, 0xe1, 0xff, 0xe1, 0xff,
		  0xff, 0xf0, 0xff, 0xe1, 0xff,
		  0xff, 0xf8, 0x7f, 0x03, 0xff,
		  0xff, 0xfe, 0x00, 0x1f, 0xff,
		  0xff, 0xff, 0x81, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff,
		  0xff, 0xff, 0xff, 0xff, 0xff
};


void Currently_Using_Pump_State_Function()
{
	sprintf(Message, "PRESS BLUE BUTTON");
	DrawString(10, 5, Message, WHITE, BLACK);

	sprintf(Message, "TO START WATERING");
	DrawString(10, 15, Message, WHITE, BLACK);

	Image(42, 28, water_drop_32x32, 40, 40, WHITE);
	OLED_Display();
}



void Oled_Control() // state machine
{

	switch(Screen_State)
	{
	case Data_Temp_Pressure_Humidity_State:
		Data_Temp_Pressure_Humidity_State_Function();
		break;

	case Liquid_Height_State:
		Liquid_Height_Function_State();
		break;

	case Temperature_Graph_State:
		Temperature_Graph_State_Function();
		break;

	case Currently_Using_Pump_State:
		Currently_Using_Pump_State_Function();
		break;
	}
}

void ChangeOledState()
{
	Screen_State = (Screen_State + 1) % 4;
	OLED_Clear(BLACK);
}



