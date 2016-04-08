
#ifndef __TOYA_FREEZING_SPELL_H__
#define __TOYA_FREEZING_SPELL_H__

#include "ToyaSpellModel.h"

class FreezingSpellModel : public SpellModel {
private:
    int _cost = 3;
public:
    FreezingSpellModel() : SpellModel() {}
    void cast(AnimationBoxModel* block) override {}
    int getCost() override { return _cost; }
};

#endif /* FreezingSpell_h */
