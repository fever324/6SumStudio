//
//  ToyaAudioController.cpp
//
//  Created by sb2345 on 4/17/16.
//

#include "ToyaAudioController.h"

auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

void AudioController::preload() {
    audio->preloadBackgroundMusic(BG_SOUND);
    audio->preloadEffect(DESTROY_EFFECT);
    audio->preloadEffect(DEATH_SOUND);
    audio->preloadEffect(PICKUP_MAGIC);
    audio->preloadEffect(FREEZE_EFFECT);
}

void AudioController::setVolume(float bg_vol, float effect_vol) {
    audio->setBackgroundMusicVolume(bg_vol);
    audio->setEffectsVolume(effect_vol);
}

void AudioController::play() {
    audio->playBackgroundMusic(BG_SOUND, true);
}

void AudioController::audioDeploy(float bg_vol, float effect_vol) {
    preload();
    setVolume(bg_vol, effect_vol);
    play();
}

void AudioController::audioTerminate() {
    audio->stopBackgroundMusic();
}

void AudioController::playDestroyEffect() {
    audio->playEffect(DESTROY_EFFECT);
}

void AudioController::playFreezeEffect() {
    audio->playEffect(FREEZE_EFFECT);
}

void AudioController::playDeathEffect() {
    audio->playEffect(DEATH_SOUND);
}

void AudioController::playPickupEffect() {
    audio->playEffect(PICKUP_MAGIC);
}

void AudioController::playEffect(const char* effect) {
    audio->playEffect(effect);
}


