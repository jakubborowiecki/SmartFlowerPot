/*
 * hw611.c
 *
 *  Created on: Aug 21, 2025
 *      Author: kubax
 */

#include "main.h"
#include "hw611.h"



uint8_t Read8(HW611_t *hw611, uint8_t Register)
{
	uint8_t Value;
	HAL_I2C_Mem_Read(hw611 -> hw611_i2c, ((hw611 -> Address)<<1), Register, 1, &Value, 1, HW611_I2C_TIMEOUT);

	return Value;
}

void Write8(HW611_t *hw611, uint8_t Register, uint8_t Value)
{
	HAL_I2C_Mem_Write(hw611 -> hw611_i2c, ((hw611 -> Address)<<1), Register, 1, &Value, 1, HW611_I2C_TIMEOUT);
}




uint16_t Read16(HW611_t *hw611, uint8_t Register)
{
	uint8_t Value[2];
	HAL_I2C_Mem_Read(hw611 -> hw611_i2c, ((hw611 -> Address)<<1), Register, 1, Value, 2, HW611_I2C_TIMEOUT);



	return ((Value[1] << 8) | Value[0]);
}

uint32_t Read24(HW611_t *hw611, uint8_t Register)
{
	uint8_t Value[3];
	HAL_I2C_Mem_Read(hw611 -> hw611_i2c, ((hw611 -> Address)<<1), Register, 1, Value, 3, HW611_I2C_TIMEOUT);



	return ((Value[0] << 16 | Value[1] << 8) | Value[2]);
}


void HW611_SetMode(HW611_t *hw611, uint8_t Mode)
{
	uint8_t Tmp;
	if (Mode > 3) Mode = 3;
	Tmp = Read8(hw611, HW611_CONTROL);
	Tmp = Tmp & 0xFC;
	Tmp |= Mode & 0x03;
	Write8(hw611, HW611_CONTROL, Tmp);
}


void HW611_SetPressureOversampling(HW611_t *hw611, uint8_t P_Oversampling)
{
	uint8_t Tmp;
	if (P_Oversampling > 5) P_Oversampling = 5;
	Tmp = Read8(hw611, HW611_CONTROL);
	Tmp = Tmp & 0xE3;

	Tmp |= ((P_Oversampling << 2) & 0x1C);
	Write8(hw611, HW611_CONTROL, Tmp);
}


void HW611_SetTemperatureOversampling(HW611_t *hw611, uint8_t T_Oversampling)
{
	uint8_t Tmp;
	if (T_Oversampling > 8) T_Oversampling = 8;
	Tmp = Read8(hw611, HW611_CONTROL);
	Tmp = Tmp & 0x1F;

	Tmp |= ((T_Oversampling << 5) & 0xE0);
	Write8(hw611, HW611_CONTROL, Tmp);
}


float HW611_ReadTemperatureEndValue(HW611_t *hw611)
{

	int32_t var1, var2, T;

	uint32_t adc_T;

	adc_T = HW611_ReadTemperatureRaw(hw611);

	var1 = ((((adc_T>>3) - ((int32_t)(hw611->t1)<<1))) * ((int32_t)(hw611->t2))) >> 11;

	var2 = (((((adc_T>>4) - ((int32_t)(hw611->t1))) * ((adc_T>>4) - ((int32_t)(hw611->t1)))) >> 12) *
	((int32_t)(hw611->t3))) >> 14;

	hw611->t_fine = var1 + var2;

	T = ((hw611->t_fine) * 5 + 128) >> 8;

	return (float)(T/100.0);

}
uint8_t HW611_ReadPressureTemperatureEndValue(HW611_t *hw611, float *Temperature, float *Pressure)
{
	// Have to read temperature first
	*Temperature = HW611_ReadTemperatureEndValue(hw611);

	int32_t var1, var2;
	uint32_t p;

	var1 = (((int32_t)(hw611->t_fine))>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)(hw611->p6));
	var2 = var2 + ((var1*((int32_t)(hw611->p5)))<<1);
	var2 = (var2>>2)+(((int32_t)(hw611->p4))<<16);
	var1 = ((((hw611->p3) * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)(hw611->p2)) * var1)>>1))>>18;
	var1 =((((32768+var1))*((int32_t)(hw611->p1)))>>15);

	if (var1 == 0)
	{
		return 1;
	}


	int32_t adc_P = HW611_ReadPressureRaw(hw611);

	p = (((int32_t)(((int32_t)1048576)-adc_P)-(var2>>12)))*3125;

	if (p < 0x80000000)
	{
		p = (p << 1) / ((uint32_t)var1);
	}
	else
	{
		p = (p / (uint32_t)var1) * 2;
	}
	var1 = (((int32_t)(hw611->p9)) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)(hw611->p8)))>>13;

	p = (uint32_t)((int32_t)p + ((var1 + var2 + (hw611->p7)) >> 4));

	*Pressure = (float)(p/100.0);

	return 0;

}



int32_t HW611_ReadTemperatureRaw(HW611_t *hw611)
{
	int32_t Tmp;

	Tmp = (int32_t)Read24(hw611, HW611_TEMPDATA);
	Tmp >>= 4;

	return Tmp;
}

int32_t HW611_ReadPressureRaw(HW611_t *hw611)
{
	int32_t Tmp;

	Tmp = (int32_t)Read24(hw611, HW611_PRESSUREDATA);
	Tmp >>= 4;

	return Tmp;
}




uint8_t HW611_Init(HW611_t *hw611, I2C_HandleTypeDef *i2c, uint8_t address)
{
	uint8_t ChipID;

	hw611 -> hw611_i2c = i2c;
	hw611 -> Address = address;

	ChipID = Read8(hw611, HW611_CHIPID);

	if (ChipID != 0x58)
	{
		return 1;
	}

	hw611 -> t1 = Read16(hw611, HW611_DIG_T1);
	hw611 -> t2 = Read16(hw611, HW611_DIG_T2);
	hw611 -> t3 = Read16(hw611, HW611_DIG_T3);

	hw611 -> p1 = Read16(hw611, HW611_DIG_P1);
	hw611 -> p2 = Read16(hw611, HW611_DIG_P2);
	hw611 -> p3 = Read16(hw611, HW611_DIG_P3);
	hw611 -> p4 = Read16(hw611, HW611_DIG_P4);
	hw611 -> p5 = Read16(hw611, HW611_DIG_P5);
	hw611 -> p6 = Read16(hw611, HW611_DIG_P6);
	hw611 -> p7 = Read16(hw611, HW611_DIG_P7);
	hw611 -> p8 = Read16(hw611, HW611_DIG_P8);
	hw611 -> p9 = Read16(hw611, HW611_DIG_P9);

	HW611_SetMode(hw611, HW611_NORMALMODE);
	HW611_SetPressureOversampling(hw611, HW611_ULTRAHIGHRES);
	HW611_SetTemperatureOversampling(hw611, HW611_TEMPERATURE_20BIT);

	return 0;
}


