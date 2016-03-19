#include "ToyaOverviewModel.h"


#define OVERVIEW_BUTTON_NORMAL "textures/overviewButton.png"
#define OVERVIEW_BUTTON_PRESSED "textures/overviewResumeButton.png"

#define RESET_BUTTON_IMAGE "textures/resetButton.png"

using namespace cocos2d;



bool OverviewModel::init(const Vec2& pos) {
    return init(pos, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool OverviewModel::init(const Vec2& pos, const Vec2& scale){

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
    
    pauseButton->addTouchEventListener(CC_CALLBACK_2(OverviewModel::pauseButtonTouchEvent, this));
    resetButton->addTouchEventListener(CC_CALLBACK_2(OverviewModel::resetButtonTouchEvent, this));
    
    
    this->addChild(resetButton);
    this->addChild(pauseButton);
    
    paused = false;
    
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


void OverviewModel::resetButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            reseted = true;
            if(paused) {
                resumeFromPause();
                pauseButton->setSelected(false);

            }
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



void OverviewModel::pauseButtonPressed() {
    currentPlayTime += getCurrentDuration();
    Director::getInstance()->pause();     // Stop the world
//    pauseButton->setSelected(true);

    paused = true;
    gameController->setDebug(true);

}

void OverviewModel::resumeFromPause() {
    startTime = current_time();
    Director::getInstance()->resume();
    paused = false;
    gameController->setDebug(false);
}


void OverviewModel::reset() {
    currentPlayTime = 0;
    startTime = current_time();
    reseted = false;
    paused = false;
}

