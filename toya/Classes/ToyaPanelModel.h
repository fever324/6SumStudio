#ifndef __TOYA_PANEL_MODEL_H__
#define __TOYA_PANEL_MODEL_H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "ui/CocosGUI.h"
#include "ToyaFreezingSpellModel.h"
#include "ToyaDestructionSpellModel.h"
#include <ui/UICheckBox.h>

using namespace cocos2d;

#pragma mark -
#pragma mark Panel Model

#define NO_SPELL_SELECTED 0
#define FREEZING_SPELL_SELECTED 1
#define DESTRUCTION_SPELL_SELECTED 2


class PanelModel : public Node {
private:
    int _totalMana;
    int _currentMana;
    int _selection;
    
    ui::CheckBox* _freezingSpellCB;
    FreezingSpellModel* _freezingSpell;
    
    ui::CheckBox* _destructionSpellCB;
    DestructionSpellModel* _destructionSpell;
    
    Label* manaLabel;
    
protected:
    void updateLabelText();
    void updateButtons();

public:
    int getTotalMana() { return _totalMana; }
    int getCurrentMana() { return _currentMana; }
    
    bool deduceMana(int cost);
    void addMana(int mana);
    void setTotalMana(int mana){ _totalMana = mana; updateLabelText(); updateButtons();}
    void setCurrentMana(int mana){ _currentMana = mana; updateLabelText(); updateButtons();}
    
    int getSpell();
    void setSpell(int i);
    
    bool init();
    bool init(const Vec2& pos, const int totalMana);
    
    void reset();
    
#pragma mark -
#pragma mark Static Constructors
    
    static PanelModel* create();
    static PanelModel* create(const Vec2& pos, const int totalMana);
    
    void freezingTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void destructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
};

#endif
