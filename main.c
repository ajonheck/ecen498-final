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

    //audioProcessingInit();

    // after main() exits the DSP/BIOS scheduler starts
}

Void taskFxn(Arg value_arg)
{
    // enter pseudo main
	int16_t out;
	int16_t x = 1;
    while(1)
    {
    	MBX_post(&MBX_TSK_pam_symbol_in, &x, ~0);
		if(MBX_pend(&MBX_TSK_pam_symbol_out, &out, 0) == TRUE && out != x-3)
		{
			out = 0;
		}
    	x++;
    }
}

void myIDLThread(void)
{
	counter++;
}

