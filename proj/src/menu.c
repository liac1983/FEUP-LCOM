#include "menu.h"

extern uint16_t xRes, yRes;

static Button ** menuButtons;
static Button ** pauseButtons;

extern uint8_t scancode;
extern struct packet pack;

extern Cursor * cursor;
extern GameState gameState;
extern Level * level;



xpm_image_t background_menu;
xpm_image_t clean_background_menu;
xpm_image_t pause_menu;

void LoadMenu(bool load1) {
  loadBackground();
  cursor = loadCursor();
  menuButtons = (Button **) malloc(3 * sizeof(Button *));
  menuButtons[0] = createButton(BUTTON_PLAY, 38, 384);
  menuButtons[1] = createButton(BUTTON_HELP, 306, 384);
  menuButtons[2] = createButton(BUTTON_EXIT, 574, 384);
  static xpm_image_t name;
  xpm_load(name_xpm, XPM_8_8_8_8, &name);
  addButtonToBack(menuButtons[0]);
  addButtonToBack(menuButtons[1]);
  addButtonToBack(menuButtons[2]);
  addToBackground(&name, 250, 70);
  drawMenu();
}

void loadBackground() {
    xpm_load(background_menu_xpm, XPM_8_8_8_8, &background_menu);
    xpm_load(background_menu_xpm, XPM_8_8_8_8, &clean_background_menu);
}


void addToBackground(xpm_image_t * img, int x, int y) {
  uint32_t * map = (uint32_t *)background_menu.bytes;
  uint32_t * mapImg = (uint32_t *)img->bytes;


  for(int i = 0; i < img->width; i++) {
    for (int j = 0; j < img->height; j++) {
      if (*(mapImg + i + j*img->width) != xpm_transparency_color(XPM_8_8_8_8))
        *(map + (x + i) + (y + j) * xRes) = *(mapImg + i + j*img->width);
    }
  }
}

void drawMenu() {

  uint32_t* map = (uint32_t*) background_menu.bytes;

  for(unsigned int i = 0; i < background_menu.width; i++) {
    for (unsigned int j = 0; j < background_menu.height; j++) {
      vg_draw_pixel(i,j,*(map + i + j*xRes));
    }
  }
}


void menuIh(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool onPlay = false, onHelp = false, onExit = false;
  bool clickPlay = false, clickHelp = false, clickExit = false;

  switch (device) {
    case TIMER:

      switch(check_collision_main_menu()) {
        case 0:
          if (onPlay) {
            onPlay = false;
            menuButtons[0]->isMouse = false;
            addButtonToBack(menuButtons[0]);
            drawButton(menuButtons[0]);
          }
          else if (onHelp) {
            onHelp = false;
            menuButtons[1]->isMouse = false;
            addButtonToBack(menuButtons[1]);
            drawButton(menuButtons[1]);
          }
          else if (onExit) {
            onExit = false;
            menuButtons[2]->isMouse = false;
            addButtonToBack(menuButtons[2]);
            drawButton(menuButtons[2]);
          }
          break;
        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickExit = true;
            break;
          }
          if (!onExit) {
            onExit = true;
            menuButtons[2]->isMouse = true;
            addButtonToBack(menuButtons[2]);
            drawButton(menuButtons[2]);
          }
          break;
        case 2:
          if (mouseEvent->type == LB_RELEASED) {
            clickPlay = true;
            break;
          }
          if (!onPlay) {
            onPlay = true;
            menuButtons[0]->isMouse = true;
            addButtonToBack(menuButtons[0]);
            drawButton(menuButtons[0]);
          }
          break;
        case 3: 
          if (mouseEvent->type == LB_RELEASED) {
            clickHelp = true;
            break;
          }
          if (!onHelp) {
            onHelp = true;
            menuButtons[1]->isMouse = true;
            addButtonToBack(menuButtons[1]);
            drawButton(menuButtons[1]);
          }
          break;
      }
      break;
    case KEYBOARD:
      break;
    case MOUSE:
      mouseEvent = mouseEvents(&pack);
      mouse_update(&pack);
      break;
  }
  
  if(clickHelp) {
    gameState = HELP;
    drawHelp();
  } else if (clickPlay) {
    gameState = PLAY;
    LoadPlay(1,true);
  } else if (clickExit) {
    gameState = EXIT;}
}

