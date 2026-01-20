#ifndef INC_PROTOCOL_H_
#define INC_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// Definisi Konstanta Protokol
#define RX_BUF_SIZE     64
#define TX_BUF_SIZE     64

typedef enum {
    OK = 0x00,
    ERR_INVALID_FRAME_LENGTH    = 0xE1,
    ERR_INVALID_CRC             = 0xE2,
    ERR_INVALID_FUNCTION_CODE   = 0xE3,
    ERR_INVALID_ACCESS          = 0xE4,
    ERR_INVALID_PAYLOAD_SIZE    = 0xE5,
    ERR_INTERNAL_SYSTEM         = 0xE6
} Err_StatusTypeDef;

typedef struct {
    uint8_t  head[2];
    uint8_t  uid[12];
    uint8_t  addr;
    uint8_t  cmd;
    uint8_t  size;
    uint8_t  payload[12];
    uint8_t  crc[2];
    uint8_t  tail[2];
} frame_t;

// Public Functions
uint16_t Protocol_CalcCRC(uint8_t *data, uint8_t startIndex, uint16_t length);
bool Protocol_IsHeadTailCorrect(uint8_t *head, uint8_t *tail);
Err_StatusTypeDef Protocol_ParseFrame(uint8_t *data, uint16_t len, frame_t *frm);
uint8_t Protocol_BuildPacket(frame_t *frm, uint8_t *buffer, bool isError, uint8_t errorCode);

#endif /* INC_PROTOCOL_H_ */
