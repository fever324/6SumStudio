//
//  ToyaAvatarModel.cpp
//
//  Author: Walker White
//  Version: 1/15/15
//

#include "ToyaAvatarModel.h"
#include <cornell/CUPolygonNode.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

using namespace cocos2d;
#define SIGNUM(x)  ((x > 0) - (x < 0))

#pragma mark -
#pragma mark Physics Constants
/** the amout to shrink the body in three dimensions. **/
#define AVATAR_SHRINK 0.2f
/** The amount to shrink the sensor fixture (horizontally) relative to the image */
#define AVATAR_SSHRINK  0.1f
/** Height of the sensor attached to the player's feet */
#define SENSOR_HEIGHT   0.1f
/** The density of the character */
#define AVATAR_DENSITY    1.0f
/** Debug color for the sensor */
#define DEBUG_COLOR     Color3B::RED


#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a new avatar at the origin.
 *
 * The avatar is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @return  An autoreleased physics object
 */
AvatarModel* AvatarModel::create() {
    AvatarModel* avatar = new (std::nothrow) AvatarModel();
    if (avatar && avatar->init()) {
        avatar->autorelease();
        return avatar;
    }
    CC_SAFE_DELETE(avatar);
    return nullptr;
}

/**
 * Creates a new avatar at the given position.
 *
 * The avatar is scaled so that 1 pixel = 1 Box2d unit
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 *
 * @return  An autoreleased physics object
 */
AvatarModel* AvatarModel::create(const Vec2& pos) {
    AvatarModel* avatar = new (std::nothrow) AvatarModel();
    if (avatar && avatar->init(pos)) {
        avatar->autorelease();
        return avatar;
    }
    CC_SAFE_DELETE(avatar);
    return nullptr;
}

/**
 * Creates a new avatar at the given position.
 *
 * The avatar is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 *
 * @return  An autoreleased physics object
 */
AvatarModel* AvatarModel::create(const Vec2& pos, const Vec2& scale) {
    AvatarModel* avatar = new (std::nothrow) AvatarModel();
    if (avatar && avatar->init(pos,scale)) {
        avatar->autorelease();
        return avatar;
    }
    CC_SAFE_DELETE(avatar);
    return nullptr;
}


#pragma mark -
#pragma mark Initializers

/**
 * Initializes a new avatar at the given position.
 *
 * The avatar is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool AvatarModel::init(const Vec2& pos, const Vec2& scale) {
    float cscale = Director::getInstance()->getContentScaleFactor();
    SceneManager* scene = AssetManager::getInstance()->getCurrent();
    
    Texture2D* image = scene->get<Texture2D>("bear");
    
    // Multiply by the scaling factor so we can be resolution independent
    Size avatarSize = Size(image->getContentSize().width*cscale*AVATAR_SHRINK/scale.x,image->getContentSize().height*cscale*AVATAR_SHRINK/scale.y);
    
    if (CapsuleObstacle::init(pos, avatarSize)) {
        
        setDensity(AVATAR_DENSITY);
        setFriction(0.0f);      // HE WILL STICK TO WALLS IF YOU FORGET
        setFixedRotation(true); // OTHERWISE, HE IS A WEEBLE WOBBLE
        
        // Gameplay attributes
        //        _movement = AVATAR_INITIAL_SPEED;
        _faceRight  = true;
        _isGrounded = false;
        setDrawScale(scale);
        setLinearVelocity((Vec2){AVATAR_INITIAL_SPEED,0});
        
        PolygonNode* sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(AVATAR_SHRINK);
        setSceneNode(sprite);
        
        return true;
    }
    
    return false;
}

bool AvatarModel::init(const Vec2& pos, const Vec2& scale, const std::string& avatarTexture) {
    _avatarTexture = avatarTexture;
    return init(pos, scale);
}


#pragma mark -
#pragma mark Attribute Properties

/**
 * Sets left/right movement of this character.
 *
 * This is the result of input times avatar force.
 *
 * @param value left/right movement of this character.
 */
void AvatarModel::setMovement(float value) {
    _movement = value;
}


#pragma mark -
#pragma mark Physics Methods

/**
 * Create new fixtures for this body, defining the shape
 *
 * This is the primary method to override for custom physics objects
 */

void AvatarModel::createSensor(b2Fixture* sensorFixture, b2Vec2 corners[], std::string* sensorName) {
    b2FixtureDef sensorDef;
    sensorDef.density = AVATAR_DENSITY;
    sensorDef.isSensor = true;
    
    b2PolygonShape sensorShape;
    sensorShape.Set(corners,4);
    
    sensorDef.shape = &sensorShape;
    sensorFixture = _body->CreateFixture(&sensorDef);
    sensorFixture->SetUserData(sensorName);
}

