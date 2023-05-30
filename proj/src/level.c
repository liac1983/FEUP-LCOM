#include "level.h"

extern unsigned int xRes, yRes;

Level * level;

Candy * createCandy(int x, int y) {
    
  Candy * candy = (Candy *) malloc (sizeof(Candy));

  candy->x = x;
  candy->y = y;

  xpm_image_t img;

  xpm_load(candy_xpm, XPM_8_8_8_8, &img);

  candy->img = img;

  return candy;
}

Level * loadLevel(unsigned int levelNum) {
  free(level);
  level = (Level *) malloc (sizeof(Level));
  level->levelNum = levelNum;
  loadLevelBack(levelNum);
  level->level_back = (uint32_t*) level->background.bytes;

  drawCandyLevel();
  drawLevelBackground();

  return level;
}

void loadLevelBack(unsigned int levelNum){
  switch (levelNum) {
    case 1:
      xpm_load(level1_xpm, XPM_8_8_8_8, &level->background);
      level->numCandy = 1;
      level->candyList = (Candy **) malloc (level->numCandy * sizeof(Candy *));
      level->candyList[0] = createCandy(500,400);
    
      break;
    case 2:
      xpm_load(level2_xpm, XPM_8_8_8_8, &level->background);
      level->numCandy = 2;
      level->candyList = (Candy **) malloc (level->numCandy * sizeof(Candy *));
      level->candyList[0] = createCandy(500,400);
      level->candyList[1] = createCandy(300,400);
    
      break;
    case 3:
      xpm_load(level3_xpm, XPM_8_8_8_8, &level->background);
      level->numCandy = 3;
      level->candyList = (Candy **) malloc (level->numCandy * sizeof(Candy *));
      level->candyList[0] = createCandy(500,400);
      level->candyList[1] = createCandy(300,400);
      level->candyList[2] = createCandy(200,400);
      
      break;
    default:
      break;
  }

}

void destroyLevel() {
  if (level == NULL)
    return;

  if (level->background.bytes) free(level->background.bytes);

  free(level);

  level = NULL; 
}

void drawLevelBackground() {
  for(unsigned int i = 0; i < level->background.width; i++) {
    for (unsigned int j = 0; j < level->background.height; j++) {
      vg_draw_pixel(i,j,*(level->level_back + i + j*xRes));
    }
  }
}

void drawCandyLevel() {
  int x, y;

  for (int k = 0; k < level->numCandy; k++) {
    x = level->candyList[k]->x;
    y = level->candyList[k]->y;
    uint32_t * candy_map = (uint32_t *)level->candyList[k]->img.bytes;
    for(int i = 0; i < level->candyList[k]->img.width; i++) {
      for (int j = 0; j < level->candyList[k]->img.height; j++) {
        if (*(candy_map + i + j*level->candyList[k]->img.width) != xpm_transparency_color(XPM_8_8_8_8))
          *(level->level_back + (x + i) + (y + j) * xRes) = *(candy_map + i + j*level->candyList[k]->img.width);
      }
    }
  }
}

void cleanCandy(Candy * candy) {
  for(int i = candy->x; i < candy->x + 32; i++) {
    for (int j = candy->y; j < candy->y + 32; j++) {
      level->level_back[i + j * xRes] = 0x000000;
    }
  }
}
