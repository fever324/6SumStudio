#include "ToyaMenuModel.h"
#include "ui/UIButton.h"
#include <cornell.h>
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include <cornell/CUGenericLoader.h>
#include <SimpleAudioEngine.h>
#include "ToyaProgressModel.h"

#define Level_BUTTON_NORMAL "textures/level.png"
#define Level_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define START_BUTTON_IMAGE "textures/start_normal.png"
#define REPLAY_BUTTON_IMAGE "textures/resetButton.png"
#define NEXT_BUTTON_IMAGE "textures/nextLevel.png"
#define GOMAIN_BUTTON_IMAGE "textures/back.png"
#define RESUME_BUTTON_IMAGE "textures/resumeButton.png"

#define MUTE_BUTTON_NORMAL "textures/mute-sound.png"
#define MUTE_BUTTON_SELECTED "textures/sound.png"

#include "Constants.h"

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
        
        
        auto image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("select_level_background");
        auto select = AssetManager::getInstance()->getCurrent()->get<Texture2D>("select_level");
        
        Sprite* bg = Sprite::createWithTexture(image);
        Sprite* words= Sprite::createWithTexture(select);
        float cscale = Director::getInstance()->getContentScaleFactor();
        bg->setScale(cscale);
        words->setScale(cscale);
        
        bg->setAnchorPoint(Vec2(0,0));
        words->setAnchorPoint(Vec2(0,1));
        
        words->setPosition(size.x / 2 - words->getContentSize().width/2, size.y-25);
        this->addChild(bg,0);
        this->addChild(words,1);
        
        createLevelButtons(10,size);

        
    } else if (mtype == "welcome") {
        // welcome page, only has one button as start button
        // _gomain default as false
        _gomain = false;

        Button* start = createButton("textures/menubg.png", Vec2(1024, 576));
    
        // set _gomain to true, so we can go to main menu
        start->addTouchEventListener(CC_CALLBACK_2(MenuModel::startButtonTouchEvent, this));
        
        this->addChild(start);
        
    } else if (mtype == "tutorial") {
        // welcome page, only has one button as start button
        // _gomain default as false
        _resume = false;
        
        LayerColor* bgColor = LayerColor::create(Color4B(0, 0, 0, 100));
        this->addChild(bgColor);
//
//        auto image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("select_level_background");
//        Sprite* bg = Sprite::createWithTexture(image);
//        float cscale = Director::getInstance()->getContentScaleFactor();
//        bg->setScale(cscale*0.8);
////        bg->ignoreAnchorPointForPosition(true);
//        bg->setAnchorPoint(Vec2(-0.1*cscale,-0.1*cscale));
//        this->addChild(bg,0);
        
        Button* resume = createButton("textures/help.png", Vec2(1024,576));
        resume->setScale(0.8);
        
        // set _gomain to true, so we can go to main menu
        resume->addTouchEventListener(CC_CALLBACK_2(MenuModel::resumeButtonTouchEvent, this));
        
        this->addChild(resume);
    } else if (mtype == "levelWin") {
        /* create the win page
         * three btns: replay, gomain, next
         * two labels
         * several sprites
         */
        _replay = false;
        _gomain = false;
        _next = false;
        
        LayerColor* bgColor = LayerColor::create(Color4B(0, 0, 0, 100));
        this->addChild(bgColor);
        
        Button* replay = createButton(REPLAY_BUTTON_IMAGE, Vec2(4*size.x/8-50, 200));
        Button* gomain = createButton(GOMAIN_BUTTON_IMAGE, Vec2(4*size.x/8+50, 200));
        Button* next = createButton(NEXT_BUTTON_IMAGE, Vec2(4*size.x/8+150, 200));
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
        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,576));
        bg->setScale(0.5);
        bg->setPosition(Vec2(size.x/2.0f,size.y/2.0f));
        bg->ignoreAnchorPointForPosition(true);
        bg->setAnchorPoint(Vec2(-0.5,-0.5));
        
        this->addChild(bg);
        
        
        createLevelStars(6,scale,size);
        
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
        
        LayerColor* bgColor = LayerColor::create(Color4B(0, 0, 0, 100));
        this->addChild(bgColor);
        
        
        Button* replay = Button::create(REPLAY_BUTTON_IMAGE);
        Button* gomain = Button::create(GOMAIN_BUTTON_IMAGE);
        
        replay->setAnchorPoint(Vec2(0.5,0.5));
        gomain->setAnchorPoint(Vec2(0.5,0.5));
        
        replay->setPosition(Vec2(415, 150));
        gomain->setPosition(Vec2(605, 150));
        
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        
        _timenode = Label::create();
        _timenode->setPosition(size.x/2.0f,size.y-300);
        _timenode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        _timenode->setScale(0.5);
        
        Label* failnode = Label::create();
        failnode->setColor(DEBUG_COLOR);
        failnode->setString("GAME OVER");
        failnode->setPosition(size.x/2.0f,size.y-200);
        failnode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        failnode->setScale(0.5);
        
        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("failbg");
        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,576));
        bg->setScale(0.5);
        bg->setPosition(Vec2(size.x/2.0f,size.y/2.0f));
        bg->ignoreAnchorPointForPosition(true);
        bg->setAnchorPoint(Vec2(-0.5,-0.5));
        
        this->addChild(bg);
        
        
