/*
 * psg.h
 *
 *  Created on: Jul 17, 2024
 *      Author: sela
 */

#ifndef INC_PSG_H_
#define INC_PSG_H_

#ifdef __cplusplus
extern "C" {
#endif
//#define FCLK 1800000

#define NOTE_ON		(0x90)
#define NOTE_OFF	(0x80)
#define CONTROL		(0xb0)
/* message parameter */
#define MIN_NOTE 0
#define MAX_NOTE 127

#define SUSTAIN		(0x40)
/* control parameter */

void init_psg();

#ifdef __cplusplus
}
#endif

#endif /* INC_PSG_H_ */
