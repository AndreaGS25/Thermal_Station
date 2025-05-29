/*************************************************************************//**
 *
 *    @file	    	tsens.h
 *    @date			27.05.2025
 *    @version  	0.1.0
 *
 *    @author 		Gemetti Andrea
 *
 ******************************************************************************
 *
 *    @brief		implement function for use the HC-06 serial Bluetooth
 *
 ******************************************************************************/

#ifndef INC_TSENS_H_
#define INC_TSENS_H_

/* Public includes -----------------------------------------------------------*/
#include <stdbool.h>
#include "adc.h"
#include "spi.h"

/* Public define -------------------------------------------------------------*/
extern ADC_HandleTypeDef hadc;
extern SPI_HandleTypeDef hspi1;

/* Public data types definition ----------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/
void analog_tsens_init(ADC_HandleTypeDef *hadc,uint32_t channel);
uint32_t get_analog_tsens(void);
#endif /* INC_UART_BLE_H_ */













