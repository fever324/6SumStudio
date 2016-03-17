//
//  JSLevelModel.cpp
//  Wireframe
//
//  Created by Walker White on 1/19/16.
//
//
#include <platform/CCFileUtils.h>
#include <cornell/CUJSONReader.h>

#include "ToyaLevelModel.h"
#include "ToyaLevelConstants.h"

#include "ToyaWorldModel.h"
#include "ToyaBlockModel.h"
#include "ToyaAvatarModel.h"
#include "ToyaExitDoorModel.h"
#include "ToyaRemovableBlockModel.h"
#include "ToyaBarrierModel.h"

#pragma mark -
#pragma mark Static Constructors

/**
 * Creates a new game level with no source file.
 *
 * The source file can be set at any time via the setFile() method. This method
 * does NOT load the asset.  You must call the load() method to do that.
 *
 * @return  an autoreleased level file
 */
LevelModel* LevelModel::create() {
    LevelModel* level = new (std::nothrow) LevelModel();
    if (level && level->init()) {
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
    
}

/**
 * Creates a new game level with the given source file.
 *
 * This method does NOT load the level. You must call the load() method to do that.
 * This method returns false if file does not exist.
 *
 * @return  an autoreleased level file
 */
LevelModel* LevelModel::create(std::string file) {
    LevelModel* level = new (std::nothrow) LevelModel();
    if (level && level->init(file)) {
        level->autorelease();
        return level;
    }
    CC_SAFE_DELETE(level);
    return nullptr;
}

/**
 * Creates a new, empty level.
 */
LevelModel::LevelModel(void) : Asset(),
_root(nullptr),
_avatar(nullptr),
_goalDoor(nullptr)
{
    _bounds.size.setSize(1.0f, 1.0f);
}

/**
 * Destroys this level, releasing all resources.
 */
LevelModel::~LevelModel(void) {
    unload();
    clearRootNode();
}


#pragma mark -
#pragma mark Drawing Methods
/**
 * Sets the drawing scale for this game level
 *
 * The drawing scale is the number of pixels to draw before Box2D unit. Because
 * mass is a function of area in Box2D, we typically want the physics objects
 * to be small.  So we decouple that scale from the physics object.  However,
 * we must track the scale difference to communicate with the scene graph.
 *
 * We allow for the scaling factor to be non-uniform.
 *
 * @param value  the drawing scale for this game level
 */
void LevelModel::setDrawScale(const Vec2& value) {
//    if (_avatar != nullptr) {
//        _avatar->setDrawScale(value);
//    }
//    if (_goalDoor != nullptr) {
//        _avatar->setDrawScale(value);
//    }
//    for(auto it = _crates.begin(); it != _crates.end(); ++it) {
//        (*it)->setDrawScale(value);
//    }
//    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
//        (*it)->setDrawScale(value);
//    }
}

/**
 * Clears the root scene graph node for this level
 */
void LevelModel::clearRootNode() {
    if (_root == nullptr) {
        return;
    }

    _root->release();
    _root = nullptr;
}

/**
 * Sets the scene graph node for drawing purposes.
 *
 * The scene graph is completely decoupled from the physics system.  The node
 * does not have to be the same size as the physics body. We only guarantee
 * that the node is positioned correctly according to the drawing scale.
 *
 * @param value  the scene graph node for drawing purposes.
 *
 * @retain  a reference to this scene graph node
 * @release the previous scene graph node used by this object
 */
void LevelModel::setRootNode(Node* node) {
    if (_root != nullptr) {
        clearRootNode();
    }
    
    _root = node;
    _root->retain();
    _scale.set(_root->getContentSize().width/_bounds.size.width,
               _root->getContentSize().height/_bounds.size.height);
    

    _world = WorldModel::create(_root->getContentSize());
    
    _root->addChild(_world->getWorldNode(),0);
    _root->addChild(_world->getDebugNode(),1);
    
    // Add the individual elements
    PolygonNode* poly;
    WireNode* draw;
    
    if (_avatar != nullptr) {
        _avatar->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::create();
        _avatar->setSceneNode(poly);
        
        _world->addToWorldNode(poly, AVATAR_PRIORITY);
        
//        draw = WireNode::create();
//        draw->setColor(_avatar->getDebugColor());
//        draw->setOpacity(_avatar->getDebugOpacity());
//        _avatar->setDebugNode(draw);
//        _debugnode->addChild(draw);
    }
    
    if (_goalDoor != nullptr) {
        _goalDoor->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::create();
        _goalDoor->setSceneNode(poly);
        
        _world->addToWorldNode(poly, GOAL_PRIORITY);
        
//        draw = WireNode::create();
//        draw->setColor(_goalDoor->getDebugColor());
//        draw->setOpacity(_goalDoor->getDebugOpacity());
//        _goalDoor->setDebugNode(draw);
//        _debugnode->addChild(draw);
    }
    
    for(auto it = _crates.begin(); it != _crates.end(); ++it) {
        RemovableBlockModel* crate = *it;
        crate->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::create();
        crate->setSceneNode(poly);
        
        _world->addToWorldNode(poly,CRATE_PRIORITY);
        
//        draw = WireNode::create();
//        draw->setColor(crate->getDebugColor());
//        draw->setOpacity(crate->getDebugOpacity());
//        crate->setDebugNode(draw);
//        _debugnode->addChild(draw);
    }
    
    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
        BlockModel* wall = *it;
        wall->setDrawScale(_scale.x, _scale.y);
        poly = PolygonNode::create();
        wall->setSceneNode(poly);
        
        _world->addToWorldNode(poly,WALL_PRIORITY);
        
//        draw = WireNode::create();
//        draw->setColor(wall->getDebugColor());
//        draw->setOpacity(wall->getDebugOpacity());
//        wall->setDebugNode(draw);
//        _debugnode->addChild(draw);
    }
}

