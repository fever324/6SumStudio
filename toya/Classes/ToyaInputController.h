//
//  ToyaInputController.h
//  toya
//
//  Created by 6SumStudio on 2/27/16.
//
//

#ifndef __TOYA_INPUT_CONTROLLER_H__
#define __TOYA_INPUT_CONTROLLER_H__

#include <cocos2d.h>
#include <cornell/CUKeyboardPoller.h>
#include <cornell/CUAccelerationPoller.h>
#include <cornell/CUTouchListener.h>
#include "base/CCEventListenerTouch.h"

#define HISTORY_LENGTH 5

using namespace cocos2d;
using namespace std;

#pragma mark Polled Input
/**
 * Class to represent player input in the toya.
 *
 * This input handler uses the polling input API provided by the Cornell Extensions
 * (in contrast to the callback API required by Cocos2d).  However, there is some
 * callback functionality for touch support.  This allows us to handle gestures.
 *
 * Unlike handlers like the Keyboard poller, this class is not a singleton.  It
 * must be allocated before use.  However, you will notice that we do not do any
 * input initialization in the constructor.  This allows us to allocate this controller
 * as a field without using pointers. We simply add the class to the header file
 * of its owner, and delay initialization (via the method start()) until later.
 * This is one of the main reasons we like to avoid initialization in the constructor.
 */
class InputController {
private:
    // KEYBOARD EMULATION
    
    int _count = 0;
    /** Whether the left arrow key is down */
    float  _keyLeft;
    /** Whether the left arrow key is down */
    float  _keyRight;
    /** Whether the reset key is down */
    bool  _keyReset;
    /** Whether the debug key is down */
    bool  _keyDebug;
    /** Whether the exit key is down */
    bool  _keyExit;
    
    int swipe;
    
    // whether the start key is down
    bool _keyStart;
    
    
    
    /** whether it is rotating or not */
    bool _keyRotate;
    /** whether tap or not */
    bool _keyTap;
    
    /** The timestamp for the beginning of the current gesture */
    timestamp_t _timestamp;
    
    // current number of touch events
    int _touchCount;
    
    // rotation angles
    float _turning;
    // rotation angle for keyboard
    float _keyTurning;
    
    float inputHistory[HISTORY_LENGTH] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float inputHistoryWeights[HISTORY_LENGTH] = {0.7f, 0.2f, 0.07f, 0.02f, 0.01f};
    float _smoothedTurning;
    
protected:
    // EVENT LISTENERS
    /** Listener to process touch events */
    TouchListener* _touchListener;
    
    /** Whether or not this controller is currently active. */
    bool _active;
    
    // Input results
    /** Whether the reset action was chosen. */
    bool _resetPressed;
    /** Whether the debug toggle was chosen. */
    bool _debugPressed;
    /** Whether the exit action was chosen. */
    bool _exitPressed;
    
    bool _released;
    
    bool _select;
    Vec2 _ctouch;

#pragma mark Internal Touch Management
    
    /** Information representing a single "touch" (possibly multi-finger) */
    struct TouchInstance {
        /** The current touch position */
        Vec2 start;
        /** The touch id for future reference */
        int  touchid;
        /** the start and stop of this position */
        Vec2 stop;
    };
    
    /** The current touch location for the left zone */
    TouchInstance _touch1;
    /** The current touch location for the right zone */
    TouchInstance _touch2;
    
    /** The timestamp for the beginning of the current swipe gesture */
    timestamp_t _rotateTime;
    
    /** check whether it is a rotation or not **/
    bool checkRotate(timestamp_t current);
    bool checkTap(timestamp_t current);
    int  checkSwipe(const Vec2& start, const Vec2& stop, timestamp_t current);
    void resetTouch(TouchInstance* t);
    
    
public:
    /**
     * Creates a new input controller.
     *
     * This constructor does NOT do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    InputController(); // Don't initialize.  Allow stack based
    
    /**
     * Disposes of this input controller, releasing all listeners.
     */
    ~InputController();
    
    /**
     * Initializes the input control for the given drawing scale.
     *
     * This method works like a proper constructor, initializing the input
     * controller and allocating memory.  However, it still does not activate
     * the listeners.  You must call start() do that.
     *
     * @return true if the controller was initialized successfully
     */
    bool init();
    
    /**
     * Starts the input processing for this input controller.
     *
     * This method must be called AFTER the input controller is initialized
     */
    void    start();
    
    /**
     * Stops the input processing for this input controller.
     *
     * This method will not dispose of the input controller. It can be restarted.
     */
    void    stop();
    
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
    void  update(float dt);
    
    /**
     * Clears any buffered inputs so that we may start fresh.
     */
    void clear();
    
    
#pragma mark -
#pragma mark Input Results
    
    /**
     * Returns true if the reset button was pressed.
     *
     * @return true if the reset button was pressed.
     */
    bool didReset() const { return _resetPressed; }
    
    bool didTap() const { return _keyTap; }
    
    /* return true if it is a rotation */
    bool didRotate() const { return _keyRotate; }
    
    // return status of start
    bool didStart() const { return _keyStart; }
    
    bool didSwipe() const {return swipe != 0; }
    
    int getSwipeDirection() {int temp = swipe; swipe = 0; return temp;}
    /** return turning */
    float getTurning() const { return _smoothedTurning; }
    
    /** return gravity */
    Vec2 getGravity(Vec2& gravity, float rotation) const {
        Vec2 newGravity;
        if (rotation >= 0) {
            newGravity.x = -gravity.x * sin(rotation*3.1415/180);
            newGravity.y = gravity.y * cos(rotation*3.1415/180);
        } else {
            newGravity.x = -gravity.x * sin(rotation*3.1415/180);
            newGravity.y = gravity.y * cos(rotation*3.1415/180);
        }
        
        return newGravity;
    }
    
    void calculateNewSmoothedTurning(float newTurning);
    
    /**
     * Returns true if the player wants to go toggle the debug mode.
     *
     * @return true if the player wants to go toggle the debug mode.
     */
    bool didDebug() const { return _debugPressed; }
    
    /**
     * Returns true if the exit button was pressed.
     *
     * @return true if the exit button was pressed.
     */
    bool didExit() const { return _exitPressed; }
    
    bool didSelect() const { return _select; }
    bool didRelease() const {return _released; }
    bool setRelease(bool r) {_released = r;}
    bool setActive(bool value) {_active = value;}
    const Vec2& getSelection() const { return _ctouch; }
    
    
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
    bool    touchBeganCB(Touch* t, timestamp_t time);
    
    /**
     * Callback for the end of a touch event
     *
     * @param t     The touch information
     * @param event The associated event
     */
    void    touchEndedCB(Touch* t, timestamp_t time);
    
    /**
     * Callback for a touch movement event
     *
     * @param t     The touch information
     * @param event The associated event
     */
    void    touchMovedCB(Touch* t, timestamp_t time);
    
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
    void    touchCancelCB(Touch* t, timestamp_t time);
};


#endif /* __TOYA_INPUT_CONTROLLER_H__ */
