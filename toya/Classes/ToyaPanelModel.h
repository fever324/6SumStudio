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

// define the cooldown for magic releasing
#define MAGIC_COOLDOWN 40
#define NO_SPELL_SELECTED 0
#define FREEZING_SPELL_SELECTED 1
#define DESTRUCTION_SPELL_SELECTED 2


class PanelModel : public Node {
private:
    int _totalMana;
    int _currentMana;
    int _selection;
    
    int _magicCoolDown;
    
    ui::CheckBox* _freezingSpellCB;
    ui::CheckBox* _destructionSpellCB;
    
    FreezingSpellModel* _freezingSpell;
    DestructionSpellModel* _destructionSpell;
    
    AnimationNode* magicBar;
    
protected:
    void updateMagicBar();
    void updateButtons();

public:
    int getTotalMana() { return _totalMana; }
    int getCurrentMana() { return _currentMana; }
    
    bool deduceMana(int cost);
    void addMana(int mana);
    void setTotalMana(int mana){ _totalMana = mana; updateMagicBar(); updateButtons();}
    void setCurrentMana(int mana){ _currentMana = mana; updateMagicBar(); updateButtons();}
    
    ui::CheckBox* getFreezeSpellCB(){return _freezingSpellCB;}
    ui::CheckBox* getDestroySpellCB(){return _destructionSpellCB;}
    
    int getSpell();
    void setSpell(int i);
    
    bool init();
    bool init(const Vec2& pos, const int initalMana, const int totalMana);
    
    void reset();
    
    // reset the magic cooldown time to default
    void resetMagicCoolDown() {
        _magicCoolDown = MAGIC_COOLDOWN;
    }
    
    // get if it is in magic cooldown time or not
    bool isMagicCoolDown() {
        return _magicCoolDown == 0;
    }
    
    void update(float dt) {
        if (_magicCoolDown > 0) {
            _magicCoolDown --;
        }
    }
    
#pragma mark -
#pragma mark Static Constructors
    
    static PanelModel* create();
    static PanelModel* create(const Vec2& pos, const int initalMana, const int totalMana);
    
    void freezingTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void destructionTouchEvent(Ref *sender, ui::Widget::TouchEventType type);
    void disableButton(){
        _freezingSpellCB->setEnabled(false);
        _destructionSpellCB->setEnabled(false);
    };
    void enableButton(){
        updateButtons();
    };
};

#endif