void drawPause() {

  uint32_t* map = (uint32_t*) pause_menu.bytes;

  for(unsigned int i = 0; i < pause_menu.width; i++) {
    for (unsigned int j = 0; j < pause_menu.height; j++) {
      vg_draw_pixel(i,j,*(map + i + j*xRes));
    }
  }
}


void addToPause(xpm_image_t * img, int x, int y) {
  uint32_t * map = (uint32_t *)pause_menu.bytes;
  uint32_t * mapImg = (uint32_t *)img->bytes;


  for(int i = 0; i < img->width; i++) {
    for (int j = 0; j < img->height; j++) {
      if (*(mapImg + i + j*img->width) != xpm_transparency_color(XPM_8_8_8_8))
        *(map + (x + i) + (y + j) * xRes) = *(mapImg + i + j*img->width);
    }
  }
}

void loadPause() {
    xpm_load(pause_menu_xpm, XPM_8_8_8_8, &pause_menu);
}

void loadPauseMenu() {
  loadBackground();
  cursor = loadCursor();
  pauseButtons = (Button **) malloc(2 * sizeof(Button *));
  pauseButtons[0] = createButton(BUTTON_CONTINUE,150,384);
  pauseButtons[1] = createButton(BUTTON_EXIT,450,384);
  static xpm_image_t t;
  xpm_load(pause_title_xpm, XPM_8_8_8_8, &t);
  addButtonToBack(pauseButtons[0]);
  addButtonToBack(pauseButtons[1]);
  addToBackground(&t, 250, 200);
  drawMenu();
}

void pauseMenuIh(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool onContinue = false, onExit = false;
  bool clickContinue = false, clickExit = false;

  switch (device) {
    case TIMER:
      switch(check_collision_pause_menu()) {
        case 0:
          if (onContinue) {
            onContinue = false;
            pauseButtons[0]->isMouse = false;
            addButtonToBack(pauseButtons[0]);
            drawButton(pauseButtons[0]);
          } else if (onExit) {
            onExit = false;
            pauseButtons[1]->isMouse = false;
            addButtonToBack(pauseButtons[1]);
            drawButton(pauseButtons[1]);
          }
          break;
        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickContinue = true;
          } else if (!onContinue) {
            onContinue = true;
            pauseButtons[0]->isMouse = true;
            addButtonToBack(pauseButtons[0]);
            drawButton(pauseButtons[0]);
          }
          break;
        case 2:
          if (mouseEvent->type == LB_RELEASED) {
            clickExit = true;
          } else if (!onExit) {
            onExit = true;
            pauseButtons[1]->isMouse = true;
            addButtonToBack(pauseButtons[1]);
            drawButton(pauseButtons[1]);
          }
      }
      break;
    case KEYBOARD:
      if (scancode == ESC) {
        gameState = PLAY;
        LoadPlay(level->levelNum, false);
      }
      break;
    case MOUSE:
      mouseEvent = mouseEvents(&pack);
      mouse_update(&pack);
      break;
  }
  if (clickContinue){
    gameState = PLAY;
    LoadPlay(level->levelNum, false);
  } else if(clickExit) {
    gameState = MAINMENU;
    destroyLevel();
    LoadMenu(false);
  }
}

void drawHelp() {
  xpm_image_t img;
  xpm_load(help_menu_xpm, XPM_8_8_8_8, &img);

  uint32_t* map = (uint32_t*) img.bytes;

  for(unsigned int i = 0; i < img.width; i++) {
    for (unsigned int j = 0; j < img.height; j++) {
      vg_draw_pixel(i,j,*(map + i + j*xRes));
    }
  }
}



void helpMenuIh(Device device) {
  switch (device) {
    case TIMER:
      break;
    case KEYBOARD:
      if (scancode == ESC) {
        gameState = MAINMENU;
        LoadMenu(false);
      }
      break;
    case MOUSE:
      break;
  }
}


