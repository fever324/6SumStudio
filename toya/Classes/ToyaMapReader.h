//
//  ToyaMapReader.h
//  toya
//
//  Created by 6SumStudio Li on 4/6/16.
//
//

#ifndef __TOYA_MAP_READER_H__
#define __TOYA_MAP_READER_H__



#include <cocos2d.h>
#include <cornell/CUKeyboardPoller.h>
#include <cornell/CUAccelerationPoller.h>
#include <cornell/CUTouchListener.h>
#include "base/CCEventListenerTouch.h"

using namespace cocos2d;
using namespace std;
class MapReader  {  // Block demo game controller line 420

public:
    static Vec2 getToyaCoordinate(float x, float y);

    
}




#endif /* __TOYA_MAP_READER_H__ */
