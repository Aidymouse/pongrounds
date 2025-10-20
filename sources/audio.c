#include "audio.h"
#include "defines.h"

Music msc_theme;
Music msc_tracks[NUM_TRACKS];


void load_audio() {
	msc_theme = LoadMusicStream(ASSETS_PATH"music/Superpong Theme.mp3");

	msc_tracks[0] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay Theme 1.mp3");
	msc_tracks[1] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay 2.mp3");
	msc_tracks[2] = LoadMusicStream(ASSETS_PATH"music/Superpong Gameplay Theme 3.mp3");

}

void unload_audio() {
	UnloadMusicStream(msc_theme);
	UnloadMusicStream(msc_tracks[0]);
	UnloadMusicStream(msc_tracks[1]);
	UnloadMusicStream(msc_tracks[2]);
}

void init_music_mind(MusicMind *mind) {
	mind->playing = 0;
	mind->fade = 0;

	mind->volume[0] = 0;
	mind->volume[1] = 0; 
	mind->fade_timer[0] = 0;
	mind->fade_timer[1] = 0;
	mind->fade_timer_start[0] = 0;
	mind->fade_timer_start[1] = 0;
}

/** 
 * Starts the process of fading from one track to another
 * */
void queue_track(MusicMind *mind, Music *new, float fade_duration) {
	//if (mind->playing)
	if (mind->playing != 0) {
		mind->fade = mind->playing;
	}

	mind->playing = new;
	PlayMusicStream(*mind->playing);

	// Reset timers
}

void music_mind_update(float dt, MusicMind *mind) {
	if (mind->playing != 0 && IsMusicStreamPlaying(*mind->playing)) {
		UpdateMusicStream(*mind->playing);
		// TODO: update timers
	}

	if (mind->fade != 0 && IsMusicStreamPlaying(*mind->fade)) { 
		UpdateMusicStream(*mind->fade);
		// TODO: update timers
		
		if (mind->fade_timer[1] <= 0) {
			StopMusicStream(*mind->fade);
		}
	}
}
