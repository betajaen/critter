#ifndef CRITTER_CONFIGURATION
#define CRITTER_CONFIGURATION

/*
  Add functions to make use of the Ogre terrain system.
  Set 1 to enable
  Set 0 to disable

  This feature is only available for Ogre 1.7.0+
*/
#define Critter_UsesOgreTerrain 0

/*
  When a Character Controller's Y value is below this number, an displacement
  vector should be given to move the character out of that range.
  
  Set 1 to enable
  Set 0 to disable
  
  This feature is only available if the Character System is enabled.
*/
#define Critter_UsesCharacterMinimumYThreshold 1
#define Critter_UsesCharacterThresholdRange -5
#define Critter_UsesCharacterThresholdDisplacement 5


/* 
  Maximum number of seperate animaton states applied to an Ogre Entity.
*/
#define CRITTER_MAX_ANIMATION_SECTIONS 4

#endif