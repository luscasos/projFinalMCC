/*
 * ModBus.h
 *
 *  Created on: 7 de dez de 2018
 *      Author: Rafae Hiller & Lucas Odair
 *      Instituto Federal de Santa Catarina
 */

#ifndef MODBUS_H_
#define MODBUS_H_

uint16_t CRC16_2(uint8_t *buf, int len);
uint8_t *create_buffer(uint8_t addr, uint8_t cmd, uint16_t reg, uint16_t data);
uint8_t volatile buffer[8];

#endif /* MODBUS_H_ */
