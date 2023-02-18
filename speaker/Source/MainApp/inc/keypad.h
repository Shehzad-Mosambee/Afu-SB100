#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include "MercuryAudio.h"

#define VOLUME  AUDIO_VOLUME_LEVEL3

extern const char amr_didi[];

int powerkey_shortpress_callback(void);
int powerkey_longpress_callback(void);
void keypad_init_matrix(void);
int keypad_init_io(void);
void play_notification(void);
#endif