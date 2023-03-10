/*#########################################################################
                 Amoi Tech Mercury Application Build System
                 All Rights Reserved 2017-2020
                 Author:
#########################################################################*/

#ifndef _MERCURY_GRAPHICS_H_
#define _MERCURY_GRAPHICS_H_

#include "MercuryDef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _POINT {
  WORD left;
  WORD top;
} POINT, *PPOINT;

typedef struct _tagRECTL {
  WORD left;
  WORD top;
  WORD width;
  WORD height;
}RECTL, *PRECTL, *LPRECTL;

typedef struct _devicemodew {
  DWORD dmBitsPerPel;
  DWORD dmPelsWidth;
  DWORD dmPelsHeight;
  DWORD dmDisplayFlags;
  DWORD dmDisplayFrequency;
  DWORD dmDisplayOrientation;
  DWORD dmDisplayTimeout;
  DWORD dmBacklightLevel;
} DEVMODEW,* PDEVMODEW,* NPDEVMODEW,* LPDEVMODEW;

#define LCD_RED    0xF800
#define LCD_GREEN  0x07E0
#define LCD_BLUE   0x001F

typedef enum{
    DMDO_0,
    DMDO_90,
    DMDO_180,
    DMDO_270,
    MERCURY_ENUM_ORIENTATION_MAX = 0x7fffffff
}DisplayOrientation;

typedef enum{
    ST7789H2_3WIRE_9BIT_2DATA,
    ST7789H2_4WIRE_8BIT_1DATA,
    ST7789V2_3WIRE_9BIT_2DATA,
    ILI9342C_4WIRE_8BIT_1DATA,
    ST7735_4WIRE_8BIT_1DATA,
    EXTERN_ST7789HV2_3WIRE_9BIT_2DATA,
    EXTERN_ST7789HV2_4WIRE_8BIT_1DATA,
    EXTERN_ILI9342C_4WIRE_8BIT_1DATA,
    GC9305_3WIRE_9BIT_2DATA,
    GC9305_4WIRE_8BIT_1DATA,
    ST7571_4WIRE_8BIT_1DATA,
    UC1509_4WIRE_8BIT_1DATA,
    EXTERN_BLACKWHITE_LCD,
    ST7567_4WIRE_8BIT_1DATA,
    GC9106_4WIRE_8BIT_1DATA,
    EXTERN_COLOR_LCD,
    LCD_TYPE_MAX,
    MERCURY_ENUM_LCDTYPE_MAX = 0x7fffffff
}LCD_TYPE;

typedef enum{
    CLOCK_5M,
    CLOCK_10M,
    CLOCK_15M,
    CLOCK_20M,
    CLOCK_25M,
    CLOCK_MAX,
    MERCURY_LCD_MAX = 0x7fffffff
}LCD_CLK;

// begin cust lcd drv

typedef enum
{
    HAL_GOUDA_SPI_LINE_4 = 0,
    HAL_GOUDA_SPI_LINE_3 = 1,
    HAL_GOUDA_SPI_LINE_4_START_BYTE = 2,
    HAL_GOUDA_SPI_LINE_3_2_LANE = 3,
} HAL_GOUDA_SPI_LINE_T;

typedef enum
{
    LCD_DIRECT_NORMAL = 0x00,
    LCD_DIRECT_ROT_90, // Rotation 90    
    LCD_DIRECT_ROT_180,
    LCD_DIRECT_ROT_270,
} lcdDirect_t;

// LCD Contrller type
typedef enum
{
    LCD_CTRL_SPI = 0, //
    LCD_CTRL_CSI
} LCD_CTRL_MODULE_E;

typedef struct
{
    void (*init)(void);
    void (*enterSleep)(bool is_sleep);
    void (*checkEsd)(void);
    void (*setDisplayWindow)(uint16 left, uint16 top, uint16 right, uint16 bottom);
    void (*invlaidateRect)(uint16 left, uint16 top, uint16 right, uint16 bottom);
    void (*invalidate)(void);
    void (*close)(void);
    void (*rotationInvalidateRect)(uint16 left, uint16 top, uint16 right, uint16 bottom, lcdDirect_t angle);
    void (*setBrushDirection)(lcdDirect_t direct_type);
    uint32 (*readId)(void);
} lcdOperations_t;

typedef struct
{
    uint32 dev_id;
    uint32 width;
    uint32 height;
    HAL_GOUDA_SPI_LINE_T bus_mode;
    LCD_CTRL_MODULE_E controller;
    lcdOperations_t *operation;
    bool is_use_fmark;
    uint32 fmark_delay;
    uint32 freq;
} lcdSpec_t;

