#include "MercuryPlat.h"
#include "MercuryThread.h"

#include "SDK_API_LED.h"

#include "led.h"
#include "led_test.h"

void test_led_light(void)
{
	// LED light loop
	while(1)
	{
		ret_check(sdk_light_control(LED_RED),"Procedure Failed!");

		Sleep(1000);
		ret_check(sdk_light_control(LED_OFF),"Procedure Failed!");

		Sleep(1000);
		ret_check(sdk_light_control(LED_GREEN),"Procedure Failed!");

		Sleep(1000);
		ret_check(sdk_light_control(LED_OFF),"Procedure Failed!");

		Sleep(1000);
		ret_check(sdk_light_control(LED_RED | LED_GREEN),"Procedure Failed!");

		Sleep(1000);
		ret_check(sdk_light_control(LED_OFF),"Procedure Failed!");
        
		Sleep(1000);
	}
}

void test_led_blink(void)
{
    // sdk_blink_control(LED_RED,1000,100);
    sdk_blink_control(LED_RED | LED_GREEN,500,500);
}

void test_led(void)
{
    // test_led_light();

    test_led_blink();
}