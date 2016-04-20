//
//  ToyaMapReader.h
//  toya
//
//  Created by 6SumStudio Li on 4/6/16.
//
//

#ifndef __TOYA_MAP_READER_H__
#define __TOYA_MAP_READER_H__

#include <cocos2d.h>
#include "ToyaBlockFactory.h"
#include "ToyaGameController.h"
#include "ToyaExitDoorModel.h"
#include "Constants.h"

#define DIRT_LAYER "dirt"
#define ROCK_LAYER "rock"
#define ICE_DIRT_LAYER "ice"
#define SAND_DIRT_LAYER "sand"
#define GRASS_DIRT_LAYER "grass"
#define LAVA_LAYER "lava"

#define DIRT_TEXTURE "dirt"
#define ROCK_TEXTURE "rock"
#define ICE_DIRT_TEXTURE "ice"
#define SAND_DIRT_TEXTURE "sand"
#define GRASS_DIRT_TEXTURE "grass"

#define REMOVABLE_DRAW_LAYER     1
#define NONREMOVABLE_DRAW_LAYER  2
#define GOAL_DRAW_LAYER          3
#define AVATAR_DRAW_LAYER        4
#define BARRIER_DRAW_LAYER       4
#define POTION_DRAW_LAYER        5
#define STAR_DRAW_LAYER          1

static float BLOCK_SIZE[] = {1, 1};


using namespace cocos2d;
using namespace std;


class GameController;
class ExitDoor;

class MapReader : public Ref  {  

protected:
    GameController* gameController;
    TMXTiledMap*    map;
    Size            tileSize;
    Size            mapSize;
    float           cscale;
    
public:
    MapReader(GameController* gameController);
    
    /**
     Returns the expected level play time
     */
    void loadMap(const std::string& mapFile);
    
    void createTheMap(){
        createBackground();
        createRemovableBlocks();
        createNonRemovableBlocks();
        createMovingObstacles();
        createMagicPotions();
        createLava();
    }
    
    void createRemovableBlocks();
    void createTheBlocks(TMXLayer* layer);
    void createNonRemovableBlocks();
    void createLavaBlocks(TMXLayer* layer);
    void createLava();
    void createBackground();
    void createMovingObstacles();
    void createMagicPotions();
    
    int getExpectedPlayTime();
    
    /**
     Returns number of stars in this level.
     */
    int createStars();
    
    WorldModel* createTheWorld();
    
    PanelModel* createMagicPanel();
    
    ExitDoorModel* createGoalDoor();
    
    AvatarModel* createAvatar();
    
    void reset();
    
    ~MapReader();

    
};




#endif /* __TOYA_MAP_READER_H__ */
