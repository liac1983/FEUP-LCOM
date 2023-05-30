#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include "i8042.h"
#include "utils3.h"
#include <stdbool.h>
#include <stdint.h>

extern uint8_t scancode;
extern int count_in;
extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status, r;
  message msg;
  uint32_t irq_set = BIT(1);
  uint8_t bit_no = 0;

  uint8_t bytes[2];
  bool ReadSecond = false;

  if (kbd_subscribe_int(&bit_no) != 0) {
    return 1;
  }

  while (scancode != ESC) {
    /* You may want to use a different condition */
    /* Get a request message. */
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            kbd_handle_read_second(&ReadSecond, bytes);
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) {
    return 1;
  }
  kbd_print_no_sysinb(count_in);
  return 0;
}

int(kbd_test_poll)() {
  uint8_t bytes[2];
  bool ReadSecond = false;

  while (scancode != ESC) {
    kbc_ih();

    if (scancode == 0x00) {
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }
    kbd_handle_read_second(&ReadSecond, bytes);
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  kbd_print_no_sysinb(count_in);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  int ipc_status, r;
  message msg;
  uint32_t irq_set_timer = BIT(0);
  uint32_t irq_set_kbd = BIT(1);
  uint8_t bit_no = 0;

  uint8_t bytes[2];
  bool ReadSecond = false;

  if (timer_subscribe_int(&bit_no) != 0)
    return 1;
  if (kbd_subscribe_int(&bit_no) != 0)
    return 1;

  while (n != 0 && scancode != ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih();
            kbd_handle_read_second(&ReadSecond, bytes);
            counter = 0;
            continue;
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counter % 60 == 0 && counter != 0) {
              n--;
            }
          }
          break;
        default:
          break;
      }
    }
  }

  if (timer_unsubscribe_int() != 0)
    return 1;
  if (kbd_unsubscribe_int() != 0)
    return 1;
  kbd_print_no_sysinb(count_in);
  return 1;
}
