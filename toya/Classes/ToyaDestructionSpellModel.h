
#ifndef __TOYA_DESTRUCTION_SPELL_MODEL_H__
#define __TOYA_DESTRUCTION_SPELL_MODEL_H__

#include "ToyaSpellModel.h"

class DestructionSpellModel : public SpellModel {
private:
    int _cost = 2;
public:
    DestructionSpellModel() : SpellModel() {}
    
    void cast(BlockModel* block) override {}
    int getCost() override { return _cost; }
};

#endif /* ToyaDestructionSpellModel_h */
