////
////  JSLevelModel.hpp
////  JSONDemo
////
////  This module provides a level that can be loaded from a JSON file.  It reads the JSON
////  file and creates the objects in the level.  Objects are one of four categories: the
////  rocket, the exit, the walls, and the crates.
////
////  This class is a subclass of Asset so that we can use a GenericLoader to manage the
////  the loading.  Even though this class defines a load method, there is the question of
////  when the method is called. The GenericLoader allows this object to be loaded in a
////  separate thread along with all of the other assets.
////
////  This file is based on the CS 3152 PhysicsDemo Lab by Don Holden, 2007
////
////  Author: Walker White
////  Version: 1/20/16
////
//#ifndef __TOYA_SCENE_MODEL_H__
//#define __TOYA_SCENE_MODEL_H__
//#include <cocos2d.h>
//#include <vector>
//#include <cornell/CUWorldController.h>
//#include <cornell/CUAsset.h>
//#include <cornell/CUJSONReader.h>
//
//using namespace cocos2d;
//
///** Forward references to the various classes used by this level */
//class WorldModel;
//class AvatarModel;
//class BlockModel;
//class RemovableBlockModel;
//class ExitDoorModel;
//
//#pragma mark -
//#pragma mark Level Model
///**
// * Class that represents a dynamically loaded level in the game
// *
// * This class is a subclass of Asset so that we can use it with a GenericLoader. As with
// * all assets, this class SHOULD NOT make any references to AssetManager in the load/unload
// * methods. Assets should be treated as if they load in parallel, not in sequence.  Therefore,
// * it is unsafe to assume that one asset loads before another.  If this level needs to connect
// * to other assets (sound, images, etc.) this should take place after asset loading, such as
// * during scene graph initialization.
// */
//class LevelModel : public Asset {
//protected:
//    /** The root node of this level */
//    Node* _root;
//    /** The bounds of this level in physics coordinates */
//    Rect _bounds;
//    /** The global gravity for this level */
//    Vec2 _gravity;
//    /** The level drawing scale (difference between physics and drawing coordinates) */
//    Vec2 _scale;
//    
////    /** Reference to the physics root of the scene graph */
////    Node* _worldnode;
////    /** Reference to the debug root of the scene graph */
////    Node* _debugnode;
//    
//    // Physics objects for the game
//    /** The physics word; part of the model (though listeners elsewhere) */
//    WorldModel* _world;
//    /** Reference to the rocket/player avatar */
//    AvatarModel* _avatar;
//    /** Reference to the goalDoor (for collision detection) */
//    ExitDoorModel* _goalDoor;
//    
//    /** Reference to all the active crates */
//    std::vector<RemovableBlockModel*> _crates;
//    /** Reference to all the walls */
//    std::vector<BlockModel*> _walls;
//    
//    
//#pragma mark Internal Helper Methods
//    /**
//     * Loads the singular rocket object
//     *
//     * The rocket will will be stored in _rocket field and retained.
//     * If the rocket fails to load, then _rocket will be nullptr.
//     *
//     * @param  reader   a JSON reader with cursor ready to read the rocket
//     *
//     * @retain the rocket
//     * @return true if the rocket was successfully loaded
//     */
//    bool loadAvatar(JSONReader& reader);
//    
//    /**
//     * Loads the singular exit door
//     *
//     * The exit door will will be stored in _goalDoor field and retained.
//     * If the exit fails to load, then _goalDoor will be nullptr.
//     *
//     * @param  reader   a JSON reader with cursor ready to read the exit
//     *
//     * @retain the exit door
//     * @return true if the exit door was successfully loaded
//     */
//    bool loadGoalDoor(JSONReader& reader);
//    
//    /**
//     * Loads a single wall object
//     *
//     * The wall will be retained and stored in the vector _walls.  If the
//     * wall fails to load, then it will not be added to _walls.
//     *
//     * @param  reader   a JSON reader with cursor ready to read the wall
//     *
//     * @retain the wall
//     * @return true if the wall was successfully loaded
//     */
//    bool loadWall(JSONReader& reader);
//    
//    /**
//     * Loads a single crate object
//     *
//     * The crate will be retained and stored in the vector _crates.  If the
//     * crate fails to load, then it will not be added to _crates.
//     *
//     * @param  reader   a JSON reader with cursor ready to read the crate
//     *
//     * @retain the crate
//     * @return true if the crate was successfully loaded
//     */
//    bool loadCrate(JSONReader& reader);
//    
//    /**
//     * Converts the string to a color
//     *
//     * Right now we only support the following colors: yellow, red, blur, green,
//     * black, and grey.
//     *
//     * @param  name the color name
//     *
//     * @return the color for the string
//     */
//    Color3B parseColor(std::string name);
//    
//    /**
//     * Clears the root scene graph node for this level
//     */
//    void clearRootNode();
//    
//public:
//#pragma mark Static Constructors
//    /**
//     * Creates a new game level with no source file.
//     *
//     * The source file can be set at any time via the setFile() method. This method
//     * does NOT load the asset.  You must call the load() method to do that.
//     *
//     * @return  an autoreleased level file
//     */
//    static LevelModel* create();
//    
//    /**
//     * Creates a new game level with the given source file.
//     *
//     * This method does NOT load the level. You must call the load() method to do that.
//     * This method returns false if file does not exist.
//     *
//     * @return  an autoreleased level file
//     */
//    static LevelModel* create(std::string file);
//    
//#pragma mark Model Access
//    /**
//     * Returns the rocket in this game level
//     *
//     * @return the rocket in this game level
//     */
//    AvatarModel* getRocket() { return _avatar; }
//    
//    /**
//     * Returns the exit door in this game level
//     *
//     * @return the exit door in this game level
//     */
//    ExitDoorModel* getExit() { return _goalDoor; }
//    
//    WorldModel* getWorld() { return _world; }
//    
//    
//#pragma mark Physics Attributes
//    /**
//     * Returns the bounds of this level in physics coordinates
//     *
//     * @return the bounds of this level in physics coordinates
//     */
//    const Rect& getBounds() const   { return _bounds; }
//    
//    /**
//     * Returns the global gravity for this level
//     *
//     * @return the global gravity for this level
//     */
//    const Vec2& getGravity() const { return _gravity; }
//    
//    /**
//     * Activates all of the physics objects in this level
//     *
//     * @param  controller  the world controller to manage physics
//     */
//    void activate(WorldController* controller);
//    
//    
//#pragma mark Drawing Methods
//    /**
//     * Returns the drawing scale for this game level
//     *
//     * The drawing scale is the number of pixels to draw before Box2D unit. Because
//     * mass is a function of area in Box2D, we typically want the physics objects
//     * to be small.  So we decouple that scale from the physics object.  However,
//     * we must track the scale difference to communicate with the scene graph.
//     *
//     * We allow for the scaling factor to be non-uniform.
//     *
//     * @return the drawing scale for this game level
//     */
//    const Vec2& getDrawScale() const { return _scale; }
//    
//    /**
//     * Sets the drawing scale for this game level
//     *
//     * The drawing scale is the number of pixels to draw before Box2D unit. Because
//     * mass is a function of area in Box2D, we typically want the physics objects
//     * to be small.  So we decouple that scale from the physics object.  However,
//     * we must track the scale difference to communicate with the scene graph.
//     *
//     * We allow for the scaling factor to be non-uniform.
//     *
//     * @param value  the drawing scale for this game level
//     */
//    void setDrawScale(const Vec2& value);
//    
//    /**
//     * Returns the scene graph node for drawing purposes.
//     *
//     * The scene graph is completely decoupled from the physics system.  The node
//     * does not have to be the same size as the physics body. We only guarantee
//     * that the node is positioned correctly according to the drawing scale.
//     *
//     * @return the scene graph node for drawing purposes.
//     */
//    Node* getRootNode() const { return _root; }
//    
//    /**
//     * Sets the scene graph node for drawing purposes.
//     *
//     * The scene graph is completely decoupled from the physics system.  The node
//     * does not have to be the same size as the physics body. We only guarantee
//     * that the node is positioned correctly according to the drawing scale.
//     *
//     * @param value  the scene graph node for drawing purposes.
//     *
//     * @retain  a reference to this scene graph node
//     * @release the previous scene graph node used by this object
//     */
//    void setRootNode(Node* node);
//    
//    /**
//     * Toggles whether to show the debug layer of this game world.
//     *
//     * The debug layer displays wireframe outlines of the physics fixtures.
//     *
//     * @param  flag whether to show the debug layer of this game world
//     */
//    void showDebug(bool flag);
//    
//    
//#pragma mark -
//#pragma mark Asset Loading
//    /**
//     * Loads this game level from the source file
//     *
//     * This load method should NEVER access the AssetManager.  Assets are loaded in
//     * parallel, not in sequence.  If an asset (like a game level) has references to
//     * other assets, then these should be connected later, during scene initialization.
//     *
//     * @return true if successfully loaded the asset from a file
//     */
//    virtual bool load() override;
//    
//    /**
//     * Unloads this game level, releasing all sources
//     *
//     * This load method should NEVER access the AssetManager.  Assets are loaded and
//     * unloaded in parallel, not in sequence.  If an asset (like a game level) has
//     * references to other assets, then these should be disconnected earlier.
//     */
//    virtual void unload() override;
//    
//    /**
//     * Resets this game level, restoring all objects to their original position.
//     */
//    void reset();
//    
//    
//    //#pragma mark -
//    //#pragma mark Initializers
//    //CC_CONSTRUCTOR_ACCESS:
//    /**
//     * Creates a new, empty level.
//     */
//    LevelModel(void);
//    
//    /**
//     * Destroys this level, releasing all resources.
//     */
//    virtual ~LevelModel(void);
//};
//
//#endif /* defined(__TOYA_SCENE_MODEL_H__) */
