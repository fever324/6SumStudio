#ifndef __TOYA_MENU_MODEL_H__
#define __TOYA_MENU_MODEL_H__

#include <iostream>
#include <cstdio>
#include <ctime>
#include "ui/CocosGUI.h"


using namespace cocos2d;


#pragma mark -
#pragma mark Menu Model

/**
 * The game world.
 */

class MenuModel : public Node{
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(MenuModel);
    
    
    ui::CheckBox* pauseButton; //pause button
    ui::Button* resetButton;
    bool paused;
    bool reseted;
    
    
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
    MenuModel() : Node(){ }
    
    
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
    static MenuModel* create();
    static MenuModel* create(const Vec2& pos);
    static MenuModel* create(const Vec2& pos, const Vec2& scale);
    
#pragma mark -
#pragma mark Destructors
    
#pragma mark -
#pragma mark Attribute Properties
    
#pragma mark -
#pragma mark Event Listeners
    void pauseButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void resetButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
#pragma mark -
#pragma mark Helper Functions
    void reset();
};

#endif /* defined(__TOYA_MENU_MODEL_H__) */
