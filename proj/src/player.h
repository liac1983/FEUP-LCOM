#pragma once

#define MAKE_ARROW_UP 0x48
#define MAKE_ARROW_DOWN 0x50
#define MAKE_ARROW_RIGHT 0x4D
#define MAKE_ARROW_LEFT 0x4B
#define BREAK_ARROW_UP 0xC8
#define BREAK_ARROW_DOWN 0xD0
#define BREAK_ARROW_RIGHT 0xCD
#define BREAK_ARROW_LEFT 0xCB

#include <lcom/lcf.h>

#include "controllers/graphics/graphics.h"
#include "controllers/keyboard/keyboard.h"
#include "level.h"
#include "game.h"

#include "images/player.xpm"

/** @structPlayer
 * @brief Struct relative to Player
 * @var Player::img
 * Player XPM
 * @var Player::x
 * Position of the player's x's
 * @var Player::y
 * Player y position
 * @var Player::speed
 * Player Speed
 * @var Player::Candy
 * Boolean: 1 if the Player has the candy, 0 otherwise
 * @var Player::direction
 * Direction the Player is facing
 */

typedef struct {
  xpm_image_t img;

  int x, y;
  int speed;
  int candy;
  
  Direction direction;
} Player;

/**
 * @brief Allocate memory and create the Player
 * @param playerNum Player Number
 * @return Player created
 */

Player * createPlayer ();


/**
 * @brief Displays the Player on screen
 * @param player Player to display on screen
 */

void drawPlayer(Player * player);

/**
 * @brief Remove Player from screen
 * @param player Player to remove from screen
 */

void cleanPlayer(Player * player);

/**
 * @brief Player Direction Control
 * @param player Player controlling direction
 * @param scancode Scancode received from Keyboard
 * @param up Boolean: 1 if Player is face Up, 0 otherwise
 * @param down Boolean: 1 if Player is facing Down, 0 otherwise
 * @param left Boolean: 1 if Player is facing Left, 0 otherwise
 * @param right Boolean: 1 if Player is facing Right, 0 otherwise
 */

void UpdateMoveDir(Player * player, uint8_t scancode, bool * up, bool * down, bool * left, bool * right);

/**
 * @brief Player Movement Control
 * @param player Player controlling movement
 * @param upPtr Boolean: 1 if Player faces Up, 0 otherwise
 * @param downPtr Boolean: 1 if Player faces Down, 0 otherwise
 * @param leftPtr Boolean: 1 if Player turns Left, 0 otherwise
 * @param rightPtr Boolean: 1 if Player turns Right, 0 otherwise
 */

void checkMove(Player * player, bool up, bool down, bool left, bool right);

/**
 * @brief Checks if Player collides with Walls
 * @param player Player checking for collisions
 * @param direction Direction the Player is facing
 * @return Boolean: 1 if there are collisions, 0 otherwise
 */

bool checkCollWithWalls(Player * player, Direction direction);

/**
 * @brief Checks if the Player collides with the candys
 * @param player Player checking for collisions
 * @param direction Direction the Player is facing
 * @return Boolean: 1 if there are collisions, 0 otherwise
 */
 
bool checkCollWithCandy(Player * player);
