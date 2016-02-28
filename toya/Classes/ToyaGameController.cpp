//
//  ToyaGameController.cpp
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#include "ToyaGameController.h"
#include "ToyaInputController.h"
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>

#include <string>
#include <iostream>
#include <sstream>

using namespace cocos2d;
using namespace std;

#pragma mark -
#pragma mark Level Geography

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   32.0f
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  18.0f
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -3.0f

/** To automate the loading of crate files */
#define NUM_CRATES 2

/** The initial avatar position */
float AVATAR_POS[] = {24, 4};
/** The goal door position */
float GOAL_POS[] = {6, 12};

#pragma mark Assset Constants
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE       "earth"
/** The key for the rocket texture in the asset manager */
#define ROCK_TEXTURE        "rocket"
/** The key for the win door texture in the asset manager */
#define GOAL_TEXTURE        "goal"
/** The key prefix for the multiple crate assets */
#define CRATE_PREFIX        "crate"
/** The key for the fire textures in the asset manager */
#define MAIN_FIRE_TEXTURE   "flames"
#define RGHT_FIRE_TEXTURE   "flames-right"
#define LEFT_FIRE_TEXTURE   "flames-left"

#define AVATAR_TEXTURE      "avatar"
#define BLOCK_TEXTURE       "block"

/** Color to outline the physics nodes */
#define DEBUG_COLOR     Color3B::YELLOW
/** Opacity of the physics outlines */
#define DEBUG_OPACITY   192

/** The key for collisions sounds */
#define COLLISION_SOUND     "bump"
/** The key for the main afterburner sound */
#define MAIN_FIRE_SOUND     "burn"
/** The key for the right afterburner sound */
#define RGHT_FIRE_SOUND     "sounds/sideburner-left.mp3"
/** The key for the left afterburner sound */
#define LEFT_FIRE_SOUND     "sounds/sideburner-right.mp3"

/** The key for the font reference */
#define PRIMARY_FONT        "retro"

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY       0.0f
/** Density of the crate objects */
#define CRATE_DENSITY       1.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION      0.1f
/** Friction of the crate objects */
#define CRATE_FRICTION      0.2f
/** Angular damping of the crate objects */
#define CRATE_DAMPING       1.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f
/** Threshold for generating sound on collision */
#define SOUND_THRESHOLD     3

#pragma mark -
#pragma mark Initialization

/**
 * Creates a new game world with the default values.
 *
 * This constructor does not allocate any objects or start the controller.
 * This allows us to use a controller without a heap pointer.
 */
