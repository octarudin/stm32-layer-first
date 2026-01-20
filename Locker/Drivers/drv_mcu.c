/*
 * drv_mcu.c
 *
 *  Created on: Jan 19, 2026
 *      Author: compu
 */
#include "drv_mcu.h"

static UART_HandleTypeDef *phuart;
volatile uint8_t Drv_RxIdleFlag = 0;
volatile uint8_t Drv_TxBusyFlag = 0;

void Drv_Init(UART_HandleTypeDef *huart) {
    phuart = huart;
    HAL_GPIO_WritePin(RS485_CT_GPIO_Port, RS485_CT_Pin, GPIO_PIN_RESET);
}

void Drv_RS485_Send(uint8_t *data, uint16_t len) {
    while (Drv_TxBusyFlag); // Wait for previous TX
    Drv_TxBusyFlag = 1;
    HAL_GPIO_WritePin(RS485_CT_GPIO_Port, RS485_CT_Pin, GPIO_PIN_SET);
    HAL_UART_Transmit_DMA(phuart, data, len);
}

void Drv_RS485_Receive_DMA(uint8_t *buffer, uint16_t size) {
    HAL_UART_Receive_DMA(phuart, buffer, size);
}

uint16_t Drv_RS485_GetRxLength(uint16_t buffer_size) {
    return buffer_size - __HAL_DMA_GET_COUNTER(phuart->hdmarx);
}

void Drv_GetUID(uint8_t* output) {
    uint32_t hw_uid[3];
    hw_uid[0] = HAL_GetUIDw0();
    hw_uid[1] = HAL_GetUIDw1();
    hw_uid[2] = HAL_GetUIDw2();
    memcpy(output, hw_uid, sizeof(hw_uid));
}

bool Drv_CheckUIDMatch(uint8_t *uid_to_check) {
    uint8_t hw_uid[12];
    Drv_GetUID(hw_uid);
    return (memcmp(uid_to_check, hw_uid, 12) == 0);
}

void Drv_Lock_Set(bool state) {
    HAL_GPIO_WritePin(LOCK_OUT_GPIO_Port, LOCK_OUT_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool Drv_Lock_ReadInput(void) {
    return (HAL_GPIO_ReadPin(LOCK_IN_GPIO_Port, LOCK_IN_Pin) == GPIO_PIN_SET);
}

void Drv_LED_Set(uint8_t mask) {
    HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, (mask & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, (mask & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, (mask & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Drv_Flash_Write(uint32_t address, uint8_t *data, uint16_t length) {
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInit;
    uint32_t pageError = 0;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.PageAddress = FLASH_USER_START_ADDR;
    eraseInit.NbPages = 1;

    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) == HAL_OK) {
        for (uint8_t i = 0; i < length; i += 4) {
            uint32_t data32 = 0;
            memcpy(&data32, &data[i], 4);
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address + i, data32);
        }
    }
    HAL_FLASH_Lock();
}

void Drv_Flash_Read(uint32_t address, uint8_t *buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++)
        buffer[i] = *(uint8_t*)(address + i);
}

// Callbacks from Main ISR
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        Drv_TxBusyFlag = 0;
        HAL_GPIO_WritePin(RS485_CT_GPIO_Port, RS485_CT_Pin, GPIO_PIN_RESET);
    }
}