void AvatarModel::createFixtures() {
    if (_body == nullptr) {
        return;
    }
    
    CapsuleObstacle::createFixtures();
    
    // Left Sensor Definition
    b2Vec2 leftCorners[4];
    leftCorners[0].x = -getWidth()/2.0f;
    leftCorners[0].y = AVATAR_SSHRINK*getHeight()/2.0f;
    leftCorners[1].x = -getWidth()/2.0f;
    leftCorners[1].y = -AVATAR_SSHRINK*getHeight()/2.0f;
    leftCorners[2].x = -getWidth()/2.0f+SENSOR_HEIGHT;
    leftCorners[2].y = -AVATAR_SSHRINK*getHeight()/2.0f;
    leftCorners[3].x = -getWidth()/2.0f+SENSOR_HEIGHT;
    leftCorners[3].y = AVATAR_SSHRINK*getHeight()/2.0f;
    
    createSensor(_leftSensorFixture, leftCorners, getLeftSensorName());
    
    
    // Right Sensor Definition
    b2Vec2 rightCorners[4];
    rightCorners[0].x = getWidth()/2.0f;
    rightCorners[0].y = AVATAR_SSHRINK*getHeight()/2.0f;
    rightCorners[1].x = getWidth()/2.0f;
    rightCorners[1].y = -AVATAR_SSHRINK*getHeight()/2.0f;
    rightCorners[2].x = getWidth()/2.0f+SENSOR_HEIGHT;
    rightCorners[2].y = -AVATAR_SSHRINK*getHeight()/2.0f;
    rightCorners[3].x = getWidth()/2.0f+SENSOR_HEIGHT;
    rightCorners[3].y = AVATAR_SSHRINK*getHeight()/2.0f;
    
    createSensor(_rightSensorFixture, rightCorners, getRightSensorName());
    
    // Top Sensor Definition
    b2Vec2 topCorners[4];
    topCorners[0].x = -AVATAR_SSHRINK*getWidth()/2.0f;
    topCorners[0].y = getHeight()/2.0f+SENSOR_HEIGHT;
    topCorners[1].x = -AVATAR_SSHRINK*getWidth()/2.0f;
    topCorners[1].y = getHeight()/2.0f;
    topCorners[2].x = AVATAR_SSHRINK*getWidth()/2.0f;
    topCorners[2].y = getHeight()/2.0f;
    topCorners[3].x = AVATAR_SSHRINK*getWidth()/2.0f;
    topCorners[3].y = getHeight()/2.0f+SENSOR_HEIGHT;
    
    createSensor(_topSensorFixture, topCorners, getTopSensorName());
    
    // Bottom Sensor Definition
    b2Vec2 bottomCorners[4];
    bottomCorners[0].x = -AVATAR_SSHRINK*getWidth()/2.0f;
    bottomCorners[0].y = -getHeight()/2.0f;
    bottomCorners[1].x = -AVATAR_SSHRINK*getWidth()/2.0f;
    bottomCorners[1].y = -getHeight()/2.0f-SENSOR_HEIGHT;
    bottomCorners[2].x = AVATAR_SSHRINK*getWidth()/2.0f;
    bottomCorners[2].y = -getHeight()/2.0f-SENSOR_HEIGHT;
    bottomCorners[3].x = AVATAR_SSHRINK*getWidth()/2.0f;
    bottomCorners[3].y = -getHeight()/2.0f;
    
    createSensor(_bottomSensorFixture, bottomCorners, getBottomSensorName());
}

/**
 * Release the fixtures for this body, reseting the shape
 *
 * This is the primary method to override for custom physics objects.
 */
void AvatarModel::releaseFixtures() {
    if (_body != nullptr) {
        return;
    }
    
    CapsuleObstacle::releaseFixtures();
    if (_leftSensorFixture != nullptr) {
        _body->DestroyFixture(_leftSensorFixture);
        _leftSensorFixture = nullptr;
    }
    if (_rightSensorFixture != nullptr) {
        _body->DestroyFixture(_rightSensorFixture);
        _rightSensorFixture = nullptr;
    }
    if (_topSensorFixture != nullptr) {
        _body->DestroyFixture(_topSensorFixture);
        _topSensorFixture = nullptr;
    }
    if (_bottomSensorFixture != nullptr) {
        _body->DestroyFixture(_bottomSensorFixture);
        _bottomSensorFixture = nullptr;
    }
}

/**
 * Applies the force to the body of this avatar
 *
 * This method should be called after the force attribute is set.
 */
void AvatarModel::applyForce() {
    if (!isActive()) {
        return;
    }
    // Don't want to be moving. Damp out player motion
    if (getMovement() == 0.0f) {
        b2Vec2 force(-getDamping()*getVX(),0);
        _body->ApplyForce(force,_body->GetPosition(),true);
    }
    
    // Velocity too high, clamp it
    if (fabs(getVX()) >= getMaxSpeed()) {
        setVX(SIGNUM(getVX())*getMaxSpeed());
    } else {
        b2Vec2 force(getMovement(),0);
        _body->ApplyForce(force,_body->GetPosition(),true);
    }
    
}

/**
 * Updates the object's physics state (NOT GAME LOGIC).
 *
 * We use this method to reset cooldowns.
 *
 * @param delta Number of seconds since last animation frame
 */
void AvatarModel::update(float dt) {
    CapsuleObstacle::update(dt);
    
    int direction = isFacingRight() ? 1 : -1;
    setMovement(direction*getForce());
    applyForce();
}


#pragma mark -
#pragma mark Scene Graph Methods

/**
 * Redraws the outline of the physics fixtures to the debug node
 *
 * The debug node is use to outline the fixtures attached to this object.
 * This is very useful when the fixtures have a very different shape than
 * the texture (e.g. a circular shape attached to a square texture).
 */
void AvatarModel::resetDebugNode() {
    CapsuleObstacle::resetDebugNode();
    float w = AVATAR_SSHRINK*_dimension.width*_drawScale.x;
    float h = SENSOR_HEIGHT*_drawScale.y;
    Poly2 poly(Rect(-w/2.0f,-h/2.0f,w,h));
    poly.traverse(Poly2::Traversal::INTERIOR);
    
    _sensorNode = WireNode::createWithPoly(poly);
    _sensorNode->setColor(DEBUG_COLOR);
    _sensorNode->setPosition(Vec2(_debug->getContentSize().width/2.0f, 0.0f));
    _debug->addChild(_sensorNode);
}
