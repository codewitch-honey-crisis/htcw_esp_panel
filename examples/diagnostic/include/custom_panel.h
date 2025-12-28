// you don't actually need a #ifdef guard on a custom panel for many projects, only for projects where you might support more than one device (either custom, or otherwise)
#ifdef C6DEVKITC1 // Works, but is a custom kit
#define LCD_I2C_HOST    0
#define LCD_DMA
#define LCD_I2C_ADDR 0x3C
#define LCD_I2C_PULLUP
#define LCD_CONTROL_PHASE_BYTES 1
#define LCD_DC_BIT_OFFSET 6
#define LCD_BIT_DEPTH 1
#define LCD_PIN_NUM_SCL 11
#define LCD_PIN_NUM_SDA 10
#define LCD_PIN_NUM_RST -1
#define LCD_PANEL esp_lcd_new_panel_ssd1306
#define LCD_HRES 128
#define LCD_VRES 64
#define LCD_COLOR_SPACE LCD_COLOR_GSC
#define LCD_CLOCK_HZ (400 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X true
#define LCD_MIRROR_Y true
#define LCD_INVERT_COLOR true
#define LCD_SWAP_XY false
#define LCD_DIVISOR 1
#define LCD_VENDOR_CONFIG esp_lcd_panel_ssd1306_config_t vendor_config = {\
    .height = LCD_VRES,\
};
#define LCD_TRANSLATE static uint8_t ssd1306_buffer[(LCD_HRES*LCD_VRES*LCD_BIT_DEPTH+7)/8];\
     for (int y = y1; y <= y2; y++) {\
        for (int x = x1; x <= x2; x++) {\
            bool chroma_color = (((uint8_t*)bitmap)[(LCD_HRES >> 3) * (y - y1)  + ((x - x1) >> 3)] & 1 << (7 - (x - x1) % 8));\
            uint8_t *buf = ssd1306_buffer + LCD_HRES * (y >> 3) + (x);\
            if (chroma_color) {\
                (*buf) &= ~(1 << (y % 8));\
            } else {\
                (*buf) |= (1 << (y % 8));\
            }\
        }\
    }\
    bitmap = ssd1306_buffer + LCD_HRES * (y1 >> 3) + (x1);
#endif