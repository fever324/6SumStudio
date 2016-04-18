#include "ToyaMenuModel.h"
#include "ui/UIButton.h"
#include <cornell.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <cornell/CUGenericLoader.h>

#define Level_BUTTON_NORMAL "textures/level.png"
#define Level_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define START_BUTTON_IMAGE "textures/start_normal.png"
#define REPLAY_BUTTON_IMAGE "textures/resetButton.png"
#define NEXT_BUTTON_IMAGE "textures/nextLevel.png"
#define GOMAIN_BUTTON_IMAGE "textures/back.png"
#define RESUME_BUTTON_IMAGE "textures/resumeButton.png"

#include "Constants.h"



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
        // default value for status
        _start = false;
        _gomain = false;
        _level = 0;
        
        LayerColor* bg = LayerColor::create(Color4B(175, 211, 102, 255));
        this->addChild(bg);
        
        createLevelButtons(4,scale,size);

        
    } else if (mtype == "welcome") {
        // welcome page, only has one button as start button
        // _gomain default as false
        _gomain = false;
        
        Button* start = createButton(START_BUTTON_IMAGE, scale, Vec2(size.x/2, 100));
        // set _gomain to true, so we can go to main menu
        start->addTouchEventListener(CC_CALLBACK_2(MenuModel::startButtonTouchEvent, this));
        
//        LayerColor* bg = LayerColor::create(Color4B(53, 53, 53, 255));
//        this->addChild(bg);

        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("menubg");
        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,1024));
        bg->setScale(1);
        bg->setAnchorPoint(Vec2(0,0.4));
        
        this->addChild(bg);
        
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
        Button* replay = createButton(REPLAY_BUTTON_IMAGE, scale, Vec2(2*size.x/8, 200));
        Button* gomain = createButton(GOMAIN_BUTTON_IMAGE, scale, Vec2(4*size.x/8, 200));
        Button* next = createButton(NEXT_BUTTON_IMAGE, scale, Vec2(6*size.x/8, 200));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        next->addTouchEventListener(CC_CALLBACK_2(MenuModel::nextButtonTouchEvent, this));
        
        Label* winnode = Label::create();
        _timenode = Label::create();
        _timenode->setPosition(size.x/2.0f,size.y-300);
        _timenode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        
        winnode->setString("VICTORY!");
        winnode->setColor(DEBUG_COLOR);
        winnode->setPosition(size.x/2.0f,size.y-200);
        
        winnode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        
        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("winbg");
        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,1024));
        bg->setScale(1);
        bg->setAnchorPoint(Vec2(0,0.4));
        this->addChild(bg);
        
        this->addChild(replay);
        this->addChild(gomain);
        this->addChild(next);
        this->addChild(winnode);
        this->addChild(_timenode);

    } else if (mtype == "levelFail") {
        /* create the fail page
         * two btns: replay, gomain
         * two labels
         * several sprites
         */
        _replay = false;
        _gomain = false;
        _next = false;
        Button* replay = createButton(REPLAY_BUTTON_IMAGE, scale, Vec2(4*size.x/8, 200));
        Button* gomain = createButton(GOMAIN_BUTTON_IMAGE, scale, Vec2(4*size.x/8+100, 200));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        
        Label* failnode = Label::create();
        _timenode = Label::create();
        _timenode->setPosition(size.x/2.0f,size.y-300);
        _timenode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        failnode->setColor(DEBUG_COLOR);
        
        failnode->setString("GAME OVER");
        failnode->setPosition(size.x/2.0f,size.y-200);
        failnode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        
        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("failbg");
        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,1024));
        bg->setScale(1);
        bg->setAnchorPoint(Vec2(0,0.4));
        this->addChild(bg);
        
        this->addChild(replay);
        this->addChild(gomain);
        this->addChild(failnode);
        this->addChild(_timenode);

    } else if (mtype == "pause") {
        /* create the pause page
         * two btns: replay, gomain
         * several sprites
         */
        _gomain = false;
        _next = false;
        _resume = false;
        Button* replay = createButton(REPLAY_BUTTON_IMAGE, scale, Vec2(2*size.x/8, 100));
        Button* resume = createButton(RESUME_BUTTON_IMAGE, scale, Vec2(4*size.x/8, 100));
        Button* gomain = createButton(GOMAIN_BUTTON_IMAGE, scale, Vec2(6*size.x/8, 100));
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        resume->addTouchEventListener(CC_CALLBACK_2(MenuModel::resumeButtonTouchEvent, this));
        this->addChild(replay);
        this->addChild(resume);
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
        
        Button* levelButton = createButton("textures/level"+std::to_string(i)+".png", scale, Vec2(pos.x/8+i*200, pos.y-100));
        
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
void MenuModel::resumeButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _resume = true;
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

