//
//  ToyaSceneConstants.h
//  toya
//
//  Created by 6SumStudio on 3/13/16.
//
//

#ifndef __TOYA_LEVEL_CONSTANTS_H__
#define __TOYA_LEVEL_CONSTANTS_H__


/** The scene graph priorities of the various objects */
#define GOAL_PRIORITY       0
#define WALL_PRIORITY       1
#define BARRIER_PRIORITY    2
#define CRATE_PRIORITY      3
#define AVATAR_PRIORITY     4

/** The global fields of the level model */
#define WORLD_FIELD         "world"
#define WIDTH_FIELD         "width"
#define HEIGHT_FIELD        "height"
#define GRAVITY_FIELD       "gravity"

#define AVATAR_FIELD        "avatar"
#define GOALDOOR_FIELD      "exit"
#define BARRIERS_FIELD      "barriers"
#define WALLS_FIELD         "walls"
/* removable block */
#define CRATES_FIELD        "crates"

/** The physics fields for each object */
#define POSITION_FIELD      "pos"
#define SIZE_FIELD          "size"
#define BODYTYPE_FIELD      "bodytype"
#define DENSITY_FIELD       "density"
#define FRICTION_FIELD      "friction"
#define RESTITUTION_FIELD   "restitution"
#define DAMPING_FIELD       "damping"
#define ROTATION_FIELD      "rotation"
#define STATIC_VALUE        "static"

/** The drawing fields for each object */
#define TEXTURE_FIELD       "texture"
#define DEBUG_COLOR_FIELD   "debugcolor"
#define DEBUG_OPACITY_FIELD "debugopacity"

/** The wall specific fields */
#define VERTICES_FIELD      "vertices"
#define BOUNDARY_FIELD      "boundary"


#endif /* __TOYA_LEVEL_CONSTANTS_H__ */