//  ToyaRemovableBlockModel.cpp
//  Toya
//  Created by 6Sum Studio on 2/27/16.
//

#include "ToyaMovingObstacle.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define FREEZE_STATE 1

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

MovingObstacleModel* MovingObstacleModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes) {
    MovingObstacleModel* movingObstacle = new (std::nothrow) MovingObstacleModel();
    if (movingObstacle && movingObstacle->init(stateCount, rowCount, columnCount, textureKey, pos, size, scale, routes)) {
        movingObstacle->autorelease();
        return movingObstacle;
    }
    CC_SAFE_DELETE(movingObstacle);
    return nullptr;
}

bool MovingObstacleModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes) {
    AnimationBoxModel::init(stateCount, rowCount, columnCount, textureKey, pos, size, scale);
    this->setName(MOVING_OBSTACLE_NAME);
    this->setBodyType(b2_staticBody);
    
    return true;
}

void MovingObstacleModel::freeze(Node* parent, Node* parentDebugNode, WorldController* world) {
    _currState = FREEZE_STATE;
    _parent = parent;
    _parentDebugNode = parentDebugNode;
    _frameCount = 0;
    _world = world;
    
    this->getBody()->SetActive(false);
}

void MovingObstacleModel::update(float dt) {
    AnimationBoxModel::update(dt);
    if(_frameCount == _columnCount * FRAME_PER_STEP && _currState == DESTROY_STATE) {
        _parent->removeChild(this->getSceneNode());
        _parentDebugNode->removeChild(this->getDebugNode());
        
        _world->removeObstacle(this);
        
        _parent = nullptr;
        _parentDebugNode = nullptr;
        _world = nullptr;
    }
}