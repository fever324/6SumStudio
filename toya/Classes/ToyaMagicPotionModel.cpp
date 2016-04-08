#include "ToyaMagicPotionModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>
#include "Constants.h"

#define NORMAL_STATE 0
#define PICKING_STATE 1

MagicPotionModel* MagicPotionModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int points) {
    MagicPotionModel* magicPotion = new (std::nothrow) MagicPotionModel();
    if (magicPotion && magicPotion->init(stateCount, rowCount, columnCount, textureKey, pos, size, scale, points)) {
        magicPotion->autorelease();
        return magicPotion;
    }
    CC_SAFE_DELETE(magicPotion);
    return nullptr;
}

bool MagicPotionModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int points) {
    if(AnimationBoxModel::init(stateCount, rowCount, columnCount, textureKey, pos, size, scale)) {
        setName(REMOVABLE_OBJECT_NAME);
        setBodyType(b2_staticBody);
        setDrawScale(scale);
        _points = points;
        
        WireNode* draw = WireNode::create();
        draw->setColor(Color3B::RED);
        draw->setOpacity(193);
        setDebugNode(draw);
        return true;
    }
    
    return false;
}

void MagicPotionModel::pickUp(Node* parent, Node* parentDebugNode, WorldController* world) {
    _currState = PICKING_STATE;
    _parent = parent;
    _parentDebugNode = parentDebugNode;
    _frameCount = 0;
    _world = world;
    _animationNode->setFrame(2);
    pickedUp = true;
}

void MagicPotionModel::update(float dt) {
    AnimationBoxModel::update(dt);
    if(pickedUp) {
        getBody()->SetActive(false);
    }
    if(_currState == PICKING_STATE) {
        _parent->removeChild(getSceneNode());
        _parentDebugNode->removeChild(getDebugNode());
        
        _world->removeObstacle(this);
        
        _parent = nullptr;
        _parentDebugNode = nullptr;
        _world = nullptr;
    }
}