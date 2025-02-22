/*
 * Sound effects header for Cubior
 * by Brian Handy
 * 10/03/12
 * Sfx class for 3d platformer
 */

#ifndef SFX
#define	SFX

#include <cstdio> // for argv and argc

#include "gameplay.h"

  void initSfx(int, char**);
  void sfxLoop();
  void playTestSfx();
  void cycleSoundVolume();
  void resetVolumes();
  int getSoundVolume();
  int getSoundVolumeNum();

#endif