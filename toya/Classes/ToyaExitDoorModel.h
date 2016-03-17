/**
 * ToyaBarrierModel.h
 *
 * Created by sb2345 on 3/13/2016.
**/

#ifndef __TOYA_EXITDOOR_MODEL_H__
#define __TOYA_EXITDOOR_MODEL_H__


#include "cornell/CUAnimationNode.h"
#include "ToyaBlockModel.h"
#include <cornell/CUAnimationNode.h>


#define GOAL_TEXTURE "goal"
#define GOAL_REACHED_TEXTURE "goal-reached"

using namespace cocos2d;

class ExitDoorModel : public BoxObstacle {
protected:
    /** The state of exit door: open(0) or close(1) **/
    int _state;

#pragma mark -
#pragma mark Static Constructors
public:
    /**
     * Creates a new exit at origin.
     *
     * @return  An auto-released physics object
     */
    static ExitDoorModel* create();

    /**
     * Creates a new exit with the given position.
     *
     * @param  pos      Initial position
     *
     * @return  An auto-released physics object
     */
    static ExitDoorModel* create(const Vec2& pos);

    /**
     * Creates a new exit with the given position and size.
     *
     * @param  pos      Initial position
	 * @param  size   	The dimensions of the exit.
     *
     * @return  An auto-released physics object
     */
    static ExitDoorModel* create(const Vec2& pos, const Size& size);

#pragma mark -
#pragma mark Accessors
    /**
     * Returns the state of this exit.
     *
     * @return the state applied to this exit.
     */
    const int& getExitState() const { return _state; }



#pragma mark -
#pragma mark Initializers
CC_CONSTRUCTOR_ACCESS:
    /*
     * Creates a new exit at the origin.
     */
    ExitDoorModel(void) : BoxObstacle() { }

    /**
     * Destroys this exit, releasing all resources.
     */
    virtual ~ExitDoorModel(void);

    /**
     * ?
     */
    virtual bool init() override {
        return init(Vec2::ZERO, Size::ZERO);
    }

    /**
     * ?
     */
    virtual bool init(const Vec2& pos) override {
        return init(pos, Size(1,1));
    }

    /**
     * ?
     */
    virtual bool init(const Vec2& pos, const Size& size) override;



#pragma mark -
#pragma mark Instance methods
    void open();

};

#endif