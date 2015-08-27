#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include "BASS/bass.h"
#include <map>

enum ChannelMapper
{
	BACKGROUND = 0,
	SHOOTS = 1,
	MENU = 2,
	EFFECTS = 3
};

enum SoundMapperShoots
{
	FIRE1 = 100,
	FIRE2 = 101,
	FIRE3 = 102,
	MISSIL = 103
};

enum SoundMenu
{
	LETTERMOVE = 300,
	MOUSEOVER = 301,
	LOSESONG = 302,
	WINSONG = 303
};


enum SoundMapperMusic
{
	BACKGROUND1 = 200,
	INTROSONG = 201,
	BACKDEF = 202
};

enum SoundEffects{
	TAKEOFF = 400,
	SWAPCAM = 401,
	MOTORS = 402,
	MSELECT = 403,
	EXPL1 = 404, 
	EXPL2 = 405,
	EXPL3 = 406,
	HIT = 407,
	ALARM = 408
};


class SoundManager{

static SoundManager * instance;

public: 
	std::multimap<int, HCHANNEL> channelsMap; //Cada canal es identificado por un entero
	std::multimap<int, std::pair<HSAMPLE, HCHANNEL>> soundsMap; //A cada sample le corresponde un int 
	int soundCounter;

	SoundManager():soundCounter(0)
	{
		newChannel(BACKGROUND);
		newChannel(SHOOTS);
		newChannel(MENU);
		newChannel(EFFECTS);
	}

	static SoundManager * Instance()
	{
		if(!instance)
		{
			BASS_Init(1, 44100, 0, 0, NULL);
			instance = new SoundManager();
		}

		return instance;
	}

	void pushSound(std::string _dir, int _soundid, int _channel, bool _loop=false )
	{
		HSAMPLE newSample;

		if(_loop)
			newSample = BASS_SampleLoad(false, _dir.c_str(),0,0,3, BASS_SAMPLE_LOOP);
		else
			newSample = BASS_SampleLoad(false, _dir.c_str(),0,0,3,0);

		auto it = channelsMap.find(_channel);
		soundsMap.insert(std::pair<int, std::pair<HSAMPLE, HCHANNEL>>(_soundid, std::pair<HSAMPLE, HCHANNEL>(newSample, it->second)));
	}

	void newChannel(int _channelid)
	{
		HCHANNEL newChannel;
		channelsMap.insert(std::pair<int, HCHANNEL>(_channelid, newChannel));
	}

	void playSound(int _index)
	{
		auto it = soundsMap.find(_index);
		HCHANNEL channel = it->second.second;
		channel = BASS_SampleGetChannel(it->second.first,false);
		BASS_ChannelPlay(channel, true);
	}
	void stopSound(int _index)
	{
		auto it = soundsMap.find(_index);
		BASS_SampleStop(it->second.first);
	}

	
};

#endif SOUNDMANAGER_H