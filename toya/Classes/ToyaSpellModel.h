
#ifndef __TOYA_SPELL_MODEL_H__
#define __TOYA_SPELL_MODEL_H__

#include "ToyaAnimationBoxModel.h"

class SpellModel {
public:
    SpellModel() {}
    virtual void cast(AnimationBoxModel* block) = 0;
    virtual int getCost() = 0;
};

#endif /* defined(__TOYA_SPELL_MODEL_H__) */
