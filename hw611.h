/*
 * hw611.h
 *
 *  Created on: Aug 21, 2025
 *      Author: kubax
 */

#ifndef INC_HW611_H_
#define INC_HW611_H_


#define HW611_I2C_TIMEOUT	1000

// Temperature resolution
#define HW611_TEMPERATURE_16BIT 1
#define HW611_TEMPERATURE_17BIT 2
#define HW611_TEMPERATURE_18BIT 3
#define HW611_TEMPERATURE_19BIT 4
#define HW611_TEMPERATURE_20BIT 5

// Pressure oversampling
#define HW611_ULTRALOWPOWER	1
#define HW611_LOWPOWER			2
#define HW611_STANDARD			3
#define HW611_HIGHRES			4
#define HW611_ULTRAHIGHRES		5

// t_standby time
#define BME280_STANDBY_MS_0_5	0
#define BME280_STANDBY_MS_10	6
#define BME280_STANDBY_MS_20	7
#define BME280_STANDBY_MS_62_5	1
#define BME280_STANDBY_MS_125	2
#define BME280_STANDBY_MS_250 	3
#define BME280_STANDBY_MS_500	4
#define BME280_STANDBY_MS_1000	5

// IIR filter
#define BME280_FILTER_OFF	0
#define BME280_FILTER_X2 	1
#define BME280_FILTER_X4 	2
#define BME280_FILTER_X8	3
#define BME280_FILTER_X16 	4

// Mode
#define HW611_SLEEPMODE		0
#define HW611_FORCEDMODE	1
#define HW611_NORMALMODE	3

//
//	Coeffs registers
//
#define	HW611_DIG_T1		0x88
#define	HW611_DIG_T2		0x8A
#define	HW611_DIG_T3		0x8C

#define	HW611_DIG_P1		0x8E
#define	HW611_DIG_P2		0x90
#define	HW611_DIG_P3		0x92
#define	HW611_DIG_P4		0x94
#define	HW611_DIG_P5		0x96
#define	HW611_DIG_P6		0x98
#define	HW611_DIG_P7		0x9A
#define	HW611_DIG_P8		0x9C
#define	HW611_DIG_P9		0x9E

//
//	Registers
//
#define	HW611_CHIPID			0xD0
#define	HW611_VERSION			0xD1
#define	HW611_SOFTRESET			0xE0
#define	HW611_CAL26				0xE1  // R calibration stored in 0xE1-0xF0
#define	HW611_STATUS			0xF3
#define	HW611_CONTROL			0xF4
#define	HW611_CONFIG			0xF5
#define	HW611_PRESSUREDATA		0xF7
#define	HW611_TEMPDATA			0xFA

//
//	Control bits
//
#define	HW611_MEASURING			(1<<3) // Conversion in progress




typedef struct
{

	I2C_HandleTypeDef *hw611_i2c;
	uint8_t Address;

	int16_t t2, t3, p2, p3, p4, p5, p6, p7, p8, p9;
	uint16_t t1, p1;

	int32_t t_fine;


}HW611_t;


uint8_t HW611_Init(HW611_t *hw611, I2C_HandleTypeDef *i2c, uint8_t address);
void HW611_SetMode(HW611_t *hw611, uint8_t Mode);
void HW611_SetPressureOversampling(HW611_t *hw611, uint8_t P_Oversampling);
void HW611_SetTemperatureOversampling(HW611_t *hw611, uint8_t T_Oversampling);
float HW611_ReadTemperatureEndValue(HW611_t *hw611);
int32_t HW611_ReadTemperatureRaw(HW611_t *hw611);
int32_t HW611_ReadPressureRaw(HW611_t *hw611);
float HW611_ReadTemperatureEndValue(HW611_t *hw611);
uint8_t HW611_ReadPressureTemperatureEndValue(HW611_t *hw611, float *Temperature, float *Pressure);




#endif /* INC_HW611_H_ */
