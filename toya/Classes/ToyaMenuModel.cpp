#include "ToyaMenuModel.h"


#define Level_BUTTON_NORMAL "textures/level.png"
#define Level_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define START_BUTTON_IMAGE "textures/start_normal.png"

using namespace cocos2d;



bool MenuModel::init(const Vec2& pos) {
    return init(pos, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool MenuModel::init(const Vec2& pos, const Vec2& scale){
    
//    pauseButton = ui::CheckBox::create(OVERVIEW_BUTTON_NORMAL, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_NORMAL);
    
    createLevelButtons(4,scale,pos);
    
    startButton = ui::Button::create(START_BUTTON_IMAGE);
    
    startButton->setScale(scale.x, scale.y);
    
    
    Vec2 startButtonPosition = Vec2(pos/2);
    startButtonPosition.x -= startButton->getContentSize().width / 2.0f;
    startButtonPosition.y -= startButton->getContentSize().height / 2.0f;
    
    startButton->setPosition(startButtonPosition);
    
    startButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::startButtonTouchEvent, this));
    
    
    this->addChild(startButton);
    
    
    // default value for status
    _start = false;
    _level = 0;
    
    return true;
}

MenuModel* MenuModel::create(const Vec2& pos){
    MenuModel* menu = new (std::nothrow) MenuModel();
    
    if (menu && menu->init(pos)) {
        menu->autorelease();
        return menu;
    }
    CC_SAFE_DELETE(menu);
    return nullptr;
}

void MenuModel::createLevelButtons(int count, const Vec2& scale,const Vec2& pos){
    int i = 0;
    for (; i < count; i++) {
        
        ui::Button* levelButton = ui::Button::create("textures/level"+std::to_string(i)+".png");
        
        levelButton->setScale(scale.x, scale.y);
        
        Vec2 lbPos = Vec2(pos.x/8+i*100, pos.y-100);
        lbPos.x -= levelButton->getContentSize().width / 2.0f;
        lbPos.y -= levelButton->getContentSize().height / 2.0f;
        
        levelButton->setPosition(lbPos);
        levelButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::levelButtonTouchEvent, this));
        _levelMap[levelButton->_ID] = i;
        this->addChild(levelButton);
    }
}

MenuModel* MenuModel::create(const Vec2& pos, const Vec2& scale) {
    MenuModel* menu = new (std::nothrow) MenuModel();
    
    if (menu && menu->init(pos, scale)) {
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
            break;
        default:
            break;
    }
}

void MenuModel::startButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _start = !_start;
            break;
        default:
            break;
    }
}


void MenuModel::reset() {

}

