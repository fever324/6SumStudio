#ifndef ToyaAnimationBox_h
#define ToyaAnimationBox_h

#include <cocos2d.h>
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <cornell/CUAnimationNode.h>
#include "Constants.h"

#define FRAME_PER_STEP 13

using namespace std;
using namespace cocos2d;

class AnimationBoxModel : public BoxObstacle {
private:
protected:
    void animate();

    /* The total state count. */
    int _stateCount;
    /* Current state of the object. */
    int _currState;
    
    int _frameCount;
    int _rowCount;
    int _columnCount;
    int _updatePerFrame;
    std::string _textureKey;
    
    AnimationNode* _animationNode;
    int _cycle;
    
    bool isAnimating;
    
    
    
public:
    static AnimationBoxModel* create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int updatesPerFrame);
    virtual bool init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int updatesPerFrame);
    
    virtual void update(float dt) override;
    
    virtual void resetSceneNode() override;
    
    virtual void resetDebugNode() override;
    
    virtual void replaceAnimationTexture(int rowCount, int columnCount, std::string textureKey);
    
    virtual ~AnimationBoxModel(void);
    
    void setIsAnimating(bool animating) { isAnimating = animating; _cycle = 1;}
};


#endif /* ToyaAnimationBox_h */
