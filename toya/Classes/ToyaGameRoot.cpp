//
//  ToyaGameRoot.cpp
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#include "ToyaGameRoot.h"

using namespace cocos2d;


#pragma mark -
#pragma mark Loading Screen

#define DEFAULT_FONT_SIZE   64.0f
#define LOADING_FONT_NAME   "felt"
#define LOADING_MESSAGE     "Loading..."


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
    AssetManager::getInstance()->startScene(scene);
    
    // Create a "loading" screen
    _preloaded = false;
    displayLoader();
    RootLayer::start(); // YOU MUST END with call to parent
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
    if (_preloaded && !_gameplay.isActive() && complete) {
        // Transfer control to the gameplay subcontroller
        removeAllChildren();
        _gameplay.init(this);
    } else if (_gameplay.isActive()) {
        _gameplay.update(deltaTime);
    } else if (!_preloaded) {
        _preloaded = true;
        _gameplay.preload();
    }
}


#pragma mark -
#pragma mark Internal Helpers

/**
 * Builds the scene graph for the loading screen.
 *
 * The loading screen is what we run while we are waiting for the asynchronous
 * loader for finish with the textures.  Right now, the loading screen is just
 * the word "Loading...".  We do this because the loading screen can only use
 * assets that have been loaded already, and the font is the only thing that
 * is guaranteed to be loaded at start.
 */
void ToyaRoot::displayLoader() {
    // Load the font NOW
    AssetManager::getInstance()->getCurrent()->load<TTFont>(LOADING_FONT_NAME, "fonts/MarkerFelt.ttf");
    
    Size size = getContentSize();
    Vec2 center(size.width/2.0f,size.height/2.0f);
    
    // Create the message label.
    auto label = Label::create();
    label->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(LOADING_FONT_NAME)->getTTF());
    label->setAnchorPoint(Vec2(0.5f,0.5f));
    label->setPosition(center);
    label->setString(LOADING_MESSAGE);
    
    // Add the label as a child to this layer
    addChild(label, 1);
}