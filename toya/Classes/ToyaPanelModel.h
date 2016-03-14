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
    SpellModel* getSpell();
    
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
