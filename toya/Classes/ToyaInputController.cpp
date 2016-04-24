//
//  ToyaInputController.cpp
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#include "ToyaInputController.h"

#define RANGE_CLAMP(x,y,z)  (x < y ? y : (x > z ? z : x))

// the key to use for start the main game -> for main interface
#define START_KEY EventKeyboard::KeyCode::KEY_S

/** The key to use for reseting the game */
#define RESET_KEY EventKeyboard::KeyCode::KEY_R
/** The key for toggling the debug display */
#define DEBUG_KEY EventKeyboard::KeyCode::KEY_D
/** The key for exitting the game */
#define EXIT_KEY  EventKeyboard::KeyCode::KEY_ESCAPE

/** How fast a double click must be in milliseconds */
#define EVENT_DOUBLE_CLICK  400
/** How far we must swipe left or right for a gesture */
#define EVENT_SWIPE_LENGTH  200
/** How fast we must swipe left or right for a gesture */
#define EVENT_SWIPE_TIME   1000
/** How far we must turn the tablet for the accelerometer to register */
#define EVENT_ACCEL_THRESH  M_PI/10.0f
/** Whether to active the accelerometer */
#define USE_ACCELEROMETER       false
/** Adjustment factor for accelerometer input */
#define ACCELEROM_FACTOR  200.0f

/** Adjustment factor for touch input */
#define KEY_ADJUST_FACTOR  1.0f
// Key board rotation degree increment
#define KEYBOARD_ROTATION_INCREMENT 1.0f
// maxium rotation degree
#define INPUT_MAXIMUM_ROTATION 90.0f
// max rotate time
#define EVENT_ROTATE_TIME 100000
// minimum rotation
#define MIN_ROTATION 1.0f


#pragma mark -
#pragma mark Input Controller
/**
 * Creates a new input controller.
 *
 * This constructor does NOT do any initialzation.  It simply allocates the
 * object. This makes it safe to use this class without a pointer.
 */
InputController::InputController() :
_active(false),
_resetPressed(false),
_debugPressed(false),
_exitPressed(false),
_touchListener(nullptr)
{
    _keyLeft  = false;
    _keyRight = false;
    _keyReset = false;
    _keyDebug = false;
    _keyExit  = false;
    _keyRotate = false;
    
    swipe = 0;
    
    // ready to remove
    _keyStart = false;
    
    _select = false;
    
    _touch1.touchid = -1;
    _touch2.touchid = -1;
    
    _smoothedTurning = 0;
    
}

/**
 * Disposes of this input controller, releasing all listeners.
 */
InputController::~InputController() {
    if (_touchListener != nullptr) {
        _touchListener->release();
        _touchListener = nullptr;
    }
}

/**
 * Initializes the input control for the given drawing scale.
 *
 * This method works like a proper constructor, initializing the input
 * controller and allocating memory.  However, it still does not activate
 * the listeners.  You must call start() do that.
 *
 * @return true if the controller was initialized successfully
 */
bool InputController::init() {
    _timestamp = current_time();
    _rotateTime = current_time();
    // Create the touch listener. This is an autorelease object.
    _touchListener = TouchListener::create();
    if (_touchListener != nullptr) {
        _touchListener->retain();
        _touchListener->onTouchBegan = [this](Touch* t, timestamp_t time) {
            return touchBeganCB(t,time);
        };
        _touchListener->onTouchMoved = [=](Touch* t, timestamp_t time) {
            return this->touchMovedCB(t,time);
        };
        _touchListener->onTouchEnded = [=](Touch* t, timestamp_t time) {
            return this->touchEndedCB(t,time);
        };
        _touchListener->onTouchCancelled = [=](Touch* t, timestamp_t time) {
            return this->touchCancelCB(t,time);
        };
        return true;
    }
    return false;
}

/**
 * Starts the input processing for this input controller.
 *
 * This method must be called AFTER the input controller is initialized
 */
void InputController::start() {
    if (!_active) {
        _active = true;
        // Priority the input
        // BUG: Cocos2D always prioritizes touch
        KeyboardPoller::start(1);
        _touchListener->start(2);
        AccelerationPoller::start(3);
    }
}

/**
 * Stops the input processing for this input controller.
 *
 * This method will not dispose of the input controller. It can be restarted.
 */
void InputController::stop() {
    if (_active) {
        _active = false;
        _touchListener->stop();
        AccelerationPoller::stop();
        KeyboardPoller::stop();
    }
}


