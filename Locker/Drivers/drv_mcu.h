#ifndef INC_DRV_MCU_H_
#define INC_DRV_MCU_H_

#include "main.h" // Mengakses HAL dan Pin Defines
#include <stdbool.h>

// Definisi Flash
#define FLASH_USER_START_ADDR   0x08003C00
#define FLASH_USER_END_ADDR     0x08003FFF

// Init
void Drv_Init(UART_HandleTypeDef *huart);

// RS485 & UID
void Drv_RS485_Send(uint8_t *data, uint16_t len);
void Drv_RS485_Receive_DMA(uint8_t *buffer, uint16_t size);
uint16_t Drv_RS485_GetRxLength(uint16_t buffer_size);
void Drv_GetUID(uint8_t* output);
bool Drv_CheckUIDMatch(uint8_t *uid_to_check);

// GPIO Control
void Drv_Lock_Set(bool state);
bool Drv_Lock_ReadInput(void);
void Drv_LED_Set(uint8_t mask); // Mask bit: 0=R, 1=G, 2=B

// Flash Memory
void Drv_Flash_Write(uint32_t address, uint8_t *data, uint16_t length);
void Drv_Flash_Read(uint32_t address, uint8_t *buffer, uint16_t length);

// Flags (managed by ISR)
extern volatile uint8_t Drv_RxIdleFlag;
extern volatile uint8_t Drv_TxBusyFlag;

#endif /* INC_DRV_MCU_H_ */
