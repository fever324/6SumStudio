#ifndef __TOYA_MENU_MODEL_H__
#define __TOYA_MENU_MODEL_H__

#include <iostream>
#include <cstdio>
#include <ctime>
#include "ui/CocosGUI.h"

#include "ToyaAudioController.h"
#include "ToyaProgressModel.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

#pragma mark -
#pragma mark Menu Model

/**
 * The game world.
 */

class MenuModel: public Node {
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(MenuModel);
    
    CheckBox* pauseButton; //pause button
    Button* startButton;
    Label* _timenode;
    
    
    // for level menu
    // Button* replay;
    // Button* goMain;
    // Button* next;
    
    // start status
    bool _start;
    // level
    int _level;
    // go next level or not
    bool _next;
    // go back or not
    bool _gomain;
    // replay
    bool _replay;
    
    bool _resume;
    // mute
    bool _mute;
    
    std::vector<int> _levelIndex;
    // store the button id with level index
    std::map<int, int> _levelMap;
    // store the button
    std::map<int, Button*> _levelButtonMap;
    // store the star sprite with number of stars we need to show
    std::map<int, Sprite*> _levelStarMap;
    
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
    MenuModel(): Node() {}
    
    
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
    virtual bool init() override { return init("main",Vec2::ZERO, Vec2::ONE); }
    virtual bool init(std::string mtype, const Vec2& size, const Vec2& scale);
    virtual bool init(std::string mtype, const Vec2& size);

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
    static MenuModel* create(std::string mtype);
    static MenuModel* create(std::string mtype, const Vec2& size);
    static MenuModel* create(std::string mtype, const Vec2& size, const Vec2& scale);
    
    void createLevelButtons(int count, const Vec2& pos);
    
    void createLevelStars(int count,const Vec2& scale, const Vec2& pos);
    
    Button* createButton(const std::string& texture, const Vec2& pos);
    
    CheckBox* createButton(const string& normal, const string& selected, const Vec2& scale, const Vec2& pos);
    
#pragma mark -
#pragma mark Destructors
    
#pragma mark -
#pragma mark Attribute Properties
    
#pragma mark -
#pragma mark Event Listeners
    void pauseButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void startButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void levelButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void replayButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void resumeButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void gomainButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void nextButtonTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    
    void muteButtonTouchEvent(Ref* sender, ui::Widget::TouchEventType type);
    
#pragma mark -
#pragma mark Helper Functions
    void reset();
    
    bool didStart(){return _start;}
    
    int getLevel(){return _level;}
    
    bool didReplay(){return _replay;}
    
    bool didResume(){return _resume;}
    
    bool didGoMain(){return _gomain;}
    
    bool didNext(){return _next;}
    
    bool isMute() { return _mute; }
    
    void setMute(bool flag) { _mute = flag; }
    
    void setGoMain(bool value){_gomain = value;}
    
    void setResume(bool value) {_resume = value;}
    
    void resetStatus(){_replay = false;_gomain = false; _next = false;_start = false; resetLevelButtons();}
    
    void showTime(double time, int stars){
        std::ostringstream s;
        s << "Time Cost  " << time << std::endl;
        _timenode->setString(s.str());
        CC_ASSERT(stars>=1 && stars <=3);
        // All stars are set to visible initially
        // Index 0 - 2 are the normal bright star
        // Index 3 - 4 are the dark grey star
        if (stars == 1){
            // index 3 and 4 are the grey stars in win menu
            _levelStarMap[0]->setVisible(true);
            _levelStarMap[1]->setVisible(false);
            _levelStarMap[2]->setVisible(false);
            _levelStarMap[3]->setVisible(true);
            _levelStarMap[4]->setVisible(true);
        } else if(stars ==2) {
            _levelStarMap[0]->setVisible(true);
            _levelStarMap[1]->setVisible(true);
            _levelStarMap[2]->setVisible(false);
            _levelStarMap[3]->setVisible(false);
            _levelStarMap[4]->setVisible(true);
        } else {
            _levelStarMap[0]->setVisible(true);
            _levelStarMap[1]->setVisible(true);
            _levelStarMap[2]->setVisible(true);
            _levelStarMap[3]->setVisible(false);
            _levelStarMap[4]->setVisible(false);
        }
    }
    
    void resetLevelButtons();
    
    
    void updateLevelButton(int level, std::string texture, std::string action){
        if (action == "update"){
            _levelButtonMap[level]->loadTextureNormal(texture);
        }else if (action == "unlock"){
            _levelButtonMap[level]->loadTextureNormal(texture);
            _levelButtonMap[level]->setEnabled(true);
        }
    }
    
};

#endif /* defined(__TOYA_MENU_MODEL_H__) */
