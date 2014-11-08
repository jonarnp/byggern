/*
 * spotifyControl.cpp
 *
 * Created: 08.11.2014 19:59:06
 *  Author: Flua
 */ 

#include "spotifyControl.h"

	
void SpotifyControl::next_song()
{
	Serial.println("next|");
}
	
void SpotifyControl::prev_song()
{
	Serial.println("prev|");
}
	
void SpotifyControl::play_pause()
{
	Serial.println("pause|");
}
