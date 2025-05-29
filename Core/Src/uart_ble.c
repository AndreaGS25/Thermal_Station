/*************************************************************************//**
 *
 *    @file	    	uart_ble.c
 *    @date			15.05.2025
 *    @version  	0.1.0
 *
 *    @author 		Gemetti Andrea
 *
 ******************************************************************************
 *
 *    @brief		Implements the uart_ble function for HC-06 module
 ******************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "uart_ble.h"
#include "usart.h"
/* Private define ------------------------------------------------------------*/

struct ble_buffer {
	uint8_t tx_buf[TX_BUFFER_BIM];
	uint8_t rx_buf[RX_BUFFER_BIM];
};

/* Private data types definition ---------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
struct ble_buffer local_buffer;
UART_HandleTypeDef *ble_uart;

/* Private variables ---------------------------------------------------------*/
bool end_msg_flag;
bool tx_transmit_flag;
uint8_t com_len;
/* Private function prototypes -----------------------------------------------*/

/* Functions definition ------------------------------------------------------*/

/**
  * @brief
  * @return
  */
void uart_ble_init(UART_HandleTypeDef *huart){
	ble_uart = huart;
	HAL_UART_Receive_IT(ble_uart, (uint8_t*)&local_buffer.rx_buf[0], 1);
}

/**
  * @brief
  * @return
  */
void uart_ble_set_boutrate(uint32_t baudrate){
	HAL_UART_DeInit(ble_uart);
	ble_uart->Init.BaudRate = baudrate;
	HAL_UART_Init(ble_uart);
}

/**
  * @brief
  * @return
  */
bool uart_ble_transmit(uint8_t* text){
	if (tx_transmit_flag == false) {
		tx_transmit_flag = true;
		HAL_UART_Transmit_IT(ble_uart, (char *) text, strlen((char *)text));
		return true;
	}
	return false;
}


/**
 * @brief
 * @return
 */
bool uart_ble_pop_cmd(uint8_t *cmd)
{
	if (end_msg_flag==true){
		strncpy((char *)cmd, (char *) local_buffer.rx_buf, strlen((char *)local_buffer.rx_buf)+1);
		end_msg_flag = false;
		return true;
	}
	return false;
}



/**
  * @brief
  * @return
  */
void uart_ble_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == ble_uart->Instance){
		com_len ++;
		if (local_buffer.rx_buf[com_len-1] == '\n' ||
				local_buffer.rx_buf[com_len-1] == '\r'){
			if(com_len>2){
				end_msg_flag = true;
				local_buffer.rx_buf[com_len-1] = '\0';
				com_len = 0;
			}else{
				com_len = 0;
			}
		}
		HAL_UART_Receive_IT(ble_uart, (uint8_t*)&local_buffer.rx_buf[com_len], 1);
	}
}

/**
  * @brief
  * @return
  */
void uart_ble_TxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == ble_uart->Instance){
		tx_transmit_flag = false;
	}
}

