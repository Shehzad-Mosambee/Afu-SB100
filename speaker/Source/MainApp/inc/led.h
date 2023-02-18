#ifndef _LED_H_
#define _LED_H_

#define ret_check(ret, error) \
    {                                       \
        if (0 != ret)                       \
        {                                   \
            MercuryTrace(error "\r\n");     \
        }                                   \
    }

void led_blink_loop(void);
void led_close(void);

#endif