
#include <stdio.h> 
#include <string.h>

#define NOB_IMPLEMENTATION

//#define TARGET_WIN32
//#define TARGET_UNIX

#include "nob.h"

#define PROGRAM_NAME "pongrounds"

int main(int argc, char **argv) {

	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = { 0 };

	#ifdef WIN32
		//printf("Windows");
	#else
		//printf("Unix");
	#endif

	const char* source_files[] = {
		"anim.c",
		"init.c",
		"state_change.c",
		"Vec2.c",
		"helper.c",
		"items.c",

		"obj/ball.c",
		"obj/explosion.c",
		"obj/general.c",
		"obj/paddle.c",
		"obj/rocket.c",
		"obj/sword.c",

		"states/menu.c",
		"states/pick_item.c",
		"states/pong.c",
		"states/victory.c",

		"main.c",
	};
	int NUM_SOURCE_FILES = 17;
	
	nob_mkdir_if_not_exists("build");

	nob_cmd_append(&cmd, "clang", "-o", PROGRAM_NAME);

	for (int s=0; s<NUM_SOURCE_FILES; s++) {
		const char* file = source_files[s];
		
		Nob_String_Builder s = { 0 };
		nob_sb_appendf(&s, "sources/");
		nob_sb_appendf(&s, "%s", file);
		nob_sb_append_null(&s);
		nob_cmd_append(&cmd, s.items);
	}

	nob_cmd_append(&cmd, "-Iinclude");

	nob_cmd_append(&cmd, "-L./lib", "-lraylib", "-Wl,-rpath,./lib");

	nob_cmd_run_sync(cmd);

	return 0;
}

