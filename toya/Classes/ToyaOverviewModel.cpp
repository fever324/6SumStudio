#include "ToyaOverviewModel.h"


#define BUTTON_NORMAL "textures/overviewButton.png"
#define BUTTON_PRESSED "textures/overviewResumeButton.png"
using namespace cocos2d;



bool OverviewModel::init(const Vec2& pos) {
    return init(pos, Vec2::ZERO);
}

/*
 *  Input position is the upper right corner of the screen.
 *  Actual position defines where the center of the pause button should be
 */
bool OverviewModel::init(const Vec2& pos, const Vec2& scale){

    pauseButton = ui::CheckBox::create(BUTTON_NORMAL, BUTTON_PRESSED, BUTTON_PRESSED, BUTTON_PRESSED, BUTTON_NORMAL);

    
    pauseButton->setScale(scale.x, scale.y);
    
    
    Vec2 actualPosition = Vec2(pos);
    actualPosition.x -= pauseButton->getContentSize().width / 2.0f;
    actualPosition.y -= pauseButton->getContentSize().height / 2.0f;
    
    pauseButton->setPosition(actualPosition);
    pauseButton->addTouchEventListener(CC_CALLBACK_2(OverviewModel::touchEvent, this));
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

void OverviewModel::touchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {
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

double OverviewModel::getCurrentPlayTime() {
    return currentPlayTime + getCurrentDuration();
}

double OverviewModel::getCurrentDuration() {
    return elapsed_millis(startTime, current_time()) / 1000.0f;
}



void OverviewModel::pauseButtonPressed() {
    currentPlayTime += getCurrentDuration();
    Director::getInstance()->pause();     // Stop the world
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
}

