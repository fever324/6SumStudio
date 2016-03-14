#include "ToyaPanelModel.h"
#include "ToyaWorldModel.h"

#define NO_SPELL_SELECTED 0
#define CONSTRUCTION_SPELL_SELECTED 1
#define DESTRUCTION_SPELL_SELECTED 2

#define UNSELECTED_IMAGE "textures/1.png"
#define SELECTED_IMAGE "textures/goaldoor.png"

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
    
    _constructionSpell = new ConstructionSpellModel();
    _constructionSpellCB = ui::CheckBox::create(UNSELECTED_IMAGE,SELECTED_IMAGE,SELECTED_IMAGE,SELECTED_IMAGE,UNSELECTED_IMAGE);
    this->addChild(_constructionSpellCB);
    _constructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::constructionTouchEvent, this));
    
    _destructionSpell = new DestructionSpellModel();
    _destructionSpellCB = ui::CheckBox::create(UNSELECTED_IMAGE,SELECTED_IMAGE,SELECTED_IMAGE,SELECTED_IMAGE,UNSELECTED_IMAGE);
    this->addChild(_destructionSpellCB);
    _destructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::destructionTouchEvent, this));
    
    return true;
}

bool PanelModel::init(const Vec2& pos) {
    init();

    Vec2 actualPosition = Vec2(pos);
    actualPosition.x += (_constructionSpellCB->getContentSize().width + _destructionSpellCB->getContentSize().width) / 2.0f;
    actualPosition.y -= _constructionSpellCB->getContentSize().height / 2.0f;
    
    setPosition(actualPosition);

    _constructionSpellCB->setPosition(Vec2(-_constructionSpellCB->getContentSize().width/2.0f,0));
    _destructionSpellCB->setPosition(Vec2(_destructionSpellCB->getContentSize().width/2.0f,0));
    
    return true;

}

void PanelModel::constructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type) {
    switch(type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(_selection == DESTRUCTION_SPELL_SELECTED) {
                _destructionSpellCB->setSelected(false);
                _selection = CONSTRUCTION_SPELL_SELECTED;
            }
            else if(_selection == CONSTRUCTION_SPELL_SELECTED) {
                _selection = NO_SPELL_SELECTED;
            }
            else
                _selection = CONSTRUCTION_SPELL_SELECTED;
            break;
        default:
            break;
    }
}

void PanelModel::destructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type) {
    switch(type) {
        case ui::Widget::TouchEventType::BEGAN:
            if(_selection == CONSTRUCTION_SPELL_SELECTED) {
                _constructionSpellCB->setSelected(false);
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

SpellModel* PanelModel::getSpell() {
    if(_selection == NO_SPELL_SELECTED) return nullptr;
    if(_selection == CONSTRUCTION_SPELL_SELECTED) return _constructionSpell;
    return _destructionSpell;
}

bool PanelModel::deduceTotalMana(int cost) {
    if(cost > _totalMana) return false;
    _totalMana -= cost;
    return true;
}
