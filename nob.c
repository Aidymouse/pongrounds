
#include <stdio.h> 
#include <string.h>

#define NOB_IMPLEMENTATION

//#define TARGET_WIN32
#define TARGET_UNIX

#include "nob.h"

#define PROGRAM_NAME "pongrounds"

/* All paths assume target is created in the top level folder. */
/* That is, to distribute, just delete all the C files n shit lmao */

int main(int argc, char **argv) {

	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = { 0 };

	const char* source_files[] = {
		"anim.c",
		"init.c",
		"state_change.c",
		"Vec2.c",
		"helper.c",
		"items.c",
		"textures.c",
		"audio.c",
		"controls.c",

		"obj/ball.c",
		"obj/explosion.c",
		"obj/general.c",
		"obj/paddle.c",
		"obj/rocket.c",
		"obj/sword.c",
		"obj/decoration.c",

		"states/menu.c",
		"states/pick_item.c",
		"states/pong.c",
		"states/victory.c",
		"states/pause.c",

		"main.c",
	};
	int NUM_SOURCE_FILES = 22;
	
	nob_mkdir_if_not_exists("build");

	#ifdef TARGET_UNIX
		nob_cmd_append(&cmd, "clang", "-o", PROGRAM_NAME);
	#endif
	#ifdef TARGET_WIN32
		nob_cmd_append(&cmd, "mingw", "-o", PROGRAM_NAME);
	#endif

	for (int s=0; s<NUM_SOURCE_FILES; s++) {
		const char* file = source_files[s];
		
		Nob_String_Builder s = { 0 };
		nob_sb_appendf(&s, "sources/");
		nob_sb_appendf(&s, "%s", file);
		nob_sb_append_null(&s);
		nob_cmd_append(&cmd, s.items);
	}

	nob_cmd_append(&cmd, "-Iinclude");

	// Libraries
	#if defined(TARGET_UNIX)
		nob_cmd_append(&cmd, "-L./lib/linux");
		nob_cmd_append(&cmd, "-Wl,-rpath,./lib/linux");
		nob_cmd_append(&cmd, "-lraylib");

	#elif defined(TARGET_WIN32)
		nob_cmd_append(&cmd, "-L./lib/win32");
		nob_cmd_append(&cmd, "-Wl,-rpath,./lib/win32");
		nob_cmd_append(&cmd, "-lraylibdll");

	#endif


	// Other stuff
	nob_cmd_append(&cmd, "-D", "ASSETS_PATH=\"./assets/\"");
	
	#if defined(TARGET_UNIX)
		nob_cmd_append(&cmd, "-lm");
	#endif

	nob_cmd_run_sync(cmd);

	return 0;
}

