/**
 * ToyaBlockModel.cpp
 *
 * Created by sb2345 on 2/27/16.
 */

#include "ToyaBlockModel.h"
#include <cornell/CUAssetManager.h>
#include <cornell/CUSceneManager.h>

using namespace cocos2d;

/**
 * Creates a new block at the origin.
 *
 * The block is 1 unit by 1 unit in size. The block is scaled so that
 * 1 pixel = 1 Box2d unit
 *
 * @return  An autoreleased physics object
 */
BlockModel* BlockModel::create() {
    BlockModel* block = new (std::nothrow) BlockModel();
    if (block && block->init()) {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

/**
 * Creates a new block with the given position.
 *
 * @param   pos  Initial position in world coordinates
 *
 * @return  An autoreleased physics object
 */
BlockModel* BlockModel::create(const Vec2& pos) {
    BlockModel* block = new (std::nothrow) BlockModel();
    if (block && block->init(pos)) {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

/**
 * Creates a new block with the given position and size.
 *
 * @param   pos  Initial position in world coordinates
 * @param   size    The dimensions of the box.
 *
 * @return  An autoreleased physics object
 */
BlockModel* BlockModel::create(const Vec2& pos, const Size& size) {
    BlockModel* block = new (std::nothrow) BlockModel();
    if (block && block->init(pos, size)) {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}


/**
 * Creates a new block with the given position, size and texture.
 *
 * @param   pos Initial position in world coordinates
 * @param   size    The dimensions of the box.
 * @param   texture The material of the box.
 *
 * @return  An autoreleased physics object
 */
BlockModel* BlockModel::create(const Vec2& pos, const Size& size, const String& texture) {
    BlockModel* block = new (std::nothrow) BlockModel();
    if (block && block->init(pos, size, texture)) {
        block->autorelease();
        return block;
    }
    CC_SAFE_DELETE(block);
    return nullptr;
}

/**
 * Initializes a new block with the given position and size.
 *
 * @param  pos      Initial position in coordinates.
 * @param  size   	The dimensions of the box.
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool BlockModel::init(const Vec2& pos, const Size& size) {
    BoxObstacle::init(pos, size);
    return true;
}

/**
 * Initializes a new block with the given position, size and texture.
 *
 * @param  pos      Initial position in coordinates.
 * @param  size   	The dimensions of the box.
 * @param  texture  The material of the box.
 *
 * @return  true if the obstacle is initialized properly, false otherwise.
 */
bool BlockModel::init(const Vec2& pos, const Size& size, const String& texture) {
    BoxObstacle::init(pos, size);
    _texture = texture;
    return true;
}

/**
 * Destructor of this block, releasing all resources.
 */
BlockModel::~BlockModel(void) {
    // We do not own any of these, so we just set to null
    _size = nullptr;
    _texture = nullptr;
    _pos = nullptr;
}