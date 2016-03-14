
#ifndef __TOYA_CREATION_SPELL_H__
#define __TOYA_CREATION_SPELL_H__

#include "ToyaSpellModel.h"

class ConstructionSpellModel : public SpellModel {
private:
    int _cost = 3;
public:
    ConstructionSpellModel() : SpellModel() {}
    void cast(BlockModel* block) override {}
    int getCost() override { return _cost; }
};

#endif /* CreationSpell_h */
