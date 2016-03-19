/**
 * ToyaJSBlockModel.h
 *
 * Created by sb2345 on 3/14/16.
 */
#ifndef __TOYA_JS_BLOCK_MODEL_H__
#define __TOYA_JS_BLOCK_MODEL_H__

#include <cornell/CUPolygonObstacle.h>
#include <cornell/CUPolygonNode.h>

using namespace cocos2d;

/**
 * A wall door in the rocket lander game.
 *
 * Note that this class does keeps track of its own assets via keys, so that
 * they can be attached later as part of the scene graph.
 */
class JSBlockModel : public PolygonObstacle {
private:
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CC_DISALLOW_COPY_AND_ASSIGN(JSBlockModel);
    
protected:
    /** The texture filmstrip for the rocket ship */
    std::string _wallTexture;

    
#pragma mark -
#pragma mark Static Constructors
public:
    /**
     * Performs any necessary additions to the scene graph node.
     *
     * This method is necessary for custom physics objects that are composed
     * of multiple scene graph nodes.  In this case, it is because we
     * manage our own afterburner animations.
     */
    bool _isRemovable;
    virtual void resetSceneNode() override;
    
    // virtual void setSceneNode();
    
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
    static JSBlockModel* create(const Poly2& poly);
    
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
    static JSBlockModel* create(const Poly2& poly, const Vec2& anchor);
    
    //
    static JSBlockModel* createWithScale(const Poly2& poly, const Vec2& scale);
    
    //
    static JSBlockModel* createWithTexture(const Poly2& poly, const Vec2& scale, const std::string& texture);
    
#pragma mark -
#pragma mark Animation
    /**
     * Returns the texture (key) for this wall
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @return the texture (key) for this wall
     */
    const std::string& getTextureKey() const { return _wallTexture; }
    
    /**
     * Returns the texture (key) for this wall
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @param  strip    the texture (key) for this wall
     */
    void setTextureKey(std::string strip) { _wallTexture = strip; }
    
    bool isRemovable(){
        return _isRemovable;
    }
    
    void setRemovable(bool value){
        _isRemovable = value;
    }
    
#pragma mark -
#pragma mark Initializers
CC_CONSTRUCTOR_ACCESS:
    /*
     * Creates a new wall at the origin.
     */
    JSBlockModel(void) : PolygonObstacle() { }
    
    /**
     * Destroys this wall, releasing all resources.
     */
    virtual ~JSBlockModel(void) {}
    
    /**
     * Initializes a wall from (not necessarily convex) polygon
     *
     * The anchor point (the rotational center) of the polygon is at the
     * center of the polygons bounding box.
     *
     * @param poly   The polygon vertices
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Poly2& poly) override { return init(poly,Vec2(0.5,0.5)); }
    
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
    virtual bool init(const Poly2& poly, const Vec2& anchor) override;
    
    //
    // virtual bool init(const Poly2& poly, const Vec2& anchor, const Vec2& scale);
    
};
#endif /* defined(__TOYA_BLOCK_MODEL_H__) */