/*
 * protocol.c
 *
 *  Created on: Jan 19, 2026
 *      Author: compu
 */
#include "protocol.h"


uint16_t Protocol_CalcCRC(uint8_t *data, uint8_t startIndex, uint16_t length) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = startIndex; i < startIndex+length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

bool Protocol_IsHeadTailCorrect(uint8_t *head, uint8_t *tail) {
    if (head[0] != 0xAB || head[1] != 0xFF) return false;
    if (tail[0] != 0xFF || tail[1] != 0xBA) return false;
    return true;
}

Err_StatusTypeDef Protocol_ParseFrame(uint8_t *data, uint16_t len, frame_t *frm) {
    // 1. Validasi Panjang Minimum
    if (len < 21) return ERR_INVALID_FRAME_LENGTH;

    // 2. Validasi Header (Wajib AB FF)
    if (data[0] != 0xAB || data[1] != 0xFF) return ERR_INVALID_FRAME_LENGTH;

    uint8_t p = 0;

    // 3. Parsing Data
    memcpy(frm->head, &data[0], 2);
    memcpy(frm->uid, &data[2], 12);     p += 14;
    frm->addr   = data[14];             p += 1;
    frm->cmd    = data[15];             p += 1;
    frm->size   = data[16];             p += 1;
    memcpy(frm->payload, &data[17], frm->size); p += frm->size;

    // Ambil CRC dan Tail
    memcpy(frm->crc, &data[p], 2);
    memcpy(frm->tail, &data[len-2], 2);

    // 4. Hitung dan Validasi CRC
    // Panjang data = p - 2 (Total pointer dikurangi 2 byte Header)
    uint16_t crc16Local = Protocol_CalcCRC(data, 2, p - 2);
    uint16_t crcVal = (frm->crc[1]<<8) | frm->crc[0];

    if (crc16Local != crcVal) return ERR_INVALID_CRC;

    return OK;
}

// Fungsi helper untuk menyusun buffer kirim
uint8_t Protocol_BuildPacket(frame_t *frm, uint8_t *buffer, bool isError, uint8_t errorCode) {
    uint8_t p = 0;

    // Modifikasi CMD berdasarkan tipe respon
    if (isError) {
        frm->cmd = (2<<4) | (frm->cmd & 0x0F);
        frm->size = 1;
        frm->payload[0] = errorCode;
    } else {
        // Jika bukan polling response (ACK biasa)
        frm->cmd = (1<<4) | (frm->cmd & 0x0F);
    }

    // Set Header/Tail default jika belum diset
    frm->head[0] = 0xAB; frm->head[1] = 0xFF;
    frm->tail[0] = 0xFF; frm->tail[1] = 0xBA;

    memcpy(buffer,    frm->head, 2);    p += 2;
    memcpy(buffer+p,  frm->uid,  12);   p += 12;
    memcpy(buffer+p, &frm->addr, 1);    p += 1;
    memcpy(buffer+p, &frm->cmd,  1);    p += 1;
    memcpy(buffer+p, &frm->size, 1);    p += 1;
    memcpy(buffer+p,  frm->payload, frm->size); p += frm->size;

    uint16_t crc16Local = Protocol_CalcCRC(buffer, 2, p-2);
    buffer[p++] = (uint8_t)(crc16Local & 0xFF);
    buffer[p++] = (uint8_t)((crc16Local >> 8) & 0xFF);
    memcpy(buffer+p,  frm->tail, 2);    p += 2;

    return p;
}
