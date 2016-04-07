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

#define REMOVABLE_LAYER "removables"

#define REMOVABLE_DRAW_LAYER     1
#define NONREMOVABLE_DRAW_LAYER  2
#define GOAL_DRAW_LAYER          3
#define AVATAR_DRAW_LAYER        4
#define BARRIER_DRAW_LAYER       4

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
    float           cscale;
    Size            mapSize;
    
public:
    MapReader(GameController* gameController);
    
    void loadMap(const std::string& mapFile);
    
    void createRemovableBlocks();

    void createNonRemovableBlocks();
    
    void createBackground();
    
    void createMovingObstacles();
    
    ExitDoorModel* createGoalDoor();
    
    AvatarModel* createAvatar();
    
    void reset();

    
};




#endif /* __TOYA_MAP_READER_H__ */
