//  ToyaRemovableBlockModel.cpp
//  Toya
//  Created by 6Sum Studio on 2/27/16.
//

#include "ToyaMovingObstacle.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define NORMAL_STATE 0
#define FREEZE_STATE 1
#define FREEZING_TIME 3

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

MovingObstacleModel* MovingObstacleModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes, int speed) {
    MovingObstacleModel* movingObstacle = new (std::nothrow) MovingObstacleModel();
    if (movingObstacle && movingObstacle->init(stateCount, rowCount, columnCount, textureKey, pos, size, scale, routes, speed)) {
        movingObstacle->autorelease();
        return movingObstacle;
    }
    CC_SAFE_DELETE(movingObstacle);
    return nullptr;
}

bool MovingObstacleModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes, int speed) {
    
    if(AnimationBoxModel::init(stateCount, rowCount, columnCount, textureKey, pos, size, scale)) {
        setName(MOVING_OBSTACLE_NAME);
        setBodyType(b2_kinematicBody);
        
        setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        setActive(false);
        
        // Gameplay attributes
        _faceRight  = true;
        _routes = routes;
        _nextPos = 1;
        _freezeTime = 0;
        _speed = speed / 60.0f;
        setDrawScale(scale);
        
        PolygonNode* sprite = PolygonNode::create(Rect(0, 0, size.width, size.height));
//        sprite->setScale(AVATAR_SHRINK);
        setSceneNode(sprite);
        
        WireNode* draw = WireNode::create();
        draw->setColor(Color3B::RED);
        draw->setOpacity(193);
        setDebugNode(draw);
        return true;
    }
    
    return false;
}

void MovingObstacleModel::freeze(Node* parent, Node* parentDebugNode, WorldController* world) {
    if(_currState != FREEZE_STATE) {
        _currState = FREEZE_STATE;
        if(_parent == nullptr) _parent = parent;
        if(_parentDebugNode == nullptr) _parentDebugNode = parentDebugNode;
        _frameCount = 0;
        _freezeTime = 0;
        if(_world == nullptr) _world = world;
    }
}

void MovingObstacleModel::update(float dt) {
    if(_currState == FREEZE_STATE) {
        _freezeTime += dt;
        if(_freezeTime >= FREEZING_TIME) {
            _currState = NORMAL_STATE;
        }
    }
    else if(_currState == NORMAL_STATE) {
        int direction = _faceRight ? 1 : -1;
        _movement = direction * _speed;
        setPosition(Vec2{getPosition().x+_movement, getPosition().y});
        if(getPosition().x > _routes[0].x/64 + _routes[1].x) _faceRight = false;
        if(getPosition().x < _routes[0].x/64) _faceRight = true;
    }
    AnimationBoxModel::update(dt);
}