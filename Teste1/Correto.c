#include <lcom/lcf.h>
#include <lcom/pp.h>
#include <stdint.h>

#define TIMER_0 0x20
#define TIMER_1 0x21
#define TIMER_1 0x22
#define CTRL_REG 0x23
#define STAT_REG 0x23
#define IRQ_LINE 10
#define BIT(n) (n << 1)

#define SEL_TIMER0 0x00
#define SEL_TIMER1 BIT(6)
#define SEL_TIMER2 BIT(7)

#define PERIODIC_MODE 0x00
#define ALARM_MODE BIT(4)

#define MICROSECOND 0x00
#define MILLISECOND BIT(0)
#define SECOND BIT(1)

#define TIMER0_INT BIT(0)
#define TIMER1_INT BIT(1)
#define TIMER2_INT BIT(2)

enum l3223_time_units
{
l3223_microsecond,
l3223_millisecond,
l3223_second
};

int hook_id = 0;

int main(int argc, char *argv[]) {

    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/pp/trace.txt");

    lcf_log_output("/home/lcom/labs/pp/output.txt");

    if (lcf_start(argc, argv))
    return 1;

    lcf_cleanup();

    return 0;
}

int timer_configuration(int timer, int interval, enum l3223_time_units
unit)
{
    uint8_t control_word = 0x00;
    if(timer == 0)
    control_word = SEL_TIMER0;
    else if(timer == 1)
    control_word = SEL_TIMER1;
    else if(timer == 2)
    control_word = SEL_TIMER2;
    else
    {
        printf("Error in timer_configuration() - selecting timer! \n");
        return 1;
    }
    control_word = control_word | ALARM_MODE;
    if(unit = l3223_microsecond)
        control_word = control_word | MICROSECOND;
    else if(unit = l3223_millisecond)
        control_word = control_word | MILLISECOND;
    else if(unit = l3223_second)
        control_word = control_word | SECOND;
    else
    {
        printf("Error in timer_configuration() - selecting time unit!
        \n");
        return 1;
    }
    if(sys_outb(CTRL_REG, control_word) != 0)
    {
        printf("Error in timer_configuration() - sys_outb to control
        register! \n");
        return 1;
    }
    //get LSB
    uint8_t lsb = interval & 0x00FF;
    //get MSB
    uint8_t msb = (interval & 0xFF00) >> 8;

    if(sys_outb(TIMER_0 + timer, lsb) != 0)
    {
        printf("Error in timer_configuration() - sys_outb LSB to timer!
        \n");
        return 1;
    }
    if(sys_outb(TIMER_0 + timer, msb) != 0)
    {
        printf("Error in timer_configuration() - sys_outb MSB to timer!
        \n");
        return 1;
    }
    return 0;
}

int pp_test_alarm(int timer, int interval, enum l3223_time_units unit);
{
    int ipc_status, r, irq_set = BIT(hook_id);
    message msg;
    bool done = false;
    if(timer_configuration(timer, interval, unit) != 0)
    {
        printf("Error in timer_configuration()! \n");
        return 1;
    }
    //subscribe interrupts
    if(sys_irqsetpolicy(IRQ_LINE, IRQ_REENABLE, &hook_id) != 0)
    {
        printf("Error in sys_irqsetpolicy()!\n");
        return 1;
    }

    // interrupt loop
    while (!done) {
        // Get a request message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: // hardware
                interrupt notification
                    if (msg.m_notify.interrupts & irq_set)
                    { // subscribed interrupt
                        // handle interrupt
                        uint32_t status;
                        sys_inb(STAT_REG, &status);
                        if((((status & TIMER0_INT) != 0) && (timer == 0)) ||
                        (((status & TIMER1_INT) != 0) && (timer == 1)) ||
                        (((status & TIMER2_INT) != 0) && (timer == 2)))
                        {
                        //alarm active
                        pp_print_alarm(timer, interval, unit);
                        done = true;
                        }
                    }
                    break;
                    default:
                    break;
                }
            }
        }
        if(sys_irqrmpolicy(&hook_id) != 0)
        {
            printf("Error in sys_irqrmpolicy()! \n");
            return 1;
        }

    return r;
}