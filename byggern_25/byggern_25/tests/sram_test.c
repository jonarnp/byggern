#include <avr/io.h>
#include <stdio.h>

void SRAM_test(void)
{
	// Start address for the SRAM
	volatile char *ext_ram = (char *) 0x1800;

	uint16_t i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;

	printf("Starting SRAM test...\r\n");

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			werrors++;
		}
	}

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
  		if (ext_ram[i] != testvalue) {
			printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}

	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}

void SRAM_test_count(uint8_t errors[512])
{
	// Start address for the SRAM
	volatile char *ext_ram = (char *) 0x1800;

	uint16_t i, werrors, rerrors;
	werrors = 0;
	rerrors = 0;
	unsigned char testvalue;

	printf("Starting SRAM test...\r\n");

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		ext_ram[i] = testvalue;
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (write phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			if(i<512) errors[i] += 1;
			werrors++;
		}
	}

	for (i = 0; i < 0x800; i++) {
		testvalue = ~(i % 256);
		if (ext_ram[i] != testvalue) {
			//printf("SRAM error (read phase): ext_ram[%d] = %02X (should be %02X)\r\n", i, ext_ram[i], testvalue);
			rerrors++;
		}
	}

	printf("SRAM test completed with %d errors in write phase and %d errors in read phase\r\n", werrors, rerrors);
}
