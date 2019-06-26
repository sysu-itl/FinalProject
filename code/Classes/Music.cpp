#include "Music.h"
#include "SimpleAudioEngine.h"

Music* Music::instance = nullptr;

Music* Music::getInstance() {
	if (instance == nullptr) {
		instance = new Music();
	}
	return instance;
}

Music::Music() {
	this->volumn = 1.0f;
}

void Music::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/bgm1.mp3");
	audio->preloadEffect("music/die.mp3");
	audio->preloadEffect("music/jump.mp3");
	audio->preloadEffect("music/eatgold.mp3");
	audio->preloadEffect("music/eatshoe.mp3");
	audio->setBackgroundMusicVolume(this->volumn);
	audio->setEffectsVolume(this->volumn/2);
}

// 0: die, 1: jump, 2: eat gold, 3: eat shoe 
void Music::playMusic(int mode) {

	switch (mode)
	{
	case 0:
		this->songId = SimpleAudioEngine::getInstance()->playEffect("music/die.mp3");
		break;
	case 1:
		this->songId = SimpleAudioEngine::getInstance()->playEffect("music/jump.mp3");
		break;
	case 2:
		this->songId = SimpleAudioEngine::getInstance()->playEffect("music/eatgold.mp3");
		break;
	case 3:
		this->songId = SimpleAudioEngine::getInstance()->playEffect("music/eatshoe.mp3");
		break;
	default:
		break;
	}
}

void Music::stopCurrentEffectMusic() {
	SimpleAudioEngine::getInstance()->stopEffect(this->songId);
	this->songId = -1;
}
void Music::stopAllEffectMusic() {
	SimpleAudioEngine::getInstance()->stopAllEffects();
	this->songId = -1;
}

void Music::pauseCurrentEffectMusic() {
	SimpleAudioEngine::getInstance()->pauseEffect(this->songId);
}

void Music::pauseAllEffectMusic() {
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void Music::playBackgroundMusic() {
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm1.mp3", true);
}

void Music::pauseBackgroundMusic() {
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void Music::stopBackgroundMusic() {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}