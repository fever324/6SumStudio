#include "ToyaMenuModel.h"
#include "ui/UIButton.h"

#define Level_BUTTON_NORMAL "textures/level.png"
#define Level_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define START_BUTTON_IMAGE "textures/start_normal.png"

using namespace cocos2d;
using namespace ui;



bool MenuModel::init(std::string mtype,const Vec2& size) {
    return init(mtype, size, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool MenuModel::init(std::string mtype, const Vec2& size, const Vec2& scale){
    
    // main menu
    if (mtype == "main") {
        // level selector page
        createLevelButtons(4,scale,size);
        // default value for status
        _start = false;
        _gomain = false;
        _level = 0;
    } else if (mtype == "welcome") {
        // welcome page, only has one button as start button
        // _gomain default as false
        _gomain = false;
        
        Button* start = createButton(START_BUTTON_IMAGE, scale, Vec2(size.x/8, 100));
        // set _gomain to true, so we can go to main menu
        start->addTouchEventListener(CC_CALLBACK_2(MenuModel::startButtonTouchEvent, this));

        this->addChild(start);
    } else if (mtype == "levelWin") {
        /* create the win page
         * three btns: replay, gomain, next
         * two labels
         * several sprites
         */
        _replay = false;
        _gomain = false;
        _next = false;
        Button* replay = createButton(START_BUTTON_IMAGE, scale, Vec2(2*size.x/8, 100));
        Button* gomain = createButton(START_BUTTON_IMAGE, scale, Vec2(4*size.x/8, 100));
        Button* next = createButton(START_BUTTON_IMAGE, scale, Vec2(6*size.x/8, 100));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        next->addTouchEventListener(CC_CALLBACK_2(MenuModel::nextButtonTouchEvent, this));
        this->addChild(replay);
        this->addChild(gomain);
        this->addChild(next);
    } else if (mtype == "levelFail") {
        /* create the fail page
         * two btns: replay, gomain
         * two labels
         * several sprites
         */
        _replay = false;
        _gomain = false;
        _next = false;
        Button* replay = createButton(START_BUTTON_IMAGE, scale, Vec2(2*size.x/8, 100));
        Button* gomain = createButton(START_BUTTON_IMAGE, scale, Vec2(6*size.x/8, 100));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        this->addChild(replay);
        this->addChild(gomain);

    } else if (mtype == "pause") {
        /* create the pause page
         * two btns: replay, gomain
         * several sprites
         */
        _gomain = false;
        _next = false;
        Button* replay = createButton(START_BUTTON_IMAGE, scale, Vec2(2*size.x/8, 100));
        Button* gomain = createButton(START_BUTTON_IMAGE, scale, Vec2(6*size.x/8, 100));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        this->addChild(replay);
        this->addChild(gomain);

    }
    
    return true;
}

MenuModel* MenuModel::create(std::string mtype,const Vec2& size){
    MenuModel* menu = new (std::nothrow) MenuModel();
    
    if (menu && menu->init(mtype, size)) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE(menu);
    return nullptr;
}

void MenuModel::createLevelButtons(int count, const Vec2& scale,const Vec2& pos){
    int i = 0;
    for (; i < count; i++) {
        
        Button* levelButton = createButton("textures/level"+std::to_string(i)+".png", scale, Vec2(pos.x/8+i*100, pos.y-100));
        
        levelButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::levelButtonTouchEvent, this));
        
        _levelMap[levelButton->_ID] = i;
        
        this->addChild(levelButton);
    }
}


Button* MenuModel::createButton(const std::string &texture,const Vec2& scale,const Vec2& pos){
    Button* button = Button::create(texture);
    button->setScale(scale.x, scale.y);
    Vec2 bPos = Vec2(pos.x, pos.y);
    bPos.x -= button->getContentSize().width / 2.0f;
    bPos.y -= button->getContentSize().height / 2.0f;
    button->setPosition(bPos);
    return button;
}


MenuModel* MenuModel::create(std::string mtype, const Vec2& size, const Vec2& scale) {
    MenuModel* menu = new (std::nothrow) MenuModel();
    
    if (menu && menu->init(mtype, size, scale)) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE(menu);
    return nullptr;
}

void MenuModel::pauseButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            break;
        default:
            break;
    }
}

void MenuModel::levelButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _level = _levelMap[sender->_ID];
            // select the level and start immediately
            _start = true;
            break;
        default:
            break;
    }
}

void MenuModel::startButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _gomain = true;
            break;
        default:
            break;
    }
}

void MenuModel::replayButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _replay = true;
            break;
        default:
            break;
    }
}
void MenuModel::gomainButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _gomain = true;
            break;
        default:
            break;
    }
}
void MenuModel::nextButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _next = true;
            break;
        default:
            break;
    }
}



void MenuModel::reset() {

}

