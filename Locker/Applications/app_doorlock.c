/*
 * app_doorlock.c
 *
 *  Created on: Jan 19, 2026
 *      Author: compu
 */
#include "app_doorlock.h"
#include "drv_mcu.h"
#include "protocol.h"
#include <stdio.h>  // Tambahkan ini untuk sprintf

// Memory Map Addresses
#define UNIQUE_ADDR         0x00
#define LAST_ACCESSED_ADDR  0x0C
#define CONTROL_ADDR        0x14
#define LOCK_INPUT_ADDR     0x15
#define POLL_MODE_ADDR      0xFF

// Command Types
#define CMD_WRITE 0x01
#define CMD_READ  0x02

// Internal Variables
static uint8_t memory[256];
static uint8_t rxBuffer[RX_BUF_SIZE];
static uint8_t txBuffer[TX_BUF_SIZE];
static frame_t currentFrame;
static bool    pendingLockFrame = false;
static uint8_t pollCounter = 0;
static uint8_t doorLockCycles = 0;

// Helper Functions
static void App_HandleWrite(void);
static void App_HandleRead(void);
static void App_UpdateIO(void);
static Err_StatusTypeDef App_ExecuteWrite(uint8_t addr, uint8_t *data, uint8_t size);
static Err_StatusTypeDef App_ExecuteRead(uint8_t addr, uint8_t size, uint8_t *outData);

void App_Init(void) {
	// Mulai mendengarkan data (DMA RX)
    Drv_RS485_Receive_DMA(rxBuffer, RX_BUF_SIZE);
}

void App_SignalLockTrigger(void) {
    pendingLockFrame = true;
}

void App_Process(void) {
    // 1. Check RX Data
    if (Drv_RxIdleFlag) {
        Drv_RxIdleFlag = 0;
        uint16_t len = Drv_RS485_GetRxLength(RX_BUF_SIZE);

        // Stop DMA sementara jika perlu, atau parsing buffer langsung
        Err_StatusTypeDef status = Protocol_ParseFrame(rxBuffer, len, &currentFrame);

        if (status == OK) {
            // Check Head/Tail & UID
            if (Protocol_IsHeadTailCorrect(currentFrame.head, currentFrame.tail)) {
                bool isPoll = (currentFrame.addr == POLL_MODE_ADDR);
                bool uidMatch = Drv_CheckUIDMatch(currentFrame.uid);

                if (uidMatch || isPoll) {
                    if (currentFrame.cmd == CMD_WRITE) App_HandleWrite();
                    else if (currentFrame.cmd == CMD_READ) App_HandleRead();
                    // else: invalid cmd err
                }
            }
        } else {
             // Kirim Error Response jika CRC/Len salah (opsional, sesuai requirement lama)
             uint8_t len = Protocol_BuildPacket(&currentFrame, txBuffer, true, status);
             Drv_RS485_Send(txBuffer, len);
        }

        // Restart DMA
        Drv_RS485_Receive_DMA(rxBuffer, RX_BUF_SIZE);
    }

    // 2. Door Lock Input Event (Interrupt driven)
    memory[LOCK_INPUT_ADDR] = Drv_Lock_ReadInput();
    if (pendingLockFrame) {
        pendingLockFrame = false;
        // Build asynchronous packet
        frame_t evtFrame;
        Drv_GetUID(evtFrame.uid);
        evtFrame.addr = LOCK_INPUT_ADDR;
        evtFrame.cmd = CMD_READ; // Report status
        evtFrame.size = 1;
        evtFrame.payload[0] = memory[LOCK_INPUT_ADDR];

        uint8_t len = Protocol_BuildPacket(&evtFrame, txBuffer, false, 0);
        Drv_RS485_Send(txBuffer, len);
    }

    // 3. Auto-off Door Lock Logic
    if (memory[CONTROL_ADDR] & 0x08) {
        doorLockCycles++;
        if (doorLockCycles > 2) { // Logic cycle delay
            memory[CONTROL_ADDR] &= ~(1 << 3);
            App_UpdateIO();
            doorLockCycles = 0;
        }
    }

    // 4. Polling Logic
    if (memory[POLL_MODE_ADDR] == 1) {
        uint8_t uid[12];
        Drv_GetUID(uid);
        uint16_t delay = uid[0] + uid[1];
        if (pollCounter > 3) delay += uid[2];
        if (delay > 450) delay = 1;

        pollCounter++;
        // Kirim ulang frame terakhir atau status (sesuai logika asli)
        Drv_RS485_Send(txBuffer, 20); // Contoh size, sesuaikan dengan logika asli polling
        HAL_Delay(delay);
    }

    HAL_Delay(50); // Main loop delay
}

static void App_HandleWrite(void) {
    Err_StatusTypeDef err = App_ExecuteWrite(currentFrame.addr, currentFrame.payload, currentFrame.size);

    uint8_t txLen;
    if (err == OK) {
        App_UpdateIO();
        if (currentFrame.addr != POLL_MODE_ADDR) {
            txLen = Protocol_BuildPacket(&currentFrame, txBuffer, false, 0);
            Drv_RS485_Send(txBuffer, txLen);
        }
    } else {
        txLen = Protocol_BuildPacket(&currentFrame, txBuffer, true, err);
        Drv_RS485_Send(txBuffer, txLen);
    }
}

static void App_HandleRead(void) {
    Err_StatusTypeDef err = App_ExecuteRead(currentFrame.addr, currentFrame.size, currentFrame.payload);

    uint8_t txLen;
    if (err == OK) {
        txLen = Protocol_BuildPacket(&currentFrame, txBuffer, false, 0);
    } else {
        txLen = Protocol_BuildPacket(&currentFrame, txBuffer, true, err);
    }
    Drv_RS485_Send(txBuffer, txLen);
}

static Err_StatusTypeDef App_ExecuteWrite(uint8_t addr, uint8_t *data, uint8_t size) {
    // Validasi logic
    if (addr == LOCK_INPUT_ADDR) return ERR_INVALID_ACCESS;
    // ... validasi lain disalin dari kode asli ...

    if (addr == UNIQUE_ADDR) {
        Drv_Flash_Write(FLASH_USER_START_ADDR, data, size);
    } else if (addr == LAST_ACCESSED_ADDR) {
        Drv_Flash_Write(FLASH_USER_START_ADDR + 0x0C, data, size);
    } else {
        memory[addr] = data[0]; // RAM write
    }
    return OK;
}

static Err_StatusTypeDef App_ExecuteRead(uint8_t addr, uint8_t size, uint8_t *outData) {
    if (addr == CONTROL_ADDR) return ERR_INVALID_ACCESS;

    if (addr == UNIQUE_ADDR) {
        Drv_Flash_Read(FLASH_USER_START_ADDR, outData, size);
    } else if (addr == LAST_ACCESSED_ADDR) {
        Drv_Flash_Read(FLASH_USER_START_ADDR + 0x0C, outData, size);
    } else {
        outData[0] = memory[addr];
    }
    return OK;
}

static void App_UpdateIO(void) {
    uint8_t ctrl = memory[CONTROL_ADDR];
    Drv_Lock_Set((ctrl & 0x08) > 0);
    Drv_LED_Set(ctrl & 0x07); // Bit 0,1,2 untuk RGB
}
