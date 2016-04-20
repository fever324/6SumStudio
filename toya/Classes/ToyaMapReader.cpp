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
    this->map = nullptr;
}

void MapReader::loadMap(const std::string &mapFile) {
    this->map = new TMXTiledMap();
    map->initWithTMXFile(mapFile);
    tileSize = map->getTileSize();
    mapSize = map->getMapSize();
    cscale = Director::getInstance()->getContentScaleFactor();
    mapSize = map->getMapSize();

}

void MapReader::createRemovableBlocks() {
    createTheBlocks(map->getLayer(DIRT_LAYER));
    createTheBlocks(map->getLayer(ICE_DIRT_LAYER));
    createTheBlocks(map->getLayer(GRASS_DIRT_LAYER));
    createTheBlocks(map->getLayer(SAND_DIRT_LAYER));
}

void MapReader::createLava() {
    createLavaBlocks(map->getLayer(LAVA_LAYER));
}

void MapReader::createLavaBlocks(TMXLayer* layer) {
    Size size = *new Size((Vec2)BLOCK_SIZE);
    
    if(layer != nullptr) {
        Size layerSize = layer->getLayerSize();
        for (int y = 0; y < layerSize.height; y++) {
            for (int x = 0; x < layerSize.width; x++) {
                auto tileSprite = layer->getTileAt(Point(x, y));
                
                if (tileSprite) {
                    Obstacle* obj = BlockFactory::getRemovableBlock(Vec2(x+0.5,layerSize.height-y-0.5), size, gameController->getScale(),layer->getProperty("texture").asString());
                    gameController->addObstacle(obj, REMOVABLE_DRAW_LAYER);
                    obj->setName("lava");
                }
            }
        }
    }
}


void MapReader::createTheBlocks(TMXLayer* layer) {
    Size size = *new Size((Vec2)BLOCK_SIZE);
    
    if(layer != nullptr) {
        Size layerSize = layer->getLayerSize();
        for (int y = 0; y < layerSize.height; y++) {
            for (int x = 0; x < layerSize.width; x++) {
                auto tileSprite = layer->getTileAt(Point(x, y));
                
                if (tileSprite) {
                    Obstacle* obj = BlockFactory::getRemovableBlock(Vec2(x+0.5,layerSize.height-y-0.5), size, gameController->getScale(),layer->getProperty("texture").asString());
                    gameController->addObstacle(obj, REMOVABLE_DRAW_LAYER);
                }
            }
        }
    }
}


void MapReader::createNonRemovableBlocks() {

    auto layer = map->getLayer(ROCK_LAYER);
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
        

        float y_pos = ghostMap.at("y").asFloat()*cscale + 1.5*tileSize.height;
        
        
        const Size size = *new Size((Vec2){64.0f*cscale/2/_scale.x, 64.0f*cscale/_scale.y/2});
        
        vector<Vec2> routes = {(Vec2){x_pos, y_pos}, (Vec2){ghostMap.at("point1X").asFloat(), ghostMap.at("point1Y").asFloat()}};
        
        Vec2 ghostPos = (Vec2){x_pos/tileSize.width, y_pos/tileSize.height};
        MovingObstacleModel* ghostObj = MovingObstacleModel::create(2, 4, 4, texture, ghostPos, size, _scale, routes, ghostMap.at("speed").asFloat());
        gameController->addObstacle(ghostObj, BARRIER_DRAW_LAYER);
        ghostObj->setName("ghost");
    }
}

void MapReader::createMagicPotions() {
    Vec2 _scale = gameController->getScale();
    
    for(cocos2d::Value potion : map->getObjectGroup("MagicPotions")->getObjects()) {
        cocos2d::ValueMap potionMap = potion.asValueMap();
        string texture = potionMap.at("texture").asString();
        
        float x_pos = potionMap.at("x").asFloat()*cscale;
        
        float y_pos = potionMap.at("y").asFloat()*cscale + 1.5*tileSize.height;
        
        
        const Size size = *new Size((Vec2){64.0f*cscale/2/_scale.x, 64.0f*cscale/_scale.y/2});
        
        Vec2 potionPos = (Vec2){x_pos/tileSize.width, y_pos/tileSize.height};
        MagicPotionModel* potionObj = MagicPotionModel::create(2, 2, 3, texture, potionPos, size, _scale, potionMap.at("points").asInt());
        gameController->addObstacle(potionObj, POTION_DRAW_LAYER);
        potionObj->setName("potion");
    }
}


