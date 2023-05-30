#pragma once

typedef enum {TIMER, KEYBOARD, MOUSE} Device;

typedef enum {MAINMENU, PLAY, LOST, WON, PAUSEMENU, HELP, EXIT} GameState;

#include <lcom/lcf.h>
#include "controllers/graphics/graphics.h"
#include "controllers/graphics/graphics_const.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/i8042.h"
#include "controllers/i8254.h"
#include "level.h"
#include "player.h"
#include "menu.h"

/**
 *  @brief Main Game Loop
 *  @return 0 on success, 1 otherwise
 */

int (GameMainLoop)();

/**
 *  @brief Mouse Control as per Received Mouse Packet
 *  @param pack Mouse Packet with Mouse information
 *  @return  Event type caused by received Mouse Packet
 */

struct mouse_ev * mouseEvents(struct packet * pack);

/**
 *  @brief General interrupt control for any device
 *  @param device Device that generated interrupts
 */

void Interrupt(Device device);

/**
 *  @brief Play Interrupt Control for any device
 *  @param device Device that generated interrupts
 */

void PlayInterruptHandler(Device device);

/**
 *  @brief Load state of a Level
 *  @param levelNum Level Number
 *  @param begin Boolean: 1 if the Level is in the initial state, 0 otherwise
 */

void LoadPlay(unsigned int levelNum, bool begin);

/**
 *  @brief 
 *  @return Boolean: 1 if Player is near the End Zone, 0 otherwise
 */

bool checkFinalPosition(unsigned int levelNum);

/**
 *  @brief Load Won Menu
 */

void LoadWonMenu();

/**
 *  @brief Load Lost Menu
 */

void LoadLostMenu();

/**
 *  @brief Won Menu Interrupt Control for any device
 *  @param device Device that generated interrupts
 */

void gameWonIh(Device device);

/**
 *  @brief Won Menu Interrupt Control for any device
 *  @param device Device that generated interrupts
 */

void gameLostIh(Device device);
