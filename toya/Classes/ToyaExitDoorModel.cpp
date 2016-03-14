/**
 * ToyaExitDoorModel.cpp
 *
 * Created by sb2345 on 3/13/2016.
 */

#include "ToyaBlockModel.h"
#include "ToyaExitDoorModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

using namespace cocos2d;

//
ExitDoorModel* ExitDoorModel::create() {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init()) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos, const Size &size) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos, size)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos, const Size &size, const std::string &texture) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos, size, texture)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

/**
 * Creates a new exit with the given position, size, texture and state
 *
 * @param  pos      Initial position in world coordinates
 * @param  size     The dimensions of the box.
 * @param  texture  The material of the box.
 * @param  state    The state of the box
 *
 * @return  An autoreleased physics object
 */
ExitDoorModel* ExitDoorModel::create(const Vec2 &pos, const Size &size, const std::string &texture, const int &state) {
    ExitDoorModel* exit = new (std::nothrow) ExitDoorModel();
    if (exit && exit->init(pos, size, texture, state)) {
        exit->autorelease();
        return exit;
    }
    CC_SAFE_DELETE(exit);
    return nullptr;
}

//
void ExitDoorModel::setExitState(const int& value, const std::string& texture) {
    _state = value;
    _texture = texture;
}

//
bool ExitDoorModel::init(const Vec2 &pos, const Size &size) {
    BlockModel::init(pos, size);
    _state = 0;
    return true;
}

//
bool ExitDoorModel::init(const Vec2 &pos, const Size &size, const std::string &texture) {
    BlockModel::init(pos, size, texture);
    _state = 0;
    return true;
}

/**
 * Initializes a new exit with the given position, size, texture and state.
 *
 * @param  pos      Initial position in coordinates.
 * @param  size   	The dimensions of the box.
 * @param  texture  The material of the box.
 * @param  state    The state of the box
 *
 * @return  true if the object is initialized properly, false otherwise.
 */
bool ExitDoorModel::init(const Vec2 &pos, const Size &size, const std::string &texture, const int &state) {
    BlockModel::init(pos, size, texture);
    _state = state;
    return true;
}

/**
 * Destructor of this exit releasing all resources.
 */
ExitDoorModel::~ExitDoorModel(void) {
    // We do not own any of these, so we just set to null
}


