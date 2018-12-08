/*
 * ModBus.c
 *
 *  Created on: 7 de dez de 2018
 *      Author: rafae
 */


#include <stdio.h>
#include <stdlib.h> // esse é para o malloc //
#include "ModBus.h"
#include "lib/avr_usart.h"

uint8_t volatile buffer[8];

uint16_t CRC16_2(uint8_t *buf, int len) {

	uint32_t crc = 0xFFFF;

	int i;

	for (i = 0; i < len; i++) {

		crc ^= (uint16_t) buf[i]; // XOR byte into least sig. byte of crc

		for (int i = 8; i != 0; i--) { // Loop over each bit

			if ((crc & 0x0001) != 0) { // If the LSB is set

				crc >>= 1; // Shift right and XOR 0xA001
				crc ^= 0xA001;

			} else
				// Else LSB is not set
				crc >>= 1; // Just shift right
		}
	}
	return crc;
}

uint8_t *create_buffer(uint8_t addr, uint8_t cmd, uint16_t reg, uint16_t data){


	FILE *debug = get_usart_stream();

	USART_Init(B9600);

	uint8_t  mask = 0xFF;
	uint16_t crc = 0;

	buffer[0] = addr;
	buffer[1] = cmd;
	buffer[2] = (reg>>8);
	buffer[3] = reg & mask;
	buffer[4] = (data>>8);
	buffer[5] = data & mask;

	crc = CRC16_2(buffer, 6);

	buffer[6] = (crc>>8);
	buffer[7] = crc & mask;

//	for (uint8_t i = 0; i < 8; i++) {
//
//		if(buffer[i]==0)
//			fprintf(debug, "0");
//		else
//			fprintf(debug, "%x", buffer[i]);
//	}
//	fprintf(debug, "\n\r");

	return buffer;
}
