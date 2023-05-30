#include <lcom/lcf.h>
#include <lcom/liblm.h>
#include <lcom/proj.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <lib.h>

#include "controllers/graphics/graphics.h"
#include "controllers/graphics/graphics_const.h"
#include "controllers/keyboard/keyboard.h"
#include "controllers/i8042.h"
#include "controllers/i8254.h"
#include "level.h"
#include "player.h"
#include "menu.h"
#include "game.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g2/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g2/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)() {

  if (set_graphics_mode(0x115)) return 1;
  if (init_graphics_mode(0x115)) return 1;

  if (GameMainLoop()) return 1;

  if (vg_exit() != 0) {return 1;}

  return 0;
}
