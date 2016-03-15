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
#define WORLD_WIDTH 64.0f
#define WORLD_HEIGHT 36.0f
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

    // define the scale
    _scale.set(root.width/32.0f,root.height/18.0f);
    _follow = new Follow();
    
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
    _worldnode->setAnchorPoint(Vec2(0, 2));
    
    // make sure that anchor point doesn't affect the position.
    _worldnode->ignoreAnchorPointForPosition(true);
    
//    _worldnode->setPosition(-2.5f*_scale.x,(18.0f-WORLD_HEIGHT+2.5f)*_scale.y);
    
    
    _debugnode->setPosition(Vec2(root.width/4.0f,root.height/4.0f));
    
    // scale the debugnode to half size of the screen
    _debugnode->setScale(0.5f / (WORLD_WIDTH / 32.0f));


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
    _worldnode->setRotation(0.0f);
    _debugnode->removeAllChildren();
    _follow = nullptr;
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

void WorldModel::setWorldPos(Obstacle* obj,Vec2& pos){
    _worldnode->setAnchorPoint(Vec2( pos.x / 32.0, pos.y / 18.0 ));
//    _worldnode->setAnchorPoint(Vec2( 0.5,0.5 ));
    CCLOG("Anchor Point: %f,%f", _worldnode->getAnchorPointInPoints().x / _scale.x,_worldnode->getAnchorPointInPoints().y / _scale.y);
}
void WorldModel::setFollow(Obstacle* obj){
    if (_follow == nullptr ) {
        _follow = new Follow();
    }
    _follow->initWithTarget(obj->getSceneNode());
    _worldnode->runAction(_follow);
}

void WorldModel::setWorldPos(Vec2& pos){
    
//    CCLOG("Current Avatar Pos: %f,%f",pos.x,pos.y);
    
    _worldnode->setAnchorPoint(Vec2(pos.x/64.0f, (36.0f-pos.y) / 36.0 ));
    
    pos.x = -(2.5 + pos.x - 16.0)*_scale.x;
    pos.y = (9.0f-WORLD_HEIGHT + 18.0*2-pos.y)*_scale.y;
    _worldnode->setPosition(pos);

//    if ( pos.x / 32.0f > 0.5 || (18.0*2-pos.y) / 18.0f > 0.5){
//        pos.x = pos.x*_scale.x > _rootnode->getContentSize().width / 2 ? pos.x*_scale.x : _rootnode->getContentSize().width / 2;
//        pos.y = pos.y*_scale.y < _rootnode->getContentSize().height / 2 ? pos.y*_scale.y : _rootnode->getContentSize().height / 2;
//        _worldnode->setAnchorPoint(Vec2(pos.x/64.0f, (36.0f-pos.y) / 36.0 ));
//        
//        CCLOG("Current Anchor point: %f,%f",_worldnode->getAnchorPoint().x,_worldnode->getAnchorPoint().y);
//        
//        pos.x = -(2.5 + pos.x - 16.0)*_scale.x < -2.5f*_scale.x ? -(2.5 + pos.x - 16.0)*_scale.x :  -2.5f*_scale.x;
//        pos.y = (9.0f-WORLD_HEIGHT + 18.0*2-pos.y)*_scale.y > (18.0f-WORLD_HEIGHT+2.5f)*_scale.y ? (9.0f-WORLD_HEIGHT + 18.0*2-pos.y)*_scale.y : (18.0f-WORLD_HEIGHT+2.5f)*_scale.y;
//        _worldnode->setPosition(pos);

//    }
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



