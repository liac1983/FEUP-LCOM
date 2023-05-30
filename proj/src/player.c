#include "player.h"

extern unsigned int xRes, yRes;
extern uint8_t scancode;
extern Level * level;
extern GameState gameState;

Player * createPlayer () {
  Player * player = (Player *) malloc (sizeof(Player));

  player->x = 0;
  player->y = 0;
  player->speed = 4;
  player->candy = 0;

  xpm_image_t img;

  xpm_load(player_xpm, XPM_8_8_8_8, &img);
  player->img = img;

  free(&img);

  return player;
}

void drawPlayer(Player * player) {
  uint32_t* map = (uint32_t*) player->img.bytes;
  for(int i = 0; i < player->img.width; i++) {
    for (int j = 0; j < player->img.height; j++) {
      if (*(map + i + j*player->img.width) != xpm_transparency_color(XPM_8_8_8_8))
        vg_draw_pixel(player->x+i,player->y+j,*(map + i + j*player->img.width));
    }
  }
}

void cleanPlayer(Player * player) {
  for (int i = player->x; i <= player->x + player->img.width; i++) {
    for (int j = player->y; j <= player->y + player->img.height; j++) {
      if (i < (int)xRes - 1 && j < (int)yRes - 1)
        vg_draw_pixel(i,j,*(level->level_back + i + j * xRes));
    }
  }
}

void checkMove(Player * player, bool up, bool down, bool left, bool right) {
  if (up && !down && !right && !left) player->direction = UP;
  else if (!up && down && !right && !left) player->direction = DOWN;
  else if (!up && !down && right && !left) player->direction = RIGHT;
  else if (!up && !down && !right && left) player->direction = LEFT;
  
  if ((up && !checkCollWithWalls(player, UP)) ||
        (down && !checkCollWithWalls(player, DOWN)) ||
        (right && !checkCollWithWalls(player, RIGHT)) ||
        (left && !checkCollWithWalls(player, LEFT))) {
        cleanPlayer(player);
        if (up && player->y - player->speed < 0)
            player->y = 0;
        else if (down && player->y + player->speed + player->img.height > (int) yRes)
            player->y = (int) yRes - player->img.height;
        else if (right && player->x + player->speed + player->img.width > (int) xRes)
            player->x = (int) xRes - player->img.width;
        else if (left && player->x - player->speed < 0)
            player->x = 0;
        else {
            if (up)
                player->y -= player->speed;
            else if (down)
                player->y += player->speed;
            else if (right)
                player->x += player->speed;
            else if (left)
                player->x -= player->speed;
        }
    }

    drawPlayer(player);

  checkCollWithCandy(player);
}

void UpdateMoveDir(Player * player, uint8_t scancode, bool * up, bool * down, bool * left, bool * right) {
  if (scancode == MAKE_ARROW_UP) {
    *up = true;
    player->direction = UP;
  } else if (scancode == MAKE_ARROW_DOWN) {
    *down = true;
    player->direction = DOWN;
  } else if (scancode == MAKE_ARROW_RIGHT) {
    *right = true;
    player->direction = RIGHT;
  } else if (scancode == MAKE_ARROW_LEFT) {
    *left = true;
    player->direction = LEFT;
  }
  else if (scancode == BREAK_ARROW_UP) 
    *up = false;  
  else if (scancode == BREAK_ARROW_DOWN) 
    *down = false;
  else if (scancode == BREAK_ARROW_RIGHT) 
    *right = false;
  else if (scancode == BREAK_ARROW_LEFT) 
    *left = false;
}

bool checkCollWithWalls(Player * player, Direction direction) {
  uint32_t blue = 0x00FFFF;
  uint32_t pink = 0xFF075E;
  int startX, startY, endX, endY;

  switch (direction) {
    case UP:
      startX = player->x;
      startY = player->y - 3;
      endX = player->x + player->img.width;
      endY = player->y;
      break;
    case DOWN:
      startX = player->x;
      startY = player->y + player->img.height;
      endX = player->x + player->img.width;
      endY = player->y + 3 + player->img.height;
      break;
    case LEFT:
      startX = player->x - 3;
      startY = player->y;
      endX = player->x;
      endY = player->y + player->img.height;
      break;
    case RIGHT:
      startX = player->x + player->img.width;
      startY = player->y;
      endX = player->x + 3 + player->img.width;
      endY = player->y + player->img.height;
      break;
    default:
      return false;
  }

  for (int i = startX; i <= endX; i++) {
    for (int j = startY; j <= endY; j++) {
      if (level->level_back[i + j * xRes] == blue || level->level_back[i + j * xRes] == pink)
        return true;
    }
  }

  return false;
}

bool checkCollWithCandy(Player * player) {
  for (int k = 0; k < level->numCandy; k++) {
      Candy * candy = level->candyList[k];
    if (candy->x != -1) {
      if (player->x < candy->x && player->x + player->img.width > candy->x && player->y < candy->y && player->y + player->img.height > candy->y) {
        cleanCandy(candy);
        deleteXpm(candy->img, candy->x, candy->y);
        level->candyList[k]->x = -1;
        return true;
      }
      else if (player->x < candy->x + candy->img.width && player->x + player->img.width > candy->x + candy->img.width && player->y < candy->y && player->y + player->img.height > candy->y) {
        cleanCandy(candy);
        deleteXpm(candy->img, candy->x, candy->y);
        level->candyList[k]->x = -1;
        return true;
      }
      else if (player->x < candy->x && player->x + player->img.width > candy->x && player->y < candy->y + candy->img.height && player->y + player->img.height > candy->y + candy->img.height) {
        cleanCandy(candy);
        deleteXpm(candy->img, candy->x, candy->y);
        level->candyList[k]->x = -1;
        return true;
      }
      else if (player->x < candy->x + candy->img.width && player->x + player->img.width > candy->x + candy->img.width && player->y < candy->y + candy->img.height && player->y + player->img.height > candy->y + candy->img.height) {
        cleanCandy(candy);
        deleteXpm(candy->img, candy->x, candy->y);
        level->candyList[k]->x = -1;
        return true;
      }
    }
  }

  return false;
}
