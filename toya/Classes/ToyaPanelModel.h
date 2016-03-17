#ifndef __TOYA_PANEL_MODEL_H__
#define __TOYA_PANEL_MODEL_H__

#include <iostream>
#include <cstdio>
#include <ctime>
#include "ui/CocosGUI.h"
#include "ToyaConstructionSpellModel.h"
#include "ToyaDestructionSpellModel.h"
#include <UICheckBox.h>

using namespace cocos2d;

#pragma mark -
#pragma mark Panel Model

#define NO_SPELL_SELECTED 0
#define CONSTRUCTION_SPELL_SELECTED 1
#define DESTRUCTION_SPELL_SELECTED 2

class PanelModel : public Node {
private:
    int _totalMana;
    int _selection;
    
    ui::CheckBox* _constructionSpellCB;
    ConstructionSpellModel* _constructionSpell;
    
    ui::CheckBox* _destructionSpellCB;
    DestructionSpellModel* _destructionSpell;
public:
    int getTotalMana() { return _totalMana; }
    bool deduceTotalMana(int cost);
    int getSpell();
    
    bool init();
    bool init(const Vec2& pos);
    
#pragma mark -
#pragma mark Static Constructors
    
    static PanelModel* create();
    static PanelModel* create(const Vec2& pos);
    
    void constructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void destructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
};

#endif
