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

#define WRITE_LEN (192 + 1)
#define LEN_DL_DISP (96+1)

extern MBX_Obj MBX_TSK_disp_in;

void tsk_disp(void)
{
	int16_t i, disp_val, upper_byte, lower_byte;
	int16_t dl_disp[LEN_DL_DISP];
	int16_t cmd[WRITE_LEN];
	int16_t cmd_i = 1;
	osd9616_init( );   // Initialize  Display

	osd9616_send(0x00,0x2e);  // Deactivate Scrolling

	osd9616_send(0x00,0XC0);  // Set normal display mode

	// set vertical fill
	osd9616_send(0x00, 0x20); // change addressing mode command
	osd9616_send(0x00, 0x01); // set addressing mode to vertical mode

	// Set column boundaries
	osd9616_send(0x00, 0x21); // set column command
	osd9616_send(0x00, 0x00); // set low column to 0
	osd9616_send(0x00, 0x5F); // set high column to 95

	// Set page boundaries
	osd9616_send(0x00, 0x22); // set page command
	osd9616_send(0x00, 0x00); // set low page to 0
	osd9616_send(0x00, 0x01); // set high page to 1
	while(1)
	{
		MBX_pend(&MBX_TSK_disp_in, &disp_val, ~0);
		dl_disp[1] = disp_val;
		for(i = LEN_DL_DISP; i > 1; i --)
		{
			upper_byte = (dl_disp[i] & 0xFF00) >> 8;
			lower_byte = dl_disp[i] & 0x00FF;

			cmd[cmd_i] = upper_byte;
			cmd_i ++;
			cmd[cmd_i] = lower_byte;
			cmd_i ++;

			if(cmd_i == WRITE_LEN)
			{
				cmd_i = 0;
				cmd[cmd_i] = 0x40;
				cmd_i ++;
				EZDSP5502_I2C_write( OSD9616_I2C_ADDR, cmd, WRITE_LEN );

			}
			dl_disp[i] = dl_disp[i-1];
		}
	}
}