/**
 * Processes the currently cached inputs.
 *
 * This method is used to to poll the current input state.  This will poll the
 * keyboad and accelerometer.
 *
 * This method also gathers the delta difference in the touches. Depending on
 * the OS, we may see multiple updates of the same touch in a single animation
 * frame, so we need to accumulate all of the data together.
 */
void InputController::update(float dt) {
    if (!_active) {
        return;
    }
    
// Only process keyboard on desktop
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    KeyboardPoller* keys = KeyboardPoller::getInstance();
    keys->update();
    
    _keyReset  = keys->keyPressed(RESET_KEY);
    _keyDebug  = keys->keyPressed(DEBUG_KEY);
    _keyExit   = keys->keyPressed(EXIT_KEY);
    
    // for main interface, press k to start (new level)
    _keyStart = keys->keyPressed(START_KEY);
    
    
    // rotation angles increase the longer you hold a key.
    if (keys->keyDown(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        _keyLeft = KEYBOARD_ROTATION_INCREMENT;
    } else {
        _keyLeft  = 0.0f;
        _keyRotate = false;
    }
    if (keys->keyDown(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        _keyRight = KEYBOARD_ROTATION_INCREMENT;
    } else {
        _keyRight  = 0.0f;
        _keyRotate = false;
    }
    
    // Clamp everything so it does not fly off to infinity.
//    _keyLeft  = (_keyLeft  > INPUT_MAXIMUM_ROTATION ? INPUT_MAXIMUM_ROTATION : _keyLeft);
//    _keyRight = (_keyRight > INPUT_MAXIMUM_ROTATION ? INPUT_MAXIMUM_ROTATION : _keyRight);
//    
    // update _keyTurning
    _keyTurning = 0;
    _keyTurning += _keyLeft;
    _keyTurning -= _keyRight;

    // TODO: RANGE_CLAMP
//    _keyTurning = RANGE_CLAMP(_keyTurning/KEY_ADJUST_FACTOR,-INPUT_MAXIMUM_ROTATION,INPUT_MAXIMUM_ROTATION);

    
    _turning = _keyTurning;
    calculateNewSmoothedTurning(_turning);
    if ( fabs(_turning) > 0.5f || fabs(_smoothedTurning) > 0.0f) {
        _keyRotate = true;
    }else{
        _keyRotate = false;
    }
    
#else
    // mobile controls
    if (USE_ACCELEROMETER) {
        Acceleration acc = AccelerationPoller::getInstance()->getAcceleration();
        
        // Apply to _turning directly.
        _turning = acc.x * ACCELEROM_FACTOR - acc.y * ACCELEROM_FACTOR;
    }
#endif
    
    _resetPressed = _keyReset;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _keyReset = false;
#endif
}

/**
 * Clears any buffered inputs so that we may start fresh.
 */
void InputController::clear() {
    _resetPressed = false;
    _keyTurning = 0.0f;
    _turning = 0.0f;
    
    _keyLeft  = 0.0f;
    _keyRight = 0.0f;
    
    _touch1.touchid = -1;
    _touch2.touchid = -1;
    
    _timestamp = current_time();
}

#pragma mark -
#pragma mark Touch Callbacks

/**
 * Callback for the beginning of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 *
 * @return True if the touch was processed; false otherwise.
 */
bool InputController::touchBeganCB(Touch* t, timestamp_t current) {
    
    _touchCount ++;
    _select = (_touchCount == 1);
    
    if(_select) {
        _ctouch.set(t->getLocation());
    }
    
    _touchCount = _touchCount > 2 ? 2 : _touchCount;
    Vec2 pos = t->getLocation();
    if (_touch1.touchid == -1) {
        _touch1.start = pos;
        _touch1.touchid = t->getID();
        _touch1.stop = pos;
    } else if (_touch2.touchid == -1) {
        _touch2.start = pos;
        _touch2.touchid = t->getID();
        _touch2.stop = pos;
    } else {
        CCLOG("No more than 2 fingers");
    }
    
    _timestamp = current;
    _rotateTime = current;
    return true;
}

/**
 * Callback for the end of a touch event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchEndedCB(Touch* t, timestamp_t current) {
    
    if (_touch1.touchid == t->getID()) {
        resetTouch(&_touch1);
    } else if (_touch2.touchid == t->getID()) {
        resetTouch(&_touch2);
    }
    _keyTap = checkTap(current);
    _touchCount --;
    _touchCount = _touchCount < 0 ? 0 : _touchCount;
    _keyRotate = false;
    
    _select = false;
    _released = true;
}

bool InputController::checkTap(timestamp_t current) {
    // tap
    if (_touchCount != 1) {
        return false;
    }
    // tap should have same stop position as start position.
    if (_touch1.start.x == _touch1.stop.x && _touch1.start.y == _touch1.stop.y) {
        return true;
    }
    if (_touch2.start.x == _touch2.stop.x && _touch2.start.y == _touch2.stop.y) {
        return true;
    }
    return false;
}

bool InputController::checkRotate(timestamp_t current) {
    // calculate the turning
//    CCLOG("touch count: %d", _touchCount);
    if (_touchCount != 2) {
        _turning = 0.0f;
        return false;
    }
    
    float temp = 0;
    
    if ( _touch1.start.x < _touch2.start.x ){
        // t1 on the left
        
        // initial angle
        float angle0 = atanf( (_touch2.start.y - _touch1.start.y) / (_touch2.start.x - _touch1.start.x) ) * 180/M_PI;
        
        // end angle
        float angle1 = atanf( (_touch2.stop.y - _touch1.stop.y) / (_touch2.stop.x - _touch1.stop.x) ) * 180/M_PI;
        
        // temporary turning
        temp = angle0 - angle1;
        
    } else {
        // t1 on the right
        
        // initial angle
        float angle0 = atanf( (_touch1.start.y - _touch2.start.y) / (_touch1.start.x - _touch2.start.x) ) * 180/M_PI;
        // end angle
        float angle1 = atanf( (_touch1.stop.y - _touch2.stop.y) / (_touch1.start.x - _touch2.stop.x) ) * 180/M_PI;
        
        // temporary turning
        temp = angle0 - angle1;
    }
    
    _touch1.start = _touch1.stop;
    _touch2.start = _touch2.stop;
    
    if(elapsed_millis(_rotateTime,current) <= EVENT_ROTATE_TIME){
        _turning = temp;
        calculateNewSmoothedTurning(_turning);
        return true;
    }
    _rotateTime = current;
    _turning = 0.0f;
    return false;
}

void InputController::calculateNewSmoothedTurning(float newTurning) {
    for(int i = HISTORY_LENGTH - 1 ; i >= 1; i--) {
        inputHistory[i] = inputHistory[i-1];
    }
    
    inputHistory[0] = newTurning;
    
    _smoothedTurning = 0.0f;
    for(int i = 0; i < HISTORY_LENGTH; i++) {
        _smoothedTurning += inputHistory[i] * inputHistoryWeights[i];
    }
}

/**
 * Callback for a touch movement event
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchMovedCB(Touch* t, timestamp_t current) {
    if(!_active) return;
    
    if(_touchCount > 1) {
        if (t->getID() == _touch1.touchid) {
            _touch1.stop = t->getLocation();
            _keyRotate = checkRotate(current);
        } else if (t->getID() == _touch2.touchid) {
            _touch2.stop = t->getLocation();
            _keyRotate = checkRotate(current);
        } else {
            return;
        }
    } else if(_touchCount == 1) {
        if(t->getID() == _touch1.touchid) {
            swipe = checkSwipe(_touch1.start, t->getLocation(), current);
        }
    }
    
}

/**
 * Callback for the cancellation of a touch event
 *
 * Cancellation occurs when an external event—for example,
 * an incoming phone call—disrupts the current app’s event
 * processing.
 *
 * @param t     The touch information
 * @param event The associated event
 */
void InputController::touchCancelCB(Touch* t, timestamp_t current) {
    // Update the timestamp
    _timestamp = current;
    _rotateTime = current;
    _keyLeft  = false;
    _keyRight = false;
    _keyReset = false;
    _keyExit  = false;
    _keyRotate = false;
    
    _keyStart = false;
    
    _select = false;
    
    _touch1.touchid = -1;
    _touch2.touchid = -1;
    _turning = 0;
}

/**
 * reset touch instance to default
 *
 */
void InputController::resetTouch(TouchInstance* t) {
    // reset the touch
    t->touchid = -1;
    t->start.x = 0;
    t->start.y = 0;
    t->stop.x = 0;
    t->stop.y = 0;
}

int InputController::checkSwipe(const Vec2& start, const Vec2& stop, timestamp_t current) {
    // Look for swipes up that are "long enough"
    float xdiff = (stop.x-start.x);
    if (elapsed_millis(_rotateTime,current) < EVENT_SWIPE_TIME) {
        float thresh = EVENT_SWIPE_LENGTH;
        if (xdiff > thresh) {
            return 1;
        } else if (xdiff < thresh) {
            return -1;
        }
    }
    return 0;
}