/**
 * Toggles whether to show the debug layer of this game world.
 *
 * The debug layer displays wireframe outlines of the physics fixtures.
 *
 * @param  flag whether to show the debug layer of this game world
 */
void LevelModel::showDebug(bool flag) {
//    if (_debugnode != nullptr) {
//        _debugnode->setVisible(flag);
//    }
}


#pragma mark -
#pragma mark Asset Loading
/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel::load() {
    
    JSONReader reader;
    reader.initWithFile(_file);
    if (!reader.startJSON()) {
        CCASSERT(false, "Failed to load level file");
        return false;
    }
    
    // Initial geometry
    float w = reader.getNumber(WIDTH_FIELD);
    float h = reader.getNumber(HEIGHT_FIELD);
    // scaled width and height
    float sw = reader.getNumber(S_WIDTH_FIELD);
    float sh = reader.getNumber(S_HEIGHT_FIELD);
    
    float g = reader.getNumber(GRAVITY_FIELD);
    
    
    _bounds.size.setSize(w, h);
    _gravity.set(0,g);
    
    _scale.set(1024/sw,576/sh);
    
    /** Create the physics world */
    _world = WorldModel::create(Size(w, h),_gravity);
    // Parse the bear
//    if (!loadAvatar(reader)) {
//        CCASSERT(false, "Failed to load rocket");
//        return false;
//    }

//    if (!loadGoalDoor(reader)) {
//        CCASSERT(false, "Failed to load goal door");
//        return false;
//    }
//
//    if (reader.startObject(WALLS_FIELD)) {
//        // Convert the object to an array so we can see keys and values
//        int wsize = reader.startArray();
//        for(int ii = 0; ii < wsize; ii++) {
//            loadWall(reader);
//            reader.advance();
//        }
//        reader.endArray();
//        reader.endObject();
//    } else {
//        reader.endObject();
//        CCASSERT(false, "Failed to load walls");
//        return false;
//    }
//
//    if (reader.startObject(CRATES_FIELD)) {
//        // Convert the object to an array so we can see keys and values
//        int csize = reader.startArray();
//        for(int ii = 0; ii < csize; ii++) {
//            loadCrate(reader);
//            reader.advance();
//        }
//        reader.endArray();
//        reader.endObject();
//    } else {
//        reader.endObject();
//        CCASSERT(false, "Failed to load crates");
//        return false;
//    }
    
    return true;
}

/**
 * Unloads this game level, releasing all sources
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded and
 * unloaded in parallel, not in sequence.  If an asset (like a game level) has
 * references to other assets, then these should be disconnected earlier.
 */
void LevelModel::unload() {
//    if (_avatar != nullptr) {
//        if (_world != nullptr) {
//            _world->removeObstacle(_avatar);
//        }
//        _avatar->release();
//        _avatar = nullptr;
//    }
//    if (_goalDoor != nullptr) {
//        if (_world != nullptr) {
//            _world->removeObstacle(_goalDoor);
//        }
//        _goalDoor->release();
//        _goalDoor = nullptr;
//    }
//    for(auto it = _crates.begin(); it != _crates.end(); ++it) {
//        if (_world != nullptr) {
//            _world->removeObstacle(*it);
//        }
//        (*it)->release();
//    }
//    _crates.clear();
//    for(auto it = _walls.begin(); it != _walls.end(); ++it) {
//        if (_world != nullptr) {
//            _world->removeObstacle(*it);
//        }
//        (*it)->release();
//    }
//    _walls.clear();
//    if (_world != nullptr) {
//        _world->clear();
//        _world->release();
//        _world = nullptr;
//    }
}


#pragma mark -
#pragma mark Individual Loaders

