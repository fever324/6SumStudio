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
    float cscale = Director::getInstance()->getContentScaleFactor();

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
        
        Button* replay = Button::create(REPLAY_BUTTON_IMAGE);
        Button* gomain = Button::create(GOMAIN_BUTTON_IMAGE);
        Button* next = Button::create(NEXT_BUTTON_IMAGE);

        Vec2 mid = Vec2(0.5, 0.5);
        replay->setAnchorPoint(mid);
        gomain->setAnchorPoint(mid);
        next->setAnchorPoint(mid);
        
        replay->setPosition(Vec2(341.33, 100));
        gomain->setPosition(Vec2(516, 100));
        next->setPosition(Vec2(682, 100));
        
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        next->addTouchEventListener(CC_CALLBACK_2(MenuModel::nextButtonTouchEvent, this));
        
        Label* winnode = Label::create();
        
        _timenode = Label::create();
        _timenode->setPosition(size.x/2.0f,size.y-300);
        _timenode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        _timenode->setScale(0.7);
        
        
        _collectionNode = Label::create();
        _collectionNode->setPosition(size.x/2.0f - 70, size.y-300-50);
        _collectionNode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        _collectionNode->setString("Honey: ");
        _collectionNode->setScale(0.7);
        
        
        winnode->setString("GOOD JOB");
        // winnode->setColor(DEBUG_COLOR);
        winnode->setPosition(size.x/2.0f,size.y-100);
        winnode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
        
        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("winbg");
        Sprite* bg = Sprite::createWithTexture(image);
        bg->setScale(cscale);
        bg->setAnchorPoint(Vec2{0.5f, 0.5f});
        bg->setPosition(Vec2(size.x/2, size.y/2));
        
        this->addChild(bg);
        
        
        createWinMenuStars(scale,size);
        createHoney(_collectionNode);
        
        this->addChild(replay);
        this->addChild(gomain);
        this->addChild(next);
        this->addChild(winnode);
        this->addChild(_timenode);
        this->addChild(_collectionNode);

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
        _timenode->setScale(0.7);
        
        Label* failnode = Label::create();
        failnode->setColor(DEBUG_COLOR);
        failnode->setString("GAME OVER");
        failnode->setPosition(size.x/2.0f,size.y-200);
        failnode->setTTFConfig(AssetManager::getInstance()->getCurrent()->get<TTFont>(PRIMARY_FONT)->getTTF());
//        failnode->setScale(0.5);
        
        Texture2D* image = AssetManager::getInstance()->getCurrent()->get<Texture2D>("failbg");
        Sprite* bg = Sprite::createWithTexture(image);
        
        Texture2D* deadBearIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>("die_bear");
        Sprite* bearDeath = Sprite::createWithTexture(deadBearIMG);
        bearDeath->setScale(2);
        bearDeath->setPosition(size.x/2, size.y/2);
        FiniteTimeAction* up = MoveTo::create(1, Vec2(size.x/2, size.y/2+20));
        FiniteTimeAction* down = MoveTo::create(1, Vec2(size.x/2, size.y/2-20));
        auto repeat = Sequence::create(up, down, NULL);
        bearDeath->runAction(RepeatForever::create(repeat));

        bg->setScale(cscale);
        bg->setAnchorPoint(Vec2{0.5f, 0.5f});
        bg->setPosition(Vec2(size.x/2, size.y/2));
        


        
        this->addChild(bg);
        this->addChild(replay);
        this->addChild(gomain);
        this->addChild(failnode);
        this->addChild(_timenode);
        this->addChild(bearDeath);

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
        
        replay->setPosition(Vec2(225, 60));
        resume->setPosition(Vec2(415, 60));
        gomain->setPosition(Vec2(605, 60));
        mute->setPosition(Vec2(795, 60));

        
        replay->addTouchEventListener(CC_CALLBACK_2(MenuModel::replayButtonTouchEvent, this));
        gomain->addTouchEventListener(CC_CALLBACK_2(MenuModel::gomainButtonTouchEvent, this));
        resume->addTouchEventListener(CC_CALLBACK_2(MenuModel::resumeButtonTouchEvent, this));
        mute->addTouchEventListener(CC_CALLBACK_2(MenuModel::muteButtonTouchEvent,this));
        
        this->addChild(replay);
        this->addChild(resume);
        this->addChild(gomain);
        this->addChild(mute);
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

void MenuModel::createWinMenuStars(const Vec2& scale,const Vec2& screenSize){
    
    float cscale = Director::getInstance()->getContentScaleFactor();

    // create the stars after you finish the level successful
    Texture2D* starIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>("star");
    Texture2D* noStarIMG = AssetManager::getInstance()->getCurrent()->get<Texture2D>("greystar");
    Sprite* star1 = Sprite::createWithTexture(starIMG);
    Sprite* star2 = Sprite::createWithTexture(starIMG);
    Sprite* star3 = Sprite::createWithTexture(starIMG);
    Sprite* noStar2 = Sprite::createWithTexture(noStarIMG);
    Sprite* noStar3 = Sprite::createWithTexture(noStarIMG);
    
    star1->setAnchorPoint(Vec2(0.5,0.5));
    star2->setAnchorPoint(Vec2(0.5,0.5));
    star3->setAnchorPoint(Vec2(0.5,0.5));
    noStar2->setAnchorPoint(Vec2(0.5,0.5));
    noStar3->setAnchorPoint(Vec2(0.5,0.5));
    
    star1->setScale(cscale * 0.8);
    star2->setScale(cscale * 0.8);
    star3->setScale(cscale * 0.8);
    noStar2->setScale(cscale * 0.8);
    noStar3->setScale(cscale * 0.8);
    
    star1->setPosition(Vec2(268,338));
    star2->setPosition(Vec2(512,392));
    star3->setPosition(Vec2(756,338));
    noStar2->setPosition(Vec2(512,392));
    noStar3->setPosition(Vec2(756,338));
    
    _levelStarMap[0] = star1;
    _levelStarMap[1] = star2;
    _levelStarMap[2] = star3;
    _levelStarMap[3] = noStar2;
    _levelStarMap[4] = noStar3;
    
    star1->setVisible(true);
    star2->setVisible(true);
    star3->setVisible(true);
    noStar2->setVisible(true);
    noStar3->setVisible(true);
    
    this->addChild(star1);
    this->addChild(star2);
    this->addChild(star3);
    this->addChild(noStar2);
    this->addChild(noStar3);
  
}


