#pragma once

#include <lcom/lcf.h>

#include "controllers/graphics/graphics.h"

#include "images/exit.xpm"
#include "images/play.xpm"
#include "images/help.xpm"
#include "images/continue.xpm"
#include "images/exit_highlighted.xpm"
#include "images/play_highlighted.xpm"
#include "images/help_highlighted.xpm"
#include "images/continue_highlighted.xpm"

typedef enum {BUTTON_EXIT, BUTTON_PLAY, BUTTON_HELP, BUTTON_CONTINUE} ButtonType;

/** @struct Button
 *  @brief Button struct
 *  @var Button::x
 *  Button x position
 *  @var Button::y
 *  Button y position
 *  @var Button::buttonImg
 *  Normal Button XPM (Cursor Outside It)
 *  @var Button::highlightedImg
 *  Highlighted Button XPM (Cursor Over It)
 *  @var Button::buttonType
 *  Button Type
 *  @var Button::mouseOver
 *  Boolean: 1 if the Cursor is over the Button and 0 otherwise
 */

typedef struct {
  unsigned int x, y;

  xpm_image_t buttonImg;
  xpm_image_t highlightedImg;

  ButtonType t_button;

  bool isMouse;

} Button;

/**
 *  @brief Allocate memory and create Button
 *  @param buttonType Type of Button to create
 *  @param x Position of the Button's x's
 *  @param y Position of the Button's y's
 *  @return Button created
 */

Button * createButton(ButtonType t_button, unsigned int x, unsigned int y);

/**
 *  @brief Displays On-Screen Button
 *  @param button Button to display on screen
 */
void drawButton(Button * button);
/**
 *  @brief Add Button to Background of Level
 *  @param button Button to add to Level Background
 */
void addButtonToBack(Button *button);
