#include "ToyaAnimationBoxModel.h"

AnimationBoxModel* AnimationBoxModel::create(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int updatesPerFrame) {
    AnimationBoxModel* animationBox = new (std::nothrow) AnimationBoxModel();
    if (animationBox && animationBox->init(stateCount, rowCount, columnCount, textureKey, pos, size, scale, updatesPerFrame)) {
        animationBox->autorelease();
        return animationBox;
    }
    CC_SAFE_DELETE(animationBox);
    return nullptr;
}

bool AnimationBoxModel::init(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, int updatesPerFrame) {

    if(BoxObstacle::init(pos, size)) {
        
        _updatePerFrame = updatesPerFrame;
        _stateCount = stateCount;
        _currState = 0;
        _frameCount = 0;
        _rowCount = rowCount;
        _columnCount = columnCount;
        _textureKey = textureKey;
        
        
        // use scale, 1x1 in game means scale.x * scale.y in design
        PolygonNode* pnode = PolygonNode::create(Rect(0, 0, scale.x, scale.y));
        // here set the scale
        // TODO: change the scale to imageSize / textureSize
        
        float cscale = Director::getInstance()->getContentScaleFactor();

        pnode->setScale(0.5*cscale);
        
        setSceneNode(pnode);

        // use cscale to fix the gap between blocks
        setDrawScale(scale);
        
        WireNode* draw = WireNode::create();
        
        draw->setColor(Color3B::YELLOW);
        
        draw->setOpacity(193);
        
        setDebugNode(draw);
        
        return true;
    }
    
    return false;
}

void AnimationBoxModel::animate() {
    if(!isAnimating) {
        return;
    }
    if(_animationNode->getFrame() == 0) {
        _cycle = 1;
    } else if (_animationNode->getFrame() == _columnCount-1) {
        _cycle = -1;
    }
    
    int base = _columnCount * _currState;
    
    if(++_frameCount % _updatePerFrame == 0) {
        _animationNode->setFrame(base + (_animationNode->getFrame()+_cycle) % _columnCount);
        
        if(_frameCount == _updatePerFrame * _columnCount + 1) {
            _frameCount = 0;
        }
    }
}

void AnimationBoxModel::update(float dt) {
    BoxObstacle::update(dt);
    animate();
}

void AnimationBoxModel::resetSceneNode() {
    BoxObstacle::resetSceneNode();
    
    float cscale = Director::getInstance()->getContentScaleFactor();
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    pnode->setOpacity(0);
    
    if(pnode != nullptr) {
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        
        Rect bounds;
        bounds.size = getDimension();
        bounds.size.width  /= _drawScale.x*cscale;
        bounds.size.height /= _drawScale.y*cscale;
        
//        std::cout << _textureKey << endl;
        Texture2D* image = assets->get<Texture2D>(_textureKey);
        
        pnode->removeChild(_animationNode);
        
        _animationNode = AnimationNode::create(image, _rowCount, _columnCount, _rowCount*_columnCount);
        
        pnode->addChild(_animationNode);
        _animationNode->setPosition(pnode->getContentSize().width/2.0f,pnode->getContentSize().height/2.0f);
        _animationNode->setFrame(0);
        isAnimating = true;
    }
}

void AnimationBoxModel::replaceAnimationTexture(int rowCount, int columnCount, std::string textureKey) {
    
    _textureKey = textureKey;
    _rowCount   = rowCount;
    _columnCount  = columnCount;
    
    resetSceneNode();
}

void AnimationBoxModel::resetDebugNode() {
    BoxObstacle::resetDebugNode();
}

AnimationBoxModel::~AnimationBoxModel(void) {
    _animationNode = nullptr;
}