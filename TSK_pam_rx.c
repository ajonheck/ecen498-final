/*
 * TSK_pam_rx.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */
#include <std.h>
#include <string.h>
#include <mbx.h>
#include "hellocfg.h"
#include "fir.h"
#include "TSK_pam.h"

extern MBX_Obj MBX_TSK_pam_channel_output;
extern MBX_Obj MBX_TSK_pam_symbol_out;

extern int16_t rrc_h[LEN_H];
extern int16_t channel_h[LEN_H];

int16_t dl_rrc[LEN_DL];
int16_t dl_channel[LEN_DL];
int16_t rx[LEN_CHANNEL_FRAME];
int16_t y[LEN_CHANNEL_FRAME];

tsk_pam_rx()
{
	memset(dl_channel, 0, sizeof(int16_t)*LEN_DL);
	memset(dl_rrc, 0, sizeof(int16_t)*LEN_DL);
	int16_t i = 0, out = 0;

	// seed the decision index to compensate for the 3 filters applied through the system
	int16_t decision_index = 3 * ( LEN_H - 1 ) / 2;
	// wait for first frame to avoid control flow
	MBX_pend(&MBX_TSK_pam_channel_output, &rx, ~0);
	// apply channel filter
	fir_filter(rx, LEN_CHANNEL_FRAME, channel_h, LEN_H, y, dl_channel);
	// apply match filter
	fir_filter(y, LEN_CHANNEL_FRAME, rrc_h, LEN_H, rx, dl_rrc);

	while(1)
	{
		// decode until frame empty or an int is recovered
		while(i < BITS_PER_INT16 && decision_index < LEN_CHANNEL_FRAME)
		{
			if(rx[decision_index] > 0)
			{
				out |= ( 0x1 << i);
			}
			i++;
			decision_index += SPB;
		}
		// Check if a new frame is needed
		if(decision_index >= LEN_CHANNEL_FRAME)
		{
			decision_index = 0;
			// wait for an rx frame
			MBX_pend(&MBX_TSK_pam_channel_output, &rx, ~0);
			// apply channel filter
			fir_filter(rx, LEN_CHANNEL_FRAME, channel_h, LEN_H, y, dl_channel);
			// apply match filter
			fir_filter(y, LEN_CHANNEL_FRAME, rrc_h, LEN_H, rx, dl_rrc);
		}
		else
		{
			// If no new frame was needed, must have an output value
			i = 0;
			MBX_post(&MBX_TSK_pam_symbol_out, &out, ~0);
			out = 0;
		}

	}
}
