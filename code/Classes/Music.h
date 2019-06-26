#pragma once

#include "cocos2d.h"
#include "AudioEngine.h"
#include "SettingScene.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;


class Music
{
public:
	static Music* getInstance();
	void preloadMusic();
	void playMusic(int mode);
	void setEffectVolume(float volumn);
	void setBackgroundVolume(float volumn);
	void stopCurrentEffectMusic();
	void playBackgroundMusic();
	void pauseBackgroundMusic();
	void stopBackgroundMusic();

private:
	Music();
	static Music* instance;
	int backgroudSongId,
		effectSongId;
	float	MusicVolumn,
			EffectVolumn;
};