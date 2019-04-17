/*
 * TSK_pam_tx.c
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */
#include <std.h>
#include <stdint.h>
#include <mbx.h>
#include "hellocfg.h"

extern MBX_Obj MBX_TSK_pam_tx_input;

tsk_pam_tx()
{
	int16_t x = 0;
	while(1)
	{
		MBX_pend(&MBX_TSK_pam_tx_input, &x, ~0);
	}
}
