/*
 * TSK_pam_channel.c
 *
 *  Created on: Apr 18, 2019
 *      Author: Andy
 */

#include <std.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <mbx.h>
#include <c55x.h>
#include "hellocfg.h"
#include "TSK_pam.h"

#define NSUM 16

extern MBX_Obj MBX_TSK_pam_channel_input;
extern MBX_Obj MBX_TSK_pam_channel_output;

int16_t frame[LEN_CHANNEL_FRAME];
int16_t i;

double gaussrand();

tsk_pam_channel()
{
	double sigma = 0.70711;
	int40_t count = 0;
	double cumulative = 0;
	double mean = 0;
	double sig_sum = 0;
	double exp_sigma = 0;
	while(1)
	{
		MBX_pend(&MBX_TSK_pam_channel_input, &frame, ~0);
		for(i = 0; i < LEN_CHANNEL_FRAME; i++)
		{
			count ++;

			// generate noise bounded between +/- 1
			int32_t noise = gaussrand() * sigma * 32767.0;
			int32_t channel_data = noise + frame[i];
			if(channel_data > 32767)
			{
				channel_data = 32767;
			}
			else if (channel_data < -32768)
			{
				channel_data = -32768;
			}
			frame[i] = (int16_t) channel_data;
			cumulative += noise;
			sig_sum += noise * noise;
			mean = cumulative / count;
			exp_sigma = sqrt(sig_sum / (count -  1));
		}
		MBX_post(&MBX_TSK_pam_channel_output, &frame, ~0);
	}
}

double gaussrand()
{
	double x = 0;
	int i;
	for(i = 0; i < NSUM; i++)
	{
		x += (double)rand()/(double) RAND_MAX;
	}
	x -= NSUM / 2.0;
	x /= sqrt(NSUM / 12.0);
	return x;
}