// end cust lcd drv

typedef enum{
    MERCURY_BLACK = 0,
    MERCURY_DIMGRAY,
    MERCURY_GRAY,
    MERCURY_DARK_GRAY,
    MERCURY_SILVER,
    MERCURY_LIGHT_GRAY,
    MERCURY_GAINSBORO,
    MERCURY_WHITE_SMOKE,
    MERCURY_WHITE,
    MERCURY_SNOW,
    MERCURY_IRON_GRAY,
    MERCURY_SAND_BEIGE,
    MERCURY_ROSY_BROWN,
    MERCURY_LIGHT_CORAL,
    MERCURY_INDIAN_RED,
    MERCURY_BROWN,
    MERCURY_FIRE_BRICK,
    MERCURY_MAROON,
    MERCURY_DARK_RED,
    MERCURY_STRONG_RED,
    MERCURY_RED,
    MERCURY_PERSIMMON,
    MERCURY_MISTY_ROSE,
    MERCURY_SALMON,
    MERCURY_SCARLET,
    MERCURY_TOMATO,
    MERCURY_DARK_SALMON,
    MERCURY_CORAL,
    MERCURY_ORANGE_RED,
    MERCURY_LIGHT_SALMON,
    MERCURY_VERMILION,
    MERCURY_SIENNA,
    MERCURY_TROPICAL_ORANGE,
    MERCURY_CAMEL,
    MERCURY_APRICOT,
    MERCURY_COCONUT_BROWN,
    MERCURY_SEASHELL,
    MERCURY_SADDLE_BROWN,
    MERCURY_CHOCOLATE,
    MERCURY_BURNT_ORANGE,
    MERCURY_SUN_ORANGE,
    MERCURY_PEACH_PUFF,
    MERCURY_SAND_BROWN,
    MERCURY_BRONZE,
    MERCURY_LINEN,
    MERCURY_HONEY_ORANGE,
    MERCURY_PERU,
    MERCURY_SEPIA,
    MERCURY_OCHER,
    MERCURY_BISQUE,
    MERCURY_TANGERINE,
    MERCURY_DARK_ORANGE,
    MERCURY_ANTIQUE_WHITE,
    MERCURY_TAN,
    MERCURY_BURLY_WOOD,
    MERCURY_BLANCHED_ALMOND,
    MERCURY_NAVAJO_WHITE,
    MERCURY_MARIGOLD,
    MERCURY_PAPAYA_WHIP,
    MERCURY_PALE_OCRE,
    MERCURY_KHAKI,
    MERCURY_MOCCASIN,
    MERCURY_OLD_LACE,
    MERCURY_WHEAT,
    MERCURY_PEACH,
    MERCURY_ORANGE,
    MERCURY_FLORAL_WHITE,
    MERCURY_GOLDENROD,
    MERCURY_DARK_GOLDENROD,
    MERCURY_COFFEE,
    MERCURY_JASMINE,
    MERCURY_AMBER,
    MERCURY_CORNSILK,
    MERCURY_CHROME_YELLOW,
    MERCURY_GOLDEN,
    MERCURY_LEMON_CHIFFON,
    MERCURY_LIGHT_KHAKI,
    MERCURY_PALE_GOLDENROD,
    MERCURY_DARK_KHAKI,
    MERCURY_MIMOSA,
    MERCURY_CREAM,
    MERCURY_IVORY,
    MERCURY_BEIGE,
    MERCURY_LIGHT_YELLOW,
    MERCURY_LIGHT_GOLDENROD_YELLOW,
    MERCURY_CHAMPAGNE_YELLOW,
    MERCURY_MUSTARD,
    MERCURY_MOON_YELLOW,
    MERCURY_OLIVE,
    MERCURY_CANARY_YELLOW,
    MERCURY_YELLOW,
    MERCURY_MOSS_GREEN,
    MERCURY_LIGHT_LIME,
    MERCURY_OLIVE_DRAB,
    MERCURY_YELLOW_GREEN,
    MERCURY_DARK_OLIVE_GREEN,
    MERCURY_APPLE_GREEN,
    MERCURY_GREEN_YELLOW,
    MERCURY_GRASS_GREEN,
    MERCURY_LAWN_GREEN,
    MERCURY_CHARTREUSE,
    MERCURY_FOLIAGE_GREEN,
    MERCURY_FRESH_LEAVES,
    MERCURY_BRIGHT_GREEN,
    MERCURY_COBALT_GREEN,
    MERCURY_HONEYDEW,
    MERCURY_DARK_SEA_GREEN,
    MERCURY_LIGHT_GREEN,
    MERCURY_PALE_GREEN,
    MERCURY_IVY_GREEN,
    MERCURY_FOREST_GREEN,
    MERCURY_LIME_GREEN,
    MERCURY_DARK_GREEN,
    MERCURY_GREEN,
    MERCURY_LIME,
    MERCURY_MALACHITE,
    MERCURY_MINT,
    MERCURY_CELADON_GREEN,
    MERCURY_EMERALD,
    MERCURY_TURQUOISE_GREEN,
    MERCURY_VERIDIAN,
    MERCURY_HORIZON_BLUE,
    MERCURY_SEA_GREEN,
    MERCURY_MEDIUM_SEA_GREEN,
    MERCURY_MINT_CREAM,
    MERCURY_SPRING_GREEN,
    MERCURY_PEACOCK_GREEN,
    MERCURY_MEDIUM_SPRING_GREEN,
    MERCURY_MEDIUM_AQUAMARINE,
    MERCURY_AQUAMARINE,
    MERCURY_CYAN_BLUE,
    MERCURY_AQUA_BLUE,
    MERCURY_TURQUOISE_BLUE,
    MERCURY_TURQUOISE,
    MERCURY_LIGHT_SEA_GREEN,
    MERCURY_MEDIUM_TURQUOISE,
    MERCURY_LIGHT_CYAN,
    MERCURY_BABY_BLUE,
    MERCURY_PALE_TURQUOISE,
    MERCURY_DARK_SLATE_GRAY,
    MERCURY_TEAL,
    MERCURY_DARK_CYAN,
    MERCURY_CYAN,
    MERCURY_AQUA,
    MERCURY_DARK_TURQUOISE,
    MERCURY_CADET_BLUE,
    MERCURY_PEACOCK_BLUE,
    MERCURY_POWDER_BLUE,
    MERCURY_STRONG_BLUE,
    MERCURY_LIGHT_BLUE,
    MERCURY_PALE_BLUE,
    MERCURY_SAXE_BLUE,
    MERCURY_DEEP_SKY_BLUE,
    MERCURY_SKY_BLUE,
    MERCURY_LIGHT_SKY_BLUE,
    MERCURY_MARINE_BLUE,
    MERCURY_PRUSSIAN_BLUE,
    MERCURY_STEEL_BLUE,
    MERCURY_ALICE_BLUE,
    MERCURY_SLATE_GRAY,
    MERCURY_LIGHT_SLATE_GRAY,
    MERCURY_DODGER_BLUE,
    MERCURY_MINERAL_BLUE,
    MERCURY_AZURE,
    MERCURY_WEDGWOOD_BLUE,
    MERCURY_LIGHT_STEEL_BLUE,
    MERCURY_COBALT_BLUE,
    MERCURY_PALE_DENIM,
    MERCURY_CORNFLOWER_BLUE,
    MERCURY_SALVIA_BLUE,
    MERCURY_DARK_POWDER_BLUE,
    MERCURY_SAPPHIRE,
    MERCURY_INTERNATIONAL_KLEIN_BLUE,
    MERCURY_CERULEAN_BLUE,
    MERCURY_ROYAL_BLUE,
    MERCURY_DARK_MINERAL_BLUE,
    MERCURY_ULTRAMARINE,
    MERCURY_LAPIS_LAZULI,
    MERCURY_GHOST_WHITE,
    MERCURY_LAVENDER,
    MERCURY_PERIWINKLE,
    MERCURY_MIDNIGHT_BLUE,
    MERCURY_NAVY_BLUE,
    MERCURY_DARK_BLUE,
    MERCURY_MEDIUM_BLUE,
    MERCURY_BLUE,
    MERCURY_WISTERIA,
    MERCURY_DARK_SLATE_BLUE,
    MERCURY_SLATE_BLUE,
    MERCURY_MEDIUM_SLATE_BLUE,
    MERCURY_MAUVE,
    MERCURY_LILAC,
    MERCURY_MEDIUM_PURPLE,
    MERCURY_AMETHYST,
    MERCURY_GRAYISH_PURPLE,
    MERCURY_HELIOTROPE,
    MERCURY_MINERAL_VIOLET,
    MERCURY_BLUE_VIOLET,
    MERCURY_VIOLET,
    MERCURY_INDIGO,
    MERCURY_DARK_ORCHID,
    MERCURY_DARK_VIOLET,
    MERCURY_PANSY,
    MERCURY_MALLOW,
    MERCURY_OPERA_MAUVE,
    MERCURY_MEDIUM_ORCHID,
    MERCURY_PAIL_LILAC,
    MERCURY_THISTLE,
    MERCURY_CLEMATIS,
    MERCURY_PLUM,
    MERCURY_LIGHT_VIOLET,
    MERCURY_PURPLE,
    MERCURY_DARK_MAGENTA,
    MERCURY_MAGENTA,
    MERCURY_FUCHSIA,
    MERCURY_ORCHID,
    MERCURY_PEARL_PINK,
    MERCURY_OLD_ROSE,
    MERCURY_ROSE_PINK,
    MERCURY_MEDIUM_VIOLET_RED,
    MERCURY_MAGENTA_ROSE,
    MERCURY_ROSE,
    MERCURY_RUBY,
    MERCURY_CAMELLIA,
    MERCURY_DEEP_PINK,
    MERCURY_FLAMINGO,
    MERCURY_CORAL_PINK,
    MERCURY_HOT_PINK,
    MERCURY_BURGUNDY,
    MERCURY_SPINEL_RED,
    MERCURY_CARMINE,
    MERCURY_BABY_PINK,
    MERCURY_CARDINAL_RED,
    MERCURY_LAVENDER_BLUSH,
    MERCURY_PALE_VIOLET_RED,
    MERCURY_CERISE,
    MERCURY_SALMON_PINK,
    MERCURY_CRIMSON,
    MERCURY_PINK,
    MERCURY_LIGHT_PINK,
    MERCURY_SHELL_PINK,
    MERCURY_ALIZARIN_CRIMSON,
    MERCURY_PALETTE_MAX_NUM,         // Reserved, user can't use it.
    MERCURY_ENUM_PALETEE_MAX = 0x7fffffff
}MERCURY_PALETEE_ARRAY_INDEX_E;

