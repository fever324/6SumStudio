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
#include "ToyaWorldModel.h"
#include "ToyaAvatarModel.h"
#include "ToyaOverviewModel.h"
#include "ToyaPanelModel.h"
#include "ToyaExitDoorModel.h"
#include "ToyaMenuModel.h"
#include "ToyaAudioController.h"

#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "ToyaBlockFactory.h"
#include "ToyaMapReader.h"
#include "ToyaProgressModel.h"

// This is not part of cornell.h and SHOULD come last
#include <cornell/CUGenericLoader.h>

//class LevelModel;

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
class OverviewModel;
class MapReader;
class GameController {
protected:
    /** The scene manager for this game demo */
    SceneManager* _assets;
    
    /** Controller for abstracting out input away from layer */
    InputController* _input;
    
    WorldModel* _theWorld;
    MapReader*  _mapReader;
    
    /** Reference to the root node of the scene graph */
    RootLayer* _rootnode;
    
    /** reference to the level model */
//    LevelModel* _level;
    
    
    /** The world scale (computed from root node) */
    Vec2 _scale;
    
    // Physics objects for the game
    /** Reference to the goalDoor (for collision detection) */
    ExitDoorModel* _goalDoor;

//    BlockModel* _door;

    BoxObstacle* _barrier;
    BoxObstacle* _barrier1;
    BoxObstacle* _barrier2;

    /** Reference to the player avatar */
    AvatarModel* _avatar;
    
    AvatarModel* _avatarShadow;
    
    OverviewModel* _overview;
    
    PanelModel* _panel;
    
    ObstacleSelector* _selector;
    
    AudioController* _audio;
    
    
    
    /** three menus: pause, win, fail **/
    MenuModel* _pauseMenu;
    MenuModel* _winMenu;
    MenuModel* _failMenu;
    MenuModel* _helpMenu;
    
    /** Whether or note this game is still active */
    bool _active;
    /** Mark set to handle more sophisticated collision callbacks */
    unordered_set<b2Fixture*> _sensorFixtures;
    /** Whether we have completed this "game" */
    bool _complete;
    /** Whether or not debug mode is active */
    bool _debug;
    /** Whether or not reset mode is active */
    bool _reset;
    int _cooldown;
    bool _youWin;
    
    // indicate current level
    int _currentLevel;
    
    int _starsFound;
    int _maxStarCount;
    
    int _expectedPlayTime;
    
    bool _preload;
    
    Node* _bgNode;
    float _lastScale;
    Vec2 _lastPos;
    Vec2 _lastAnchor;
    float _lastAngle;
    bool _paused;
    
#pragma mark Internal Object Management
    /**
     * Lays out the game geography.
     */
    void populate();
    
    void addFirstTutorial(Vec2 pos);    
    
public:
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
    
    Vec2 getScale() {return _scale;}
    SceneManager* getAssets(){return _assets;}
    RootLayer* getRootNode(){return _rootnode;}
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
    bool init(RootLayer* root, InputController* input, int playLevel);
    
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
    bool init(RootLayer* root, InputController* input, int playLevel, const Rect& rect);
    
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
    bool init(RootLayer* root, InputController* input, int playLevel, const Rect& rect, const Vec2& gravity);
    
    
#pragma mark -
#pragma mark State Access
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool isActive() const { return _active; }
    void setActive(bool value) { _active = value; }
    
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
    bool getPause(){return _paused;}
    void setDebug(bool value) { _debug = value; _theWorld->setDebug(value); }
    void setMap(bool value);
    
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
    bool didWin(){return _youWin;}
    
    bool didGoMain(){return _failMenu->didGoMain() || _winMenu->didGoMain() || _pauseMenu->didGoMain();}
    
    void setComplete(bool value) { _complete = value; _youWin = value;}
    
    void setFail(bool value) { _complete = value; _youWin = false;}
    
    bool finishPreload() { return _preload; }
    
    int getOverallStarCount(bool levelCompleted, float time, int starsFound);
    
    void displayDeathPanel();
    
    
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
    void clear() ;
    
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
    
    Vec2 getRelativePosition(const Vec2& physicalPosition, Vec2& centerPosition, float turningAngel);
    
    
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
    void endContact(b2Contact* contact);
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

#pragma mark -
#pragma mark helper
    
    void togglePause(bool showOrNot){_pauseMenu->setVisible(showOrNot);};
    void toggleWin(bool showOrNot){_winMenu->setVisible(showOrNot);};
    void toggleFail(bool showOrNot){_failMenu->setVisible(showOrNot);};
    void toggleHelp(bool showOrNot){_helpMenu->setVisible(showOrNot);};
    
    void updateMenu(MenuModel* menu){
        // unlock next level if next level is locked
        if (ProgressModel::getInstance()->isLocked(_currentLevel+1)){
            menu->updateLevelButton(_currentLevel+1, "textures/level.png", "unlock");
        }
        // here we need to update the texture of current level with latest or highest score
        // texture will be consisted of textures/level.png and ProgressModel::getInstance()->getStar()
        menu->updateLevelButton(_currentLevel, "textures/level.png", "update");
    }
    
};

#endif /* defined(__TOYA_GAME_CONTROLLER_H__) */
