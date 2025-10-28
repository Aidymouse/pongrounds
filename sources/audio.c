#include "audio.h"
#include "defines.h"
#include "helper.h"

Music msc_theme;
Music msc_tracks[NUM_TRACKS];
Sound sound_resources[32];

// Needs to match order of SOUND_RESOURCES in defines.h
const int num_sound_resources = 18;
const char* sound_resource_filenames[] = {
	ASSETS_PATH"sounds/Paddle Hit 1.mp3",
	ASSETS_PATH"sounds/Paddle Hit 2.mp3",
	ASSETS_PATH"sounds/Paddle Hit 3.mp3",
	ASSETS_PATH"sounds/Paddle Hit 4.mp3",
	ASSETS_PATH"sounds/Ball Hits Side 1.mp3",
	ASSETS_PATH"sounds/Ball Hits Side 2.mp3",
	ASSETS_PATH"sounds/Card Highlight Sound.mp3",
	ASSETS_PATH"sounds/Card Select Sound.mp3",
	ASSETS_PATH"sounds/Missle Launch.mp3",
	ASSETS_PATH"sounds/Missle Fly.mp3",
	ASSETS_PATH"sounds/Missle Impact.mp3",
	ASSETS_PATH"sounds/Sword Swing.mp3",
	ASSETS_PATH"sounds/Sword Swing 2.mp3",
	ASSETS_PATH"sounds/Sword Slice.mp3",
	ASSETS_PATH"sounds/Paddle Breaks.mp3",
	ASSETS_PATH"sounds/Paddle Duplicated.mp3",
	ASSETS_PATH"sounds/Expired Panadol.mp3",
	ASSETS_PATH"sounds/Double Damage.mp3",
};

SoundEffect sfx[MAX_SFX] = {
	{ // Ball hits paddle
		{ SR_PADDLE_HIT_1, SR_PADDLE_HIT_2, SR_PADDLE_HIT_3, SR_PADDLE_HIT_4 },
		{ 0.5, 0.5, 0.5, 0.5 },
		4
	},
	{ // Ball hits side
		{ SR_BALL_HIT_WALL_1, SR_BALL_HIT_WALL_2 },
		{ 1, 1 },
		2
	},
	{ { SR_CARD_HIGHLIGHT }, { 1 }, 1 }, // Card Highlight
	{ { SR_CARD_SELECT }, { 1 }, 1 }, 
	{ { SR_MISSILE_LAUNCH }, { 1 }, 1 }, 
	{ { SR_MISSILE_FLY }, { 1 }, 1 }, 
	{ { SR_MISSILE_IMPACT }, { 1 }, 1 }, 
	{ { SR_SWORD_SWING_1, SR_SWORD_SWING_2 }, { 1, 1 }, 2 }, 
	{ { SR_SWORD_SLICE_1 }, { 1 }, 1 }, 
	{ { SR_PADDLE_BREAKS }, { 1 }, 1 }, 
	{ { SR_PADDLE_DUPLICATED }, { 1 }, 1 }, 
	{ { SR_EXPIRED_PANADOL }, { 1 }, 1 }, 
	{ { SR_BALL_DAMAGE_DOUBLED }, { 0.5 }, 1 }, 
	{ { SR_PADDLE_DUPLICATED }, { 1 }, 1 },  // Antique console activates, placeholder
	{ { SR_PADDLE_BREAKS }, { 1 }, 1 },  // Hydraulic press breaks ball, placeholder
	{ { SR_BALL_HIT_WALL_1 }, { 1 }, 1 },  // Ball Score Sound
	0
};


void load_audio() {
	// Music
	msc_theme = LoadMusicStream(ASSETS_PATH"music/Superpong Theme.mp3");

	msc_tracks[0] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay Theme 1.mp3");
	msc_tracks[1] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay 2.mp3");
	msc_tracks[2] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay Theme 3.mp3");
	
	SetMusicVolume(msc_theme, 1); // This one is a good volume already
	SetMusicVolume(msc_tracks[0], MAX_MUSIC_VOLUME);
	SetMusicVolume(msc_tracks[1], MAX_MUSIC_VOLUME);
	SetMusicVolume(msc_tracks[2], MAX_MUSIC_VOLUME);

	// SFX
	for (int s = 0; s < num_sound_resources; s++) {
		sound_resources[s] = LoadSound(sound_resource_filenames[s]);
	}

}

