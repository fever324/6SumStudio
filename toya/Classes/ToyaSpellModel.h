
#ifndef __TOYA_SPELL_MODEL_H__
#define __TOYA_SPELL_MODEL_H__

#include "ToyaBlockModel.h"

class SpellModel {
public:
    SpellModel() {}
    virtual void cast(BlockModel* block) = 0;
    virtual int getCost() = 0;
};

#endif /* defined(__TOYA_SPELL_MODEL_H__) */