int DisplayInit(void);
int DisplayMutilInit(LCD_TYPE lcdType);
int DisplayInitCustLCD(const lcdSpec_t*pCustLcd,int lcdType);
int DisplayDeinit();
int DisplayBitMap(RECTL* prclTrg,  const LPBYTE ImageData);
int DisplayBitMapGet(RECTL* prclTrg,  uint16 *ImageData, uint32 len);
int DisplayString(POINT * prclTrg,  const LPBYTE String);
int DisplayTransparentString(POINT * prclTrg,  const LPBYTE String);
int DisplayHorizLine(POINT * prclTrg,  uint32 Length, uint32 Width, MERCURY_PALETEE_ARRAY_INDEX_E Color);
int DisplayVertiLine(POINT * prclTrg,  uint32 Length, uint32 Width, MERCURY_PALETEE_ARRAY_INDEX_E Color);
int DisplayGetMode(DEVMODEW * prclTrg);
int DisplaySetColor(MERCURY_PALETEE_ARRAY_INDEX_E fgColor, MERCURY_PALETEE_ARRAY_INDEX_E bgColor);
int DisplayClearScreen(void);

/**
 *  ??????LCD Backlight??????
 *
 * @param [uint32] Brightness: ????????????
 *		?????????????????????????????????????????????????????????11-13mA
 *		 0: turn off, 1: 5mA,  	2: 15mA,
 *		 3: 26mA,	 4: 38.6mA,	5: 51.2mA,
 *		 6: 63.8mA,	 7: 76.4mA,	 8: 89mA,
 *		 9: 100.8mA, 	10: 113.4mA,	11: 126mA,
 *		 12: 138.6mA,	13: 151.2mA,	14: 163.8mA,
 * @return :[BOOL]: TRUE: success; FALSE : fail
 */
int DisplaySetBrightness(uint16 Brightness);
int DisplaySetScreenOffTimeout(uint16 Timeout);
int DisplayGetDirection(DisplayOrientation *Dir);
int DisplaySetDirection(DisplayOrientation Dir);
int DisplayScreenOn(void);
int DisplayRLE_BMP(RECTL * prclTrg,  const LPBYTE ImageData, uint32 DataLen);
int DisplayPaintEnd(void);
int DisplayLineRGB565(POINT *p, uint32 Length, uint32 Height, uint16 Color);
int LCD_SetColorRGB565(uint16 fgColor, uint16 bgColor);
int DisplayPointRGB565(POINT *p ,uint16 Color);
uint16 DisplayGetRGB565(MERCURY_PALETEE_ARRAY_INDEX_E paleteIndex);
int DisplaySetClock(LCD_CLK clk);
int DisplaySendCommand(uint16 cmd);
int DisplaySendData(uint16 data);
uint32 DisplayReadId(uint16 addr,uint16 lcdLineMode,bool isUseFmark,uint32 fmark_delay);
// uint:mv
int DisplaySetIOVDD(uint16 vol);
void DisplayResetLcdPin(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_MERCURY_GRAPHICS_H_

