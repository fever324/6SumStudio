#include "ToyaAnimationBoxModel.h"

AnimationBoxModel* AnimationBoxModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size) {
    AnimationBoxModel* animationBox = new (std::nothrow) AnimationBoxModel();
    if (animationBox && animationBox->init(stateCount, rowCount, columnCount, textureKey, pos, size)) {
        animationBox->autorelease();
        return animationBox;
    }
    CC_SAFE_DELETE(animationBox);
    return nullptr;
}

bool AnimationBoxModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size) {
    
    if(BoxObstacle::init(pos, size)) {
        
        _stateCount = stateCount;
        _currState = 0;
        _frameCount = 0;
        _rowCount = rowCount;
        _columnCount = columnCount;
        _textureKey = textureKey;
        
        PolygonNode* pnode = PolygonNode::create(Rect(0, 0, size.width, size.height));
        _debug->setpoly
        setSceneNode(pnode);
        
        return true;
    }
    
    return false;
}

void AnimationBoxModel::animate() {
    if(_animationNode->getFrame() == 0) {
        _cycle = 1;
    } else if (_animationNode->getFrame() == _columnCount-1) {
        _cycle = -1;
    }
    
    int base = _columnCount * _currState;
    
    if(++_frameCount % FRAME_PER_STEP == 0) {
        _animationNode->setFrame(base + (_animationNode->getFrame()+1) % _columnCount);
        
        if(_frameCount == FRAME_PER_STEP * _columnCount) {
            _frameCount = 0;
        }
    }
}

void AnimationBoxModel::update(float dt) {
    BoxObstacle::update(dt);
    animate();
}

void AnimationBoxModel::resetSceneNode() {
    float cscale = Director::getInstance()->getContentScaleFactor();
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    
    if(pnode != nullptr) {
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        
        Rect bounds;
        bounds.size = getDimension();
        bounds.size.width  *= _drawScale.x/cscale;
        bounds.size.height *= _drawScale.y/cscale;
        
        Texture2D* image = assets->get<Texture2D>(_textureKey);
        
        _animationNode = AnimationNode::create(image, _rowCount, _columnCount, _rowCount*_columnCount);
        
        pnode->addChild(_animationNode);
        _animationNode->setPosition(pnode->getContentSize().width/2.0f,pnode->getContentSize().height/2.0f);
        
        BoxObstacle::resetDebugNode();
    }
}

AnimationBoxModel::~AnimationBoxModel(void) {
    delete[] _animationNode;
    _animationNode = nullptr;
}