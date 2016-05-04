#ifndef __TOYA_OVERVIEW_MODEL_H__
#define __TOYA_OVERVIEW_MODEL_H__

#include <iostream>
#include <cstdio>
#include <ctime>
#include "ui/CocosGUI.h"
#include "ToyaGameController.h"
#include <Cornell/CUTimestamp.h>


using namespace cocos2d;

class GameController;

#pragma mark -
#pragma mark Overview Model

/**
 * The game world.
 */

class OverviewModel : public Node{
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(OverviewModel);
    
    
    ui::Button* pauseButton; //pause button
    ui::Button* helpButton; //help button
    bool showhelp;
    bool paused;
    bool reseted;

    double currentPlayTime;
    timestamp_t startTime;
    timestamp_t endTime;
    
    GameController* gameController;
    
    double getCurrentDuration();
    
    
protected:


    
public:
    
CC_CONSTRUCTOR_ACCESS:
#pragma mark Hidden Constructors
    /**
     * Creates a degenerate Avatar object.
     *
     * This constructor does not initialize any of the dude values beyond
     * the defaults.  To use a DudeModel, you must call init().
     */
    OverviewModel() : Node(){ }

    
#pragma mark -
#pragma mark Initializers
    /**
     * Initializes a overview panel.
     *
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(Vec2::ZERO, Vec2::ONE); }
    virtual bool init(const Vec2& pos, const Vec2& scale);
    virtual bool init(const Vec2& pos);



    
#pragma mark -
#pragma mark Static Constructors

    
    
    /**
     * Creates a new overview panel with the given position and size.
     *
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  size     The dimensions of the box.
     *
     * @return  An autoreleased physics object
     */
    static OverviewModel* create();
    static OverviewModel* create(const Vec2& pos);
    static OverviewModel* create(const Vec2& pos, const Vec2& scale);
    
#pragma mark -
#pragma mark Destructors

#pragma mark -
#pragma mark Attribute Properties
    double getCurrentPlayTime();
    
    void setEndTime(timestamp_t t){ endTime = t;}
    void setGameController(GameController* gc) { gameController = gc;}
    ui::Button* getPauseButton() {return pauseButton;}

    
#pragma mark - 
#pragma mark Event Listeners
    void pauseButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void helpButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
#pragma mark -
#pragma mark Helper Functions
    void pauseButtonPressed();
    void resumeFromPause();
    void enableButton(bool value);
    void enableAllButton(bool value);
    
    bool hasReseted() {return this->reseted;}
    void reset();
    bool didPause(){ return paused;}
    bool didHelp(){ return showhelp;}
    
};

#endif /* defined(__TOYA_OVERVIEW_MODEL_H__) */

