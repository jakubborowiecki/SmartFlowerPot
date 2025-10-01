/*
 * button.h
 *
 *  Created on: Jul 12, 2025
 *      Author: kubax
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

// states for state machine
typedef enum{
	IDLE = 0,
	DEBOUNCE,
	PRESSED,
	REPEAT

}BUTTON_STATE;

// struct for button

typedef struct{
	BUTTON_STATE State;
	GPIO_TypeDef *GPIOPort;
	uint16_t GpioPin;

	uint32_t LastTick;
	uint32_t TimerDebounce; // stala
	uint32_t LongPress;
	uint32_t TimerRepeat;

	void(*ButtonPressed)(void);
	void(*ButtonRepeat)(void);
	void(*ButtonLongPress)(void);
	void(*ReleaseLongPress)(void);
}TButton_t;

// public function
void ButtonRegisterPressCallback(TButton_t *Key, void *Callback);
void ButtonRegisterHoldCallback(TButton_t *Key, void *Callback);
void ButtonRegisterLongPressCallback(TButton_t *Key, void *Callback);
void ReleaseButtonRegisterCallback(TButton_t *Key, void *Callback);



void ButtonSetDebounce(TButton_t *Key, uint32_t new_time_debounce);

void ButtonTask(TButton_t *Key);
void ButtonInitKey(TButton_t *Key, GPIO_TypeDef *GPIOPort, uint16_t GpioPin, uint32_t TimerDebounce, uint32_t LongPress, uint32_t TimerRepeat);



#endif /* INC_BUTTON_H_ */
