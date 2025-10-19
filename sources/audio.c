#include "audio.h"

Music theme;

void load_audio() {
	theme = LoadMusicStream(ASSETS_PATH"music/Superpong Theme.mp3");
}

void unload_audio() {
	UnloadMusicStream(theme);
}
