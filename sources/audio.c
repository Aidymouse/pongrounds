#include "audio.h"
#include "defines.h"
#include "helper.h"

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

	mind->fade_timer = 0;
	mind->fade_timer_start = 0;
}

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
		StopMusicStream(*mind->fade);
	}

	// Reset timers
	mind->fade_timer = fade_duration;
	mind->fade_timer_start = fade_duration;
}

void music_mind_update(float dt, MusicMind *mind) {
	if (mind->fade_timer > 0) {
		mind->fade_timer -= dt;
		float volume = mind->fade_timer / mind->fade_timer_start;

		SetMusicVolume(*mind->fade, volume);
		SetMusicVolume(*mind->playing, 1-volume);
	
		if (mind->fade_timer <= 0) {
			StopMusicStream(*mind->fade);
			SetMusicVolume(*mind->fade, 0);
			SetMusicVolume(*mind->playing, 1);
			
		}
	}



	if (mind->playing != 0 && IsMusicStreamPlaying(*mind->playing)) {
		UpdateMusicStream(*mind->playing);

		// Change track if we're getting toward the end of this one
		if (GetMusicTimeLength(*mind->playing) - GetMusicTimePlayed(*mind->playing) < 15) {
			int new_idx = randInt(0, NUM_TRACKS-1);
			// TODO: i can do better than this. but not now
			while (new_idx == mind->track_idx) {
				new_idx = randInt(0, NUM_TRACKS-1);
			}
			mind->track_idx = new_idx;
			queue_track(mind, &msc_tracks[new_idx], 10);
		}
	}

	if (mind->fade != 0 && IsMusicStreamPlaying(*mind->fade)) { 
		UpdateMusicStream(*mind->fade);
	}
}
