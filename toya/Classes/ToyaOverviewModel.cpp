#include "ToyaOverviewModel.h"


#define OVERVIEW_BUTTON_NORMAL "textures/pauseButton.png"
#define OVERVIEW_BUTTON_PRESSED "textures/pauseButton.png"
#define OVERVIEW_BUTTON_DISABLE "textures/pauseButton_disable.png"

#define RESET_BUTTON_IMAGE "textures/resetButton.png"
#define HELP_BUTTON_NORMAL "textures/helpButton.png"
#define HELP_BUTTON_DISABLE "textures/helpButton_disable.png"
using namespace cocos2d;



bool OverviewModel::init(const Vec2& pos) {
    return init(pos, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool OverviewModel::init(const Vec2& pos, const Vec2& scale){

    pauseButton = ui::Button::create(OVERVIEW_BUTTON_NORMAL,OVERVIEW_BUTTON_NORMAL,OVERVIEW_BUTTON_DISABLE);
    helpButton = ui::Button::create(HELP_BUTTON_NORMAL,HELP_BUTTON_NORMAL,HELP_BUTTON_DISABLE);
//    float cscale = Director::getInstance()->getContentScaleFactor();
//    pauseButton->setScale(cscale);
//    helpButton->setScale(cscale);
    
    
    Vec2 pauseButtonPosition = Vec2(pos);
    pauseButtonPosition.x -= pauseButton->getContentSize().width / 2.0f;
    pauseButtonPosition.y -= pauseButton->getContentSize().height / 2.0f;
    
    pauseButton->setPosition(pauseButtonPosition);
    helpButton->setPosition(Vec2(pauseButtonPosition.x-pauseButton->getContentSize().width,pauseButtonPosition.y));
    
    pauseButton->addTouchEventListener(CC_CALLBACK_2(OverviewModel::pauseButtonTouchEvent, this));
    helpButton->addTouchEventListener(CC_CALLBACK_2(OverviewModel::helpButtonTouchEvent, this));
    
    
    this->addChild(pauseButton);
    this->addChild(helpButton);
    
    paused = false;
    showhelp = false;
    
    startTime = current_time();
    currentPlayTime = 0;
    return true;
}

OverviewModel* OverviewModel::create(const Vec2& pos){
    OverviewModel* overview = new (std::nothrow) OverviewModel();
    
    if (overview && overview->init(pos)) {
        overview->autorelease();
        return overview;
    }
    CC_SAFE_DELETE(overview);
    return nullptr;
}

OverviewModel* OverviewModel::create(const Vec2& pos, const Vec2& scale) {
    OverviewModel* overview = new (std::nothrow) OverviewModel();
    
    if (overview && overview->init(pos, scale)) {
        overview->autorelease();
        return overview;
    }
    CC_SAFE_DELETE(overview);
    return nullptr;
}

void OverviewModel::pauseButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(!paused) {
                pauseButtonPressed();
            } else {
                resumeFromPause();
            }
            break;
        default:
            break;
    }
}


void OverviewModel::helpButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            showhelp = !showhelp;
            paused = !paused;
            break;
        default:
            break;
    }
}

double OverviewModel::getCurrentPlayTime() {
    return currentPlayTime + getCurrentDuration();
}

double OverviewModel::getCurrentDuration() {
    return elapsed_millis(startTime, current_time()) / 1000.0f;
}

void OverviewModel::enableButton(bool value) {
    pauseButton->setEnabled(value);
    helpButton->setEnabled(value);
    pauseButton->setTouchEnabled(value);
    helpButton->setTouchEnabled(value);
}

void OverviewModel::pauseButtonPressed() {
    currentPlayTime += getCurrentDuration();
    paused = true;
    gameController->setMap(true);
}

void OverviewModel::resumeFromPause() {
    startTime = current_time();
    paused = false;
    // only reset the map from pause
    if (!showhelp) {
        gameController->setMap(false);
    }
    showhelp = false;
}


void OverviewModel::reset() {
    currentPlayTime = 0;
    startTime = current_time();
    reseted = false;
    paused = false;
    pauseButton->setEnabled(true);
    helpButton->setEnabled(true);
    resumeFromPause();
}