void unload_audio() {
	UnloadMusicStream(msc_theme);
	UnloadMusicStream(msc_tracks[0]);
	UnloadMusicStream(msc_tracks[1]);
	UnloadMusicStream(msc_tracks[2]);

	for (int s = 0; s < num_sound_resources; s++) {
		UnloadSound(sound_resources[s]);
	}
}

void init_music_mind(MusicMind *mind) {
	mind->playing = 0;
	mind->fade = 0;

	mind->fade_timer = 0;
	mind->fade_timer_start = 0;
	mind->mode = MM_REPEAT;
}

/** MUSIC **/

/** 
 * Starts the process of fading from one track to another
 * */
void queue_track(MusicMind *mind, Music *new, float fade_duration) {
	//if (mind->playing)
	if (mind->playing != 0) {
		mind->fade = mind->playing;
		//PlayMusicStream(*mind->fade); // I think you have to be playing to even get here
	}

	mind->playing = new;
	PlayMusicStream(*mind->playing);

	if (mind->fade != 0 && fade_duration == 0) {
		SetMusicVolume(*mind->fade, MAX_MUSIC_VOLUME); // Reset volume to 1 for next time
		StopMusicStream(*mind->fade);
	}

	// Reset timers
	mind->fade_timer = fade_duration;
	mind->fade_timer_start = fade_duration;
}

/** SFX **/
void play_sfx(MusicMind *mind, SFX sound_effect_id) {

	if (mind->num_playing_sounds >= MAX_SFX) { return; }

	// Get a variant of a sound
	SoundEffect effect = sfx[sound_effect_id];
	int variant_idx = randInt(0, effect.num_variants-1);

	// Copy the base sound resource and start playing it
	Sound desired = sound_resources[effect.variants[variant_idx]];
	Sound copy = LoadSoundAlias(desired);

	SetSoundVolume(copy, effect.volumes[variant_idx]);
	
	mind->playing_sfx[mind->num_playing_sounds] = copy;
	PlaySound(copy);
}

void music_mind_update(float dt, MusicMind *mind) {

	// SFX
	for (int s=0; s<mind->num_playing_sounds; s++) {
		if (IsSoundPlaying( mind->playing_sfx[s] ) == false ) {
			UnloadSoundAlias(mind->playing_sfx[s]);
			mind->playing_sfx[s] = mind->playing_sfx[mind->num_playing_sounds];
			mind->num_playing_sounds -= 1;
			s -= 1;
		}
	}
	
	// MUSIC
	if (mind->fade_timer > 0) {
		mind->fade_timer -= dt;
		float volume = MAX_MUSIC_VOLUME * (mind->fade_timer / mind->fade_timer_start);

		SetMusicVolume(*mind->fade, volume);
		SetMusicVolume(*mind->playing, MAX_MUSIC_VOLUME-volume);
	
		if (mind->fade_timer <= 0) {
			StopMusicStream(*mind->fade);
			SetMusicVolume(*mind->fade, 1);
			SetMusicVolume(*mind->playing, 1);
			
		}
	}



	if (mind->playing != 0 && IsMusicStreamPlaying(*mind->playing)) {
		UpdateMusicStream(*mind->playing);

		// Change track if we're getting toward the end of this one
		if (mind->mode == MM_PLAYLIST && GetMusicTimeLength(*mind->playing) - GetMusicTimePlayed(*mind->playing) < 15) {
			int new_idx = randInt(0, NUM_TRACKS-1);
			// TODO: i can do better than this. but not now
			while (new_idx == mind->track_idx) {
				new_idx = randInt(0, NUM_TRACKS-1);
			}
			mind->track_idx = new_idx;
			queue_track(mind, &msc_tracks[new_idx], 5);
		} else if (mind->mode == MM_REPEAT) {
			
		}
	}

	if (mind->fade != 0 && IsMusicStreamPlaying(*mind->fade)) { 
		UpdateMusicStream(*mind->fade);
	}

}
