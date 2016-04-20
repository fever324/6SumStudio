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
    
    static void setBackgroundVolume(float bg_vol);
    
    static void setEffectVolume(float effect_vol);
    
    static void play();
    
public:
    /* A method to preload all required sound assets, set volume and deploy */
    static void audioDeploy(float bg_vol, float effect_vol);
    
    static void audioBackgroundDeploy(float bg_vol);
    
    static void audioEffectDeploy(float effect_vol);
    
    static void audioTerminate();
    
    static void audioPauseAll();
    
    static void audioResumeAll();
    
    // Effect 
    static void playEffect(const char* effect);
    
    static void playEffect(const char* effect, float gain);
    
    static void playDestroyEffect();
    
    static void playFreezeEffect();
    
    // Death
    static void playDeathEffect();
    
    // Win
    static void playWinEffect();
    
    // Potion/Star
    static void playPickupPotion();
    
    static void playPickupStar();
};

#endif /* ToyaAudioController */
