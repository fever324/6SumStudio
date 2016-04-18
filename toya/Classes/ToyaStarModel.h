#ifndef TOYA_STAR_MODEL_H
#define TOYA_STAR_MODEL_H

#include <stdio.h>
#include "ToyaAnimationBoxModel.h"
#define ANIMATIONSPEED  13

using namespace std;
using namespace cocos2d;

class StarModel : public AnimationBoxModel {
protected:
    Node* _parent;
    Node* _parentDebugNode;
    WorldController* _world;
    bool pickedUp;
public:
    static StarModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale);
    
    bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale);
    
    void update(float dt) override;
    
    void pickUp(Node* parent, Node* parentDebugNode, WorldController* world);
};

#endif /* ToyaStarModel_hpp */
