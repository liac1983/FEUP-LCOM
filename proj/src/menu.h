#pragma once

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "controllers/i8254.h"
#include "controllers/mouse/mouse.h"
#include "game.h"
#include "button.h"

#include "images/background_menu.xpm"
#include "images/logo.xpm"
#include "images/name.xpm"
#include "images/pause_title.xpm"
#include "images/help_menu.xpm"
#include "images/pause_menu.xpm"
#include "images/won_menu.xpm"
#include "images/lost_menu.xpm"

/**
 * @brief Load Main Menu Background
 * @param load1 Boolean: 1 if this is the first time the Main Menu is loaded, 0 otherwise
 */

void LoadMenu(bool load1);

/**
 * @brief Loads Level Background
 */

void loadBackground();


/**
 * @brief Control of Interruptions of a given device related to the Main Menu
 * @param device Device that generated interrupts
 */

void menuIh(Device device);

/**
 * @brief Adds the specified XPM to the Level Bottom at the specified position
 * @param img XPM to add to Level Bottom
 * @param x Position of the x where the XPM will be added
 * @param y Position of the y where the XPM will be added
 */

void addToBackground(xpm_image_t * img, int x, int y);

/**
 * @brief Displays the Background of the Level on the screen
 */

void drawMenu();


/**
 * @brief Control of Interruptions of a given device related to the Help Menu
 * @param device Device that generated interrupts
 */

void helpMenuIh(Device device);

/**
 * @brief Brings the Help Menu Background to the screen
 */

void drawHelp();


/**
 * @brief Control of Interruptions of a given device related to the Pause Menu
 * @param device Device that generated interrupts
 */

void pauseMenuIh(Device device);

/**
 * @brief Load Pause Menu Background
 */

void loadPauseMenu();

/**
 * @brief Loads the Pause button
 */

void loadPause();

/**
 * @brief introduces the Pause button
 */

void drawPause();

/**
 * @brief Adds the specified XPM to the Pause Menu at the specified position
 * @param img XPM to add to Pause Menu
 * @param x Position of the x where the XPM will be added
 * @param y Position of the y where the XPM will be added
 */

void addToPause(xpm_image_t * img, int x, int y);
