//
//  ToyaMapReader.cpp
//  toya
//
//  Created by 6SumStudio on 4/6/16.
//
//

#include "ToyaMapReader.h"

MapReader::MapReader(GameController* gameController){
    this->gameController = gameController;
}

void MapReader::reset(){
    this->map->release();
    this->map = nullptr;
}

void MapReader::loadMap(const std::string &mapFile) {
    this->map = new TMXTiledMap();
    map->initWithTMXFile(mapFile);
    tileSize = map->getTileSize();
    cscale = Director::getInstance()->getContentScaleFactor();
    mapSize = map->getMapSize();

}

void MapReader::createRemovableBlocks() {
    
    const Size size = *new Size((Vec2)BLOCK_SIZE);

    auto layer = map->getLayer(REMOVABLE_LAYER);
    if(layer != nullptr) {
        Size layerSize = layer->getLayerSize();
        for (int y = 0; y < layerSize.height; y++) {
            for (int x = 0; x < layerSize.width; x++) {
                auto tileSprite = layer->getTileAt(Point(x, y));
                
                if (tileSprite) {
                    Obstacle* obj = BlockFactory::getRemovableBlock(Vec2(x+0.5,layerSize.height-y-0.5), size, gameController->getScale());
                    gameController->addObstacle(obj, REMOVABLE_DRAW_LAYER);
                }
            }
        }
    }
}


void MapReader::createNonRemovableBlocks() {

    auto layer = map->getLayer("nonremovables");
    if (layer != nullptr) {
        Obstacle* obj;
        bool makeIt = false;
        Vec2 start;
        Vec2 stop;
        Size layerSize = layer->getLayerSize();
        Vec2 _scale = gameController->getScale();

        bool onRoad = false;
        for (int y = 0; y < layerSize.height; y++) {
            makeIt = false;
            onRoad = false;
            for (int x = 0; x < layerSize.width; x++) {
                
                auto tileSprite = layer->getTileAt(Point(x, y));
                
                // meet the last block and it is not a valid block
                if (x == layerSize.width-1 && !tileSprite) {
                    continue;
                }
                
                // meet the last block, end this line and create the block;
                if (x == layerSize.width-1 && tileSprite) {
                    // if on road, then create the stop
                    // else, set the single block
                    if (!onRoad) {
                        start.x = x;
                        start.y = y;
                    }
                    stop.x = x+1;
                    stop.y = y;
                    makeIt = true;
                }
                
                // make the blocks
                if (makeIt) {
                    float wall[8] = {start.x,layerSize.height-start.y,stop.x,layerSize.height-stop.y,stop.x,layerSize.height-stop.y-1, start.x, layerSize.height-start.y-1};
                    Poly2 wall1(wall,8);
                    wall1.triangulate();
                    obj = BlockFactory::getNonRemovableBlock(wall1,_scale, layer->getProperty("texture").asString());
                    gameController->addObstacle(obj, NONREMOVABLE_DRAW_LAYER);
                    makeIt = false;
                }
                
                
                // first time we meet a block
                if (tileSprite && !onRoad) {
                    onRoad = true;
                    start.x = x;
                    start.y = y;
                }
                
                // if no continuous blocks (only one block)
                if (!tileSprite && x - start.x == 1) {
                    stop.x = x;
                    stop.y = y;
                    makeIt = true;
                    onRoad = false;
                }
                
                // last time we meet a continuous block
                if (!tileSprite && onRoad) {
                    // find the end point
                    stop.x = x;
                    stop.y = y;
                    makeIt = true;
                    onRoad = false;
                }
            }
            
        }
    }
}

void MapReader::createMovingObstacles() {
    Vec2 _scale = gameController->getScale();
    
    for(cocos2d::Value ghost : map->getObjectGroup("Ghosts")->getObjects()) {
        cocos2d::ValueMap ghostMap = ghost.asValueMap();
        string texture = ghostMap.at("texture").asString();
        
        float x_pos = ghostMap.at("x").asFloat()*cscale;
        float y_pos = ghostMap.at("y").asFloat()*cscale;
        
        
        const Size size = *new Size((Vec2){64.0f*cscale*0.3f/_scale.x, 64.0f*cscale*0.3f/_scale.y});
        
        vector<Vec2> routes = {(Vec2){x_pos, y_pos}, (Vec2){ghostMap.at("point1X").asFloat(), ghostMap.at("point1Y").asFloat()}};
        
        Vec2 ghostPos = (Vec2){x_pos/tileSize.width, y_pos/tileSize.height};
        MovingObstacleModel* ghostObj = MovingObstacleModel::create(2, 4, 4, texture, ghostPos, size, _scale, routes, ghostMap.at("speed").asFloat(), ghostMap.at("faceRight").asBool());
        gameController->addObstacle(ghostObj, BARRIER_DRAW_LAYER);
        ghostObj->setName("ghost");
    }

}

void MapReader::createBackground() {
    
    TMXLayer* rootLayer = map->getLayer("rootLayer");
    Size rootSize = rootLayer->getLayerSize();
    Texture2D* image = gameController->getAssets()->get<Texture2D>(rootLayer->getProperty("backgroundImage").asString());
    Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,576));
    bg->setAnchorPoint(Vec2(0,0));
    gameController->getRootNode()->addChild(bg);
    
}

ExitDoorModel* MapReader::createGoalDoor() {

    Vec2 _scale = gameController->getScale();
    TMXObjectGroup* goalDoorGroup = map->getObjectGroup("GoalDoor");
    ValueMap door = goalDoorGroup->getObject("Door");
    float goal_x = door.at("x").asFloat();
    float goal_y = door.at("y").asFloat();
    
    Vec2 goalPos = (Vec2){goal_x/tileSize.width, goal_y/tileSize.height};
    
    Texture2D* image = gameController->getAssets()->get<Texture2D>(goalDoorGroup->getProperty("texture").asString());
    
    
    Size goalSize = Size(image->getContentSize().width/_scale.x, image->getContentSize().height/_scale.y);
    ExitDoorModel* _goalDoor = ExitDoorModel::create(goalPos, goalSize/8);
    _goalDoor->setDrawScale(_scale.x, _scale.y);
    
    WireNode* draw = WireNode::create();
    draw->setColor(Color3B::YELLOW);
    draw->setOpacity(193);
    _goalDoor->setDebugNode(draw);
    
    
    gameController->addObstacle(_goalDoor, GOAL_DRAW_LAYER);
    return _goalDoor;
}

AvatarModel* MapReader::createAvatar() {
    Vec2 _scale = gameController->getScale();

    TMXObjectGroup* avatarGroup = map->getObjectGroup("Avatar");
    ValueMap avatar = avatarGroup->getObject("Avatar");
    string avatar_texture = avatar.at("texture").asString();
    float avatar_x = avatar.at("x").asFloat()*cscale;
    float avatar_y = avatar.at("y").asFloat()*cscale;
    Vec2 avatarPos = (Vec2){avatar_x/tileSize.width, avatar_y/tileSize.height};
    AvatarModel* _avatar = AvatarModel::create(avatarPos,_scale, avatar_texture);
    gameController->addObstacle(_avatar, AVATAR_DRAW_LAYER);
    _avatar->setName("avatar");
    
    return _avatar;
}





