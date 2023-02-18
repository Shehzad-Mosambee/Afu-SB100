#ifndef _SDK_API_DISPLAY_H
#define _SDK_API_DISPLAY_H

#include "MercuryDef.h"
#include "MercuryGraphics.h"

#define LCD_TYPE_UC1617       0
#define LCD_TYPE_ILI9342C     1
#define LCD_TYPE_ST7789       2

#define LCD_DEV_TYPE LCD_TYPE_ST7789

#if (LCD_DEV_TYPE == LCD_TYPE_UC1617 )
#define LCD_WIDTH   96
#define LCD_HEIGHT  128
#define DEFAULT_BACKCOLOR  0
#define DEFAULT_FONTCOLOR  0xFFFF
#elif (LCD_DEV_TYPE == LCD_TYPE_ILI9342C)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#define DEFAULT_BACKCOLOR  0xFFFF
#define DEFAULT_FONTCOLOR  0
#elif (LCD_DEV_TYPE == LCD_TYPE_ST7789)
#define LCD_WIDTH   240
#define LCD_HEIGHT  320
#define DEFAULT_BACKCOLOR  0xFFFF
#define DEFAULT_FONTCOLOR  0

#endif

typedef struct 
{
	uint16 width;
	uint16 height;
	uint16 def_bgcolor;
	uint16 def_fgcolor;
}LCDDevParams, *PLCDDevParams;

// 采用的位图分辨率，固定高宽度
#define STATUS_BAR_HEIGHT   16

#define ASCII_WIDTH 8  //ascii码字符宽度8
#define HZ_WIDTH    16 //汉字宽度为16
#define HZ_HEIGHT   16 //汉字高度为16


#define DEFAULT_FGGRAYSCALE 255
#define DEFAULT_BGGRAYSCALE 0

#define DISP_EFFECT_REVERSE		0x01    //反显
#define DISP_EFFECT_UNDERLINE   0x02    //下划线

#define DISP_ALIGN_LEFT         0x1     // 靠左对齐
#define DISP_ALIGN_CENTER       0x2     // 中央对齐
#define DISP_ALIGN_RIGHT        0x3     // 靠右对齐

// 定义接下来显示的属性
typedef struct tagDisplayParams
{
    bool    continuous;     // 是否保持显示属性，直到恢复缺省属性为止。TRUE：保持，FALSE：仅生效一次
    uint32  effect;         // 设置显示效果，按 bit 设置，参见：DISP_EFFECT_XXX
    uint32  align;          // 对齐模式，按行显示文本时生效，参见：DISP_ALIGN_XXX
    uint32  fgColor;        // foreground 颜色 RGB
    uint32  bgColor;        // background 颜色 RGB
    uint32  fgGrayScale;    // foreground 灰度
    uint32  bgGrayScale;    // background 灰度
} DisplayParams, *PDisplayParams;

//lcd init
void sdk_disp_init(int lcdtype);

// 设置显示参数
int sdk_disp_set_params(PDisplayParams pDispParams);

// 获取显示参数
int sdk_disp_get_params(PDisplayParams pDispParams);

// 所有显示操作后，如果 continuous 为FALSE，均调用本函数，恢复到缺省属性
void sdk_disp_set_default_params(void);

// 模式按位设置
#define SCR_MODE_DEFAULT            0x0000
//clear all screen 显示模式，对比度取值：0 ~ 15
int sdk_disp_clear_screen(int nScreenMode);

//clear specified area
int sdk_disp_clear_area(uint32 x, uint32 y, uint32 width, uint32 height);

//draw line
int sdk_disp_line(uint32 x, uint32 y, uint32 width, uint32 height);

//draw color line
int sdk_disp_line_color(uint32 x, uint32 y, uint32 width, uint32 height, uint16 color);


// draw bitmap
int sdk_disp_bitmap(uint32 x, uint32 y, uint32 width, uint32 height,  const uint8* ImageData);

typedef struct tagDisplayStatusBar
{
    POINT    mobileSignal;      // 移动信号强度，含2G/2.5G/3G/4G
    POINT    wifiSignal;        // WIFI 信号强度
    POINT    battery;           // 电池电量
} DisplayStatusBar, *PDisplayStatusBar;
int sdk_set_status_bar(PDisplayStatusBar pBar);
/*
* level: 0-4
*/
// int sdk_draw_battery(uint32 left, uint32 top, uint32 level);
int sdk_disp_battery(uint32 level);

/*
* level: 0-4
*/
int sdk_disp_mobile_signal(uint32 level);

/*
* level: 0-3
*/
int sdk_disp_wifi_signal(uint32 level);

/*
* draw time in specified area, format: 00:00 (hour:minute)
*/
int sdk_disp_time(uint32 x, uint32 y, const char* format);

/*
* 在指定位置显示文本
* x:显示位置 
* y:显示位置
* mode:显示模式，0为正常显示，支持DISP_MODE_REVERSE和DISP_MODE_UNDERLINE
*/
// int sdk_draw_text(uint32 x, uint32 y, char *text, uint32 mode);
int sdk_disp_text(uint32 x, uint32 y, const char *text);

/*
* 在指定行显示文本
* line:行号，16像素为一行，行号从0开始 
* mode:显示模式，0为正常显示，支持DISP_MODE_REVERSE和DISP_MODE_UNDERLINE
* centralAlign: 是否居中显示，默认为靠左显示
*/
int sdk_disp_text_row(uint32 row, const char *text);

/* 
* diplay qr code in specified area
*/
// 默认中心对齐，pRect 为 NULL 时，除状态栏外，剩余屏幕区域中心对齐
int sdk_disp_qrcode(RECTL *pRect, const BYTE *qr_data, int dataLen);

#endif

