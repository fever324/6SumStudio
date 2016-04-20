//
//  ToyaWorldModel.h
//  toya
//  This is the root class for a single game scene.
//
//
//  Created by tao on 3/13/16.
//
//
#ifndef __TOYA_WORLD_MODEL_H__
#define __TOYA_WORLD_MODEL_H__

#include <cocos2d.h>
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <cornell/CUAnimationNode.h>

using namespace cocos2d;


#pragma mark -
#pragma mark World Model

/**
 * The game world.
 */

class WorldModel : public Ref{

private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(WorldModel);
    
protected:
    /** The world node */
    Node* _worldnode;
    /** The Box2D world */
    WorldController* _world;
    /** Reference to the debug root of the scene graph */
    Node* _debugnode;
    
    /** the size of the world **/
    Vec2 _size;
    
//    scale
    Vec2 _scale;
    
    Follow* _follow;
    
    /** the anchor point of the world **/
    Vec2 _anchor;
    
    /** texture for the background of the world **/
    std::string _bgTexture;
    
    /** gravity of the world **/
    Vec2 _gravity;
    
#pragma mark -
#pragma mark Scene Graph Management
    
    /**
     * Redraws the outline of the physics fixtures to the debug node
     *
     * The debug node is use to outline the fixtures attached to this object.
     * This is very useful when the fixtures have a different shape than
     * the texture.
     */
    void resetDebugNode();
    
public:
    
#pragma mark -
#pragma mark Static Constructors
    
    /**
     * Create the world.
     *
     * @return  An autoreleased physics object
     */
    static WorldModel* create();
    
    /**
     * Creates a world with the given size
     *
     * @param  size  Initial size (width x height)
     *
     * @return  An autoreleased physics object
     */
    static WorldModel* create(const Size& size);
    
    /**
     * Creates a world with the given size and anchor point
     *
     * @param  size       The dimensions of the world.
     * @param  anchor     The anchor point of the world.
     *
     * @return  An autoreleased physics object
     */
    static WorldModel* create(const Size& size, const Vec2& anchor);
    
#pragma mark Attribute Properties
    
    /**
     * return the world
     *
     * @return the size of the world.
     */
    WorldController* getWorld() const { return _world; }
    Node* getWorldNode() const { return _worldnode; }
    Node* getDebugNode() const { return _debugnode; }
    
    
    /**
     * return the rotation of the world
     *
     * @return the rotation of the world.
     */
    float getRotation() const { return _worldnode->getRotation(); }

    /**
     * return the gravity of the world
     *
     * @return the gravity of the world.
     */
    Vec2 getGravity() const { return _gravity; }
    
    /**
     * return the size of the world
     *
     * @return the size of the world.
     */
    Vec2 getSize() const { return _size; }
    
    /**
     * return the anchor point of the world
     *
     * @return the anchor point of the world.
     */
    Vec2 getAnchor() const { return _anchor; }
    
    
    /**
     * Set the gravity for the game world.
     *
     * @param the gravity
     *
     */
    void setGravity(const Vec2& gravity);
    
    /**
     * Set the rotation for the game world.
     *
     * @param the rotation
     *
     */
    void setRotation(float rotation);
    
    /**
     * Set the size for the game world.
     *
     * @param the size
     *
     */
    void setSize(const Vec2& size);
    
    /**
     * Set the anchor point for the game world.
     *
     * @param the anchor
     *
     */
    void setAnchor(const Vec2& anchor);
    
    void update(float dt);
    
    void clear();
    
    void addObstacle(Obstacle* obj, int zOrder);
//    void removeObstacle(BlockModel** objPtr);
    
    void addToWorldNode(PolygonNode* node, int priority);
    
    void setDebug(bool value);
    
    void setWin(bool value);

    void setFail(bool value);
    
    void setWorldPos(Obstacle* obj,Vec2& pos);
    
    void setFollow(Obstacle* obj);
    
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates WorldModel.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    WorldModel(){}
    
    ~WorldModel(){}
    
    /**
     * Initializes a world with the default size and default anchor point.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool init();
    
    /**
     * Initializes a new avatar at the given size.
     *
     * @param  size        Initial size of the world
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    bool init(const Size& size);
    
    /**
     * Initializes a new avatar at the given size and anchor.
     *
     * @param  size        Initial size of the world
     * @param  anchor      Initial anchor point of the world
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */

    bool init(const Size& size, const Vec2& anchor);
    
#pragma mark -
#pragma mark Animation
    /**
     * Returns the texture (key) for this rocket
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @return the texture (key) for this rocket
     */
    const std::string& getBgTexture() const { return _bgTexture; }
    
    /**
     * Returns the texture (key) for this rocket
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @param  strip    the texture (key) for this rocket
     */
    void setBgTexture(std::string strip) { _bgTexture = strip; }
};

#endif /* defined(__TOYA_WORLD_MODEL_H__) */
