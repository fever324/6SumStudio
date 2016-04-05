/**
 * ToyaExitDoorModel.cpp
 *
 * Created by sb2345 on 3/13/2016.
 */

#include "ToyaExitDoorModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define DOOR_OPEN   0
#define DOOR_CLOSED 1

using namespace cocos2d;

//
ExitDoorModel* ExitDoorModel::create() {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init()) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos, const Size &size) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos, size)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}





//
bool ExitDoorModel::init(const Vec2 &pos, const Size &size) {
    BoxObstacle::init(pos, size);
    
    // Set the physics attributes
    setBodyType(b2_staticBody);
    setSensor(true);
    
    // Set door state
    _state = DOOR_CLOSED;
    
    // Apply image
    SceneManager* am = AssetManager::getInstance()->getCurrent();
    Texture2D* image = am->get<Texture2D>(GOAL_TEXTURE);
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    Sprite* sprite = Sprite::createWithTexture(image);
    sprite->setScale(cscale/4);
    setSceneNode(sprite);
    
    return true;
}



/**
 * Destructor of this exit releasing all resources.
 */
ExitDoorModel::~ExitDoorModel(void) {
    // We do not own any of these, so we just set to null
}

void ExitDoorModel::open() {
    if(_state == DOOR_OPEN) {
        return;
    }
    _state = DOOR_OPEN;
    Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>(GOAL_REACHED_TEXTURE);
    Sprite* sprite =  (Sprite*) getSceneNode();
    sprite->setTexture(image);
}


