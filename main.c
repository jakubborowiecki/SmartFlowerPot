/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "button.h"
#include "Oled_state.h"
#include "OLED.h"
#include "hw611.h"
#include "GFX_OLED.h"
#include "fonts.h"
#include "stdio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define WATERING_TIME 1000
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
TButton_t YellowButton;
TButton_t BlueButton;
uint8_t OLED_Status;



uint8_t PermissionToWatering = 0;


uint32_t WateringTimer;


uint8_t display_stop;

extern OLED_State Screen_State;


uint32_t Potentiometr;
uint32_t Water_Height;
uint32_t Humidity_Soil;


float Temperature_main;
float Pressure_main;

uint16_t serwo_value;

HW611_t czujnik;
uint32_t timer_HW611;

uint32_t timer_serwomotor;

uint32_t timer_temp_vector;
uint8_t temp_history[60];
uint8_t index_temp_history = 0;

uint8_t status_temp_graph;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ChangeOledDisplay(void);
void ActivatePomp(void);
void set_servo_position(uint16_t position_us);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

  HAL_TIM_Base_Start_IT(&htim4);


  HAL_ADC_Start_DMA(&hadc1, &Potentiometr, 1);
  HAL_ADC_Start_DMA(&hadc2, &Humidity_Soil, 1);
  HAL_ADC_Start_DMA(&hadc3, &Water_Height, 1);


  HW611_Init(&czujnik, &hi2c1, 0x76);

  OLED_Init(&hi2c1);
  SetFont(font_8x5);
  OLED_Clear(OLED_BLACK);


  ButtonInitKey(&YellowButton, Yellow_Button_GPIO_Port, Yellow_Button_Pin, 40, 3000, 500);
  ButtonInitKey(&BlueButton, B1_GPIO_Port, B1_Pin, 40, 3000, 500);


  ButtonRegisterPressCallback(&YellowButton, ChangeOledDisplay);
  ButtonRegisterPressCallback(&BlueButton, ActivatePomp);




  timer_temp_vector = HAL_GetTick();
  timer_HW611 = HAL_GetTick();
  timer_serwomotor = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  ButtonTask(&YellowButton);
	  ButtonTask(&BlueButton);


	  Oled_Control();


	  if ((HAL_GetTick() - timer_serwomotor) > 200)
	  {

		  serwo_value = 2000 - (Potentiometr*0.341);
		  set_servo_position(serwo_value);
	  }


	  if(Screen_State == Currently_Using_Pump_State)
	  {
		  PermissionToWatering = 1;
	  }
	  else
	  {
		  PermissionToWatering = 0;
	  }



	  // data to graph
	  if ((HAL_GetTick() - timer_temp_vector) > 500)
	  {
		  temp_history[index_temp_history] = Temperature_main;
		  index_temp_history = (index_temp_history + 1) % 60;
		  timer_temp_vector = HAL_GetTick();
	  }



	  // data update
	  if ((HAL_GetTick() - timer_HW611) > 200)
	  {
		  timer_HW611 = HAL_GetTick();

		  if (czujnik.hw611_i2c -> State == HAL_I2C_STATE_READY)
		  {
			  HW611_ReadPressureTemperatureEndValue(&czujnik, &Temperature_main, &Pressure_main);
		  }
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM4)
	{
		status_temp_graph = 1;
	}
}

void ChangeOledDisplay()
{
	ChangeOledState();
}

void ActivatePomp(void)
{
	if(PermissionToWatering == 1)
	{

		HAL_GPIO_WritePin(WaterPump_GPIO_Port, WaterPump_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
		WateringTimer = HAL_GetTick();

		while((HAL_GetTick() - WateringTimer) < WATERING_TIME)
		{

		}

		HAL_GPIO_WritePin(WaterPump_GPIO_Port, WaterPump_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);


	}
}

void set_servo_position(uint16_t position_us)
{


    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, position_us);

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
