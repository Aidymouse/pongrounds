
#include "structs.h"

/** MENU **/
void init_state_menu(MenuState *menu_state);
void state_menu(struct GameState *state);
void draw_menu(MenuState *menu_state);
void button_draw(Button *btn, bool hovered);

/** VICTORY **/
void init_state_victory(VictoryState *victory_state);
void state_victory(float dt, struct GameState *state);
void draw_victory(VictoryState *victory_state);
