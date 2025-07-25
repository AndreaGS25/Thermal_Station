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
  * implementare sensore temperatura spi
  * implementare pwm to sin
  * implementare gpio expander
  * implementare sdcard
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "uart_ble.h"
#include "tsens.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim2;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t value_adc;
uint32_t value_dac=0;
extern uint8_t tsens_flag_analizer;
uint8_t msgbuffer [25] = {0};
#define maxperiod 100
#define minperiod 1
uint32_t period_t2 = 1;
bool pwm_flag = true;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
bool cmd_command_analizing(uint8_t *cmd);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim2;

void pwm_for_sin(){
	if (pwm_flag == true){
		if (period_t2 < maxperiod){
			period_t2 ++;
		}else{
			period_t2 --;
			pwm_flag = false;
		}
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period_t2);
	}else{
		if (period_t2 > minperiod){
			period_t2 --;
		}else{
			period_t2 ++;
			pwm_flag = true;
		}
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, period_t2);
	}
	/*
	if (value == 0){
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_TIM_OC_Stop_IT(&htim2, TIM_CHANNEL_1);
	}
	else if(value<=100){
		HAL_TIM_Base_Start_IT(&htim2);
		HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, value-1);
		led1_brightness = value;
	}*/
}

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
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  MX_TIM16_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t command[100];
  uart_ble_init(&huart1);
  uart_ble_transmit((uint8_t*)"status ok");
  analog_tsens_init(&hadc, ADC_CHANNEL_0);
  HAL_TIM_Base_Start_IT(&htim16);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start_IT(&htim2, TIM_CHANNEL_2);

  while (1)
  {
	  if (tsens_flag_analizer == 1){

	  }
	  if (uart_ble_pop_cmd(command) == true){
		  cmd_command_analizing(command);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14
                              |RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

bool cmd_command_analizing(uint8_t *cmd){
	if (strncmp((char*)cmd, (char*)"redLED : on", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"greenLED : on", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"yellowLED : on", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_SET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"redLED : off", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"greenLED : off", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"yellowLED : off", strlen((char*)cmd)) == 0){
		HAL_GPIO_WritePin(LED_Y_GPIO_Port, LED_Y_Pin, GPIO_PIN_RESET);
		return true;
	}else if (strncmp((char*)cmd, (char*)"LEDs toggled", strlen((char*)cmd)) == 0){
		HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
		HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
		HAL_GPIO_TogglePin(LED_Y_GPIO_Port, LED_Y_Pin);
		return true;
	}else if (strncmp((char*)cmd, (char*)"AnalogSensor Temp value =", strlen((char*)cmd)) == 0){
		uint32_t temp = get_analog_tsens();
		sprintf((char*)&msgbuffer, "T: %u,%u °C\n", (unsigned int)(temp/100), (unsigned int)(temp%100));

		//sprintf((char*)&msgbuffer, "T: %u,%u °C\n", (unsigned int)(temp/100), (unsigned int)(temp%100));
		uart_ble_transmit((uint8_t*)msgbuffer);
		//HAL_Delay(100);
		//uart_ble_transmit((uint8_t*)"ma porca la puttana ladra");

	}else if (strncmp((char*)cmd, (char*)"DigitalSensor Temp value =", strlen((char*)cmd)) == 0){
		uart_ble_transmit((uint8_t*)"digital ok");
		return true;
	}else if (strncmp((char*)cmd, (char*)"TBD =", strlen((char*)cmd)) == 0){
		uart_ble_transmit((uint8_t*)"tbd ok");
		return true;
	}else {
		return false;
	}
	return false;
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

#ifdef  USE_FULL_ASSERT
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
