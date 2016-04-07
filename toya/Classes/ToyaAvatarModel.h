//
//  ToyaAvatarModel.h
//  toya
//  This is the root class for a single game scene.
//
//
//  Created by tao on 2/27/16.
//
//
#ifndef __TOYA_AVATAR_MODEL_H__
#define __TOYA_AVATAR_MODEL_H__

#include <cornell/CUBoxObstacle.h>
#include <cornell/CUAnimationNode.h>
#include <cornell/CUCapsuleObstacle.h>
#include <cornell/CUWireNode.h>

using namespace cocos2d;

/** The texture for the character avatar */
#define AVATAR_TEXTURE    "avatar"
#define AVATAR_ANIMATION_ROWS 2
#define AVATAR_ANIMATION_COLS 3
#define AVATAR_FRAMES 6
#define AVATAR_CYCLE_PER_FRAME 30

#pragma mark -
#pragma mark Physics Constants
/** The initial speed of avatar **/
#define AVATAR_INITIAL_SPEED 3.0f
/** The factor to multiply by the input */
#define AVATAR_FORCE    2.0f
/** The amount to slow the character down */
#define AVATAR_DAMPING    5.0f
/** The maximum character speed */
#define AVATAR_MAXSPEED    5.0f
/** Identifier to allow us to track the sensor in ContactListener */
#define LEFT_SENSOR_NAME    "avatarLeftSensor"
#define RIGHT_SENSOR_NAME    "avatarRightSensor"
#define TOP_SENSOR_NAME    "avatarTopSensor"
#define BOTTOM_SENSOR_NAME    "avatarBottomSensor"

#pragma mark -
#pragma mark Avatar Model

/**
 * Player avatar for the game.
 *
 * Note that this class (unlike the other physics objects) constructs its own
 * scene graph node.  Either way is acceptable.  However, in both cases, the
 * physics object must retain ownership of the scene graph node.
 */
class AvatarModel : public CapsuleObstacle {
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(AvatarModel);
    
protected:
    /** Avatar body animation node */
    AnimationNode* _avatarBody;
    /** Animation phase cycle */
    int _cycle;
    /** Update cycle per animation */
    int _animationFrameCount;
    /** The current horizontal movement of the character */
    float _movement;
    /** Which direction is the character facing */
    bool _faceRight;
    /** Whether our feet are on the ground */
    bool _isGrounded;
    /** Reference to the sensor name (since a constant cannot have a pointer) */
    std::string _leftSensorName;
    std::string _rightSensorName;
    std::string _topSensorName;
    std::string _bottomSensorName;
    /** Sensors to detect collision. */
    b2Fixture*  _leftSensorFixture;
    b2Fixture*  _rightSensorFixture;
    b2Fixture*  _topSensorFixture;
    b2Fixture*  _bottomSensorFixture;
    /** The node for debugging the sensor */
    WireNode* _sensorNode;
    
    /** The texture filmstrip for the avatar */
    std::string _avatarTexture;
    
    bool _isRemovable;
    
#pragma mark -
#pragma mark Scene Graph Management
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    virtual void resetSceneNode() override;
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a different shape than
     * the texture.
     */
    virtual void resetDebugNode() override;
    
public:
    
#pragma mark -
#pragma mark Static Constructors
    
    /**
     * Creates a new avatar at the origin.
     *
     * The avatar is 1 unit by 1 unit in size. The avatar is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  An autoreleased physics object
     */
    static AvatarModel* create();
    
    /**
     * Creates a new avatar with the given position
     *
     * The avatar is 1 unit by 1 unit in size. The rocket is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  An autoreleased physics object
     */
    static AvatarModel* create(const Vec2& pos);
    
    /**
     * Creates a new avatar with the given position and size.
     *
     * The avatar size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  size     The dimensions of the box.
     *
     * @return  An autoreleased physics object
     */
    static AvatarModel* create(const Vec2& pos, const Vec2& scale);
    static AvatarModel* create(const Vec2& pos, const Vec2& scale, const std::string& avatarTexture);

    
#pragma mark Attribute Properties
    /**
     * Returns left/right movement of this character.
     *
     * This is the result of input times dude force.
     *
     * @return left/right movement of this character.
     */
    float getMovement() const { return _movement; }
    
    /**
     * Sets left/right movement of this character.
     *
     * This is the result of input times dude force.
     *
     * @param value left/right movement of this character.
     */
    void setMovement(float value);
    
