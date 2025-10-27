#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "raylib.h"
#include "structs.h"

extern Music msc_theme;
extern Music msc_tracks[3];

typedef enum SFX {
	SFX_PADDLE_HIT_1=0,
} SFX;

extern Sound sfx[16]; // Accessed via enum
extern Sound sfx_playing[MAX_SFX]; // Swapback array of sounds currently playing

// Loaders
void load_audio();
void unload_audio();

// Music Mind (who also controls the sound effects) fns
void music_mind_update(float dt, MusicMind *mind);
void init_music_mind(MusicMind *mind);

// Music fns
void queue_track(MusicMind *mind, Music *new, float fade_duration);

// SFX fns
void play_sfx(MusicMind *mind, SFX sfx);


#endif
