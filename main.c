/*
 *  Copyright 2010 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 */
/***************************************************************************/
/*                                                                         */
/*     H E L L O . C                                                       */
/*                                                                         */
/*     Basic LOG event operation from main.                                */
/*                                                                         */
/***************************************************************************/

#include <std.h>

#include <log.h>
#include <clk.h>
#include <tsk.h>
#include <gbl.h>
//#include "clkcfg.h"
#include <stdint.h>
#include <mbx.h>

#include "hellocfg.h"
#include "ezdsp5502.h"
#include "stdint.h"
#include "aic3204.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"

extern MBX_Obj MBX_TSK_pam_symbol_in;
extern MBX_Obj MBX_TSK_pam_symbol_out;
extern MBX_Obj MBX_TSK_disp_in;

extern void audioProcessingInit(void);

volatile int counter = 0;

void main(void)
{
    /* Initialize BSL */
    EZDSP5502_init( );

    /* Initialize I2S */
    EZDSP5502_MCBSP_init();

    PRD_IO_setup();
    // after main() exits the DSP/BIOS scheduler starts
}

#define LEN_IMG 16
#define LEN_DL_X 4

int16_t img[LEN_IMG] =
{
		0x0080,	0x00C0,	0X00E0,	0X00F0,
		0X00F8,	0X00FC, 0X00FE,	0X00FF,
		0X80FF, 0XC0FF, 0XE0FF, 0XF0FF,
		0XF8FF, 0XFCFF, 0XFEFF, 0xFFFF,

};

int16_t dl_x[LEN_DL_X];

Void taskFxn(Arg value_arg)
{
    // enter pseudo main
	int16_t out;
	int16_t x = 1;
	int16_t x_ptr = 0;
	int40_t sent = 0;
	int40_t err = 0;
	double ber = 0;
    while(1)
    {
    	MBX_post(&MBX_TSK_pam_symbol_in, &img[x], ~0);
    	x++;
    	if(x > LEN_IMG)
    	{
    		x = 0;
    	}
    	dl_x[x_ptr] = img[x];
    	x_ptr ++;
    	if(x_ptr > LEN_DL_X)
    	{
    		x_ptr = 0;
    	}
    	sent ++;

    	// account for delay through system
    	int16_t expected = dl_x[x_ptr];

    	// If there was an error, check how many bits are in error
		if(MBX_pend(&MBX_TSK_pam_symbol_out, &out, 0) == TRUE && out != expected)
		{
			int i;
			for(i = 0; i < 16; i ++)
			{
				int16_t msk = 0x01 << i;
				if( ( (expected) & msk) != (out & msk) )
				{
					err ++;
				}
			}
			ber = ( (double) err / (double) sent ) / 16;
		}

		// push to display
		MBX_post(&MBX_TSK_disp_in, &out, ~0);
    }
}

void myIDLThread(void)
{
	counter++;
}

