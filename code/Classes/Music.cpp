#include "Music.h"

Music* Music::instance = nullptr;

Music* Music::getInstance() 
{
	if (instance == nullptr) {
		instance = new Music();
	}
	return instance;
}


Music::Music() 
{
	
}

void Music::preloadMusic() 
{
	experimental::AudioEngine::lazyInit();
	
	experimental::AudioEngine::preload("music/bgm1.mp3");
	experimental::AudioEngine::preload("music/die.mp3");
	experimental::AudioEngine::preload("music/jump.mp3");
	experimental::AudioEngine::preload("music/eatgold.mp3");
	experimental::AudioEngine::preload("music/eatshoe.mp3");

	Dictionary* settingDic = SettingScene::getSettingDict();
	int MusicVolumnPercentage = settingDic->valueForKey("MusicVolumn")->intValue();
	int EffectVolumnPercentage = settingDic->valueForKey("EffectVolumn")->intValue();
	MusicVolumn = MusicVolumnPercentage / 100.0;
	EffectVolumn = EffectVolumnPercentage / 100.0;

	backgroudSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
	effectSongId = experimental::AudioEngine::INVALID_AUDIO_ID;

}

// 0: die, 1: jump, 2: eat gold, 3: eat shoe 
void Music::playMusic(int mode) 
{
	switch (mode)
	{
	case 0:
		this->effectSongId = experimental::AudioEngine::play2d("music/die.mp3");
		break;
	case 1:
		this->effectSongId = experimental::AudioEngine::play2d("music/jump.mp3");
		break;
	case 2:
		this->effectSongId = experimental::AudioEngine::play2d("music/eatgold.mp3");
		break;
	case 3:
		this->effectSongId = experimental::AudioEngine::play2d("music/eatshoe.mp3");
		break;
	default:
		break;
	}
	setEffectVolume(EffectVolumn);
}

void Music::setEffectVolume(float volumn) 
{
	EffectVolumn = volumn ;
	experimental::AudioEngine::setVolume(effectSongId, EffectVolumn);
}

void Music::setBackgroundVolume(float volumn)
{
	MusicVolumn = volumn;
	experimental::AudioEngine::setVolume(backgroudSongId, MusicVolumn);
}

void Music::stopCurrentEffectMusic() 
{
	experimental::AudioEngine::stop(this->effectSongId);
	this->effectSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
}



void Music::playBackgroundMusic() 
{
	backgroudSongId = experimental::AudioEngine::play2d("music/bgm1.mp3", true);
	setBackgroundVolume(MusicVolumn );
}

void Music::pauseBackgroundMusic() 
{
	experimental::AudioEngine::pause(backgroudSongId);
	
}

void Music::stopBackgroundMusic() 
{
	experimental::AudioEngine::stop(backgroudSongId);
	backgroudSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
}