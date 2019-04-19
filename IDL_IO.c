/*
 * IDL_poll_switches.c
 *
 *  Created on: Mar 6, 2019
 *      Author: Andy
 */
#include <std.h>
#include "ezdsp5502.h"
#include "ezdsp5502_i2cgpio.h"
#include "csl_gpio.h"
#include "stdio.h"
#include "stdint.h"
#include <mbx.h>
#include "hellocfg.h"
#include "IDL_IO.h"

#define SW_INC (SW1)
#define SW_DEC (SW0)

extern MBX_Obj MBX_TSK_pam_channel_noise_up;
extern MBX_Obj MBX_TSK_pam_channel_noise_down;

int16_t increment_switch = LOW;
int16_t decrement_switch = LOW;
int16_t reading;

void IDL_IO_setup()
{
    EZDSP5502_I2CGPIO_configLine( SW_INC, IN );
    EZDSP5502_I2CGPIO_configLine( SW_DEC, IN );
}

void tsk_poll_switches()
{
	// get filter switch reading and determine if a press event occured
	reading = EZDSP5502_I2CGPIO_readLine(SW_INC);
	if(reading != increment_switch && increment_switch == HIGH)
	{
		int16_t true = 1;
		MBX_post(&MBX_TSK_pam_channel_noise_up, &true, 0);
	}
	increment_switch = reading;

	// get disp switch reading and determine if a press event occured
	reading = EZDSP5502_I2CGPIO_readLine(SW_DEC);
	if(reading != decrement_switch && decrement_switch == HIGH)
	{
		int16_t true = 1;
		MBX_post(&MBX_TSK_pam_channel_noise_down, &true, 0);
	}
	decrement_switch = reading;
}

