#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "raylib.h"
#include "structs.h"

extern Music msc_theme;
extern Music msc_tracks[3];

// Must match the order sound effects are declared 
typedef enum SFX {
	SFX_PADDLE_HIT,
	SFX_BALL_HIT_WALL,
	SFX_CARD_HIGHLIGHT,
	SFX_CARD_SELECT,
	SFX_MISSILE_LAUNCH,
	SFX_MISSILE_FLY,
	SFX_MISSILE_IMPACT,
	SFX_SWORD_SWING,
	SFX_SWORD_SLICE,
	SFX_PADDLE_BREAKS,
	SFX_PADDLE_DUPLICATED,
	SFX_EXPIRED_PANADOL,
	SFX_BALL_DAMAGE_DOUBLED,
	SFX_ANTIQUE_CONSOLE,
	SFX_HYDRAULIC_PRESS_BALL
} SFX;

// Must match the order sounds are loaded in
typedef enum SOUND_RESOURCES {
	SR_PADDLE_HIT_1,
	SR_PADDLE_HIT_2,
	SR_PADDLE_HIT_3,
	SR_PADDLE_HIT_4,
	SR_BALL_HIT_WALL_1,
	SR_BALL_HIT_WALL_2,
	SR_CARD_HIGHLIGHT,
	SR_CARD_SELECT,
	SR_MISSILE_LAUNCH,
	SR_MISSILE_FLY,
	SR_MISSILE_IMPACT,
	SR_SWORD_SWING_1,
	SR_SWORD_SWING_2,
	SR_SWORD_SLICE_1,
	SR_PADDLE_BREAKS,
	SR_PADDLE_DUPLICATED,
	SR_EXPIRED_PANADOL,
	SR_BALL_DAMAGE_DOUBLED,
} SOUND_RESOURCES;

extern const char* sound_resource_filenames[];

extern Sound sound_resources[32]; // Accessed via enum

typedef struct SoundEffect {
	SOUND_RESOURCES variants[MAX_SFX_VARIANTS];	
	float volumes[MAX_SFX_VARIANTS]; // Control volume per variant
	int num_variants;
} SoundEffect;

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
