//
//  ToyaRemovableBlockModel.cpp
//  Toya
//  Created by 6Sum Studio on 2/27/16.
//

#include "ToyaRemovableBlockModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

#define CONSTRUCT_FRAMES 5

using namespace cocos2d;

#pragma mark -
#pragma mark Initializers

/**
 * Destroys this block, releasing all resources.
 */
RemovableBlockModel::~RemovableBlockModel(void) {
    // We do not own any of these, so we can just set to null
    _animationNode = nullptr;
}

#pragma mark -
#pragma mark Static Constructor

public:
/**
 * Creates a new removable box at the given position.
 *
 * The removable box is sized according to the given drawing scale.
 *
 * The scene graph is completely decoupled from the physics system.
 * The node does not have to be the same size as the physics body. We
 * only guarantee that the scene graph node is positioned correctly
 * according to the drawing scale.
 *
 * @param  pos      Initial position in world coordinates
 * @param  scale    The drawing scale
 * @param  texture  The texture of the removable box
 * @param  state    The state of the removable box
 *
 * @return  An autoreleased physics object
 */
static RemovableBlockModel* create(const Vec2& pos, const Vec2& scale, const String& texture, const int& state) {
    
    RemovableBlockModel* bm = new (std::nothrow) RemovableBlockModel(const Vec2& pos, const Vec2& scale, const String& texture);
    
    if (bm && bm->init()) {
        bm->autorelease();
        
        bm._state = state;
        bm._frameCount = 0;
        
        return bm;
    }
    CC_SAFE_DELETE(rocket);
    return nullptr;
}

#pragma mark -
#pragma mark Animation
/**
 * Performs any necessary additions to the scene graph node.
 *
 * This method is necessary for custom physics objects that are composed
 * of multiple scene graph nodes.  In this case, it is because we
 * manage our own afterburner animations.
 */
void RemovableBlockModel::resetSceneNode() {
    // We need to know the content scale for resolution independence
    // If the device is higher resolution than 1024x576, Cocos2d will scale it
    // This was set as the design resolution in AppDelegate
    // To convert from design resolution to real, divide positions by cscale
    float cscale = Director::getInstance()->getContentScaleFactor();
    
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    if (pnode != nullptr) {
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        
        Rect bounds;
        bounds.size = getDimension();
        bounds.size.width  *= _drawScale.x/cscale;
        bounds.size.height *= _drawScale.y/cscale;
        
        Texture2D* image = assets->get<Texture2D>(_texture);
        
        pnode->setTexture(image); Main object does not contain a image
        pnode->setPolygon(bounds);
        //
        //
        //        image  = assets->get<Texture2D>(_texture);
        //        _animationNode = AnimationNode::create(image, 1, CONSTRUCT_FRAMES, CONSTRUCT_FRAMES);
        //        _animationNode->setScale(cscale);
        //
        //        pnode->addChild(_mainBurner);   // TRANSFER OWNERSHIP
        //        _animationNode->setPosition(_animationNode);
    }
}



/**
 *   Function to destroy this block
 *   Will update the status code to STATE_DESTROYING and change frameCount = 0
 */
public void destroy() {
    _state = STATE_DESTROYING;
    _frameCount = 0;
}
/**
 *   Update block for controller access
 *   If state = STATE_DESTROYING and frameCount == MAX_FRAME_COUNT, ready for garabage collection
 */
public void update() {
    animateBlock();
}
/**
 *   Function that updates the block's animation node frame based on block state and frame count
 */
private void animateBlock() {
//    if(_state == STATE_STABLE) {
//        return;
//    }
//    
//    if(_state == STATE_GROWING) {
//        _node.image = "";
//        frameCount++;
//    }
//    
//    if(_state == STATE_DESTROYING) {
//        frameCount--;
//    }
//    
//    AnimationNode* node = _animationNode;
//    node->setFrame(frameCount);
//
//
}
