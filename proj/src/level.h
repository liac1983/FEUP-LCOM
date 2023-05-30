#pragma once

#include <lcom/lcf.h>

#include "controllers/graphics/graphics.h"
#include "game.h"

#include "images/level1.xpm"
#include "images/level2.xpm"
#include "images/level3.xpm"
#include "images/candy.xpm"


/** @struct Candy
 * @brief Struct relative to Candy
 * @var Candy::img
 * Candy's XPM
 * @var Candy::x
 * Position of Candy's x's
 * @var Candy::y
 * Candy y position
 */

typedef struct {
  xpm_image_t img;
  int x;
  int y;
} Candy;

/** @struct Level
 * @brief Struct relative to Level
 * @var Level::levelnum
 * Level Number
 * @var Level::level_background
 * Level Bottom XPM
 * @var Level::level_back
 * Pointer to Level Background Bytes
 * @var Level::CandyList
 * Array of all existing Candy objects in the Level
 * @var Level::numCandy
* Number of Candy objects in the Level
 */

typedef struct {
  unsigned int levelNum;

  xpm_image_t background;

  uint32_t * level_back;

  Candy ** candyList;
  int numCandy;

} Level;

/**
 * @brief Allocate memory and create a Candy
 * @param x Candy x position
 * @param y Candy y position
 * @return Candy created
 */

Candy * createCandy(int x, int y);

/**
 * @brief Loads the Level specified in the parameter
 * @param levelNum Level number to load
 * @return Level loaded
 */

Level * loadLevel(unsigned int levelNum);

void loadLevelBack(unsigned int levelNum);

/**
 * @brief Frees the memory occupied by the Level
 */

void destroyLevel();

/**
 * @brief Displays the background of the Level on the screen
 */

void drawLevelBackground();

/**
 * @brief Displays all Candy in the Tier on the screen
 */

void drawCandyLevel();


/**
 * @brief Removes candy from the screen
 * @param doors candys present on screen
 */

void cleanCandy(Candy * candy);
