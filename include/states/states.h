
#include "structs.h"


void init_player(struct PlayerData *player);

/** PONG */
void init_state_pong(struct PongState *pong_state);

/** PICK ITEMS */
void init_state_pick_items(struct PickItemsState *pick_items);

/** MENU **/
void init_state_menu(MenuState *menu_state);
void state_menu(float dt, struct GameState *state);
void draw_menu(MenuState *menu_state);
void button_draw(Button *btn, bool hovered);

/** VICTORY **/
void init_state_victory(VictoryState *victory_state);
void state_victory(float dt, struct GameState *state);
void draw_victory(VictoryState *victory_state);
