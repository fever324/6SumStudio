#ifndef ToyaAnimationBox_h
#define ToyaAnimationBox_h

#include <cocos2d.h>
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <cornell/CUAnimationNode.h>
#include "Constants.h"

#define FRAME_PER_STEP 30

using namespace std;
using namespace cocos2d;

class AnimationBoxModel : public BoxObstacle {
private:
    void animate();
protected:
    /* The total state count. */
    int _stateCount;
    /* Current state of the object. */
    int _currState;
    
    int _frameCount;
    int _rowCount;
    int _columnCount;
    std::string _textureKey;
    
    AnimationNode* _animationNode;
    int _cycle;
    
    
public:
    static AnimationBoxModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size);
    virtual bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size);
    
    virtual void update(float dt) override;
    
    virtual void resetSceneNode() override;
    
    virtual ~AnimationBoxModel(void);
};


#endif /* ToyaAnimationBox_h */