bool LevelModel::loadAvatar(JSONReader& reader) {
    bool success = false;
    if (reader.startObject(AVATAR_FIELD)) {
        success = true;
        success = success && reader.isVec2(POSITION_FIELD);
        Vec2 avatarPos  = reader.getVec2(POSITION_FIELD);
        
        // Get the object, which is automatically retained
        _avatar = AvatarModel::create(avatarPos,_scale);
        
        _avatar->setDensity(reader.getNumber(DENSITY_FIELD));
        _avatar->setFriction(reader.getNumber(FRICTION_FIELD));
        _avatar->setRestitution(reader.getNumber(RESTITUTION_FIELD));
        
        WireNode* draw;
        draw = WireNode::create();
        draw->setColor(parseColor(reader.getString(DEBUG_COLOR_FIELD)));
        draw->setOpacity((GLubyte)reader.getNumber(DEBUG_OPACITY_FIELD));
        _avatar->setDebugNode(draw);
        _avatar->setName(reader.getKey());
        
        std::string btype = reader.getString(BODYTYPE_FIELD);
        if (btype == STATIC_VALUE) {
            _avatar->setBodyType(b2_staticBody);
        }
        
        if (success) {
            _world->addObstacle(_avatar,AVATAR_PRIORITY);
            _world->setFollow(_avatar);
        } else {
            _avatar->release();
        }
    }
    reader.endObject();
    return success;
}

/**
 * Loads the singular exit door
 *
 * The exit door will will be stored in _goalDoor field and retained.
 * If the exit fails to load, then _goalDoor will be nullptr.
 *
 * @param  reader   a JSON reader with cursor ready to read the exit
 *
 * @retain the exit door
 * @return true if the exit door was successfully loaded
 */
bool LevelModel::loadGoalDoor(JSONReader& reader) {
    bool success = false;
    if (reader.startObject(GOALDOOR_FIELD)) {
        success = true;
        success = success && reader.isVec2(POSITION_FIELD);
        Vec2 goalPos  = reader.getVec2(POSITION_FIELD);
        
        
//        PolygonNode* sprite;
//        WireNode* draw;
//
//        // Create obstacle
//        Vec2 goalPos = ((Vec2)GOAL_POS);
//        
//        sprite = PolygonNode::createWithTexture(image);
//        
//        Size goalSize(image->getContentSize().width/_scale.x, image->getContentSize().height/_scale.y);
//        
//        
//        _goalDoor = BlockModel::create(goalPos,goalSize/6);
//        _goalDoor->setDrawScale(_scale.x, _scale.y);
//        
//        // Set the physics attributes
//        _goalDoor->setBodyType(b2_staticBody);
//        _goalDoor->setDensity(0.0f);
//        _goalDoor->setFriction(0.0f);
//        _goalDoor->setRestitution(0.0f);
//        _goalDoor->setSensor(true);
//        
//        // Add the scene graph nodes to this object
//        sprite = PolygonNode::createWithTexture(image);
//        sprite->setScale(cscale/4);
//        _goalDoor->setSceneNode(sprite);
//        
//        draw = WireNode::create();
//        draw->setColor(DEBUG_COLOR);
//        draw->setOpacity(DEBUG_OPACITY);
//        _goalDoor->setDebugNode(draw);
//        addObstacle(_goalDoor, 2); // Put this at the very back
        
        
        
        success = success && reader.isVec2(SIZE_FIELD);
        Vec2 goalSize = reader.getVec2(SIZE_FIELD);
        
        // Get the object, which is automatically retained
        _goalDoor = ExitDoorModel::create(goalPos,(Size)goalSize);
        _goalDoor->setName(reader.getKey());
        
        _goalDoor->setDensity(reader.getNumber(DENSITY_FIELD));
        _goalDoor->setFriction(reader.getNumber(FRICTION_FIELD));
        _goalDoor->setRestitution(reader.getNumber(RESTITUTION_FIELD));
        
        std::string btype = reader.getString(BODYTYPE_FIELD);
        if (btype == STATIC_VALUE) {
            _goalDoor->setBodyType(b2_staticBody);
        }
        
        // Set the texture value
        success = success && reader.isString(TEXTURE_FIELD);
//        _goalDoor->setTextureKey(reader.getString(TEXTURE_FIELD));
//        _goalDoor->setDebugColor(parseColor(reader.getString(DEBUG_COLOR_FIELD)));
//        _goalDoor->setDebugOpacity((GLubyte)reader.getNumber(DEBUG_OPACITY_FIELD));
        
        if (success) {
            _world->addObstacle(_goalDoor,GOAL_PRIORITY);
        } else {
            _goalDoor->release();
        }
    }
    reader.endObject();
    return success;
}

