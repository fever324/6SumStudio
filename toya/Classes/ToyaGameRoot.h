//
//  ToyaGameRoot.h
//  toya
//  This is the root class for a single game scene.
//
//
//  Created by 6SumStudio on 2/27/16.
//
//

#ifndef __TOYA_GAME_ROOT_H__
#define __TOYA_GAME_ROOT_H__

#include "ToyaGameController.h"
#include "ToyaMenuModel.h"
#include <cocos2d.h>
#include <cornell.h>


using namespace cocos2d;


class ToyaRoot : public RootLayer {
    
protected:
    // CONTROLLERS
    /** The primary controller for the game world */
     GameController _gameplay;
    // init the game controller
    InputController _input;
    // menu
    MenuModel* _menu;
    
    Label* _loader;
    
    /** Whether or not we have finished preloading all assets */
    bool _preloaded;
    bool _initialized;
    
    // status variables
    bool _showMenu;
    bool _silentMode;
    int _playLevel;
    
    /**
     * Builds the scene graph for the loading screen.
     *
     * The loading screen is what we run while we are waiting for the asynchronous
     * loader for finish with the textures.  Right now, the loading screen is just
     * the word "Loading...".  We do this because the loading screen can only use
     * assets that have been loaded already, and the font is the only thing that
     * is guaranteed to be loaded at start.
     */
    void toggleLoader(bool showOrNot);
    
    /**
     * show menu node or not
     * 
    */
    void toggleMenu(bool showOrNot);
    
public:
    /**
     * Starts the layer, allocating initial resources
     *
     * This method is used to bootstrap the game.  It should start up an asset manager
     * and load initial assets.
     */
    void start() override;
    
    /**
     * Updates the game for a single animation frame
     *
     * This method is called every animation frame.  There is no draw() or render()
     * counterpoint to this method; drawing is done automatically in the scene graph.
     * However, this method is responsible for updating any transforms in the scene graph.
     *
     * @param  dt   the time in seconds since last update
     */
    void update(float dt) override;
    
    /**
     * Stops the layer, releasing all resources
     *
     * This method is used to clean-up any allocation that occurred in either start or
     * update.  While Cocos2d does have rudimentary garbage collection, you still have
     * to release any objects that you have retained.
     */
    void stop() override;
    
};

#endif /* defined(__TOYA_GAME_ROOT_H__) */