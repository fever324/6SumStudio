#include <stdio.h>
#include "ToyaBlockFactory.h"
#include "Constants.h"
using namespace cocos2d;

RemovableBlockModel* BlockFactory::getRemovableBlock(const Vec2& pos, const Size& size) {
    RemovableBlockModel* removableBlock = RemovableBlockModel::create(1, 1, 1, REMOVABLE_BLOCK_TEXTURE, pos, size);
    return removableBlock;
}