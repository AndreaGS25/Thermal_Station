/*************************************************************************//**
 *
 *    @file	    	uart_ble.c
 *    @date			27.05.2025
 *    @version  	0.1.0
 *
 *    @author 		Gemetti Andrea
 *
 ******************************************************************************
 *
 *    @brief		Implements the temperature function for digital and analog
 *    				module
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include "tsens.h"
#include "uart_ble.h"
#include "adc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Private define ------------------------------------------------------------*/
ADC_HandleTypeDef *adc_digital;
SPI_HandleTypeDef *spi_digital;
ADC_ChannelConfTypeDef analog_tsens_sConfig;
uint32_t adc_tsens_value;
bool tsens_flag_analizer;
/* Private data types definition ---------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Functions definition ------------------------------------------------------*/

/**
  * @brief
  * @return
  */
void analog_tsens_init(ADC_HandleTypeDef *hadc,uint32_t channel){
	adc_digital = hadc;
	analog_tsens_sConfig.Channel = channel;
	analog_tsens_sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	analog_tsens_sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;

	HAL_ADC_ConfigChannel(adc_digital, &analog_tsens_sConfig);
	HAL_ADC_Start_DMA(adc_digital,(uint32_t*)&adc_tsens_value,1);
}

/*
  * @brief
  * @return
  */
uint32_t get_analog_tsens(void){
	HAL_ADC_ConfigChannel(adc_digital, &analog_tsens_sConfig);
	tsens_flag_analizer = false;
	HAL_ADC_Start_DMA(adc_digital,(uint32_t*)&adc_tsens_value,1);
	uint32_t ADC_temp = adc_tsens_value * 830;
	ADC_temp = (ADC_temp/100)-27315;
	return ADC_temp;
}
/**
  * @brief
  * @return
  */

/**
  * @brief
  * @return
  */

/**
  * @brief
  * @return
  */

/**
 * @brief
 * @return
 */

/**
  * @brief
  * @return
  */

/**
  * @brief
  * @return
  */

