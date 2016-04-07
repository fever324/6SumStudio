/**
 * ToyaBlockFactory.h
 *
 * Created by 6SumStudio on 3/14/16.
 */
#ifndef __TOYA_BLOCK_FACTORY_H__
#define __TOYA_BLOCK_FACTORY_H__


#include <cornell/CUPolygonObstacle.h>
#include <cornell/CUPolygonNode.h>
#include <cornell/CUBoxObstacle.h>

#include "ToyaGameController.h"
#include "ToyaInputController.h"
#include <cornell.h>
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "ToyaPanelModel.h"
#include "ToyaRemovableBlockModel.h"


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

#define DEBUG_COLOR     Color3B::GREEN

#define DEBUG_OPACITY   192



using namespace cocos2d;

class BlockFactory {
    
public:
    static BoxObstacle* create(const Vec2& pos, const Size& size) {
        BoxObstacle* obj;
        obj = BoxObstacle::create(pos,size);
        return obj;
    }
    
//    static BoxObstacle* getRemovableBlock(const Vec2& pos, const Vec2& scale,const std::string& texture) {
//        return getRemovableBlock(pos, scale, texture, true);
//    }
//    
//    static BoxObstacle* getRemovableBlock(const Vec2& pos, const Vec2& scale,
//                                          const std::string& texture, bool debug) {
//        BoxObstacle* obj;
//        
//        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
//        Texture2D* image = assets->get<Texture2D>(texture);
//        
//        PolygonNode* sprite = PolygonNode::createWithTexture(image);
//        
//        float cscale = Director::getInstance()->getContentScaleFactor();
//        sprite->setScale(cscale);
//        
//        Size realSize = Size(image->getContentSize().width/scale.x, image->getContentSize().height/scale.y);
//        
//        obj = BoxObstacle::create(pos,realSize);
//        obj->setSceneNode(sprite);
//        obj->setDrawScale(scale);
//        obj->setBodyType(b2_staticBody);
//        obj->setDensity(BASIC_DENSITY);
//        obj->setFriction(BASIC_FRICTION);
//        obj->setRestitution(BASIC_RESTITUTION);
//        
////        obj->setSensor(true);
//        obj->setName("removable");
//        
//        if (debug == true) {
//            WireNode* draw;
//            draw = WireNode::create();
//            draw->setColor(DEBUG_COLOR);
//            draw->setOpacity(DEBUG_OPACITY);
//            obj->setDebugNode(draw);
//        }
//        return obj;
//    }
    
    static RemovableBlockModel* getRemovableBlock(const Vec2& pos, const Size& size, Vec2 scale);
    
    static PolygonObstacle* getNonRemovableBlock(const Poly2& poly, const Vec2& scale,
                                              const std::string& texture) {
        return getNonRemovableBlock(poly, scale, texture, true);
    };
    
    static PolygonObstacle* getNonRemovableBlock(const Poly2& poly, const Vec2& scale,
                                              const std::string& texture, bool debug) {
        PolygonObstacle* obstacle = PolygonObstacle::create(poly);
        obstacle->setDrawScale(scale);
        obstacle->setBodyType(b2_staticBody);
        
        SceneManager* assets =  AssetManager::getInstance()->getCurrent();
        Texture2D* image = assets->get<Texture2D>(texture);
        Poly2 resize = poly * scale;
        PolygonNode* sprite = PolygonNode::createWithTexture(image, resize);
        
        obstacle->setSceneNode(sprite);
        
        // Set the physics attributes (cannot be absorbed in to a method because we may change these macros)
        obstacle->setDensity(BASIC_DENSITY);
        obstacle->setFriction(BASIC_FRICTION);
        obstacle->setRestitution(BASIC_RESTITUTION);
        
        if (debug == true) {
            WireNode* draw;
            draw = WireNode::create();
            draw->setColor(DEBUG_COLOR);
            draw->setOpacity(DEBUG_OPACITY);
            obstacle->setDebugNode(draw);
        }
        return obstacle;
    }
    
    static ExitDoorModel* getExitDoor();
};



#endif /* defined(__TOYA_BLOCK_FACTORY_H__) */