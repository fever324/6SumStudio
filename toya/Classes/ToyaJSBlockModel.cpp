/**
 * ToyaJSBlockModel.cpp
 *
 * Created by sb2345 on 2/27/16.
 */

#include "ToyaJSBlockModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

// using namespace cocos2d;

#pragma mark -
#pragma mark Static Constructors
#pragma mark -
#pragma mark Static Constructors
/**
 * Creates a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is at the
 * center of the polygons bounding box.
 *
 * Unlike most constructors, this one RETAINS the object and does not
 * autorelease.  That is because (1) this object is allocated in a
 * second thread (the asset loader thread) and (2) autoreleased objects
 * are freed in the main thread.  Hence autorelease is not thread safe,
 *
 * @param poly   The polygon vertices
 *
 * @retain a reference to the object (transferable)
 *
 * @return  An RETAINED physics object
 */
JSBlockModel* JSBlockModel::create(const Poly2& poly) {
    JSBlockModel* obstacle = new (std::nothrow) JSBlockModel();
    if (obstacle && obstacle->init(poly)) {
        // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
        obstacle->retain();
        return obstacle;
    }
    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

/**
 * Creates a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is specified as a
 * ratio of the bounding box.  An anchor point of (0,0) is the bottom left of
 * the bounding box.  An anchor point of (1,1) is the top right of the bounding
 * box.  The anchor point does not need to be contained with the bounding box.
 *
 * Unlike most constructors, this one RETAINS the object and does not
 * autorelease.  That is because (1) this object is allocated in a
 * second thread (the asset loader thread) and (2) autoreleased objects
 * are freed in the main thread.  Hence autorelease is not thread safe,
 *
 * @param  poly     The polygon vertices
 * @param  anchor   The rotational center of the polygon
 *
 * @retain a reference to the object (transferable)
 *
 * @return  An RETAINED physics object
 */
JSBlockModel* JSBlockModel::create(const Poly2& poly, const Vec2& anchor) {
    JSBlockModel* obstacle = new (std::nothrow) JSBlockModel();
    if (obstacle && obstacle->init(poly,anchor)) {
        // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
        obstacle->retain();
        return obstacle;
    }
    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

//
JSBlockModel* JSBlockModel::createWithScale(const Poly2& poly, const Vec2& scale) {
    JSBlockModel* obstacle = new (std::nothrow) JSBlockModel();
    if (obstacle && obstacle->init(poly)) {
        // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
        obstacle->retain();
        obstacle->setDrawScale(scale);
        obstacle->setBodyType(b2_staticBody);
        return obstacle;
    }
    CC_SAFE_DELETE(obstacle);
    return nullptr;

}

//
JSBlockModel* JSBlockModel::createWithTexture(const Poly2& poly, const Vec2& scale,
        const std::string& texture) {
    JSBlockModel* obstacle = new (std::nothrow) JSBlockModel();
    if (obstacle && obstacle->init(poly)) {
        obstacle->retain();
        obstacle->setDrawScale(scale);
        obstacle->setBodyType(b2_staticBody);
    }
    SceneManager* assets =  AssetManager::getInstance()->getCurrent();
    Texture2D* image = assets->get<Texture2D>(texture);
    Poly2 resize = poly * scale;
    
    PolygonNode* sprite = PolygonNode::createWithTexture(image, resize);
    obstacle->setSceneNode(sprite);
    return obstacle;
    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

#pragma mark -
#pragma mark Initializers

/**
 * Initializes a wall from (not necessarily convex) polygon
 *
 * The anchor point (the rotational center) of the polygon is specified as a
 * ratio of the bounding box.  An anchor point of (0,0) is the bottom left of
 * the bounding box.  An anchor point of (1,1) is the top right of the bounding
 * box.  The anchor point does not need to be contained with the bounding box.
 *
 * @param  poly     The polygon vertices
 * @param  anchor   The rotational center of the polygon
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool JSBlockModel::init(const Poly2& poly, const Vec2& anchor) {
    PolygonObstacle::init(poly,anchor);
    string name("wall");
    setName(name);
    _wallTexture = "";
    return true;
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
void JSBlockModel::resetSceneNode() {
    PolygonNode* pnode = dynamic_cast<PolygonNode*>(_node);
    if (pnode != nullptr) {
        // We need to know the content scale for resolution independence
        // If the device is higher resolution than 1024x576, Cocos2d will scale it
        // THIS DOES NOT FIX ASPECT RATIO PROBLEMS
        // If you are using a device with a 3:2 aspect ratio, you will need to
        // completely redo the level layout.  We can help if this is an issue.
        float cscale = Director::getInstance()->getContentScaleFactor();
        
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        Texture2D* image = assets->get<Texture2D>(_wallTexture);
        
        Poly2 poly = getPolygon();
        poly *= _drawScale/cscale;
        
        pnode->setTexture(image);
        pnode->setPolygon(poly);
        pnode->setScale(cscale);
    }
}