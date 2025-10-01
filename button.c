/*
 * botton.c
 *
 *  Created on: Jul 12, 2025
 *      Author: kubax
 */

#include "main.h"
#include "button.h"

static uint8_t ButtonIsPressed(TButton_t *Key){
	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(Key -> GPIOPort, Key -> GpioPin))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// button init
void ButtonInitKey(TButton_t *Key, GPIO_TypeDef *GPIOPort, uint16_t GpioPin, uint32_t TimerDebounce, uint32_t LongPress, uint32_t TimerRepeat)
{
	Key -> State = IDLE;
	Key -> GPIOPort = GPIOPort;
	Key -> GpioPin = GpioPin;
	Key -> TimerDebounce = TimerDebounce;
	Key -> LongPress = LongPress;
	Key -> TimerRepeat= TimerRepeat;
}
// time setting functions


void ButtonSetDebounce(TButton_t *Key, uint32_t new_time_debounce)
{
	Key -> TimerDebounce= new_time_debounce;
}




// register callbacks

void ButtonRegisterPressCallback(TButton_t *Key, void *Callback)
{
	Key -> ButtonPressed = Callback;
}

void ButtonRegisterHoldCallback(TButton_t *Key, void *Callback)
{
	Key -> ButtonRepeat = Callback;
}

void ButtonRegisterLongPressCallback(TButton_t *Key, void *Callback)
{
	Key -> ButtonLongPress = Callback;
}

void ReleaseButtonRegisterCallback(TButton_t *Key, void *Callback)
{
	Key -> ReleaseLongPress = Callback;
}



// states routine
void ButtonIdleRoutine(TButton_t *Key)
{
	if(Key -> ReleaseLongPress != NULL)
	{
	Key -> ReleaseLongPress();
	}

	if (ButtonIsPressed(Key)){
		Key -> State = DEBOUNCE;
		Key -> LastTick = HAL_GetTick();
	}
}

void ButtonDebounceRoutine(TButton_t *Key)
{


	if ((HAL_GetTick() - (Key -> LastTick)) > Key -> TimerDebounce)
	{
		if (ButtonIsPressed(Key))
		{

			Key -> State = PRESSED;
			Key -> LastTick = HAL_GetTick();

			if(Key -> ButtonPressed != NULL)
			{
			Key -> ButtonPressed();
			}
		}
		else{
			Key -> State = IDLE;
		}
	}
}





void ButtonPressedRoutine(TButton_t *Key)
{
	if(!ButtonIsPressed(Key))
	{
		Key -> State = IDLE;
	}

	else if ((HAL_GetTick() - Key -> LastTick) > Key -> LongPress)
	{
		Key -> State = REPEAT;
		Key -> LastTick = HAL_GetTick();

		if(Key -> ButtonLongPress != NULL)
		{
			Key -> ButtonLongPress();
		}

	}
}






void ButtonRepeatRoutine(TButton_t *Key)
{
	if (!ButtonIsPressed(Key))
	{
		Key -> State = IDLE;
	}



	else
	{
		if((HAL_GetTick() - Key -> LastTick) > Key -> TimerRepeat)
		{
			Key -> LastTick = HAL_GetTick();

			if(Key -> ButtonRepeat != NULL)
			{
				Key -> ButtonRepeat();

			}
		}
	}


}





// state machine
void ButtonTask(TButton_t *Key){



	switch(Key -> State)
	{
	case IDLE:
		ButtonIdleRoutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;

	case PRESSED:
		ButtonPressedRoutine(Key);
		break;

	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;

	}
}




