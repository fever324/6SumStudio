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
#include "ToyaPanelModel.h"
#include "ToyaBlockFactory.h"
#include "ToyaAudioController.h"


#include <string>
#include <iostream>
#include <sstream>

using namespace cocos2d;
using namespace std;

#pragma mark -
#pragma mark Asset Constants
/** The JSON file that is the asset directory */
#define ASSET_DIRECTORY     "jsons/assets.json"
/** The key the source file */
#define FILE_KEY           "file"
/** The key indentifying textures to load */
#define TEXTURE_KEY         "textures"
/** The key indicating if a texture is wrapped */
#define TEXTURE_WRAP        "wrap"
/** The key indentifying sounds to load */
#define SOUNDS_KEY          "sounds"
/** The key indicating the sound volume to use */
#define SOUND_VOLUME        "volume"
/** The key indentifying fonts to load */
#define FONTS_KEY           "fonts"
/** The key indentifying the fonts size to use */
#define FONT_SIZE           "size"

/** The source for our level file */
#define LEVEL_ONE_FILE      "jsons/level.json"
/** The key for our loaded level */
#define LEVEL_ONE_KEY       "level1"

#pragma mark -
#pragma mark Level Geography

/** Width of the game world in Box2d units */
#define DEFAULT_WIDTH   64.0
/** Height of the game world in Box2d units */
#define DEFAULT_HEIGHT  36.0
/** The default value of gravity (going down) */
#define DEFAULT_GRAVITY -5.0f

#pragma mark Assset Constants
/** The key for the earth texture in the asset manager */
#define EARTH_TEXTURE       "rock"
/** The key for the removable block texture in the asset manager */
#define REMOVABLE_TEXTURE   "removable"
/** The key for the win door texture in the asset manager */
#define GOAL_TEXTURE        "goal"
#define GOAL_REACHED_TEXTURE "goal-reached"


#define BACKGROUND_TEXTURE  "background"
/** Color to outline the physics nodes */
#define DEBUG_COLOR     Color3B::GREEN
#define DEBUG_GOAL_COLOR     Color3B::RED
#define DEBUG_AVATAR_COLOR  Color3B::WHITE
#define WORLD_COLOR     Color3B::BLUE
/** Opacity of the physics outlines */
#define DEBUG_OPACITY   192

#define COOL_DOWN   120

#pragma mark Physics Constants

// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY       0.0f
/** Density of the crate objects */
#define CRATE_DENSITY       1.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION      0.05f
/** Friction of the crate objects */
#define CRATE_FRICTION      0.05f
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
_goalDoor(nullptr),
_avatar(nullptr),
_active(false),
//_level(nullptr),
_complete(false),
_selector(nullptr),
_debug(false),
_cooldown(COOL_DOWN),
_reset(false),
_overview(nullptr),
_mapReader(nullptr)
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
bool GameController::init(RootLayer* root,InputController* input,int playLevel) {
    return init(root,input,playLevel,Rect(0,0,DEFAULT_WIDTH,DEFAULT_HEIGHT),Vec2(0,DEFAULT_GRAVITY));
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
bool GameController::init(RootLayer* root, InputController* input, int playLevel, const Rect& rect) {
    return init(root,input,playLevel,rect,Vec2(0,DEFAULT_GRAVITY));
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
bool GameController::init(RootLayer* root, InputController* input, int playLevel, const Rect& rect, const Vec2& gravity) {
    
    Vec2 inputscale = Vec2(root->getScaleX(),root->getScaleY());
    
    _input = input;
    _currentLevel = playLevel;
    
    _theWorld = WorldModel::create();
    
    // Create the scene graph
    WorldController* world = _theWorld->getWorld();
    _mapReader = new MapReader(this);
    
    
    // set text config ffor winnodw
    // TODO: move this part to WorldModel too.
    Label* winnode = _theWorld->getWinNode();
    Label* failnode = _theWorld->getFailNode();
    Label* timenode = _theWorld->getTimeNode();

    winnode->setTTFConfig(_assets->get<TTFont>(PRIMARY_FONT)->getTTF());
    failnode->setTTFConfig(_assets->get<TTFont>(PRIMARY_FONT)->getTTF());
    timenode->setTTFConfig(_assets->get<TTFont>(PRIMARY_FONT)->getTTF());
    
    root->addChild(_theWorld->getWorldNode(),1);
    root->addChild(_theWorld->getDebugNode(),2);
    root->addChild(winnode,3);
    root->addChild(failnode,3);
    root->addChild(timenode,3);

    
    world->onBeginContact = [this](b2Contact* contact) {
        beginContact(contact);
    };
    
    world->onEndContact = [this] (b2Contact * contact) {
        endContact(contact);
    };
    world->beforeSolve = [this](b2Contact* contact, const b2Manifold* oldManifold) {
        beforeSolve(contact,oldManifold);
    };
    
    _scale.set(root->getContentSize().width/32.0f,
               root->getContentSize().height/18.0f);
    
    _rootnode = root;
    
    populate();
    
    _active = true;
    _complete = false;
    setDebug(false);
    
    _selector = ObstacleSelector::create(world);
    _selector->retain();
    
    
    // overview panel
    _overview = OverviewModel::create(Vec2(root->getContentSize().width,root->getContentSize().height), inputscale);
    _overview->setGameController(this);
    root->addChild(_overview,3);
    
   
    
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
    if(_selector != nullptr) {
        _selector->release();
    }
    if (_theWorld != nullptr) {
        _theWorld->clear();
        _theWorld->release();
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
    _avatar->reset();
    _selector->deselect();
    _theWorld->clear();
    _input->clear();
    _mapReader->reset();
    setComplete(false);
    _overview->reset();
    _panel->reset();
    
    setFail(false);
    _reset = false;
    _cooldown = COOL_DOWN;
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
    //    float cscale = Director::getInstance()->getContentScaleFactor();
    // Note that this is different from _scale, which is the physics scale
    
    // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
    // If you are using a device with a 3:2 aspect ratio, you will need to
    // completely redo the level layout.  We can help if this is an issue.

    _mapReader->loadMap("maps/test"+std::to_string(_currentLevel)+".tmx");
    
    _mapReader->createBackground();
    _mapReader->createRemovableBlocks();
    _mapReader->createNonRemovableBlocks();
    _mapReader->createMovingObstacles();
    _mapReader->createMagicPotions();
    _goalDoor = _mapReader->createGoalDoor();
    _avatar   = _mapReader->createAvatar();
    _panel    = _mapReader->createMagicPanel();
    
    _theWorld->setFollow(_avatar);
    _avatar->setName("avatar");
    
    _audio->audioBackgroundDeploy(0.1);
    _audio->audioEffectDeploy(0.3);
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
    _theWorld->addObstacle(obj, zOrder);
}

#pragma mark -
#pragma mark Physics Handling

Vec2* GameController::getRelativePosition(const Vec2& physicalPosition, Vec2& centerPosition, float turningAngel) {
    
    Vec2 centerPosition_p = Vec2{512.0f, 288.0f};
    float dist = physicalPosition.getDistance(centerPosition_p);
    
    // Alpha is the current angle between the physical position and center
    float alpha = acos((physicalPosition.x-centerPosition_p.x) / dist) * 180.0f / M_PI;
    if(physicalPosition.y < centerPosition_p.y) alpha = -alpha;
    
    // Beta is the rotation angle
    float beta = _theWorld->getRotation();
    
    // Theta is the angle between physical position and center before any rotation happened
    float theta = alpha + beta;
    
    float originalX = centerPosition_p.x + cos(theta * M_PI / 180.0f) * dist;
    float originalY = centerPosition_p.y + sin(theta * M_PI / 180.0f) * dist;
    
    float relativeX = centerPosition.x - 16.0f + originalX / 1024.0f * 32.0f;
    float relativeY = centerPosition.y - 9.0f + originalY / 576.0f * 18.0f;
    Vec2* relativePosition = new Vec2(relativeX, relativeY);
    
    return relativePosition;
}

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
    if( _overview->hasReseted()) {
        reset();
    }
    if (_reset == true && _cooldown != 0) {
        _cooldown --;
        return;
    }
    
    if ((_reset == true || _overview->hasReseted()) && _cooldown == 0 ) {
        reset();
    }
    
    _input->update(dt);

    // Process the toggled key commands
    if (_input->didReset()) { reset(); }
    if (_input->didExit())  {
        CCLOG("Shutting down");
        _rootnode->shutdown();
    }
    if(_input->didRotate()) {
        
        float cRotation = _theWorld->getRotation() + _input->getTurning()*2;
        
        if (cRotation > 360.0f) {
            cRotation -= 360.0f;
        }
        _theWorld->setRotation(cRotation);
        _avatar->setAngle(cRotation/ 180.0f * M_PI);
        
        Vec2 gravity = Vec2(DEFAULT_GRAVITY,DEFAULT_GRAVITY);
        Vec2 newGravity = _input->getGravity(gravity,cRotation);
        
        _theWorld->setGravity(newGravity);
    }
    
    if (_input->didSelect() && _selector->isSelected()) {
        if(_panel->getSpell() == DESTRUCTION_SPELL_SELECTED) {
            _panel->setSpell(0);
            if (_selector->getObstacle()->getName() == "removable"){
                RemovableBlockModel* rmb = (RemovableBlockModel*) _selector->getObstacle();
                rmb->destroy(_theWorld->getWorldNode(), _theWorld->getDebugNode(), _theWorld->getWorld());
                _audio->playDestroyEffect();
                _panel->deduceMana(DESTRUCTION_COST);
                _selector->deselect();
                
            }
        } else if (_panel->getSpell() == FREEZING_SPELL_SELECTED) {
            _panel->setSpell(0);
            
            if(_selector->getObstacle()->getName() == "ghost") {
                MovingObstacleModel* movingObstacle = (MovingObstacleModel*) _selector->getObstacle();
                movingObstacle->freeze(_theWorld->getWorldNode(), _theWorld->getDebugNode(),
                                       _theWorld->getWorld());
                _audio->playFreezeEffect();
                _panel->deduceMana(FREEZE_COST);
                _selector->deselect();
            }
        }
    } else if (_input->didSelect()) {
        Vec2 centerPosition = _avatar->getPosition();
        Vec2 relativePosition = *getRelativePosition(_input->getSelection(), centerPosition, 0.0f);
        _selector->select(relativePosition);
        if(_avatar == _selector->getObstacle())
            _selector->deselect();
    } else if (_selector->isSelected()) {
        _selector->deselect();
    }
    
    //    update world position
    Vec2 pos = _avatar->getPosition();
    _theWorld->setWorldPos(_avatar,pos);
    
    // Turn the physics engine crank.
    _theWorld->update(dt);
    
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
void GameController::endContact(b2Contact* contact) {
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();
    
    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();
    
    void* fd1 = fix1->GetUserData();
    void* fd2 = fix2->GetUserData();
    
    Obstacle* bd1 = (Obstacle*)body1->GetUserData();
    Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    if ((_avatar->getBottomSensorName() == fd2 && _avatar != bd1)||
        (_avatar->getBottomSensorName() == fd1 && _avatar != bd2)) {
        _avatar->setGrounded(false);
    }
    
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
    b2Fixture* fix1 = contact->GetFixtureA();
    b2Fixture* fix2 = contact->GetFixtureB();
    
    b2Body* body1 = fix1->GetBody();
    b2Body* body2 = fix2->GetBody();
    
    void* fd1 = fix1->GetUserData();
    void* fd2 = fix2->GetUserData();
    
    Obstacle* bd1 = (Obstacle*)body1->GetUserData();
    Obstacle* bd2 = (Obstacle*)body2->GetUserData();
    
    if((bd1->getName() == "avatar" && bd2->getName() == "ghost") ||
       (bd1->getName() == "ghost" && bd2->getName() == "avatar")) {
        MovingObstacleModel* ghost = bd1->getName() == "ghost" ? (MovingObstacleModel*)bd1 : (MovingObstacleModel*)bd2;
        
        if(!ghost->isFrozen()) {
            _audio->playDeathEffect();
            _audio->audioTerminate();
            setFail(true);
            double time = _overview->getCurrentPlayTime();
            _theWorld->showTime(time);
            _reset = true;
        }
    }
    
    if((bd1->getName() == "avatar" && bd2->getName() == "potion") ||
       (bd1->getName() == "potion" && bd2->getName() == "avatar")) {
        MagicPotionModel* magicPotion = bd1->getName() == "potion" ?
            (MagicPotionModel*)bd1 : (MagicPotionModel*)bd2;
        magicPotion->pickUp(_theWorld->getWorldNode(), _theWorld->getDebugNode(), _theWorld->getWorld());
        _audio->playPickupPotion();
        _panel->addMana(magicPotion->getPoints());
    }
    
    // If we hit the "win" door, we are done
    if((body1->GetUserData() == _avatar && body2->GetUserData() == _goalDoor) ||
       (body1->GetUserData() == _goalDoor && body2->GetUserData() == _avatar)) {
        _goalDoor->open();
        _audio->playEffect(WIN_EFFECT);
        _audio->audioTerminate();
        setComplete(true);
        _avatar->setLinearVelocity(Vec2(0.0f, 0.0f));
        // TODO: pause it
        double time = _overview->getCurrentPlayTime();
        _theWorld->showTime(time);
        _reset = true;
    } else {
        // See if we have hit a wall.
        if ((_avatar->getLeftSensorName() == fd2 && _avatar != bd1) ||
            (_avatar->getLeftSensorName() == fd1 && _avatar != bd2)) {
            _avatar->setFacingRight(true);
        } else if ((_avatar->getRightSensorName() == fd2 && _avatar != bd1) ||
                   (_avatar->getRightSensorName() == fd1 && _avatar != bd2)) {
            _avatar->setFacingRight(false);
        } else if ((_avatar->getBottomSensorName() == fd2 && _avatar != bd1)||
                   (_avatar->getBottomSensorName() == fd1 && _avatar != bd2)) {
            _avatar->setGrounded(true);
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
    
    _assets = AssetManager::getInstance()->getCurrent();
    
    JSONReader reader;
    reader.initWithFile(ASSET_DIRECTORY);
    if (!reader.startJSON()) {
        CCASSERT(false, "Failed to load asset directory");
        return;
    }
    
    // Process textures
    if (reader.startObject(TEXTURE_KEY) > 0) {
        TextureLoader* tloader = (TextureLoader*)_assets->access<Texture2D>();
        
        // Wrap settings for identified textures
        Texture2D::TexParams params;
        params.wrapS = GL_REPEAT;
        params.wrapT = GL_REPEAT;
        params.magFilter = GL_LINEAR;
        params.minFilter = GL_NEAREST;
        
        // Convert the object to an array so we can see keys and values
        int tsize = reader.startArray();
        for(int ii = 0; ii < tsize; ii++) {
            string key = reader.getKey();
            
            // Unwrap the object
            if (reader.startObject()) {
                string value = reader.getString(FILE_KEY);
                bool wrap = reader.getBool(TEXTURE_WRAP);
                // Set wrap settings if appropriate
                if (wrap) {
                    tloader->loadAsync(key,value,params);
                } else {
                    tloader->loadAsync(key,value);
                }
            }
            
            reader.endObject();
            reader.advance();
        }
        reader.endArray();
    }
    reader.endObject();
    
    //    // Process sounds
    //    if (reader.startObject(SOUNDS_KEY) > 0) {
    //        // Convert the object to an array so we can see keys and values
    //        int ssize = reader.startArray();
    //        for(int ii = 0; ii < ssize; ii++) {
    //            string key   = reader.getKey();
    //
    //            // Unwrap the object
    //            if (reader.startObject()) {
    //                string value = reader.getString(FILE_KEY);
    //                float vol = reader.getNumber(SOUND_VOLUME);
    //                _volume.emplace(key,vol); // We have to store volume in a different place
    //                _assets->loadAsync<Sound>(key, value);
    //            }
    //
    //            reader.endObject();
    //            reader.advance();
    //        }
    //        reader.endArray();
    //    }
    //    reader.endObject();
    
    // Process fonts
    if (reader.startObject(FONTS_KEY) > 0) {
        FontLoader* floader = (FontLoader*)_assets->access<TTFont>();
        
        // Convert the object to an array so we can see keys and values
        int ssize = reader.startArray();
        for(int ii = 0; ii < ssize; ii++) {
            string key   = reader.getKey();
            
            // Unwrap the object
            if (reader.startObject()) {
                string value = reader.getString(FILE_KEY);
                float size = reader.getNumber(FONT_SIZE);
                floader->loadAsync(key, value, size);
            }
            
            reader.endObject();
            reader.advance();
        }
        reader.endArray();
    }
    reader.endObject();
    reader.endJSON();
    
    // Finally, load the level
    //    _assets->loadAsync<LevelModel>(LEVEL_ONE_KEY,LEVEL_ONE_FILE);
}