#include <stdio.h>
#include "ToyaBlockFactory.h"
#include "Constants.h"
using namespace cocos2d;

RemovableBlockModel* BlockFactory::getRemovableBlock(const Vec2& pos, const Size& size, Vec2 scale) {
    RemovableBlockModel* removableBlock = RemovableBlockModel::create(1, 2, 1, REMOVABLE_BLOCK_TEXTURE, pos, size, scale);
    return removableBlock;
}