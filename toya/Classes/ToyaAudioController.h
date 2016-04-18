/**
 * ToyaAudioController.h
 *
 * Created by sb2345 on 4/17/16.
 */
#ifndef __TOYA_AUDIO_CONTROLLER_H__
#define __TOYA_AUDIO_CONTROLLER_H__

#include <string>
#include <iostream>
#include <sstream>

// This is audio part required
#include <cornell/CUGenericLoader.h>
#include <cornell/CUSoundEngine.h>
#include <cornell/CUSound.h>
#include <SimpleAudioEngine.h>

#include "Constants.h"

using namespace cocos2d;
using namespace std;

class AudioController {
private:
    static void preload();
    
    static void setVolume(float bg_vol, float effect_vol);
    
    static void play();
    
public:
    static void audioDeploy(float bg_vol, float effect_vol);
    
    static void audioTerminate();
    
    static void playDestroyEffect();
    
    static void playFreezeEffect();
    
    static void playDeathEffect();
    
    static void playPickupEffect();
    
    static void playEffect(const char* effect);
    
};

#endif /* ToyaAudioController */
