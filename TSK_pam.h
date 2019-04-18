/*
 * TSK_pam.h
 *
 *  Created on: Apr 17, 2019
 *      Author: Andy
 */

#ifndef TSK_PAM_H_
#define TSK_PAM_H_

#include <stdint.h>

#define BITS_PER_INT16 16
#define SPB 16
#define LEN_CHANNEL_FRAME ( BITS_PER_INT16 * SPB )
#define LEN_H 129
#define LEN_DL (LEN_CHANNEL_FRAME - 1 + LEN_H)

extern int16_t rrc_h[LEN_H];

#endif /* TSK_PAM_H_ */