    /**
     * Returns true if the avatar is on the ground.
     *
     * @return true if the avatar is on the ground.
     */
    bool isGrounded() const { return _isGrounded; }
    
    /**
     * Sets whether the avatar is on the ground.
     *
     * @param value whether the avatar is on the ground.
     */
    void setGrounded(bool value) { _isGrounded = value; }
    
    /**
     * Returns how much force to apply to get the avatar moving
     *
     * Multiply this by the input to get the movement value.
     *
     * @return how much force to apply to get the dude moving
     */
    float getForce() const { return AVATAR_FORCE; }
    
    /**
     * Returns ow hard the brakes are applied to get a dude to stop moving
     *
     * @return ow hard the brakes are applied to get a dude to stop moving
     */
    float getDamping() const { return AVATAR_DAMPING; }
    
    /**
     * Returns the upper limit on avatar left-right movement.
     *
     * This does NOT apply to vertical movement.
     *
     * @return the upper limit on avatar left-right movement.
     */
    float getMaxSpeed() const { return AVATAR_MAXSPEED; }
    
    /**
     * Returns the name of the ground sensor
     *
     * This is used by ContactListener
     *
     * @return the name of the ground sensor
     */
    std::string* getLeftSensorName() { return &_leftSensorName; }
    std::string* getRightSensorName() { return &_rightSensorName; }
    std::string* getTopSensorName() { return &_topSensorName; }
    std::string* getBottomSensorName() { return &_bottomSensorName; }
    
    /**
     * Returns true if this avatar is facing right
     *
     * @return true if this avatar is facing right
     */
    bool isFacingRight() const { return _faceRight; }
    void setFacingRight(bool faceRight) {
        
        int direction = faceRight ? 1 : -1;
        
        if(_faceRight != faceRight) {
            setLinearVelocity(Vec2{direction*AVATAR_INITIAL_SPEED, getVY()});
        }
        _faceRight = faceRight;
        
        // Change facing
//        _avatarBody->flipHorizontal(!faceRight);

    }
    
    
#pragma mark Physics Methods
    /**
     * Creates the physics Body(s) for this object, adding them to the world.
     *
     * This method overrides the base method to keep your ship from spinning.
     *
     * @param world Box2D world to store body
     *
     * @return true if object allocation succeeded
     */
    void createFixtures() override;
    
    /**
     * Release the fixtures for this body, reseting the shape
     *
     * This is the primary method to override for custom physics objects.
     */
    void releaseFixtures() override;
    
    /**
     * Updates the object's physics state (NOT GAME LOGIC).
     *
     * We use this method to reset cooldowns.
     *
     * @param delta Number of seconds since last animation frame
     */
    void update(float dt) override;
    
    /**
     * Applies the force to the body of this dude
     *
     * This method should be called after the force attribute is set.
     */
    void applyForce();
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Avatar object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    AvatarModel() : CapsuleObstacle(), _leftSensorName(LEFT_SENSOR_NAME), _rightSensorName(RIGHT_SENSOR_NAME) { }
    
    /**
     * Initializes a new avatar at the origin.
     *
     * The avatar is scaled so that 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(Vec2::ZERO, Vec2::ONE); }
    
    /**
     * Initializes a new avatar at the given position.
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
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos) override { return init(pos, Vec2::ONE); }
    
    virtual bool init(const Vec2& pos, const Vec2& scale);
    
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
    virtual bool init(const Vec2& pos, const Vec2& scale, const std::string& avatarTexture);
    
    
#pragma mark -
#pragma mark Animation
    
    /**
     *  Animates the avatar
     */
    void animateAvatar();

    
    /**
     * Returns the texture (key) for this rocket
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @return the texture (key) for this rocket
     */
    const std::string& getAvatarTexture() const { return _avatarTexture; }
    
    /**
     * Returns the texture (key) for this rocket
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @param  strip    the texture (key) for this rocket
     */
    void setAvatarTexture(std::string strip) { _avatarTexture = strip; }

    void reset();
    
private:
    void createSensor(b2Fixture* sensorFixture, b2Vec2 corners[], std::string* sensorName);
    
    bool isRemovable(){
        return _isRemovable;
    }
    
    void setRemovable(bool value){
        _isRemovable = value;
    }
    
};

#endif /* defined(__TOYA_AVATAR_MODEL_H__) */
