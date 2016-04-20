#ifndef Constants_h
#define Constants_h

#define REMOVABLE_OBJECT_NAME "removable"
#define REMOVABLE_BLOCK_TEXTURE "removable"

#define MOVING_OBSTACLE_NAME "movingObstacle"
#define MAGIC_POTION_NAME "magicPotion"

#define DEBUG_COLOR     Color3B::RED
#define PRIMARY_FONT        "retro"

#define DESTRUCTION_COST 1
#define FREEZE_COST      2

// Audio Effects
#define BG_SOUND  "audios/XMAS Grains.wav"
#define DESTROY_EFFECT  "audios/fire.m4a"
#define DEATH_SOUND "audios/over.m4a"
#define PICKUP_MAGIC  "audios/pickup.m4a"
#define FREEZE_EFFECT "audios/freeze.m4a"
#define WIN_EFFECT "audios/win.m4a"
// End of Audio Effects


// define the layer order

#define LAUCH_MENU_ORDER  1
#define LEVEL_MENU_ORDER  1
#define GAME_WORLD_ORDER  2
#define PANEL_VIEW_ORDER  2
#define PAUSE_BUTTON_ORDER  2
#define PAUSE_MENU_ORDER  3
#define WIN_MENU_ORDER    3
#define FAIL_MENU_ORDER   3
#define DEBUG_NODE_ORDER  4


#define COOLDOWN    60


#endif /* Constants_h */