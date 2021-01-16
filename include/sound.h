#ifndef SOUND_H
#define SOUND_H

volatile unsigned int speaker_ticks;

unsigned int target_speaker_ticks;

void init_speaker(void);

void speaker_off(void);

void beep(unsigned int, unsigned int);

#endif
