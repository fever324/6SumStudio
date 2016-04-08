#include <stdio.h>
#include "ToyaBlockFactory.h"
#include "Constants.h"
using namespace cocos2d;

RemovableBlockModel* BlockFactory::getRemovableBlock(const Vec2& pos, const Size& size, Vec2 scale, std::string textureKey) {
    RemovableBlockModel* removableBlock = RemovableBlockModel::create(1, 2, 4, textureKey, pos, size, scale);
    return removableBlock;
}

MovingObstacleModel* BlockFactory::getMovingObstacle(int stateCount, int rowCount, int columnCount, std::string textureKey, const Vec2& pos, const Size& size, Vec2 scale, std::vector<Vec2>& routes, int speed) {
    MovingObstacleModel* movingObstacle = MovingObstacleModel::create(stateCount, rowCount, columnCount, textureKey, pos, size, scale, routes, speed);
    return movingObstacle;
}