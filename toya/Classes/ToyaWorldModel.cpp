//
//  ToyaWorldModel.cpp
//
//  Author: 6sumstudio
//  Version: 3/13/16
//

#include "ToyaWorldModel.h"
#include <cocos2d.h>
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "Constants.h"

using namespace cocos2d;

#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Drawing Constants

/** The default texture for the the world background */
#define WORLD_TEXTURE    "sky"



/** The default initial anchor point of the world, center of the camera view **/
#define ANCHOR_POINT 0.5
/** Debug color for the sensor */
#define DEBUG_COLOR     Color3B::RED

#pragma mark -
#pragma mark Physics Constants
/** The default gravity of the world **/
#define WORLD_GRAVITY  -5.0f
/** The default wind resistance of the world*/
#define WIND_RESISTANCE 0.0f



#pragma mark -
#pragma mark Static Constructors

/**
 * Create the world.
 *
 * @return  An autoreleased physics object
 */
WorldModel* WorldModel::create() {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init()) {
//        world->autorelease();
        return world;
    }
    CC_SAFE_DELETE(world);
    return nullptr;
}

/**
 * Creates a world with the given size
 *
 * @param  size  Initial size (width x height)
 *
 * @return  An autoreleased physics object
 */

WorldModel* WorldModel::create(const Size& size) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(size)) {
//        world->autorelease();
        return world;
    }
    CC_SAFE_DELETE(world);
    return nullptr;
}

/**
 * Creates a world with the given size and anchor point
 *
 * @param  size       The dimensions of the world.
 * @param  gravity     The gravity of the world.
 *
 * @return  An autoreleased physics object
 */

WorldModel* WorldModel::create(const Size& size, const Vec2& gravity) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(size,gravity)) {
//        world->autorelease();
        return world;
    }
    CC_SAFE_DELETE(world);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a world with the default size and default anchor point.
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool WorldModel::init() {
    return init(Size(WORLD_WIDTH,WORLD_HEIGHT),Vec2(0,WORLD_GRAVITY));
}

bool WorldModel::init(const Size& size) {
    return init(size,Vec2(0,WORLD_GRAVITY));
}

/**
 * Initializes a new world at the given size and anchor.
 *
 * @param  size        Initial size of the world
 * @param  anchor      Initial anchor point of the world
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */


bool WorldModel::init(const Size& size, const Vec2& gravity) {

    // define the scale
    _follow = new Follow();
    
    _world = WorldController::create(Rect(0, 0, size.width, size.height), gravity);
    _world->retain();
    _world->activateCollisionCallbacks(true);
    // Create the scene graph
    _worldnode = Node::create();
    _debugnode = Node::create();
    
    _worldnode->setContentSize(Size(DESIGN_RES_W,DESIGN_RES_H));
    _debugnode->setContentSize(Size(DESIGN_RES_W,DESIGN_RES_H));
    
    // make sure that anchor point doesn't affect the position.
    _worldnode->ignoreAnchorPointForPosition(true);
    _debugnode->ignoreAnchorPointForPosition(true);
    
    
    _tscale = MIN(0.5f / (size.width / WORLD_SCALE_X), 0.5f / (size.height / WORLD_SCALE_Y));
    // scale the debugnode to half size of the screen
    _debugnode->setScale(_tscale);
    _debugnode->setPosition(Vec2(DESIGN_RES_W/4.0f,DESIGN_RES_H/4.0f));
    _debugnode->setAnchorPoint(Vec2(0, 0));

    return true;
}

#pragma mark -
#pragma mark Attribute Properties

void WorldModel::setGravity(const Vec2& gravity){
    _world->setGravity(gravity);
}

void WorldModel::setRotation(float rotation){
    _worldnode->setRotation(rotation);
    auto g = _world->getGravity();
    
    // rotate the debug node
//    _debugnode->setRotation(rotation);
}

void WorldModel::update(float dt){
    _world->garbageCollect();
    _world->update(dt);
//    _worldnode->update(dt);
}

void WorldModel::addObstacle(Obstacle* obj, int zOrder){
    _world->addObstacle(obj);
    if (obj->getSceneNode() != nullptr) {
        _worldnode->addChild(obj->getSceneNode(),zOrder);
        _debugnode->addChild(obj->getDebugNode(),zOrder);
    }
}

void WorldModel::setAnchor(const Vec2& anchor){
    _worldnode->setAnchorPoint(anchor);
}

void WorldModel::addToWorldNode(PolygonNode* node, int priority){
    _worldnode->addChild(node, priority);
}

void WorldModel::setWorldPos(Obstacle* obj,Vec2& pos){
    _worldnode->setAnchorPoint(Vec2( pos.x / WORLD_SCALE_X, pos.y / WORLD_SCALE_Y ));
}
void WorldModel::setFollow(Obstacle* obj){
    if (_follow == nullptr ) {
        _follow = new Follow();
    }
    _follow->initWithTarget(obj->getSceneNode());
    _worldnode->runAction(_follow);
}

void WorldModel::clear(){
    _world->clear();
    _worldnode->removeAllChildren();
    _worldnode->setRotation(0.0f);
    _debugnode->removeAllChildren();
    
    _world->setGravity(Vec2(0, WORLD_GRAVITY));
    // Reset the debug background
    
    _follow = nullptr;
}

#pragma mark -
#pragma mark Physics Methods

#pragma mark -
#pragma mark Scene Graph Methods

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void WorldModel::resetDebugNode() {

}



