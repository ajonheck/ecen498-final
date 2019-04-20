/*
 * TSK_disp.c
 *
 *  Created on: Apr 19, 2019
 *      Author: Andy
 */

#include <std.h>
#include "hellocfg.h"
#include <stdint.h>
#include <mbx.h>
#include "lcd.h"

extern MBX_Obj MBX_TSK_disp_in;

int16_t bin_lut[17] =
{
		0x0000, // 0
		0x0080,	0x00C0,	0X00E0,	0X00F0, // 4 lsb of lower byte
		0X00F8,	0X00FC, 0X00FE,	0X00FF, // 4 msb of lower byte
		0X80FF, 0XC0FF, 0XE0FF, 0XF0FF, // 4 lsb of upper byte
		0XF8FF, 0XFCFF, 0XFEFF, 0xFFFF, // 4 msb of upper byte

};

#define WRITE_LEN (96+1)
#define LEN_DL_DISP (96+1)

void tsk_disp(void)
{
	int16_t count, j, i, x, disp_val, upper_byte, lower_byte;
	int16_t dl_disp[LEN_DL_DISP];
	int16_t cmd[WRITE_LEN];
	int16_t cmd_i = 1;
	x = 0;
	osd9616_init( );   // Initialize  Display

	osd9616_send(0x00,0x2e);  // Deactivate Scrolling

	osd9616_send(0x00,0XC0);  // Set normal display mode

	// set vertical fill
	osd9616_send(0x00, 0x20); // change addressing mode command
	osd9616_send(0x00, 0x01); // set addressing mode to vertical mode

	// Set column boundaries
	osd9616_send(0x00, 0x21); // set column command
	osd9616_send(0x00, 0x7E); // set low column to 126
	osd9616_send(0x00, 0x7F); // set high column to 127

	// Set page boundaries
	osd9616_send(0x00, 0x22); // set page command
	osd9616_send(0x00, 0x00); // set low page to 0
	osd9616_send(0x00, 0x01); // set high page to 1

	// setup scrolling
	osd9616_send(0x00,0x27);  // right horizontal scroll
	osd9616_send(0x00,0x00);  // dummy byte
	osd9616_send(0x00,0x00);  // start page 0
	osd9616_send(0x00,0x02);  // set scroll rate 64 frames
	osd9616_send(0x00,0x01);  // stop page 1
	osd9616_send(0x00,0x00);  // dummy byte
	osd9616_send(0x00,0xff);  // dummy byte
	while(1)
	{
		MBX_pend(&MBX_TSK_disp_in, &disp_val, ~0);
		dl_disp[1] = disp_val;
		upper_byte = (disp_val & 0xFF00) >> 8;
		lower_byte = disp_val & 0x00FF;
		// Send 2 column wide display value
		osd9616_send(0x40,upper_byte);
		osd9616_send(0x40,lower_byte);
		osd9616_send(0x40,upper_byte);
		osd9616_send(0x40,lower_byte);

		// toggle scrolling twice to shift out new values
		osd9616_send(0x00,0x2f);  // activate scrolling
		osd9616_send(0x00,0x2e);  // Deactivate Scrolling
		osd9616_send(0x00,0x2f);  // activate scrolling
		osd9616_send(0x00,0x2e);  // Deactivate Scrolling
	}
}
