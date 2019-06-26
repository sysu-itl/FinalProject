#include "Music.h"

Music* Music::instance = nullptr;

Music* Music::getInstance() 
{
	// 单例模式
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
	// 初始化音乐引擎
	experimental::AudioEngine::lazyInit();
	// 加载音乐资源
	experimental::AudioEngine::preload("music/bgm1.mp3");
	experimental::AudioEngine::preload("music/die.mp3");
	experimental::AudioEngine::preload("music/jump.mp3");
	experimental::AudioEngine::preload("music/eatgold.mp3");
	experimental::AudioEngine::preload("music/eatshoe.mp3");
	// 读取音量设置
	Dictionary* settingDic = SettingScene::getSettingDict();
	int MusicVolumnPercentage = settingDic->valueForKey("MusicVolumn")->intValue();
	int EffectVolumnPercentage = settingDic->valueForKey("EffectVolumn")->intValue();
	MusicVolumn = MusicVolumnPercentage / 100.0;
	EffectVolumn = EffectVolumnPercentage / 100.0;
	// 分离背景音乐和特效音乐
	backgroudSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
	effectSongId = experimental::AudioEngine::INVALID_AUDIO_ID;

}

// 0: die, 1: jump, 2: eat gold, 3: eat shoe 
void Music::playMusic(int mode) 
{
	// 选择性播放音频资源
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
	// 设置音频音量
	setEffectVolume(EffectVolumn);
}

void Music::setEffectVolume(float volumn) 
{
	// 设置特效音乐音量
	EffectVolumn = volumn ;
	experimental::AudioEngine::setVolume(effectSongId, EffectVolumn);
}

void Music::setBackgroundVolume(float volumn)
{
	// 设置背景音乐音量
	MusicVolumn = volumn;
	experimental::AudioEngine::setVolume(backgroudSongId, MusicVolumn);
}

void Music::stopCurrentEffectMusic() 
{
	// 停止当前特效音乐
	experimental::AudioEngine::stop(this->effectSongId);
	this->effectSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
}

void Music::playBackgroundMusic() 
{
	// 播放背景音乐
	backgroudSongId = experimental::AudioEngine::play2d("music/bgm1.mp3", true);
	setBackgroundVolume(MusicVolumn );
}

void Music::pauseBackgroundMusic() 
{
	// 暂停背景音乐
	experimental::AudioEngine::pause(backgroudSongId);
}

void Music::stopBackgroundMusic() 
{
	// 停止背景音乐
	experimental::AudioEngine::stop(backgroudSongId);
	backgroudSongId = experimental::AudioEngine::INVALID_AUDIO_ID;
}