/*
 * Music for Cubior
 * by Brian Handy
 * (Music itself by Waterflame)
 * 10/12/12
 * Music class for 3d platformer
 */

#include <cstdio> // for argv and argc
#include <SFML/Audio.hpp> // for sound manipulation

#include "music.h"
#include "gameplay.h"

// Again, not sure on these either (gameplay copies)
#include <cmath> // for trig stuff with getAngleBetween
#include <iostream>
#include <cstdio>
#include <stdlib.h> // for NULL
#include <string> // for loading a level by var passed

#include "ResourcePath.hpp" // to load (in XCode for Mac) from app's resource folder using the SFML file (combined with ResourcePath.mm)

// Buffer Vars (hold song data)
sf::Music music;
bool mute = true; //false;
bool lastMute = mute;
int defaultVolume = 50; // 40 Sounds really good! Might be a hint too quiet? 50 is perfect!

// List of songs
// Extra path ensures Resources folder is accessed for .app on Mac
#ifdef __APPLE_CC__
  string songs[] = { resourcePath() + "./music/Waterflame_MakeARunForIt+Cats+Orange.ogg" };
#else
  string songs[] = { "./music/Waterflame_MakeARunForIt+Cats+Orange.ogg" };
#endif
  //"./music/Waterflame_MakeARunForIt.ogg",
  //                 "./music/Waterflame_Cats.ogg",
  //                 "./music/Waterflame_Orange.ogg"};
int songCount = 1;
int currentSong = 0;

int deltaVolume = 10;
int minVolume = 0;
int maxVolume = 100;
int currentVolume = defaultVolume;

// Setup for sound effects
void initMusic(int argc, char** argv) {
    
  playSong(currentSong);
}

void playSong(int i) {
  // Load sounds into buffers
  music.openFromFile(songs[i]);
  music.setLoop(true);
  music.setVolume(currentVolume);
  
  // And setup sound players
  if (!mute) { music.play(); }
}

// Main loop for sound effects, called once per cycle
void musicLoop() {
  // Just unmuted? Play!
  if (!mute && lastMute) {
    music.play();
  }

  // Finally, if needed, update lastMute
  if (lastMute != mute) {
    lastMute = mute;
  }
}

void setMute(bool b) {
  mute = b;
}

void toggleMute() {
  mute = !mute;
}

void nextSong() {
  // Push song up past song count, but if already there, loop
  if (currentSong == songCount) {
    currentSong = 0;
  } else {
    currentSong++;
  }
  // If past song count (or equal to it) play no music
  // Otherwise, start the next song!
  if (currentSong == songCount) {
    music.stop();
  } else {
    playSong(currentSong);
  }
}

void increaseMusicVolume() { 
  if (currentVolume < maxVolume) {
    currentVolume += deltaVolume;
  } else if (currentVolume != maxVolume) {
      currentVolume = maxVolume;
  }
  music.setVolume(currentVolume);
}

void decreaseMusicVolume() { 
  if (currentVolume > minVolume) {
    currentVolume -= deltaVolume;
  } else if (currentVolume != minVolume) {
      currentVolume = minVolume;
  }
  music.setVolume(currentVolume);
}