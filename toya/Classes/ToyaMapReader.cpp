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
                    Obstacle* obj = BlockFactory::getRemovableBlock(Vec2(x,layerSize.height-y), size, gameController->getScale());
                    gameController->addObstacle(obj, REMOVABLE_DRAW_LAYER);
                }
            }
        }
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
   
    gameController->addObstacle(_goalDoor, GOAL_DRAW_LAYER);
    return _goalDoor;
}

AvatarModel* MapReader::createAvatar() {
    Vec2 _scale = gameController->getScale();

    TMXObjectGroup* avatarGroup = map->getObjectGroup("Avatar");
    ValueMap avatar = avatarGroup->getObject("Avatar");
    string avatar_texture = avatar.at("texture").asString();
    float avatar_x = avatar.at("x").asFloat();
    float avatar_y = avatar.at("y").asFloat();
    Vec2 avatarPos = (Vec2){avatar_x/tileSize.width, avatar_y/tileSize.height};
    AvatarModel* _avatar = AvatarModel::create(avatarPos,_scale, avatar_texture);
    gameController->addObstacle(_avatar, AVATAR_DRAW_LAYER);
    _avatar->setName("avatar");
    
    return _avatar;
}