GameController::GameController() :
_rootnode(nullptr),
_worldnode(nullptr),
_debugnode(nullptr),
_world(nullptr),
_goalDoor(nullptr),
_avatar(nullptr),
_active(false),
_complete(false),
_debug(false)
{
}


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
bool GameController::init(RootLayer* root) {
    return init(root,Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
}

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
bool GameController::init(RootLayer* root, const Rect& rect) {
    return init(root,rect,Vec2(0,DEFAULT_GRAVITY));
}


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
bool GameController::init(RootLayer* root, const Rect& rect, const Vec2& gravity) {
    Vec2 inputscale = Vec2(root->getScaleX(),root->getScaleY());
    _input.init();
    _input.start();
    
    // Create the world and attach the listeners.
    _world = WorldController::create(rect,gravity);
    _world->retain();
    _world->activateCollisionCallbacks(true);
    _world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    _world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
        beforeSolve(contact,oldManifold);
    };
    
    _scale.set(root->getContentSize().width/rect.size.width,
               root->getContentSize().height/rect.size.height);
    CCLOG("Scale is %.3f, %.3f",root->getContentSize().width,root->getContentSize().height);
    
    // Create the scene graph
    _worldnode = Node::create();
    _debugnode = Node::create();
    _winnode = Label::create();
    
    _winnode->setTTFConfig(_assets->get<TTFont>(PRIMARY_FONT)->getTTF());
    _winnode->setString("VICTORY!");
    
    _winnode->setPosition(root->getContentSize().width/2.0f,root->getContentSize().height/2.0f);
    _winnode->setColor(DEBUG_COLOR);
    _winnode->setVisible(false);
    
    root->addChild(_worldnode,0);
    root->addChild(_debugnode,1);
    root->addChild(_winnode,3);
    _rootnode = root;
    
    populate();
    _active = true;
    _complete = false;
    setDebug(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 *
 * This method is different from dispose() in that it ALSO deletes the
 * input controller.
 */
GameController::~GameController() {
    dispose();
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameController::dispose() {
    if (_world != nullptr) {
        _world->clear();
        _world->release();
    }
}


#pragma mark -
#pragma mark Initialization

/**
 * Resets the status of the game so that we can play again.
 *
 * This method disposes of the world and creates a new one.
 */
void GameController::reset() {
    _world->clear();
    _worldnode->removeAllChildren();
    _debugnode->removeAllChildren();
    
    setComplete(false);
    populate();
}



/**
 * Lays out the game geography.
 *
 * This method is really, really long.  In practice, you would replace this
 * with your serialization loader, which would process a level file.
 */
void GameController::populate() {
    // We need to know the content scale for resolution independence
    // If the device is higher resolution than 1024x576, Cocos2d will scale it
    // This was set as the design resolution in AppDelegate
    // To convert from design resolution to real, divide positions by cscale
    float cscale = Director::getInstance()->getContentScaleFactor();
    // Note that this is different from _scale, which is the physics scale
    
    // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
    // If you are using a device with a 3:2 aspect ratio, you will need to
    // completely redo the level layout.  We can help if this is an issue.
    
    
#pragma mark : Goal door
    Texture2D* image = _assets->get<Texture2D>(GOAL_TEXTURE);
    PolygonNode* sprite;
    WireNode* draw;
    
    // Create obstacle
    Vec2 goalPos = ((Vec2)GOAL_POS);
    sprite = PolygonNode::createWithTexture(image);
    Size goalSize(image->getContentSize().width/_scale.x,
                  image->getContentSize().height/_scale.y);
    _goalDoor = BoxObstacle::create(goalPos,goalSize);
    _goalDoor->setDrawScale(_scale.x, _scale.y);
    
    // Set the physics attributes
    _goalDoor->setBodyType(b2_staticBody);
    _goalDoor->setDensity(0.0f);
    _goalDoor->setFriction(0.0f);
    _goalDoor->setRestitution(0.0f);
    _goalDoor->setSensor(true);
    
    // Add the scene graph nodes to this object
    sprite = PolygonNode::createWithTexture(image);
    sprite->setScale(cscale);
    _goalDoor->setSceneNode(sprite);
    
    draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    _goalDoor->setDebugNode(draw);
    addObstacle(_goalDoor, 0); // Put this at the very back
    
    
#pragma mark : Wall polygon 1
    // Create ground pieces
    // All walls share the same texture
    image  = _assets->get<Texture2D>(EARTH_TEXTURE);
    string wname = "wall";
    
    PolygonObstacle* wallobj;
    Poly2 wall1(WALL1,22);
    wall1.triangulate();
    wallobj = PolygonObstacle::create(wall1);
    wallobj->setDrawScale(_scale.x, _scale.y);
    wallobj->setName(wname);
    
    // Set the physics attributes
    wallobj->setBodyType(b2_staticBody);
    wallobj->setDensity(BASIC_DENSITY);
    wallobj->setFriction(BASIC_FRICTION);
    wallobj->setRestitution(BASIC_RESTITUTION);
    
    // Add the scene graph nodes to this object
    wall1 *= _scale;
    sprite = PolygonNode::createWithTexture(image,wall1);
    wallobj->setSceneNode(sprite);
    
    draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    wallobj->setDebugNode(draw);
    addObstacle(wallobj,1);  // All walls share the same texture
    
    
#pragma mark : Wall polygon 2
    Poly2 wall2(WALL2,18);
    wall2.triangulate();
    wallobj = PolygonObstacle::create(wall2);
    wallobj->setDrawScale(_scale.x, _scale.y);
    wallobj->setName(wname);
    
    // Set the physics attributes
    wallobj->setBodyType(b2_staticBody);
    wallobj->setDensity(BASIC_DENSITY);
    wallobj->setFriction(BASIC_FRICTION);
    wallobj->setRestitution(BASIC_RESTITUTION);
    
    // Add the scene graph nodes to this object
    wall2 *= _scale;
    sprite = PolygonNode::createWithTexture(image,wall2);
    wallobj->setSceneNode(sprite);
    
    draw = WireNode::create();
    draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    wallobj->setDebugNode(draw);
    addObstacle(wallobj,1);
    
    
#pragma mark : Wall polygon 3
    Poly2 wall3(WALL3,8);
    wall3.triangulate();
    wallobj = PolygonObstacle::create(wall3);
    wallobj->setDrawScale(_scale.x, _scale.y);
    wallobj->setName(wname);
    
    // Set the physics attributes
    wallobj->setBodyType(b2_staticBody);
    wallobj->setDensity(BASIC_DENSITY);
    wallobj->setFriction(BASIC_FRICTION);
    wallobj->setRestitution(BASIC_RESTITUTION);
    
    // Add the scene graph nodes to this object
    wall3 *= _scale;
    sprite = PolygonNode::createWithTexture(image,wall3);
    wallobj->setSceneNode(sprite);
    
    draw = WireNode::create();
    draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    wallobj->setDebugNode(draw);
    addObstacle(wallobj,1);
    
    
#pragma mark : Crates
    for (int ii = 0; ii < 15; ii++) {
        
        // Create the sprite for this crate
        image  = _assets->get<Texture2D>("block");
        sprite = PolygonNode::createWithTexture(image);
        sprite->setScale(cscale);
        
        Vec2 boxPos(BOXES[2*ii], BOXES[2*ii+1]);
        Size boxSize(image->getContentSize().width*cscale/_scale.x,image->getContentSize().height*cscale/_scale.y);
        BoxObstacle* crate = BoxObstacle::create(boxPos,boxSize);
        crate->setDrawScale(_scale.x, _scale.y);
        crate->setName(ss.str());
        crate->setAngleSnap(0);     // Snap to the nearest degree
        
        // Set the physics attributes
        crate->setDensity(CRATE_DENSITY);
        crate->setFriction(CRATE_FRICTION);
        crate->setAngularDamping(CRATE_DAMPING);
        crate->setRestitution(BASIC_RESTITUTION);
        
        // Add the scene graph nodes to this object
        crate->setSceneNode(sprite);
        
        draw = WireNode::create();
        draw = WireNode::create();
        draw->setColor(DEBUG_COLOR);
        draw->setOpacity(DEBUG_OPACITY);
        crate->setDebugNode(draw);
        addObstacle(crate,1);  // PUT SAME TEXTURES IN SAME LAYER!!!
    }
    
#pragma mark : Avatar
    Vec2 rockPos = ((Vec2)ROCK_POS);
    image  = _assets->get<Texture2D>(ROCK_TEXTURE);
    Size rockSize(image->getContentSize().width*cscale/_scale.x,image->getContentSize().height*cscale/_scale.y);
    
    _rocket = RocketModel::create(rockPos,rockSize);
    _rocket->setDrawScale(_scale.x, _scale.y);
    
    // Create the polygon node (empty, as the model will initialize)
    sprite = PolygonNode::create();
    sprite->setScale(cscale);
    _rocket->setSceneNode(sprite);
    
    draw = WireNode::create();
    draw->setColor(DEBUG_COLOR);
    draw->setOpacity(DEBUG_OPACITY);
    _rocket->setDebugNode(draw);
    addObstacle(_rocket,3);
}

/**
 * Immediately adds the object to the physics world
 *
 * Objects have a z-order.  This is the order they are drawn in the scene
 * graph node.  Objects with the different textures should have different
 * z-orders whenever possible.  This will cut down on the amount of drawing done
 *
 * param obj The object to add
 * param zOrder The drawing order
 */
void GameController::addObstacle(Obstacle* obj, int zOrder) {
    _world->addObstacle(obj);
    if (obj->getSceneNode() != nullptr) {
        _worldnode->addChild(obj->getSceneNode(),zOrder);
    }
    if (obj->getDebugNode() != nullptr) {
        _debugnode->addChild(obj->getDebugNode(),zOrder);
    }
}


#pragma mark -
#pragma mark Physics Handling

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
void GameController::update(float dt) {
    _input.update(dt);
    
    // Process the toggled key commands
    if (_input.didDebug()) { setDebug(!isDebug()); }
    if (_input.didReset()) { reset(); }
    if (_input.didExit())  {
        CCLOG("Shutting down");
        _rootnode->shutdown();
    }
    //TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
    // Apply the force to the rocket // Hongfei TODO
    _avatar->applyForce();
    _avatar->update(dt);
    
    
    // Turn the physics engine crank.
    _world->update(dt);
}



/**
 * Processes the start of a collision
 *
 * This method is called when we first get a collision between two objects.  We use
 * this method to test if it is the "right" kind of collision.  In particular, we
 * use it to test if we make it to the win door.
 *
 * @param  contact  The two bodies that collided
 */
void GameController::beginContact(b2Contact* contact) {
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    
    // If we hit the "win" door, we are done
    if((body1->GetUserData() == _avatar && body2->GetUserData() == _goalDoor) ||
       (body1->GetUserData() == _goalDoor && body2->GetUserData() == _avatar)) {
        setComplete(true);
    } else {
        
        // If we hit any other wall, avatar turns aroung
        if((body1->GetUserData() == _avatar || (body2->GetUserData() == _avatar)) {
            _avatar.setFacingRight(!_avatar.isFacingRight());
        }
    }
}


/**
 * Handles any modifications necessary before collision resolution
 *
 * This method is called just before Box2D resolves a collision.  We use this method
 * to implement sound on contact, using the algorithms outlined in Ian Parberry's
 * "Introduction to Game Physics with Box2D".
 *
 * @param  contact  	The two bodies that collided
 * @param  oldManfold  	The collision manifold before contact
 */
void GameController::beforeSolve(b2Contact* contact, const b2Manifold* oldManifold) {
    float speed = 0;
    
    // Use Ian Parberry's method to compute a speed threshold
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);
    b2PointState state1[2], state2[2];
    b2GetPointStates(state1, state2, oldManifold, contact->GetManifold());
    for(int ii =0; ii < 2; ii++) {
        if (state2[ii] == b2_addState) {
            b2Vec2 wp = worldManifold.points[0];
            b2Vec2 v1 = body1->GetLinearVelocityFromWorldPoint(wp);
            b2Vec2 v2 = body2->GetLinearVelocityFromWorldPoint(wp);
            b2Vec2 dv = v1-v2;
            speed = b2Dot(dv,worldManifold.normal);
        }
    }
//    
//    // Play a sound if above threshold
//    if (speed > SOUND_THRESHOLD) {
//        string key = ((Obstacle*)body1->GetUserData())->getName()+((Obstacle*)body2->GetUserData())->getName();
//        Sound* source = _assets->get<Sound>(COLLISION_SOUND);
//        SoundEngine::getInstance()->playEffect(key, source, false, 0.5);
//    }
}


/**
 * Preloads the assets needed for the game.
 */
void GameController::preload() {
    // Load the textures (Autorelease objects)
    Texture2D::TexParams params;
    params.wrapS = GL_REPEAT;
    params.wrapT = GL_REPEAT;
    params.magFilter = GL_LINEAR;
    params.minFilter = GL_NEAREST;
    
    _assets = AssetManager::getInstance()->getCurrent();
    TextureLoader* tloader = (TextureLoader*)_assets->access<Texture2D>();
    _assets->loadAsync<TTFont>(PRIMARY_FONT, "fonts/RetroGame.ttf");
    tloader->loadAsync(EARTH_TEXTURE,       "textures/earthtile.png", params);
    tloader->loadAsync(AVATAR_TEXTURE,   "textures/avatar.png");
    tloader->loadAsync(BLOCK_TEXTURE,   "textures/block");
}


