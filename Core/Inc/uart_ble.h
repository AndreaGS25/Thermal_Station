/*************************************************************************//**
 *
 *    @file	    	uart_ble.h
 *    @date			15.05.2025
 *    @version  	0.1.0
 *
 *    @author 		Gemetti Andrea
 *
 ******************************************************************************
 *
 *    @brief		implement function for use the HC-06 serial Bluetooth
 *
 ******************************************************************************/

#ifndef INC_UART_BLE_H_
#define INC_UART_BLE_H_

/* Public includes -----------------------------------------------------------*/
#include "usart.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* Public define -------------------------------------------------------------*/

#define TX_BUFFER_BIM 100
#define RX_BUFFER_BIM 100 //max 254

extern UART_HandleTypeDef huart1;



/* Public data types definition ----------------------------------------------*/


/* Public variables ----------------------------------------------------------*/


/* Public function prototypes ------------------------------------------------*/

void uart_ble_init(UART_HandleTypeDef *huart);
void uart_ble_set_boutrate(uint32_t baudrate); 													//not implemented
bool uart_ble_transmit(uint8_t* text);
bool uart_ble_pop_cmd(uint8_t *cmd);
void uart_ble_RxCpltCallback(UART_HandleTypeDef *huart);
void uart_ble_TxCpltCallback(UART_HandleTypeDef *huart);
#endif /* INC_UART_BLE_H_ */













