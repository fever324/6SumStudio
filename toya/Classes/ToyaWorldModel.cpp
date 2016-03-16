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

#define DESIGN_RES_W    1024
#define DESIGN_RES_H    576

/** The default texture for the the world background */
#define WORLD_TEXTURE    "sky"

/** The default size of the world **/
#define WORLD_WIDTH 64.0f
#define WORLD_SCALE_X 32.0f
#define WORLD_HEIGHT 36.0f
#define WORLD_SCALE_Y 18.0f
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

WorldModel* WorldModel::create(const Vec2& size) {
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
 * @param  anchor     The anchor point of the world.
 *
 * @return  An autoreleased physics object
 */

WorldModel* WorldModel::create(const Vec2& size, const Vec2& anchor) {
    WorldModel* world = new (std::nothrow) WorldModel();
    if (world && world->init(size,anchor)) {
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
    return init(Vec2(WORLD_WIDTH,WORLD_HEIGHT),Vec2(0,WORLD_GRAVITY));
}

bool WorldModel::init(const Vec2& size) {
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


bool WorldModel::init(const Vec2& size, const Vec2& gravity) {

    // define the scale
    _scale.set(DESIGN_RES_W/WORLD_SCALE_X,DESIGN_RES_H/WORLD_SCALE_Y);
    _follow = new Follow();
    
    _world = WorldController::create(Rect(0, 0, size.x, size.y), gravity);
    _world->retain();
    _world->activateCollisionCallbacks(true);
    // Create the scene graph
    _worldnode = Node::create();
    _debugnode = Node::create();
    _winnode = Label::create();
    _failnode = Label::create();
    
    _timenode = Label::create();
    _timenode->setVisible(false);
    _timenode->setPosition(DESIGN_RES_W/2.0f,DESIGN_RES_H/2.0f-100);

    _winnode->setString("VICTORY!");
    _failnode->setString("GAME OVER");
    
    _winnode->setColor(DEBUG_COLOR);
    //TODO: change color
    _failnode->setColor(DEBUG_COLOR);
    _winnode->setVisible(false);
    
    _failnode->setVisible(false);
    
    _winnode->setPosition(DESIGN_RES_W/2.0f,DESIGN_RES_H/2.0f);
    
    _failnode->setPosition(DESIGN_RES_W/2.0f,DESIGN_RES_H/2.0f);
    
    _worldnode->setContentSize(Size(DESIGN_RES_W,DESIGN_RES_H));
    _debugnode->setContentSize(Size(DESIGN_RES_W,DESIGN_RES_H));
    
    // make sure that anchor point doesn't affect the position.
    _worldnode->ignoreAnchorPointForPosition(true);
    _debugnode->ignoreAnchorPointForPosition(true);
    
//    _worldnode->setPosition(-2.5f*_scale.x,(18.0f-WORLD_HEIGHT+2.5f)*_scale.y);
    
    _debugnode->setPosition(Vec2(DESIGN_RES_W/4.0f,DESIGN_RES_H/4.0f));
    
    // set the bg color
    LayerColor* bg = LayerColor::create(Color4B(53, 53, 53, 255));

    // change the scale to parameter
    bg->setContentSize(Size(DESIGN_RES_W*2,DESIGN_RES_H*2));
    //    Layer* bg;
    //    bg = Layer::create();
    //    bg->setContentSize(Size(DESIGN_RES_W,DESIGN_RES_H));
    //    bg->setColor(DEBUG_COLOR);
    _debugnode->addChild(bg);
    
    // scale the debugnode to half size of the screen
    _debugnode->setScale(0.5f / (WORLD_WIDTH / WORLD_SCALE_X));
    
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
    _world->update(dt);
//    _worldnode->update(dt);
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
    _worldnode->setAnchorPoint(Vec2( pos.x / WORLD_SCALE_X, pos.y / WORLD_SCALE_Y ));
}
void WorldModel::setFollow(Obstacle* obj){
    if (_follow == nullptr ) {
        _follow = new Follow();
    }
    _follow->initWithTarget(obj->getSceneNode());
    _worldnode->runAction(_follow);
}

void WorldModel::setWorldPos(Vec2& pos){
    // calulate the position based on the avatar position
    // maybe useless after we choose to use follow
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

void WorldModel::showTime(double time){
    std::ostringstream s;
    s << "Time Cost  " << time;
    _timenode->setString(s.str());
    _timenode->setVisible(true);
}


void WorldModel::clear(){
    _world->clear();
    _worldnode->removeAllChildren();
    _worldnode->setRotation(0.0f);
    _debugnode->removeAllChildren();
    
    
    // Reset the debug background
    LayerColor* bg = LayerColor::create(Color4B(53, 53, 53, 255));
    // change the scale to parameter
    bg->setContentSize(Size(DESIGN_RES_W*2,DESIGN_RES_H*2));
    _debugnode->addChild(bg);
    _timenode->setVisible(false);
    
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



