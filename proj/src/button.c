#include "button.h"

extern xpm_image_t background_menu;
extern xpm_image_t pause_menu;
extern unsigned int xRes, yRes;

Button * createButton(ButtonType t_button, unsigned int x, unsigned int y) {
  Button * button = (Button *) malloc (sizeof(Button));

  button->x = x;
  button->y = y;
  button->isMouse = false;
  button->t_button = t_button;

  switch (t_button) {
    case BUTTON_EXIT:
      xpm_load(exit_xpm, XPM_8_8_8_8, &button->buttonImg);
      xpm_load(exit_highlighted_xpm, XPM_8_8_8_8, &button->highlightedImg);
      break;
    case BUTTON_PLAY:
      xpm_load(play_xpm, XPM_8_8_8_8, &button->buttonImg);
      xpm_load(play_hightlighted, XPM_8_8_8_8, &button->highlightedImg);
      break;
    case BUTTON_HELP:
      xpm_load(help_xpm, XPM_8_8_8_8, &button->buttonImg);
      xpm_load(help_highlighted_xpm, XPM_8_8_8_8, &button->highlightedImg);
      break;
    case BUTTON_CONTINUE:
      xpm_load(continue_xpm, XPM_8_8_8_8, &button->buttonImg);
      xpm_load(continue_highlighted_xpm, XPM_8_8_8_8, &button->highlightedImg);
      break;
  }

  return button;
}

void drawButton(Button * button) {
  uint32_t * map;
  map = (uint32_t *) (button->isMouse ? button->highlightedImg.bytes : button->buttonImg.bytes);

  for(int i = 0; i < (button->isMouse ? button->highlightedImg.width : button->buttonImg.width); i++) {
    for (int j = 0; j < (button->isMouse ? button->highlightedImg.height : button->buttonImg.height); j++) {
      if (*((uint32_t*)(map + i + j * (button->isMouse ? button->highlightedImg.width : button->buttonImg.width))) != xpm_transparency_color(XPM_8_8_8_8)) {
        vg_draw_pixel(button->x+i, button->y+j, *((uint32_t*)(map + i + j * (button->isMouse ? button->highlightedImg.width : button->buttonImg.width))));
      }
    }
  }
}

void addButtonToMenu(Button *button, uint32_t *map) {
  uint32_t *buttonMap = (uint32_t *) (button->isMouse ? button->highlightedImg.bytes : button->buttonImg.bytes);

  for(int i = 0; i < (button->isMouse ? button->highlightedImg.width : button->buttonImg.width); i++) {
    for (int j = 0; j < (button->isMouse ? button->highlightedImg.height : button->buttonImg.height); j++) {
      if (*((uint32_t*)(buttonMap + i + j * (button->isMouse ? button->highlightedImg.width : button->buttonImg.width))) != xpm_transparency_color(XPM_8_8_8_8)) {
        *(map + (button->x + i) + (button->y + j) * xRes) = *((uint32_t*)(buttonMap + i + j * (button->isMouse ? button->highlightedImg.width : button->buttonImg.width)));
      }
    }
  }
}

void addButtonToBack(Button *button) {
  addButtonToMenu(button, (uint32_t *) background_menu.bytes);
}
