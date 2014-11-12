/*
 * spotifyControl.h
 *
 * Created: 08.11.2014 19:58:19
 *  Author: Flua
 */ 


#ifndef SPOTIFYCONTROL_H_
#define SPOTIFYCONTROL_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class SpotifyControl
{
	protected:


	public:
	void init();
	
	void next_song();
	
	void prev_song();
	
	void play_pause();
};

#endif /* SPOTIFYCONTROL_H_ */