//        createLevelStars(6,scale,size);
        
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
        _mute = false;
        _replay = false;


        Button* replay = Button::create(REPLAY_BUTTON_IMAGE);
        Button* resume = Button::create(RESUME_BUTTON_IMAGE);
        Button* gomain = Button::create(GOMAIN_BUTTON_IMAGE);
        CheckBox* mute = CheckBox::create(MUTE_BUTTON_NORMAL, MUTE_BUTTON_SELECTED);
        
        replay->setAnchorPoint(Vec2(0.5,0.5));
        resume->setAnchorPoint(Vec2(0.5,0.5));
        gomain->setAnchorPoint(Vec2(0.5,0.5));
        mute->setAnchorPoint(Vec2(0.5,0.5));
        
        replay->setPosition(Vec2(225, 100));
        resume->setPosition(Vec2(415, 100));
        gomain->setPosition(Vec2(605, 100));
        mute->setPosition(Vec2(795, 100));

        
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        resume->addTouchEventListener(CC_CALLBACK_2(MenuModel::resumeButtonTouchEvent, this));
        mute->addTouchEventListener(CC_CALLBACK_2(MenuModel::muteButtonTouchEvent,this));
        
        this->addChild(replay);
        this->addChild(resume);
        this->addChild(gomain);
        this->addChild(mute);

//        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("failbg");
//        Sprite* bg = Sprite::createWithTexture(image,Rect(0,0,1024,576));
//        LayerColor* bg = LayerColor::create(Color4B(0, 0, 0, 255));
//        bg->setScale(0.5);
//        bg->setPosition(Vec2(size.x/2.0f,size.y/2.0f));
//        bg->ignoreAnchorPointForPosition(true);
//        bg->setAnchorPoint(Vec2(-0.5,-0.5));
//
//        this->addChild(bg);
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

void MenuModel::createLevelStars(int count, const Vec2& scale,const Vec2& screenSize){
    // create the stars after you finish the level successful
    int i = 0;
    for (; i < count; i++) {
        Texture2D* starIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>("dirt");
        Sprite* star = Sprite::createWithTexture(starIMG,Rect(0,0,64,64));
        star->setScale(1);
        star->setPosition(Vec2(screenSize.x/2.0f+i*80,screenSize.y/2.0f));
        star->ignoreAnchorPointForPosition(true);
        star->setAnchorPoint(Vec2(0,0));
        star->setVisible(false);
        _levelStarMap[i] = star;
        this->addChild(star);
    }
}


void MenuModel::createLevelButtons(int count,const Vec2& size){
    // create the level buttons for level selectors
    int i = 0;
    auto pm = ProgressModel::getInstance();

    for (; i < count; i++) {
        Vec2 buttonPos = Vec2(size.x/8+60+i%4*size.x/4.0f, size.y-80- i/4*170);
        Button* levelButton = nullptr;
        
        if(!pm->isLocked(i)){
            levelButton = createButton("textures/level"+std::to_string(i)+".png", buttonPos);
            levelButton->addTouchEventListener(CC_CALLBACK_2(MenuModel::levelButtonTouchEvent, this));
            
            int stars = pm->getStars(i);
            Texture2D* starIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>(std::to_string(stars)+"star");
            Sprite* star = Sprite::createWithTexture(starIMG);
            star->setAnchorPoint(Vec2(0,1));
            
            levelButton->addChild(star);
        } else {
            levelButton = createButton("textures/lockedLevel.png", buttonPos);
        }
        
        
        _levelMap[levelButton->_ID] = i;
        _levelButtonMap[i] = levelButton;
        this->addChild(levelButton,1);
    }
}


Button* MenuModel::createButton(const std::string &texture,const Vec2& pos){
    // create a single button with texture and positionon
    Button* button = Button::create(texture);
    float cscale = Director::getInstance()->getContentScaleFactor();
    Vec2 bPos = Vec2(pos.x/cscale, pos.y/cscale);
    button->setScale(cscale*1.01);
    bPos.x -= (button->getContentSize().width / 2.0f)/cscale;
    bPos.y -= (button->getContentSize().height / 2.0f)/cscale;
    button->setPosition(bPos);
    return button;
}

// The create method for mute button
CheckBox* MenuModel::createButton(const string& normal, const string& selected, const Vec2& scale, const Vec2& pos) {
    CheckBox* button = CheckBox::create(normal, selected);
    float cscale = Director::getInstance()->getContentScaleFactor();
    Vec2 bPos = Vec2(pos.x/cscale, pos.y/cscale);
    button->setScale(cscale*1.01);
    bPos.x -= (button->getContentSize().width / 2.0f)/cscale;
    bPos.y -= (button->getContentSize().height / 2.0f)/cscale;
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

void MenuModel::muteButtonTouchEvent(cocos2d::Ref* sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            _mute = !_mute;
            if(_mute) {
                AudioController::audioPauseAll();
            } else {
                AudioController::audioResumeAll();
            }
            break;
        default:
            break;
    }
}

void MenuModel::resetLevelButtons() {
    
    auto pm = ProgressModel::getInstance();
    
    //Remove current level buttons on screen
    for (auto const& x : _levelButtonMap) {
        
        int i = x.first;
        Button* button = x.second;
        
        if(!pm->isLocked(i)) {
            button->loadTextureNormal("textures/level"+std::to_string(i)+".png");
            button->addTouchEventListener(CC_CALLBACK_2(MenuModel::levelButtonTouchEvent, this));
            button->removeAllChildren();
            int stars = pm->getStars(i);
            Texture2D* starIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>(std::to_string(stars)+"star");
            Sprite* star = Sprite::createWithTexture(starIMG);
            star->setAnchorPoint(Vec2(0,1));
            button->addChild(star);

        }
    }
}

void MenuModel::reset() {

}

