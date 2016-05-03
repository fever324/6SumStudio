#ifndef Constants_h
#define Constants_h

// define the constants for world
#define WORLD_WIDTH 64.0f
#define WORLD_HEIGHT 36.0f

//#define WORLD_SCALE_X 32.0f
//#define WORLD_SCALE_Y 18.0f
//
///** the amout to shrink the body in three dimensions. **/
//// 32 -- 1.5
//// 16 -- 1.0
//#define AVATAR_SHRINK 1.5
//// 32 -- 1.0
//// 16 -- 1.5
//#define GHOST_SHRINK 1.0
//// 32 -- 4
//// 16 -- 2
//#define DOOR_SHRINK 4

#define WORLD_SCALE_X 16.0f
#define WORLD_SCALE_Y 9.0f

/** the amout to shrink the body in three dimensions. **/
// 32 -- 1.5
// 16 -- 1.0
#define AVATAR_SHRINK 1.0
// 32 -- 1.0
// 16 -- 1.5
#define GHOST_SHRINK 1.5
// 32 -- 4
// 16 -- 2
#define DOOR_SHRINK 2

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
#define PFZ_SOUND "audios/backgroundMusic.m4a"
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


#define COOLDOWN    20

/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -2.5f


#endif /* Constants_h */