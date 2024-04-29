#include "i8042.h"
#include <lcom/lcf.h>
#include <stdint.h>

int(kbd_subscribe_int)(uint8_t *bit_no);

int(kbd_unsubscribe_int)();

int(util_sys_inb)(int port, uint8_t *value);

void(kbc_ih)();

int(kbd_enable)();

void(kbd_handle_read_second)(bool *read_second, uint8_t *bytes);
