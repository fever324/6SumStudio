#ifndef ToyaMovingObstacle_hpp
#define ToyaMovingObstacle_hpp

#include <stdio.h>
#include <vector>

#include "ToyaAnimationBoxModel.h"
#include "cornell/CUAnimationNode.h"

using namespace cocos2d;
using namespace std;

class MovingObstacleModel : public AnimationBoxModel {  // Block demo game controller line 420
private:
    Node* _parent;
    Node* _parentDebugNode;
    WorldController* _world;
    
    float _movement;
    bool _faceRight;
    
    std::vector<Vec2> routes;
public:
    static MovingObstacleModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes);
    
    bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes);
    
    void update(float dt) override;
    
    void freeze(Node* parent, Node* parentDebugNode, WorldController* world);
};

#endif /* ToyaMovingObstacle_hpp */
