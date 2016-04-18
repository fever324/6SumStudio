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

void AudioController::setBackgroundVolume(float bg_vol) {
    audio->setBackgroundMusicVolume(bg_vol);
}

void AudioController::setEffectVolume(float effect_vol) {
    audio->setEffectsVolume(effect_vol);
}

void AudioController::play() {
    audio->playBackgroundMusic(BG_SOUND, true);
}

void AudioController::audioBackgroundDeploy(float bg_vol) {
    preload();
    setBackgroundVolume(bg_vol);
    play();
}

void AudioController::audioEffectDeploy(float effect_vol) {
    preload();
    setEffectVolume(effect_vol);
}

void AudioController::audioDeploy(float bg_vol, float effect_vol) {
    preload();
    setBackgroundVolume(bg_vol);
    setEffectVolume(effect_vol);
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

void AudioController::playWinEffect() {
    playEffect(WIN_EFFECT);
}

//
void AudioController::playPickupPotion() {
    playEffect(PICKUP_MAGIC);
}

void AudioController::playPickupStar() {
    playEffect(PICKUP_MAGIC);
}

void AudioController::playEffect(const char* effect) {
    audio->playEffect(effect);
}

void AudioController::playEffect(const char* effect, float gain) {
    audio->playEffect(effect, false, 1.0f, 0.0f, gain);
}
