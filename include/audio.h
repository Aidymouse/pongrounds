#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "raylib.h"
#include "structs.h"

extern Music msc_theme;
extern Music msc_tracks[3];

void load_audio();
void unload_audio();

void init_music_mind(MusicMind *mind);
void queue_track(MusicMind *mind, Music *new, float fade_duration);
void music_mind_update(float dt, MusicMind *mind);

#endif
