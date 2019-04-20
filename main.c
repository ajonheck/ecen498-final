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

extern void audioProcessingInit(void);

volatile int counter = 0;

void main(void)
{
    /* Initialize BSL */
    EZDSP5502_init( );

    // configure the Codec chip
    // ConfigureAic3204();

    /* Initialize I2S */
    EZDSP5502_MCBSP_init();

    /* enable the interrupt with BIOS call */
    // C55_enableInt(7); // reference technical manual, I2S2 tx interrupt
    // C55_enableInt(6); // reference technical manual, I2S2 rx interrupt

    PRD_IO_setup();

    //audioProcessingInit();

    // after main() exits the DSP/BIOS scheduler starts
}

Void taskFxn(Arg value_arg)
{
    // enter pseudo main
	int16_t out;
	int16_t x = 1;
	int40_t sent = 0;
	int40_t err = 0;
	double ber = 0;
    while(1)
    {
    	MBX_post(&MBX_TSK_pam_symbol_in, &x, ~0);
    	x++;
    	sent ++;
    	// account for delay through system
    	int16_t expected = x - 4;
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
    }
}

void myIDLThread(void)
{
	counter++;
}