/**
 Returns number of stars in this level.
 */
int MapReader::createStars() {
    int starCount = 0;
    if(map->getObjectGroup("Stars") == nullptr) return starCount;
    
    Vec2 _scale = gameController->getScale();
    
    for(cocos2d::Value star : map->getObjectGroup("Stars")->getObjects()) {
        cocos2d::ValueMap starMap = star.asValueMap();
        string texture = starMap.at("texture").asString();
        
        float x_pos = starMap.at("x").asFloat()*cscale;
        
        float y_pos = starMap.at("y").asFloat()*cscale + 1.5*tileSize.height;
        
        
        const Size size = *new Size((Vec2){64.0f*cscale/2/_scale.x, 64.0f*cscale/_scale.y/2});
        
        Vec2 starPos = (Vec2){x_pos/tileSize.width+0.5f, y_pos/tileSize.height};
        StarModel* starObj = StarModel::create(2, 2, 2, texture, starPos, size, _scale);
        gameController->addObstacle(starObj, STAR_DRAW_LAYER);
        starObj->setName("star");
        starCount++;
    }
    
    return starCount;
}

void MapReader::createBackground() {
    
    TMXLayer* rootLayer = map->getLayer("rootLayer");
    Size rootSize = rootLayer->getLayerSize();
    Texture2D* image = gameController->getAssets()->get<Texture2D>(rootLayer->getProperty("backgroundImage").asString());
    Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,1024));
    bg->setScale(1);
    bg->setAnchorPoint(Vec2(0,0.3));
    gameController->getRootNode()->addChild(bg);
}

ExitDoorModel* MapReader::createGoalDoor() {

    Vec2 _scale = gameController->getScale();
    TMXObjectGroup* goalDoorGroup = map->getObjectGroup("GoalDoor");
    ValueMap door = goalDoorGroup->getObject("Door");
    float goal_x = door.at("x").asFloat()*cscale + tileSize.width/2;
    
    float goal_y = door.at("y").asFloat()*cscale + 2*tileSize.height;

    Vec2 goalPos = (Vec2){goal_x/tileSize.width, goal_y/tileSize.height};
    Texture2D* image = gameController->getAssets()->get<Texture2D>(goalDoorGroup->getProperty("texture").asString());
    
    Size goalSize = Size(image->getContentSize().width/_scale.x, image->getContentSize().height/_scale.y);
    ExitDoorModel* _goalDoor = ExitDoorModel::create(goalPos, goalSize/4);
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
    float avatar_x = avatar.at("x").asFloat()*cscale + tileSize.width/2.0f;
    float avatar_y = avatar.at("y").asFloat()*cscale + tileSize.height*2;
    Vec2 avatarPos = (Vec2){avatar_x/tileSize.width, avatar_y/tileSize.height};
    AvatarModel* _avatar = AvatarModel::create(avatarPos,_scale, avatar_texture);
    gameController->addObstacle(_avatar, AVATAR_DRAW_LAYER);
    _avatar->setName("avatar");
    
    return _avatar;
}

PanelModel* MapReader::createMagicPanel() {
    
    TMXLayer* rootLayer = map->getLayer("rootLayer");
    
    int totalMana = rootLayer->getProperty("maxMagicPoints").asInt();
    int initalMana = rootLayer->getProperty("initialMagicPoints").asInt();
    PanelModel* _panel = PanelModel::create(Vec2(0,gameController->getRootNode()->getContentSize().height), initalMana, totalMana);
    gameController->getRootNode()->addChild(_panel, PANEL_Z_ORDER);
    
    return _panel;
}

int MapReader::getExpectedPlayTime() {
    TMXLayer* rootLayer = map->getLayer("rootLayer");
    int expectedPlayTime = rootLayer->getProperty("time").asInt();
    return expectedPlayTime;
}



MapReader::~MapReader() {
    delete[] map;
}