/**
 * Loads a single wall object
 *
 * The wall will be retained and stored in the vector _walls.  If the
 * wall fails to load, then it will not be added to _walls.
 *
 * @param  reader   a JSON reader with cursor ready to read the wall
 *
 * @retain the wall
 * @return true if the wall was successfully loaded
 */
bool LevelModel::loadWall(JSONReader& reader) {
    bool success = reader.startObject();
    
    int polysize = (int)reader.getNumber(VERTICES_FIELD);
    success = success && polysize > 0;
    
    float* vertices = new float[2*polysize];
    int polyread = reader.getFloatArray(BOUNDARY_FIELD,vertices);
    success = success && 2*polysize == polyread;
    
    Poly2 wall(vertices,polyread);
    wall.triangulate();
    
    // Get the object, which is automatically retained
    BlockModel* wallobj = BlockModel::create();
    wallobj->setName(reader.getKey());
    
    std::string btype = reader.getString(BODYTYPE_FIELD);
    if (btype == STATIC_VALUE) {
        wallobj->setBodyType(b2_staticBody);
    }
    
    wallobj->setDensity(reader.getNumber(DENSITY_FIELD));
    wallobj->setFriction(reader.getNumber(FRICTION_FIELD));
    wallobj->setRestitution(reader.getNumber(RESTITUTION_FIELD));
    
    // Set the texture value
    success = success && reader.isString(TEXTURE_FIELD);
//    wallobj->setTextureKey(reader.getString(TEXTURE_FIELD));
//    wallobj->setDebugColor(parseColor(reader.getString(DEBUG_COLOR_FIELD)));
//    wallobj->setDebugOpacity((GLubyte)reader.getNumber(DEBUG_OPACITY_FIELD));
    
    if (success) {
        _walls.push_back(wallobj);
        _world->addObstacle(wallobj,WALL_PRIORITY);
    } else {
        wallobj->release();
    }
    
    reader.endObject();
    delete vertices;
    return success;
}

/**
 * Loads a single crate object
 *
 * The crate will be retained and stored in the vector _crates.  If the
 * crate fails to load, then it will not be added to _crates.
 *
 * @param  reader   a JSON reader with cursor ready to read the crate
 *
 * @retain the crate
 * @return true if the crate was successfully loaded
 */
bool LevelModel::loadCrate(JSONReader& reader) {
    
    bool success = reader.startObject();
    
//    success = success && reader.isVec2(POSITION_FIELD);
//    Vec2 cratePos  = reader.getVec2(POSITION_FIELD);
//    
//    success = success && reader.isVec2(SIZE_FIELD);
//    Vec2 crateSize = reader.getVec2(SIZE_FIELD);
//    
//    // Get the object, which is automatically retained
//    RemovableBlockModel* crate = RemovableBlockModel::create(cratePos,(Size)crateSize,"",2);
//    
//    // Using the key makes too many sounds
//    // crate->setName(reader.getKey());
//    crate->setName(reader.getString(TEXTURE_FIELD));
//    std::string btype = reader.getString(BODYTYPE_FIELD);
//    if (btype == STATIC_VALUE) {
//        crate->setBodyType(b2_staticBody);
//    }
//    
//    crate->setDensity(reader.getNumber(DENSITY_FIELD));
//    crate->setFriction(reader.getNumber(FRICTION_FIELD));
//    crate->setRestitution(reader.getNumber(RESTITUTION_FIELD));
//    crate->setAngularDamping(reader.getNumber(DAMPING_FIELD));
//    crate->setAngleSnap(0);     // Snap to the nearest degree
//    
//    // Set the texture value
//    success = success && reader.isString(TEXTURE_FIELD);
////    crate->setTextureKey(reader.getString(TEXTURE_FIELD));
////    crate->setDebugColor(parseColor(reader.getString(DEBUG_COLOR_FIELD)));
////    crate->setDebugOpacity((GLubyte)reader.getNumber(DEBUG_OPACITY_FIELD));
//    
//    if (success) {
//        _crates.push_back(crate);
//        _world->addObstacle(crate,CRATE_PRIORITY);
//    } else {
//        crate->release();
//    }
//    
//    reader.endObject();
    return success;
}

/**
 * Converts the string to a color
 *
 * Right now we only support the following colors: yellow, red, blur, green,
 * black, and grey.
 *
 * @param  name the color name
 *
 * @return the color for the string
 */
Color3B LevelModel::parseColor(std::string name) {
    if (name == "yellow") {
        return Color3B::YELLOW;
    } else if (name == "red") {
        return Color3B::RED;
    } else if (name == "green") {
        return Color3B::GREEN;
    } else if (name == "blue") {
        return Color3B::BLUE;
    } else if (name == "black") {
        return Color3B::BLACK;
    } else if (name == "gray") {
        return Color3B::GRAY;
    }
    return Color3B::WHITE;
}











