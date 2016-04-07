#include "ToyaPanelModel.h"
#include "ToyaWorldModel.h"

#define FREEZE_MAGIC_UNSELECTED_IMAGE "textures/create_magic.png"
#define FREEZE_MAGIC_SELECTED_IMAGE "textures/create_magic_selected.png"
#define FREEZE_MAGIC_DISABLED_IMAGE "textures/create_magic_disabled.png"

#define FREEZE_MAGIC_ICON            "textures/create_magic_icon.png"
#define BUTTON_BACKGROUND            "textures/button_background.png"
#define BUTTON_SELECTED_BACKGROUND   "textures/button_selected_background.png"


#define DESTROY_MAGIC_UNSELECTED_IMAGE "textures/destroy_magic.png"
#define DESTROY_MAGIC_SELECTED_IMAGE "textures/destroy_magic_selected.png"
#define DESTROY_MAGIC_DISABLED_IMAGE "textures/destroy_magic_disabled.png"

using namespace cocos2d;

PanelModel* PanelModel::create() {
    PanelModel* panel = new (std::nothrow) PanelModel();
    if (panel && panel->init()) {
        panel->autorelease();
        return panel;
    }
    CC_SAFE_DELETE(panel);
    return nullptr;
}

PanelModel* PanelModel::create(const Vec2& pos) {
    PanelModel* panel = new (std::nothrow) PanelModel();
    if (panel && panel->init(pos)) {
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

    _freezingSpellCB = ui::CheckBox::create(FREEZE_MAGIC_UNSELECTED_IMAGE, FREEZE_MAGIC_SELECTED_IMAGE, FREEZE_MAGIC_SELECTED_IMAGE, FREEZE_MAGIC_DISABLED_IMAGE, FREEZE_MAGIC_DISABLED_IMAGE);
    _destructionSpellCB = ui::CheckBox::create(DESTROY_MAGIC_UNSELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE);

    _freezingSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::freezingTouchEvent, this));
    _destructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::destructionTouchEvent, this));
    
    this->addChild(_freezingSpellCB);
    this->addChild(_destructionSpellCB);
    
    return true;
}

bool PanelModel::init(const Vec2& pos) {
    init();

    Vec2 actualPosition = Vec2(pos);
    actualPosition.x += (_freezingSpellCB->getContentSize().width + _destructionSpellCB->getContentSize().width) / 2.0f;
    actualPosition.y -= _freezingSpellCB->getContentSize().height / 2.0f;
    
    setPosition(actualPosition);

    _freezingSpellCB->setPosition(Vec2(-_freezingSpellCB->getContentSize().width/2.0f,0));
    _destructionSpellCB->setPosition(Vec2(_destructionSpellCB->getContentSize().width/2.0f,0));
    
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

bool PanelModel::deduceTotalMana(int cost) {
    if(cost > _totalMana) return false;
    _totalMana -= cost;
    return true;
}
