/**
 * ToyaNBlockModel.h
 *
 * Created by sb2345 on 3/14/16.
 */
#ifndef __TOYA_N_BLOCK_MODEL_H__
#define __TOYA_N_BLOCK_MODEL_H__


#include <cornell/CUPolygonObstacle.h>
#include <cornell/CUPolygonNode.h>
#include <cornell/CUBoxObstacle.h>

#include "ToyaGameController.h"
#include "ToyaInputController.h"
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "ToyaJSBlockModel.h"
#include "ToyaLevelModel.h"
#include "ToyaPanelModel.h"


// Physics constants for initialization
/** Density of non-crate objects */
#define BASIC_DENSITY       0.0f
/** Density of the crate objects */
#define CRATE_DENSITY       1.0f
/** Friction of non-crate objects */
#define BASIC_FRICTION      0.05f
/** Friction of the crate objects */
#define CRATE_FRICTION      0.05f
/** Angular damping of the crate objects */
#define CRATE_DAMPING       1.0f
/** Collision restitution for all objects */
#define BASIC_RESTITUTION   0.1f



using namespace cocos2d;

class NBlockModel : public Ref {
    
public:
    static BoxObstacle* create(const Vec2& pos, const Size& size) {
        BoxObstacle* obj;
        obj = BoxObstacle::create(pos,size);
        return obj;
    }
    
    static BoxObstacle* createWithTexture(const Vec2& pos, const Size& size, const std::string& texture) {
        BoxObstacle* obj;
        obj = BoxObstacle::create(pos,size);
        obj->setBodyType(b2_staticBody);
        
        float cscale = Director::getInstance()->getContentScaleFactor();
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        Texture2D* image = assets->get<Texture2D>(texture);
        Sprite* sprite;
        sprite = Sprite::createWithTexture(image);
        obj->setSceneNode(sprite);
        return obj;
    }
    
    static PolygonObstacle* createWithTexture(const Poly2& poly, const Vec2& scale, const std::string& texture) {

        float cscale = Director::getInstance()->getContentScaleFactor();

        PolygonObstacle* obstacle = new (std::nothrow) PolygonObstacle();
        if (obstacle && obstacle->init(poly)) {
            // AUTORELEASE IS NOT THREAD SAFE FOR ASSET LOADING
            obstacle->retain();
            obstacle->setDrawScale(scale);
            obstacle->setBodyType(b2_staticBody);
            
            SceneManager* assets =  AssetManager::getInstance()->getCurrent();
            Texture2D* image = assets->get<Texture2D>(texture);
            Poly2 resize = poly * scale;
            PolygonNode* sprite = PolygonNode::createWithTexture(image, resize);
            sprite->setScale(cscale);
            obstacle->setSceneNode(sprite);
            
            
            // Set the physics attributes (cannot be absorbed in to a method because we may change these macros)
            obstacle->setDensity(BASIC_DENSITY);
            obstacle->setFriction(BASIC_FRICTION);
            obstacle->setRestitution(BASIC_RESTITUTION);
            
            
            
            return obstacle;
        }
        CC_SAFE_DELETE(obstacle);
        return nullptr;
    }
};



#endif /* defined(__TOYA_N_BLOCK_MODEL_H__) */