#include "utils3.h"
#include <lcom/lcf.h>

uint8_t scancode, count_in = 0, stat;
static int hook_id;

int(kbd_subscribe_int)(uint8_t *bit_no) {
  hook_id = 1;
  *bit_no = hook_id;
  if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != 0) {
    return 1;
  }

  return 0;
}

int(kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) {
    return 1;
  }

  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t val32;
  if (sys_inb(port, &val32) != 0) {
    return 4;
  }
  *value = (val32 & 0x000000FF);
  count_in++;

  return 0;
}

void(kbc_ih)() {
  while (1) {
    if (util_sys_inb(STAT_REG, &stat) != 0) {
      scancode = 0x00;
    }

    if (stat & OBF) {
      if (util_sys_inb(OUT_BUF, &scancode) != 0) {
        scancode = 0x00;
      }
      if ((stat & (PAR_ERR | TO_ERR)) != 0) {
        scancode = 0x00;
      }
      break;
    }
  }
}

int(kbd_enable)() {
  uint8_t command_byte;

  if (sys_outb(KBC_CMD_REG, READ_CMDB) != 0)
    return 1;

  if (util_sys_inb(OUT_BUF, &command_byte) != 0) {
    return 1;
  }

  command_byte = (command_byte | INT);

  if (sys_outb(KBC_CMD_REG, OUT_BUF) != 0) {
    return 1;
  }

  if (sys_outb(WRITE_CMBD, command_byte) != 0) {
    return 1;
  }

  return 0;
}

void(kbd_handle_read_second)(bool *read_second, uint8_t *bytes) {
  if (*read_second) {
    *read_second = false;
    bytes[1] = scancode;
    kbd_print_scancode(!(scancode & BREAK), 2, bytes);
  }
  else {
    bytes[0] = scancode;
    if (scancode == SIZE) {
      *read_second = true;
    }
    else {
      kbd_print_scancode(!(scancode & BREAK), 1, bytes);
    }
  }
}
