#include "ToyaPanelModel.h"
#include "ToyaWorldModel.h"

#define CREATE_MAGIC_UNSELECTED_IMAGE "textures/create_magic.png"
#define CREATE_MAGIC_SELECTED_IMAGE "textures/create_magic_selected.png"
#define CREATE_MAGIC_DISABLED_IMAGE "textures/create_magic_disabled.png"

#define CREATE_MAGIC_ICON            "textures/create_magic_icon.png"
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

    _constructionSpell = new ConstructionSpellModel();
    _destructionSpell = new DestructionSpellModel();

    _constructionSpellCB = ui::CheckBox::create(CREATE_MAGIC_UNSELECTED_IMAGE, CREATE_MAGIC_SELECTED_IMAGE, CREATE_MAGIC_SELECTED_IMAGE, CREATE_MAGIC_DISABLED_IMAGE, CREATE_MAGIC_DISABLED_IMAGE);
    _destructionSpellCB = ui::CheckBox::create(DESTROY_MAGIC_UNSELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_SELECTED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE,DESTROY_MAGIC_DISABLED_IMAGE);

    _constructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::constructionTouchEvent, this));
    _destructionSpellCB->addTouchEventListener(CC_CALLBACK_2(PanelModel::destructionTouchEvent, this));
    
    this->addChild(_constructionSpellCB);
    this->addChild(_destructionSpellCB);
    
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

int PanelModel::getSpell() {
    return _selection;
}
void PanelModel::setSpell(int i){
    if(i == 0) {
        _constructionSpellCB->setSelected(false);
        _destructionSpellCB->setSelected(false);
    }
    _selection = i;
}

bool PanelModel::deduceTotalMana(int cost) {
    if(cost > _totalMana) return false;
    _totalMana -= cost;
    return true;
}
