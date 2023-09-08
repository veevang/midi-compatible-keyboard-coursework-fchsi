/*
 * typedef.h
 *
 *  Created on: May 27, 2022
 *      Author: CHEN YIWEI
 */

#ifndef TYPEDEF_H_
#define TYPEDEF_H_

#define MIDI_NOTE_OFF 0
#define MIDI_NOTE_ON 1

typedef enum Tune {REST, DO, RE, MI, FA, SOL, LA, SI};
typedef enum Mode {PIANO, MUSIC, MIDI};
typedef enum Rhythm{WHOLE, HALF, QUARTER, DOTTED_QUARTER, EIGHTH, DOTTED_EIGHTH, SIXTEENTH};
typedef enum Song {NO_SONG, SONG_1, SONG_2, SONG_3};

#endif /* TYPEDEF_H_ */
