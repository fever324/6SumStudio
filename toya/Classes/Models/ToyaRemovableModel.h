//
//  RemovableBlockModel.h
//
//
//  Created by 6Sum Studio on 2/27/16.
//
//

#ifndef __TOYA_REMOVABLEBLOCK_MODEL_H__
#define __TOYA_REMOVABLEBLOCK_MODEL_H__

#define STATE_GROWING         0
#define STATE_STABLE          1
#define STATE_DESTROYING      2

#define MAX_FRAME_COUNT       60


class RemovableBlockModel : public BlockModel {  // Block demo game controller line 420

private:
/** The state showing current state of removable block. **/
int _state;
/** The count of which frame should appear on the screen. Initialize to 0 if state is initialized to STATE_GROWING; initialize to MAX_FRAME_COUNT if state is initialized to STATE_STABLE. **/
int _frameCount;

#pragma mark -
#pragma mark Static Constructor

public:
/**
* Creates a new removable box at the given position.
*
* The removable box is sized according to the given drawing scale.
*
* The scene graph is completely decoupled from the physics system.
* The node does not have to be the same size as the physics body. We
* only guarantee that the scene graph node is positioned correctly
* according to the drawing scale.
*
* @param  pos      Initial position in world coordinates
* @param  scale    The drawing scale
* @param  texture  The texture of the removable box
* @param  state    The state of the removable box
*
* @return  An autoreleased physics object
*/
static RemovableBlockModel* create(const Vec2& pos, const Vec2& scale,
const String& texture, const int& state);

#pragma mark -
#pragma mark Animation
/**
*   Function to destroy this block
*   Will update the status code to STATE_DESTROYING and change frameCount = 0
*/
public void destroy();
/**
*   Update block for controller access
*   If state = STATE_DESTROYING and frameCount == MAX_FRAME_COUNT, ready for garabage collection
*/
public void update();
/**
*   Function that updates the block's animation node frame based on block state and frame count
*/
private void updateFrame();
}

#endif /** __TOYA_REMOVABLEBLOCK_MODEL_H__ **/