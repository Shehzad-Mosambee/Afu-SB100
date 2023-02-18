#include "MercuryPlat.h"
#include "MercuryThread.h"

#include "MercuryNLed.h"
#include "MercuryGpio.h"

#include "SDK_API_LED.h"

#include "led.h"

void led_close(void)
{
    ret_check(sdk_light_control(LED_OFF),"Procedure Failed!");
    ret_check(sdk_light_control(LED_RED),"Procedure Failed!");
}

void led_blink_loop(void)
{
	// LED blink loop
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


void anfu_led1_on(void){
	//D1  on
	NLedSetCurrentLedlevel(LED_FLASH_BL,0);//0?to?14,Current?range:?15?mA?to?240?mA??,step=15mA
	NledSetCurrentLed(LED_FLASH_BL,1);
}
void anfu_led1_off(void){
	//D1  off
	NLedSetCurrentLedlevel(LED_FLASH_BL,0);//0?to?14,Current?range:?15?mA?to?240?mA??,step=15mA
	NledSetCurrentLed(LED_FLASH_BL,0);
}
void anfu_led2_on(void){
	//D2  on
	MERCURY_GPIO_CFG_S gpioCfg={0};
	gpioCfg.default_val=1;
	gpioCfg.dir=OUTPUT;
	gpioCfg.int_sense=NO_INT;
	GIO_SetMode(18, &gpioCfg);
}
void anfu_led2_off(void){
	//D2  off
	MERCURY_GPIO_CFG_S gpioCfg={0};
	gpioCfg.default_val=0;
	gpioCfg.dir=OUTPUT;
	gpioCfg.int_sense=NO_INT;
	GIO_SetMode(18, &gpioCfg);
}

void anfu_led_on(void){

	//D1  on
	NLedSetCurrentLedlevel(LED_FLASH_BL,0);//0?to?14,Current?range:?15?mA?to?240?mA??,step=15mA
	NledSetCurrentLed(LED_FLASH_BL,1);

	//D2  on
	MERCURY_GPIO_CFG_S gpioCfg={0};
	gpioCfg.default_val=1;
	gpioCfg.dir=OUTPUT;
	gpioCfg.int_sense=NO_INT;
	GIO_SetMode(18, &gpioCfg);
}

void anfu_led_off(void){
	//D1  off
	NLedSetCurrentLedlevel(LED_FLASH_BL,0);//0?to?14,Current?range:?15?mA?to?240?mA??,step=15mA
	NledSetCurrentLed(LED_FLASH_BL,0);	
	//D2  off
	MERCURY_GPIO_CFG_S gpioCfg={0};
	gpioCfg.default_val=0;
	gpioCfg.dir=OUTPUT;
	gpioCfg.int_sense=NO_INT;
	GIO_SetMode(18, &gpioCfg);

}

