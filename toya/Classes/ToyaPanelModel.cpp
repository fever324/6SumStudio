#include "ToyaPanelModel.h"
#include "ToyaWorldModel.h"

#define FREEZE_MAGIC_UNSELECTED_IMAGE "textures/freeze_magic_icon.png"
#define FREEZE_MAGIC_SELECTED_IMAGE "textures/freeze_magic_selected.png"
#define FREEZE_MAGIC_DISABLED_IMAGE "textures/freeze_magic_disabled.png"

#define FREEZE_MAGIC_ICON            "textures/create_magic_icon.png"
#define BUTTON_BACKGROUND            "textures/button_background.png"
#define BUTTON_SELECTED_BACKGROUND   "textures/button_selected_background.png"


#define DESTROY_MAGIC_UNSELECTED_IMAGE "textures/destroy_magic_icon.png"
#define DESTROY_MAGIC_SELECTED_IMAGE "textures/destroy_magic_selected.png"
#define DESTROY_MAGIC_DISABLED_IMAGE "textures/destroy_magic_disabled.png"

using namespace cocos2d;
using namespace std;

PanelModel* PanelModel::create() {
    PanelModel* panel = new (std::nothrow) PanelModel();
    if (panel && panel->init()) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

PanelModel* PanelModel::create(const Vec2& pos, const int totalMana) {
    PanelModel* panel = new (std::nothrow) PanelModel();
    if (panel && panel->init(pos, totalMana)) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

bool PanelModel::init() {
    _selection = NO_SPELL_SELECTED;
    
    auto windowSize = Director::getInstance()->getWinSize();

    _freezingSpell = new FreezingSpellModel();
    _destructionSpell = new DestructionSpellModel();
    if(_freezingSpellCB == nullptr){
        _freezingSpellCB = ui::CheckBox::create(FREEZE_MAGIC_UNSELECTED_IMAGE, FREEZE_MAGIC_SELECTED_IMAGE, FREEZE_MAGIC_SELECTED_IMAGE, FREEZE_MAGIC_DISABLED_IMAGE, FREEZE_MAGIC_DISABLED_IMAGE);
        _freezingSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::freezingTouchEvent, this));
        this->addChild(_freezingSpellCB);

    }
    if(_destructionSpellCB == nullptr) {
        _destructionSpellCB = ui::CheckBox::create(DESTROY_MAGIC_UNSELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE);
        _destructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::destructionTouchEvent, this));
        this->addChild(_destructionSpellCB);

    }
    
    if(manaLabel == nullptr) {
        manaLabel = Label::create();
        manaLabel->setColor(Color3B::RED);
        manaLabel->setVisible(true);
        this->addChild(manaLabel);
    }
    
    return true;
}

bool PanelModel::init(const Vec2& pos, const int totalMana) {
    init();
    
    _totalMana = totalMana;
    _currentMana = totalMana;
    
    Vec2 actualPosition = Vec2(pos);
    actualPosition.x += (_freezingSpellCB->getContentSize().width + _destructionSpellCB->getContentSize().width) / 2.0f;
    actualPosition.y -= _freezingSpellCB->getContentSize().height / 2.0f;

    setPosition(actualPosition);
    float x= -_freezingSpellCB->getContentSize().width/2.0f;
    _freezingSpellCB->setPosition(Vec2(x,0));
    _destructionSpellCB->setPosition(Vec2(_destructionSpellCB->getContentSize().width/2.0f,0));
    
    x = actualPosition.x+_destructionSpellCB->getContentSize().width+manaLabel->getContentSize().width/3.0f;
    
    manaLabel->setPosition(x,0);
    manaLabel->setSystemFontSize(30.0f);
    updateLabelText();
    updateButtons();
    
    
    return true;

}

void PanelModel::freezingTouchEvent(Ref *sender, ui::Widget::TouchEventType type) {
    switch(type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(_selection == DESTRUCTION_SPELL_SELECTED) {
                _destructionSpellCB->setSelected(false);
                _selection = FREEZING_SPELL_SELECTED;
            }
            else if(_selection == FREEZING_SPELL_SELECTED) {
                _selection = NO_SPELL_SELECTED;
            }
            else
                _selection = FREEZING_SPELL_SELECTED;
            break;
        default:
            break;
    }
}

void PanelModel::destructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type) {
    switch(type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(_selection == FREEZING_SPELL_SELECTED) {
                _freezingSpellCB->setSelected(false);
                _selection = DESTRUCTION_SPELL_SELECTED;
            }
            else if(_selection == DESTRUCTION_SPELL_SELECTED) {
                _selection = NO_SPELL_SELECTED;
            }
            else
                _selection = DESTRUCTION_SPELL_SELECTED;
            break;
        default:
            break;
    }
}

int PanelModel::getSpell() {
    return _selection;
}
void PanelModel::setSpell(int i){
    if(i == 0) {
        _freezingSpellCB->setSelected(false);
        _destructionSpellCB->setSelected(false);
    }
    _selection = i;
}
void PanelModel::addMana(int mana) {
    int result = mana + _currentMana;
    _currentMana =  result > _totalMana ?  _totalMana : result;
    
    updateLabelText();
    updateButtons();
}

bool PanelModel::deduceMana(int cost) {
    if(cost > _totalMana) return false;
    _currentMana -= cost;
    updateLabelText();
    updateButtons();

    return true;
}


void PanelModel::updateLabelText() {
    if(manaLabel != nullptr) {
        manaLabel->setString(std::to_string(_currentMana)+"/"+std::to_string(_totalMana));
        manaLabel->updateContent();
    }
}

void PanelModel::updateButtons() {
    _destructionSpellCB->setEnabled(_currentMana >= DESTRUCTION_COST);
    _freezingSpellCB->setEnabled(_currentMana >= FREEZE_COST);
}

void PanelModel::reset(){
    manaLabel->setVisible(false);
    _currentMana = _totalMana;
}

