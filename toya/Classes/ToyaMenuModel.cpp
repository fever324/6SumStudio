#include "ToyaMenuModel.h"


#define OVERVIEW_BUTTON_NORMAL "textures/overviewButton.png"
#define OVERVIEW_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define RESET_BUTTON_IMAGE "textures/resetButton.png"

using namespace cocos2d;



bool MenuModel::init(const Vec2& pos) {
    return init(pos, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool MenuModel::init(const Vec2& pos, const Vec2& scale){
    
    pauseButton = ui::CheckBox::create(OVERVIEW_BUTTON_NORMAL, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_PRESSED, OVERVIEW_BUTTON_NORMAL);
    
    resetButton = ui::Button::create(RESET_BUTTON_IMAGE);
    
    
    
    
    pauseButton->setScale(scale.x, scale.y);
    resetButton->setScale(scale.x, scale.y);
    
    
    Vec2 pauseButtonPosition = Vec2(pos);
    pauseButtonPosition.x -= pauseButton->getContentSize().width / 2.0f;
    pauseButtonPosition.y -= pauseButton->getContentSize().height / 2.0f;
    
    Vec2 resetButtonPosition = Vec2(pauseButtonPosition);
    resetButtonPosition.x -= pauseButton->getContentSize().width / 2.0f+ resetButton->getContentSize().width / 2.0f;
    
    pauseButton->setPosition(pauseButtonPosition);
    resetButton->setPosition(resetButtonPosition);
    
    pauseButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::pauseButtonTouchEvent, this));
    resetButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::resetButtonTouchEvent, this));
    
    
    this->addChild(resetButton);
    this->addChild(pauseButton);
    
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


void MenuModel::resetButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            break;
        default:
            break;
    }
}


void MenuModel::reset() {

}

