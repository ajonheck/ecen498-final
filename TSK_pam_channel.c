/*
 * TSK_pam_channel.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Andy
 */

#include <std.h>
#include <stdint.h>
#include "hellocfg.h"
#include "TSK_pam.h"

int16_t frame[LEN_CHANNEL_FRAME];

extern MBX_Obj MBX_TSK_pam_channel_input;
extern MBX_Obj MBX_TSK_pam_channel_output;

tsk_pam_channel()
{
	while(1)
	{
		MBX_pend(&MBX_TSK_pam_channel_input, &frame, ~0);
		MBX_post(&MBX_TSK_pam_channel_output, &frame, ~0);
	}
}
