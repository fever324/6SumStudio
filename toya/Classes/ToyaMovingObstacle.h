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
    float _speed;
    bool _faceRight;
    
    float _angle;
    
    std::vector<Vec2> _routes; // A list of points that define the routes of this moving obstacle.
    int _nextPos; // The index of next turing point on the route.
    double _freezeTime; // The time of freezing
public:
    static MovingObstacleModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes, float speed, int faceRight);
    
    bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes, float speed, int faceRight);
    
    void update(float dt) override;
    
    void freeze(Node* parent, Node* parentDebugNode, WorldController* world);
};

#endif /* ToyaMovingObstacle_hpp */
