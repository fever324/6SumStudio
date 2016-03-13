/**
 * ToyaBarrierModel.h
 *
 * Created by sb2345 on 3/13/2016.
**/

#ifndef __TOYA_EXITDOOR_MODEL_H__
#define __TOYA_EXITDOOR_MODEL_H__


#include "cornell/CUAnimationNode.h"
#include "ToyaBlockModel.h"

using namespace cocos2d;

class ExitDoorModel : public BlockModel {
protected:
    /** The state of exit door: open or close **/
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

    /**
     * Creates a new exit with the given position, size and texture.
     *
     * @param  pos      Initial position
	 * @param  size   	The dimensions of the exit.
     * @param  texture  The texture of the exit
     *
     * @return  An auto-released physics object
     */
    static ExitDoorModel* create(const Vec2& pos, const Size& size, const std::string& texture);

    /**
     * Creates a new exit with the given position, size, texture and state.
     *
     * @param  pos      Initial position
	 * @param  size   	The dimensions of the exit.
     * @param  texture  The texture of the exit
     * @param  state    The state of the exit
     *
     * @return  An auto-released physics object
     */
    static ExitDoorModel* create(const Vec2& pos, const Size& size, const std::string& texture, const int& state);

#pragma mark -
#pragma mark Accessors
    /**
     * Returns the state of this exit.
     *
     * @return the state applied to this exit.
     */
    const int& getExitState() const { return _state; }

    /**
     * Sets the state applied to this exit.
     *
     * @param value  the state applied to this exit.
     */
    virtual void setExitState(const int& value, const std::string& texture);

#pragma mark -
#pragma mark Initializers
CC_CONSTRUCTOR_ACCESS:
    /*
     * Creates a new exit at the origin.
     */
    ExitDoorModel(void) : BlockModel() { }

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

    /**
     * ?
     */
    virtual bool init(const Vec2& pos, const Size& size, const std::string& texture) override;

    /**
     * ?
     */
    virtual bool init(const Vec2& pos, const Size& size, const std::string& texture, const int& state);

};

#endif