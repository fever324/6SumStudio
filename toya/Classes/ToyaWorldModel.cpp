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

using namespace cocos2d;

#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Drawing Constants
/** The default texture for the the world background */
#define WORLD_TEXTURE    "sky"
/** The default size of the world **/
#define WORLD_WIDTH 32.0f
#define WORLD_HEIGHT 18.0f
/** The default initial anchor point of the world, center of the camera view **/
#define ANCHOR_POINT 0.5
/** Debug color for the sensor */
#define DEBUG_COLOR     Color3B::RED

#pragma mark -
#pragma mark Physics Constants
/** The default gravity of the world **/
#define WORLD_GRAVITY  -8.0f
/** The default wind resistance of the world*/
#define WIND_RESISTANCE 0.0f



#pragma mark -
#pragma mark Static Constructors

/**
 * Create the world.
 *
 * @return  An autoreleased physics object
 */
WorldModel* WorldModel::create(const Size& root) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(root)) {
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

WorldModel* WorldModel::create(const Size& root,const Vec2& size) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(root,size)) {
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
 * @param  anchor     The anchor point of the world.
 *
 * @return  An autoreleased physics object
 */

WorldModel* WorldModel::create(const Size& root,const Vec2& size, const Vec2& anchor) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(root,size,anchor)) {
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
bool WorldModel::init(const Size& root) {
    return init(root,Vec2(WORLD_WIDTH,WORLD_HEIGHT),Vec2(ANCHOR_POINT,ANCHOR_POINT));
}

bool WorldModel::init(const Size& root, const Vec2& size) {
    return init(root,size,Vec2(ANCHOR_POINT,ANCHOR_POINT));
}

/**
 * Initializes a new world at the given size and anchor.
 *
 * @param  size        Initial size of the world
 * @param  anchor      Initial anchor point of the world
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */


bool WorldModel::init(const Size& root,const Vec2& size, const Vec2& anchor) {

    _world = WorldController::create(Rect(0, 0, size.x, size.y), Vec2(0, WORLD_GRAVITY));
    _world->retain();
    _world->activateCollisionCallbacks(true);
    // Create the scene graph
    _worldnode = Node::create();
    _debugnode = Node::create();
    _winnode = Label::create();
    _failnode = Label::create();

    _winnode->setString("VICTORY!");
    _failnode->setString("GAME OVER");
    
    _winnode->setColor(DEBUG_COLOR);
    //TODO: change color
    _failnode->setColor(DEBUG_COLOR);
    _winnode->setVisible(false);
    
    _failnode->setVisible(false);
    
    _winnode->setPosition(root.width/2.0f,root.height/2.0f);
    
    _failnode->setPosition(root.width/2.0f,root.height/2.0f);
    
    _worldnode->setContentSize(root);
    _worldnode->setAnchorPoint(anchor);
    _worldnode->setPosition(root.width/2.0f,root.height/2.0f);
    
    return true;
}

#pragma mark -
#pragma mark Attribute Properties

void WorldModel::setGravity(const Vec2& gravity){
    _world->setGravity(gravity);
}

void WorldModel::setRotation(float rotation){
    _worldnode->setRotation(rotation);
}

void WorldModel::update(float dt){
    _world->update(dt);
//    _worldnode->update(dt);
}

void WorldModel::clear(){
    _world->clear();
    _worldnode->removeAllChildren();
    _debugnode->removeAllChildren();
    _world->release();
}

void WorldModel::addObstacle(Obstacle* obj, int zOrder){
    _world->addObstacle(obj);
    if (obj->getSceneNode() != nullptr) {
        _worldnode->addChild(obj->getSceneNode(),zOrder);
    }
    if (obj->getDebugNode() != nullptr) {
        _debugnode->addChild(obj->getDebugNode(),zOrder);
    }
}

void WorldModel::setDebug(bool value){
    _debugnode->setVisible(value);
}

void WorldModel::setWin(bool value){
    _winnode->setVisible(value);
}

void WorldModel::setFail(bool value){
    _failnode->setVisible(value);
}

void WorldModel::setAnchor(const Vec2& anchor){
    _worldnode->setAnchorPoint(anchor);
}

void WorldModel::addToWorldNode(PolygonNode* node, int priority){
    _worldnode->addChild(node, priority);
}

void WorldModel::removeObstacle(Obstacle* obj){
    _world->removeObstacle(obj);
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



