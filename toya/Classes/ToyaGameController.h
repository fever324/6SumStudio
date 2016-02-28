//
//  ToyaGameController.cpp
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#ifndef __TOYA_GAME_CONTROLLER_H__
#define __TOYA_GAME_CONTROLLER_H__

#include <vector>
#include <cocos2d.h>
#include <cornell.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "ToyaInputController.h"
#include "ToyaBlockModel.h"
#include "ToyaAvatarModel.h"


using namespace cocos2d;
using namespace std;

#pragma mark -
#pragma mark GameController
/**
 * Instance is a controls the gameplay for the demo.
 *
 * A world has its own objects, assets, and input controller.  Thus this is
 * really a mini-GameEngine in its own right.  The only thing that it does
 * not do is create the scene graph root; that is shared with it by the
 * game root (which has scaled the scene graph to fix the device with the
 * desired aspect ratio).
 */
class GameController {
protected:
    /** The scene manager for this game demo */
    SceneManager* _assets;
    
    /** Controller for abstracting out input away from layer */
    InputController _input;
    
    /** Reference to the root node of the scene graph */
    RootLayer* _rootnode;
    /** Reference to the physics root of the scene graph */
    Node* _worldnode;
    /** Reference to the debug root of the scene graph */
    Node* _debugnode;
    /** Reference to the win message label */
    Label* _winnode;
    
    /** The Box2D world */
    WorldController* _world;
    /** The world scale (computed from root node) */
    Vec2 _scale;
    
    // Physics objects for the game
    /** Reference to the goalDoor (for collision detection) */
    BoxObstacle* _goalDoor;
    /** Reference to the player avatar */
    AvatarModel* _avatar;
    
    /** Whether or note this game is still active */
    bool _active;
    /** Whether we have completed this "game" */
    bool _complete;
    /** Whether or not debug mode is active */
    bool _debug;
    
    
#pragma mark Internal Object Management
    /**
     * Lays out the game geography.
     */
    void populate();
    
    /**
     * Immediately adds the object to the physics world
     *
     * Objects have a z-order.  This is the order they are drawn in the scene
     * graph node.  Objects with the different textures should have different
     * z-orders whenever possible.  This will cut down on the amount of drawing done
     *
     * param  obj       The object to add
     * param  zOrder    The drawing order
     */
    void addObstacle(Obstacle* obj, int zOrder);
    
    
public:
#pragma mark -
#pragma mark Initialization
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  This initializer uses the default scale.
     *
     * @param  root     The root layer for the scene graph
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(RootLayer* root);
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param  root     The root layer for the scene graph
     * @param  rect     The game bounds in Box2d coordinates
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(RootLayer* root, const Rect& rect);
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * The game world is scaled so that the screen coordinates do not agree
     * with the Box2d coordinates.  The bounds are in terms of the Box2d
     * world, not the screen.
     *
     * @param  root     The root layer for the scene graph
     * @param  rect     The game bounds in Box2d coordinates
     * @param  gravity  The gravitational force on this Box2d world
     *
     * @return  true if the controller is initialized properly, false otherwise.
     */
    bool init(RootLayer* root, const Rect& rect, const Vec2& gravity);
    
    
#pragma mark -
#pragma mark State Access
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool isActive() const { return _active; }
    
    /**
     * Returns true if debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @return true if debug mode is active.
     */
    bool isDebug() const { return _debug; }
    
    /**
     * Sets whether debug mode is active.
     *
     * If true, all objects will display their physics bodies.
     *
     * @param value whether debug mode is active.
     */
    void setDebug(bool value) { _debug = value; _debugnode->setVisible(value); }
    
    /**
     * Returns true if the level is completed.
     *
     * If true, the level will advance after a countdown
     *
     * @return true if the level is completed.
     */
    bool isComplete() const { return _complete; }
    
    /**
     * Sets whether the level is completed.
     *
     * If true, the level will advance after a countdown
     *
     * @param value whether the level is completed.
     */
    void setComplete(bool value) { _complete = value; _winnode->setVisible(value); }
    
    
#pragma mark -
#pragma mark Allocation
    /**
     * Creates a new game world with the default values.
     *
     * This constructor does not allocate any objects or start the controller.
     * This allows us to use a controller without a heap pointer.
     */
    GameController();
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~GameController();
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    /**
     * Preloads all of the assets necessary for this game world
     */
    void preload();
    
    
#pragma mark -
#pragma mark Gameplay Handling
    /**
     * Resets the status of the game so that we can play again.
     *
     * This method disposes of the world and creates a new one.
     */
    void reset() ;
    
    /**
     * Executes the core gameplay loop of this world.
     *
     * This method contains the specific update code for this mini-game. It does
     * not handle collisions, as those are managed by the parent class WorldController.
     * This method is called after input is read, but before collisions are resolved.
     * The very last thing that it should do is apply forces to the appropriate objects.
     *
     * @param  delta    Number of seconds since last animation frame
     */
    void update(float dt);
    
    
#pragma mark -
#pragma mark Collision Handling
    /**
     * Processes the start of a collision
     *
     * This method is called when we first get a collision between two objects.  We use
     * this method to test if it is the "right" kind of collision.  In particular, we
     * use it to test if we make it to the win door.
     *
     * @param  contact  The two bodies that collided
     */
    void beginContact(b2Contact* contact);
    
    /**
     * Handles any modifications necessary before collision resolution
     *
     * This method is called just before Box2D resolves a collision.  We use this method
     * to implement sound on contact, using the algorithms outlined in Ian Parberry's
     * "Introduction to Game Physics with Box2D".
     *
     * @param  contact  The two bodies that collided
     * @param  contact  The collision manifold before contact
     */
    void beforeSolve(b2Contact* contact, const b2Manifold* oldManifold);
};

#endif /* defined(__TOYA_GAME_CONTROLLER_H__) */
