//
//  RemovableBlockModel.h
//  Can be cleared if we determine to use block factory
//
//
//  Created by 6Sum Studio on 2/27/16.
//
//

#ifndef __TOYA_REMOVABLEBLOCK_MODEL_H__
#define __TOYA_REMOVABLEBLOCK_MODEL_H__

#define STATE_GROWING         0
#define STATE_STABLE          1
#define STATE_DESTROYING      2

#define MAX_FRAME_COUNT       60

#define ANIMATION_SPEED       13

#include "ToyaAnimationBoxModel.h"
#include "cornell/CUAnimationNode.h"

using namespace cocos2d;

class RemovableBlockModel : public AnimationBoxModel {  // Block demo game controller line 420
private:
    Node* _parent;
    Node* _parentDebugNode;
    bool destroyed;
    WorldController* _world;
public:
    static RemovableBlockModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale);
    
    bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale);

    void update(float dt) override;
    
    void destroy(Node* parent, Node* parentDebugNode, WorldController* world);
};

#endif /** __TOYA_REMOVABLEBLOCK_MODEL_H__ **/