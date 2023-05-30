#include "game.h"

extern unsigned int counter;
extern uint8_t scancode;
struct packet pack;
struct mouse_ev mouseEvent;
extern uint16_t xRes, yRes;

static Player * player;
extern Level * level;
extern Cursor * cursor;
GameState gameState = MAINMENU;
extern xpm_image_t background_menu;
static Button * exitButton;

int GameMainLoop() {
  int ipc_status,r;
  message msg;

  uint32_t irqSetTimer = BIT(TIMER0_IRQ);
  uint32_t irqSetKbd = BIT(KBD_IRQ);
  uint32_t irqSetMouse = BIT(MOUSE_IRQ);
  uint8_t timerBitNo, kbdBitNo, mouseBitNo;

  bool KbdReadSecond = false;
  bool MouseReadSecond = false, MouseReadThird = false;

  uint8_t kbd_bytes[2];

  uint8_t msBytes[3];

  LoadMenu(true);

  if (timer_subscribe_int(&timerBitNo) != 0) return 1;
  if (kbd_subscribe_int(&kbdBitNo) != 0) return 1;
  if (mouse_enable_data_report() != 0) return 1;
  if (mouse_subscribe_int(&mouseBitNo) != 0) return 1;

  while(gameState != EXIT) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
            if (msg.m_notify.interrupts & irqSetKbd) {
              kbc_ih();
              if (KbdReadSecond) {
                KbdReadSecond = false;
                kbd_bytes[1] = scancode;
              }
              else {
                kbd_bytes[0] = scancode;
                if (scancode == SIZE) {
                  KbdReadSecond = true;
                }
              }
              Interrupt(KEYBOARD);
            }
            if (msg.m_notify.interrupts & irqSetMouse) {
              mouseIh();
              if (!MouseReadSecond && !MouseReadThird && (scancode & BIT(3))){
                msBytes[0] = scancode;
                MouseReadSecond = true;
              }
              else if (MouseReadSecond) {
                msBytes[1] = scancode;
                MouseReadThird = true;
                MouseReadSecond = false;
              }
              else if (MouseReadThird) {
                msBytes[2] = scancode;
                MouseReadThird = false;
                for(unsigned int i = 0; i < 3; i++)
                  pack.bytes[i] = msBytes[i];
                buildPacket(&pack);
                Interrupt(MOUSE);
              }
            }
            if (msg.m_notify.interrupts & irqSetTimer) {
              timer_int_handler();
              Interrupt(TIMER);
            } 
            break;
        default:
          break; 
      }
    }
  }

  if (mouse_unsubscribe_int() != 0) return 1;
  if (mouse_disable_data_report() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (timer_unsubscribe_int() != 0) return 1;

  free(level);

  return 0;
}

void Interrupt(Device device) {
  switch (gameState){
    case MAINMENU:
      menuIh(device);
      break;
    case PAUSEMENU:
      pauseMenuIh(device);
      break;
    case HELP:
      helpMenuIh(device);
      break;
    case PLAY:
      PlayInterruptHandler(device);
      break;
    case WON:
      gameWonIh(device);
      break;
    case LOST:
      gameLostIh(device);
      break;
    case EXIT:
      break;
  }
}

void PlayInterruptHandler(Device device) {
  srand(time(NULL));
  static bool up = false, down = false, left = false, right = false;

  switch (device) {
    case TIMER:
      if (checkFinalPosition(level->levelNum)) {
        int n = 0;
        for (unsigned int i = 0; i < level->levelNum; i++) {
          if (level->candyList[i]->x != -1) {
            n++;
          }
        }
        if (n == 0) {
          LoadPlay(level->levelNum + 1, true);
        } else {
          LoadPlay(6, true);
        }
        break;
      }
      checkMove(player, up, down, left, right);
      break;
    case KEYBOARD:
      if (scancode == ESC) {
        gameState = PAUSEMENU;
        loadPauseMenu();
        break;
      }
      UpdateMoveDir(player, scancode, &up, &down, &left, &right);
      break;
    case MOUSE:
      break;
  }
}

void LoadPlay(unsigned int levelNum, bool begin) {
  if (begin) {
    switch (levelNum) {
      case 1:
        player = createPlayer();
        player->direction = UP;
        player->x = 60;
        player->y = 540;
        break;
      case 2:
        player = createPlayer();
        player->x = 30;
        player->y = 280;
        break;
      case 3:
        player = createPlayer();
        player->x = 60;
        player->y = 540;
        break;
      case 4:
        gameState = WON;
        free(player);
        LoadWonMenu();
        break;
      case 6:
        gameState = LOST;
        free(player);
        LoadLostMenu();
        break;
      default:
        break;
    }

    if (gameState == PLAY) {
      level = loadLevel(levelNum);
    }
  }
  else {
    drawLevelBackground();
    drawPlayer(player);
  }

  scancode = 0x00;
}

