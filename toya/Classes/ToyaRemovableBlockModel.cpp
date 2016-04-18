//  ToyaRemovableBlockModel.cpp
//  Toya
//  Created by 6Sum Studio on 2/27/16.
//

#include "ToyaRemovableBlockModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <SimpleAudioEngine.h>

#define DESTROY_STATE 1
#define DESTROY_EFFECT "audios/destroy.wav"

using namespace cocos2d;

#pragma mark -
#pragma mark Initializers

/**
 * Creates a new removable box at the given position.
 *
 * The removable box is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 * @param  texture  The texture of the removable box
 * @param  state    The state of the removable box
 *
 * @return  An autoreleased physics object
 */

RemovableBlockModel* RemovableBlockModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale) {
    RemovableBlockModel* removableBlock = new (std::nothrow) RemovableBlockModel();
    if (removableBlock && removableBlock->init(stateCount, rowCount, columnCount, textureKey, pos, size, scale)) {
        removableBlock->autorelease();
        return removableBlock;
    }
    CC_SAFE_DELETE(removableBlock);
    return nullptr;
}

bool RemovableBlockModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale) {
    AnimationBoxModel::init(stateCount, rowCount, columnCount, textureKey, pos, size, scale, ANIMATION_SPEED);
    setName(REMOVABLE_OBJECT_NAME);
    setBodyType(b2_staticBody);

    setIsAnimating(false);
    return true;
}

void RemovableBlockModel::destroy(Node* parent, Node* parentDebugNode, WorldController* world) {
//    _currState = DESTROY_STATE;
    setIsAnimating(true);
    _parent = parent;
    _parentDebugNode = parentDebugNode;
    _frameCount = 0;
    _world = world;
    
    replaceAnimationTexture(1, 4, "destroy_animation");
    _animationNode->setFrame(0);
    getBody()->SetActive(false);
    
    // CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(DESTROY_EFFECT);
}

void RemovableBlockModel::update(float dt) {

    AnimationBoxModel::update(dt);
    if(_frameCount == _columnCount * FRAME_PER_STEP) {
        _parent->removeChild(this->getSceneNode());
        _parentDebugNode->removeChild(this->getDebugNode());
        
        _world->removeObstacle(this);
        
        _parent = nullptr;
        _parentDebugNode = nullptr;
        _world = nullptr;
    }
}
