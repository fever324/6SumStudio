#ifndef TOYA_MAGIC_POTION_MODEL_H
#define TOYA_MAGIC_POTION_MODEL_H

#include <stdio.h>
#include "ToyaAnimationBoxModel.h"

using namespace std;
using namespace cocos2d;

class MagicPotionModel : public AnimationBoxModel {
protected:
    Node* _parent;
    Node* _parentDebugNode;
    WorldController* _world;
    
    int _points;
public:
    static MagicPotionModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int points);
    
    bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int points);
    
    void update(float dt) override;
    
    void pickUp(Node* parent, Node* parentDebugNode, WorldController* world);
};

#endif /* ToyaMagicPotionModel_h */
