/*
 * main.c
 *
 *  Created on: 7 de nov de 2018
 *      Author: rafael hiller
 */



#include <avr/interrupt.h>


#include "lib/bits.h"
#include "lib/avr_gpio.h"
#include "lib/avr_timer.h"
#include "lib/avr_extirq.h"
#include "lib/avr_usart.h"
#include "lib/avr_twi_master.h"
#include "display/lcd_i2c.h"
#include "ModBus.h"
#include <stdio.h>

#define ESTOURO 12800 //tempo de estouro é de 12800 us
#define TOP 200     //o contador irá até 200

volatile uint16_t time_1, time_2; //variáveis atribuídas ao tempo entre cada interrupção externa
volatile uint16_t over_flow;	  //no caso de estouro de timer, será acrecentado um valor


//configuração do timer
void timer0_init(){

	TIMER_0->TCCRA = SET(WGM01); //modo de operação CTC com o top em OCRA
	TIMER_0->TCCRB = SET(CS02) | SET(CS00); //prescaler = 1024
	TIMER_IRQS->TC0.BITS.OCIEA = 1; //habilitando interrupção por comparação com OCRA

	TIMER_0->OCRA = TOP; //definição do top de contagem

}

//Interrupção externa
ISR(INT1_vect){

	time_1 = over_flow;
	time_2 = TIMER_0->TCNT;


	over_flow = 0;
	TIMER_0->TCNT = 0;
}

//Interrupção timer
ISR(TIMER0_COMPA_vect){

	over_flow++;

}

//função para o cáculo do período
//parametros:
//time = tempo em que o contador parou na interrupção
//over_flow = quantidade de estouros por comparação
//t_estouro_us = tempo em micro segundos para o timer estourar, atingindo seu limite
//top = valor máximo de contagem
//return: valor do tempo em 10^-5 segundos
uint32_t calc_real_time_ms(uint16_t time, uint16_t over_flow, uint16_t t_estouro_us, uint16_t top){

	uint32_t real_time = 0;
	uint16_t relation = 0;

	relation = t_estouro_us / top;
	real_time = time;
	real_time = real_time * relation/100;
	t_estouro_us = t_estouro_us/100;
	real_time = real_time + over_flow*t_estouro_us;


	return real_time;
}



uint32_t calc_speed_kmh(uint32_t frequencie, uint32_t radius){

//	FILE *debug = get_usart_stream();
//
//	USART_Init(B9600);



	uint32_t speed = 0;

	if (frequencie > 1000) {
		frequencie = frequencie / 10;
		speed = frequencie * 63 * (radius);
		speed = speed / 1000;
		speed = (speed * 36);
	}
	else {
		speed = frequencie * 63 * (radius);
		speed = speed / 1000;
		speed = (speed * 36) / 10;
	}

	//fprintf(debug, "%d\n\r", speed);

	return speed;
}

//função para imprimir no LCD 16x2
//imprime duas casas decimais, dezena e unidade
//parâmetros: frequencia
void print_data_LCD(uint16_t dado){

	cmd_LCD_i2c(0xC0, 0);

	cmd_LCD_i2c(0xC0, 0);

	uint8_t dezena = 0, unidade = 0, decimal = 0, centezimal = 0;
	char buffer[6];

	dezena = dado/1000;
	unidade = (dado%1000)/100;
	decimal = (dado%100)/10;
	centezimal = (dado%10/1);


	buffer[0]=dezena+'0';
	buffer[1]=unidade+'0';
	buffer[2]='.';
	buffer[3]=decimal+'0';
	buffer[4]=centezimal+'0';
	buffer[5]='\0';
	escreve_LCD_i2c(buffer);


}

int main(){


//	FILE *debug = get_usart_stream();
//
	USART_Init(B9600);

	timer0_init();

	SET_BIT(DDRB, PB5);
	SET_BIT(PORTB, PB5);

	//interrupção por borda de subida
	EICRA = (1 << ISC11) | (1 << ISC10);

	//ativa as interrupções externas
	EIMSK = (1 << INT1);

	//inicializa modo lider
	TWI_Master_Initialise();

	sei();

	inic_LCD_4bits_i2c();

	escreve_LCD_i2c("Speed(Km/h):");

	uint8_t rx_pkg[16];
	uint16_t real_time;
	uint16_t frequencie = 0;
	uint32_t speed = 0;
	uint8_t *buffer;
	uint8_t i;

	while(1){

		real_time = calc_real_time_ms(time_2, time_1, ESTOURO, TOP);
		frequencie = 1000000/real_time;
		speed = calc_speed_kmh(frequencie, 28);
		print_data_LCD(speed);
		buffer = create_buffer(0x15, 0x01, 0x05, speed);

//		for(uint8_t i = 0; i < 8; i++){
//			fprintf(debug, "%x", buffer[i]);
//		}
//		fprintf(debug, "\n\r");

		for (i=0; i < 8; i++)
			USART_tx(buffer[i]);



//		for (i=0; i < 8;i++)
//			rx_pkg[i] = USART_rx();
////
		_delay_ms(1000);



	}
	//CLR_BIT(PORTB, PB5);
	return 0;
}