bool checkFinalPosition(unsigned int levelNum) {
  switch (levelNum) {
  case 1:
    if (player-> y < 27) return true;
    break;
  case 2:
    if (player-> x + player->img.width > 781) return true;
    break;
  case 3:
    if (player-> y < 27) return true;
    break;
  case 4:
    if (player-> y + player->img.height > 573) return true;
    break;
  }
  return false;
}

struct mouse_ev * mouseEvents(struct packet * pack) {
  static bool lbPressed = false, rbPressed = false, mbPressed = false;

  mouseEvent.delta_x = pack->delta_x;
  mouseEvent.delta_y = pack->delta_y;

  if (!lbPressed && pack->lb && !rbPressed && !mbPressed && !pack->mb && !pack->rb) {
    lbPressed = true;
    mouseEvent.type = LB_PRESSED;
  }
  else if (!rbPressed && pack->rb && !lbPressed && !mbPressed && !pack->mb && !pack->lb) {
    rbPressed = true;
    mouseEvent.type = RB_PRESSED;
  }
  else if (lbPressed && !pack->lb && !rbPressed && !mbPressed && !pack->mb && !pack->rb) {
    lbPressed = false;
    mouseEvent.type = LB_RELEASED;
  }
  else if (rbPressed && !pack->rb && !lbPressed && !mbPressed && !pack->mb && !pack->lb) {
    rbPressed = false;
    mouseEvent.type = RB_RELEASED;
  }
  else if (!mbPressed && pack->mb) {
    mbPressed = true;
    mouseEvent.type = BUTTON_EV;
  }
  else if(mbPressed && !pack->mb) {
    mbPressed = false;
    mouseEvent.type = BUTTON_EV;
  }
  else 
    mouseEvent.type = MOUSE_MOV;

  return &mouseEvent;
}

void LoadWonMenu() {
  loadBackground();
  cursor = loadCursor();

  xpm_image_t img;
  xpm_load(won_xpm, XPM_8_8_8_8, &img);

  uint32_t * map = (uint32_t *)img.bytes;

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (*(map + j + i * img.width) != xpm_transparency_color(XPM_8_8_8_8))
        *((uint32_t *)background_menu.bytes + j + i*xRes) = *(map + j + i * img.width);
    }
  }

  exitButton = createButton(BUTTON_EXIT,320,484);
  
  addButtonToBack(exitButton);

  drawMenu();
}

void gameWonIh(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool overexit = false;
  bool clickedexit = false;

  switch (device) {
    case TIMER:
      switch(checkCollisionWonMenu()) {
        case 0:
          if (overexit) {
            overexit = false;
            exitButton->isMouse = false;
            addButtonToBack(exitButton);
            drawButton(exitButton);
          }
          break;
        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickedexit = true;
            break;
          }
          if (!overexit) {
            overexit = true;
            exitButton->isMouse = true;
            addButtonToBack(exitButton);
            drawButton(exitButton);
          }
          break;
      }
      break;
    case KEYBOARD:
      if (scancode == ESC) {
        gameState = MAINMENU;
        LoadMenu(false);
      }
      break;
    case MOUSE:
      mouseEvent = mouseEvents(&pack);
      mouse_update(&pack);
      break;
  }

  if (clickedexit) {
    gameState = MAINMENU;
    LoadMenu(false);
  }
}

void gameLostIh(Device device) {
  static struct mouse_ev * mouseEvent;
  static bool overexit = false;
  bool clickedexit = false;

  switch (device) {
    case TIMER:
      switch(checkCollisionLostMenu()) {
        case 0:
          if (overexit) {
            overexit = false;
            exitButton->isMouse = false;
            addButtonToBack(exitButton);
            drawButton(exitButton);
          }
          break;
        case 1:
          if (mouseEvent->type == LB_RELEASED) {
            clickedexit = true;
            break;
          }
          if (!overexit) {
            overexit = true;
            exitButton->isMouse = true;
            addButtonToBack(exitButton);
            drawButton(exitButton);
          }
          break;
      }
      break;
    case KEYBOARD:
      if (scancode == ESC) {
        gameState = MAINMENU;
        LoadMenu(false);
      }
      break;
    case MOUSE:
      mouseEvent = mouseEvents(&pack);
      mouse_update(&pack);
      break;
  }

  if (clickedexit) {
    gameState = MAINMENU;
    LoadMenu(false);
  }
}


void LoadLostMenu() {
  loadBackground();
  cursor = loadCursor();

  xpm_image_t img;
  xpm_load(lost_menu_xpm, XPM_8_8_8_8, &img);

  uint32_t * map = (uint32_t *)img.bytes;

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (*(map + j + i * img.width) != xpm_transparency_color(XPM_8_8_8_8))
        *((uint32_t *)background_menu.bytes + j + i*xRes) = *(map + j + i * img.width);
    }
  }

  exitButton = createButton(BUTTON_EXIT,320,484);
  
  addButtonToBack(exitButton);

  drawMenu();
}
