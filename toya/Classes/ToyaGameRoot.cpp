//
//  ToyaGameRoot.cpp
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#include "ToyaGameRoot.h"
#include "ui/CocosGUI.h"

// This is not part of cornell.h and should come last
#include <cornell/CUGenericLoader.h>

using namespace cocos2d;


#pragma mark -
#pragma mark Loading Screen

#define DEFAULT_FONT_SIZE   64.0f
#define LOADING_FONT_NAME   "felt"
#define LOADING_MESSAGE     "Loading..."
#define START_BUTTON_IMAGE "textures/resetButton.png"


#pragma mark -
#pragma mark Gameplay Control

/**
 * Starts the layer, allocating initial resources
 *
 * This method is used to bootstrap the game.  It should start up an asset manager
 * and load initial assets.
 */
void ToyaRoot::start() {
    int scene = AssetManager::getInstance()->createScene();
    
    FontLoader* fonts = FontLoader::create();
    fonts->setDefaultSize(DEFAULT_FONT_SIZE);
    AssetManager::getInstance()->at(scene)->attach<TTFont>(fonts);
    AssetManager::getInstance()->at(scene)->attach<Texture2D>(TextureLoader::create());
    AssetManager::getInstance()->at(scene)->attach<Sound>(SoundLoader::create());
    
//    GenericLoader<LevelModel>* levels = GenericLoader<LevelModel>::create();
//    AssetManager::getInstance()->at(scene)->attach<LevelModel>(levels);
    
    AssetManager::getInstance()->startScene(scene);
    
    AssetManager::getInstance()->getCurrent()->load<TTFont>(LOADING_FONT_NAME, "fonts/MarkerFelt.ttf");
    
    Size size = getContentSize();
    Vec2 center(size.width/2.0f,size.height/2.0f);

    _loader = Label::create();
    _loader->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(LOADING_FONT_NAME)->getTTF());
    _loader->setAnchorPoint(Vec2(0.5f,0.5f));
    _loader->setPosition(center);
    _loader->setString(LOADING_MESSAGE);

    // Create a "loading" screen
    _preloaded = false;
    toggleLoader(true);
    
    RootLayer::start(); // YOU MUST END with call to parent
    
    // initial input
    _input.init();
    _input.start();
    
    // initial the Welcome and Main menu
    Vec2 inputscale = Vec2(this->getScaleX(),this->getScaleY());
    _welcome = MenuModel::create("welcome",Vec2(this->getContentSize().width,this->getContentSize().height), inputscale);
    _menu = MenuModel::create("main",Vec2(this->getContentSize().width,this->getContentSize().height), inputscale);
    toggleWelcome(false);
    toggleMenu(false);
    
    // add to rootlayer
    this->addChild(_welcome);
    this->addChild(_menu,3);
    this->addChild(_loader);
    
    
    // intial the status
    _showMenu = true;
    _silentMode = false;
    _newStart = true;
    
    // should get from progressController
    _playLevel = 2;
    
}

/**
 * Stops the layer, releasing all resources
 *
 * This method is used to clean-up any allocation that occurred in either start or
 * update.  While Cocos2d does have rudimentary garbage collection, you still have
 * to release any objects that you have retained.
 */
void ToyaRoot::stop() {
    RootLayer::stop();  // YOU MUST BEGIN with call to parent
    int scene = AssetManager::getInstance()->getCurrentIndex();
    
    // YOU MUST STOP THE SOUNDS FOR THIS SCENE BEFORE UNLOADING THEM
    SoundEngine::getInstance()->stopAll();
    AssetManager::getInstance()->stopScene(scene);
}

/**
 * Updates the game for a single animation frame
 *
 * This method is called every animation frame.  There is no draw() or render()
 * counterpoint to this method; drawing is done automatically in the scene graph.
 * However, this method is responsible for updating any transforms in the scene graph.
 *
 * @param  dt   the time in seconds since last update
 */
void ToyaRoot::update(float deltaTime) {
    RootLayer::update(deltaTime);  // YOU MUST BEGIN with call to parent
    bool complete = true;
    complete = complete && AssetManager::getInstance()->getCurrent()->isComplete();
    /*
     Before level interface:
     1. Menu Page
        a. how many buttons we have
     2. 
     
     
     
     Status:
     1. showMenu: indicate when I need to show the menu page, from gameController.
     2. silentMode: indicate whether sounds open or not, pass to gameController.
     3. playLevel: default is getting from progressController, indicate which level we are, pass to gameController.
     
     // if we have continue game setting
     4. continueOrNot: indicate whether the continue button should be visible or not, getting from progressController.
    */
    
    if (_preloaded && complete && !_gameplay.isActive() && _newStart) {
        // initial status
        // after preload all assets and game haven't started
        // show the welcome
        toggleLoader(false);
        toggleWelcome(true);
    }
    
    if (!_showMenu && !_gameplay.isActive() && complete && _preloaded) {
        // hide the menu after entering the game
        // removeAllChildren();
        CCLOG("hehe");
        _gameplay.init(this,&_input,_playLevel);
    }
    
    if (_showMenu && !_gameplay.isActive() && !_newStart){
        toggleMenu(true);
    }
    
    // update the game status if we are in game mode
    if (_gameplay.isActive()) {
        toggleMenu(false);
        _gameplay.update(deltaTime);
        if (_gameplay.didGoMain()){
            // stop the _gameplay
            _gameplay.clear();
            _showMenu = true;
            _menu->resetStatus();
        }
    }
    
    // preload all resources
    if (!_preloaded) {
        _gameplay.preload();
        _preloaded = true;
    }
    
    // show main menu
    if (_welcome->didGoMain()){
        toggleWelcome(false);
        toggleMenu(true);
        _newStart = false;
        _welcome->setGoMain(false);
    }
    
    
    // if use keyboard
    if (_input.didStart() || _menu->didStart()) {
        _showMenu = false;
        _playLevel = _menu->getLevel();
        _menu->resetStatus();
    }
    
    _input.update(deltaTime);
}