void MenuModel::createLevelButtons(int count,const Vec2& size){
    // create the level buttons for level selectors
    int i = 0;
    auto pm = ProgressModel::getInstance();

    for (; i < count; i++) {
        Vec2 buttonPos = Vec2(size.x/8+60+i%4*size.x/4.0f, size.y-80- i/4*170);
        Button* levelButton = nullptr;
        
        if(pm->isLocked(i)){
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

void MenuModel::createHoney(Node* node) {
    
    float cscale = Director::getInstance()->getContentScaleFactor();

    Texture2D* honey = AssetManager::getInstance()->getCurrent()->get<Texture2D>("honey");
    Texture2D* greyHoney = AssetManager::getInstance()->getCurrent()->get<Texture2D>("grey_honey");
    
    Sprite* honey1 = Sprite::createWithTexture(honey);
    Sprite* honey2 = Sprite::createWithTexture(honey);
    Sprite* honey3 = Sprite::createWithTexture(honey);
    Sprite* grey1 = Sprite::createWithTexture(greyHoney);
    Sprite* grey2 = Sprite::createWithTexture(greyHoney);
    Sprite* grey3 = Sprite::createWithTexture(greyHoney);
    
    Vec2 leftMid = Vec2(0,0.5);
    honey1->setAnchorPoint(leftMid);
    honey2->setAnchorPoint(leftMid);
    honey3->setAnchorPoint(leftMid);
    grey1->setAnchorPoint(leftMid);
    grey2->setAnchorPoint(leftMid);
    grey3->setAnchorPoint(leftMid);
    
    honey1->setScale(cscale);
    honey2->setScale(cscale);
    honey3->setScale(cscale);
    grey1->setScale(cscale);
    grey2->setScale(cscale);
    grey3->setScale(cscale);
    
    Vec2 leftNodeSize = node->getContentSize();
    float y = node->getPosition().y;
    float width = honey1->getContentSize().width;
    float x = node->getPosition().x + node->getContentSize().width/2 -30;
    
    honey1->setPosition(Vec2(x,y));
    honey2->setPosition(Vec2(x+width,y));
    honey3->setPosition(Vec2(x+width*2,y));
    grey1->setPosition(Vec2(x,y));
    grey2->setPosition(Vec2(x+width,y));
    grey3->setPosition(Vec2(x+width*2,y));
    
    _levelStarMap[5] = honey1;
    _levelStarMap[6] = honey2;
    _levelStarMap[7] = honey3;
    _levelStarMap[8] = grey1;
    _levelStarMap[9] = grey2;
    _levelStarMap[10] = grey3;
    
    addChild(honey1);
    addChild(honey2);
    addChild(honey3);
    addChild(grey1);
    addChild(grey2);
    addChild(grey3);

}

void MenuModel::showTime(double time, double expectedTime, int stars, int nCollected) {
    std::ostringstream s;
    s << "Time:  " << time << "/" <<expectedTime <<std::endl;
    _timenode->setString(s.str());
    CC_ASSERT(stars>=1 && stars <=3);
    // Index 0 - 2 are the normal bright star
    // Index 3 - 4 are the dark grey star
    // Index 5 - 7 are the bright honey
    // Index 8 - 10 are the grey honey
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
    
    CC_ASSERT(nCollected >= 0 && nCollected <= 3);
    
    if (nCollected == 0) {
        _levelStarMap[5]->setVisible(false);
        _levelStarMap[6]->setVisible(false);
        _levelStarMap[7]->setVisible(false);
        _levelStarMap[8]->setVisible(true);
        _levelStarMap[9]->setVisible(true);
        _levelStarMap[10]->setVisible(true);

    } else if (nCollected == 1) {
        
        _levelStarMap[5]->setVisible(true);
        _levelStarMap[6]->setVisible(false);
        _levelStarMap[7]->setVisible(false);
        _levelStarMap[8]->setVisible(false);
        _levelStarMap[9]->setVisible(true);
        _levelStarMap[10]->setVisible(true);
    } else if (nCollected == 2) {
        
        _levelStarMap[5]->setVisible(true);
        _levelStarMap[6]->setVisible(true);
        _levelStarMap[7]->setVisible(false);
        _levelStarMap[8]->setVisible(false);
        _levelStarMap[9]->setVisible(false);
        _levelStarMap[10]->setVisible(true);
    }else if (nCollected == 3) {
        
        _levelStarMap[5]->setVisible(true);
        _levelStarMap[6]->setVisible(true);
        _levelStarMap[7]->setVisible(true);
        _levelStarMap[8]->setVisible(false);
        _levelStarMap[9]->setVisible(false);
        _levelStarMap[10]->setVisible(false);
    }
}

void MenuModel::reset() {

}

