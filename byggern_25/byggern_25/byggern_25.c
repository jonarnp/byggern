/*
 * byggern_25.c
 *
 * Created: 27.08.2014 13:05:25
 *  Author: helders
 */ 

#define F_CPU 4915200UL  // 4.9152 MHz
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/pgmspace.h>

#include "bit_op.h"
#include "drivers/usart.h"
#include "drivers/xmem.h"
#include "tests/joy_test.h"
#include "drivers/joy.h"
#include "tests/slider_test.h"
#include "drivers/slider.h"
#include "drivers/oled.h"
#include "format.h"
#include "drivers/can/can_ctrl.h"
//#include "drivers/can/spi.h"

bool test = false;

int main(void)
{
	cli();
	DDRB = 0x01;
	USART_Init(MYUBRR,true);
	XMEM_En();
	//JOY_init();
	SLIDER_init();
	sei();
	
	//JOY_calibrate();	
	//SLIDER_calibrate();
	_delay_ms(1000);
	oled_init();
	uint8_t i=0;

	oled_clear();
	JOY_init();
	JOY_calibrate();
	menu_init();
	mcp2515_init();
	while(1)
	{
		
		
		//uint8_t RXFSIDH = mcp2515_read(0x10);
		//uint8_t RXFSIDL = mcp2515_read(0x11);
		//uint16_t ID = (((uint16_t)RXFSIDH)<<3) +(RXFSIDL>>5);
		//
		//printf("RXFSIDH = %02x, RXFSIDL = %02x\nID is %04x\n",RXFSIDH,RXFSIDL,ID);
		//
		//uint8_t CANCTRL = mcp2515_read(0x0F);
		//printf("CanCTRL = %02x\n",CANCTRL);
		
		_delay_ms(1000);
		
		uint8_t status = mcp2515_read_status();
		printf("Status: 0x%02x. RXIF0 = %d, RXIF1 = %d.\n", status,status&0x01,(status&0x02)>>1);
		//_delay_ms(100);
		//if (++i >=255) i=0;
		//mcp2515_select_tx_identifier(0,RXF0);
				
		if (!get_bit(MCP_Int_Port,MCP_Int_Pin))
		{
			printf("Can received!!!\n");
			buffer_recieve_t data;
			mcp2515_buffer_recieve(&data);
			printf("Recieved CAN message is:\nID: %03x\nLength: %d\n",data.id,data.length);
			for (uint8_t i = 0; i<data.length;i++)
			{
				printf("Data[%d] : %02x\n",i,data.data[i]);
			}
		}

		
		if(++i>10)
		{
			i=0;
			uint8_t testing[3] = {10,20,30};
			mcp2515_select_tx_identifier(0, 0x100);
			mcp2515_load_tx_data(0,testing,3);
			mcp2515_request_to_send(0);
			printf("Sending data\n");
		}
		if (1)
		{	
			menu_update();
			
			uint8_t f1;
			f1 = 5;
			
			//oled_print(int16_to_str(f1));
			//oled_print("Ja");
			//oled_print("test: ");
			//oled_print(str);
			//oled_print("Test test test noe sja gs sd asgm askmfhnaskjgfnakls d");
			test = true;
		}
		//ADC_gal_test();
		//_delay_ms(100);
		//test = ADC_read(4);
		//printf("ADC channel 0 %d\n",test);
		//printf("Pin test %d\n", get_bit(PINB,PINB1));
		//ADC_read_test();
		//Test_joy();
		//Test_slider();
	}
}

