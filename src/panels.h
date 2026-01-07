#ifndef PANELS_H
#define PANELS_H

// BEGIN devices (add more)

#ifdef TTGO_T1 // Works
#define LCD_SPI_HOST    SPI_3
#define LCD_PIN_NUM_MOSI 19
#define LCD_PIN_NUM_CLK 18
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 16
#define LCD_PIN_NUM_RST 23
#define LCD_PIN_NUM_BCKL 4
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_HRES 135
#define LCD_VRES 240
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 40
#define LCD_GAP_Y 52
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 1
#define LCD_DIVISOR 2
#define BUTTON_MASK (BUTTON_PIN(0)|BUTTON_PIN(35))
#define BUTTON_ON_LEVEL 0
#endif // TTGO_T1

#ifdef T_DISPLAY_S3 // untested, mine bricked
#define LCD_PIN_NUM_CS 6
#define LCD_PIN_NUM_RST 5
#define LCD_PIN_NUM_WR 8
#define LCD_PIN_NUM_RD 9
#define LCD_PIN_NUM_RS 7
#define LCD_PIN_NUM_D00 39
#define LCD_PIN_NUM_D01 40
#define LCD_PIN_NUM_D02 41
#define LCD_PIN_NUM_D03 42
#define LCD_PIN_NUM_D04 45
#define LCD_PIN_NUM_D05 46
#define LCD_PIN_NUM_D06 47
#define LCD_PIN_NUM_D07 48
#define LCD_PIN_NUM_BCKL 38
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_HRES 170
#define LCD_VRES 320
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_CLOCK_HZ (6528 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 35
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 1
#define BUTTON_MASK (BUTTON_PIN(0)|BUTTON_PIN(14))
#define BUTTON_ON_LEVEL 0
#endif // T_DISPLAY_S3

#ifdef T_QT_PRO // Untested (mine is lost)
#define LCD_SPI_HOST    SPI_3
#define LCD_BCKL_ON_LEVEL 0
#define LCD_PIN_NUM_MOSI 2
#define LCD_PIN_NUM_CLK 3
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 6
#define LCD_PIN_NUM_RST 1
#define LCD_PIN_NUM_BCKL 10
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_HRES 128
#define LCD_VRES 128
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 2
#define LCD_GAP_Y 1
#define LCD_MIRROR_X 1
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 0
#define BUTTON_MASK (BUTTON_PIN(0)|BUTTON_PIN(47))
#define BUTTON_ON_LEVEL 0
#endif // T_QT_PRO

#ifdef ESP_WROVER_KIT // Should work (Mine has a short)
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_panel_ili9341
#include "esp_lcd_panel_ili9341.h"
#endif
#define LCD_BCKL_ON_LEVEL 0
#define LCD_SPI_HOST    SPI_3
#define LCD_PIN_NUM_MISO 25
#define LCD_PIN_NUM_MOSI 23
#define LCD_PIN_NUM_CLK  19
#define LCD_PIN_NUM_CS   22
#define LCD_PIN_NUM_DC   21
#define LCD_PIN_NUM_RST  18
#define LCD_PIN_NUM_BCKL 5
#define LCD_INIT esp_lcd_new_panel_ili9341
#define LCD_HRES 240
#define LCD_VRES 320
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 0
#define LCD_INVERT_COLOR 0
#define LCD_SWAP_XY 1
#define BUTTON_MASK (BUTTON_PIN(0))
#define BUTTON_ON_LEVEL 0
#endif // ESP_WROVER_KIT

#ifdef ESP_USB_OTG // Works
#define LCD_SPI_HOST    SPI_2
#define LCD_PIN_NUM_MOSI 7
#define LCD_PIN_NUM_CLK 6
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 4
#define LCD_PIN_NUM_RST 8
#define LCD_PIN_NUM_BCKL 9
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_HRES 240
#define LCD_VRES 240
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 0
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 0
#define BUTTON_MASK (BUTTON_PIN(0)|BUTTON_PIN(10)|BUTTON_PIN(11)|BUTTON_PIN(14))
#define BUTTON_ON_LEVEL 0
#define SD_PIN_NUM_CLK 36
#define SD_PIN_NUM_D00 37
#define SD_PIN_NUM_D01 38
#define SD_PIN_NUM_D02 33
#define SD_PIN_NUM_D03 34
#define SD_PIN_NUM_CMD 35
#define SD_CLOCK_HZ (40*1000*1000)
#endif // ESP_USB_OTG

#ifdef M5STACK_CORE2 // Works
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_touch_ft6x36
//            codewitch-honey-crisis/htcw_esp_lcd_panel_ili9342
//            codewitch-honey-crisis/htcw_m5_stack_core2_power
#include "esp_lcd_panel_ili9342.h"
#include "esp_lcd_touch_ft6x36.h"
#include "m5_stack_core2_power.h"
#endif
#define LCD_SPI_HOST    SPI_3
#define LCD_PIN_NUM_MOSI 23
#define LCD_PIN_NUM_CLK 18
#define LCD_PIN_NUM_CS 5
#define LCD_PIN_NUM_DC 15
#define LCD_INIT esp_lcd_new_panel_ili9342
#define LCD_HRES 320
#define LCD_VRES 240
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 0
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 0
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SDA 21
#define TOUCH_PIN_NUM_SCL 22
#define TOUCH_INIT esp_lcd_touch_new_i2c_ft6x36
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_FT6x36_ADDRESS
#define TOUCH_UPDATE_LIMIT_MS 13
#define TOUCH_CMD_BITS 8
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define TOUCH_CLOCK_HZ (200*1000)
#define TOUCH_PIN_NUM_RST -1
#define TOUCH_VRES 280
#define TOUCH_BOTTOM_OVERHANG 40
#define POWER_I2C_HOST I2C_1
#define POWER_PIN_NUM_SDA 21
#define POWER_PIN_NUM_SCL 22
#define POWER_CLOCK_HZ (200*1000) 
#define POWER_I2C_PULLUP 1
#define POWER_INIT m5_stack_core2_power_init(0)
#define SD_SPI_HOST SPI_3
#define SD_PIN_NUM_CS 4
#define SD_PIN_NUM_MOSI 23
#define SD_PIN_NUM_MISO 38
#endif // M5STACK_CORE2

#ifdef M5STACK_FIRE // Mine bricked so it's untested
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_panel_ili9342
#include "esp_lcd_panel_ili9342.h"
#endif
#define LCD_SPI_HOST    SPI_3
#define LCD_PIN_NUM_MOSI 23
#define LCD_PIN_NUM_CLK 18
#define LCD_PIN_NUM_CS 14
#define LCD_PIN_NUM_DC 27
#define LCD_PIN_NUM_RST 33
#define LCD_PIN_NUM_BCKL 32
#define LCD_INIT esp_lcd_new_panel_ili9342
#define LCD_HRES 320
#define LCD_VRES 240
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X 0
#define LCD_MIRROR_Y 0
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 0
#define BUTTON_MASK (BUTTON_PIN(39)|BUTTON_PIN(38)|BUTTON_PIN(37))
#define BUTTON_ON_LEVEL 0
#define SD_SPI_HOST    SPI_3
#define SD_PIN_NUM_MOSI 23
#define SD_PIN_NUM_MISO 19
#define SD_PIN_NUM_CLK 18
#define SD_PIN_NUM_CS 4
#endif // M5STACK_FIRE

#ifdef M5STACK_S3_ATOM // Untested (mine is bricked)
#define LCD_SPI_HOST    SPI_3
#define LCD_PIN_NUM_MOSI 21
#define LCD_PIN_NUM_CLK 17
#define LCD_PIN_NUM_CS 15
#define LCD_PIN_NUM_DC 33
#define LCD_PIN_NUM_RST 34
#define LCD_PIN_NUM_BCKL 16
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_HRES 128
#define LCD_VRES 128
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (40 * 1000 * 1000)
#define LCD_GAP_X 2
#define LCD_GAP_Y 1
#define LCD_MIRROR_X 1
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 1
#define LCD_SWAP_XY 0
#define BUTTON_MASK (BUTTON_PIN(41))
#define BUTTON_ON_LEVEL 0
#endif // M5STACK_S3_ATOM

#ifdef MATOUCH_ESP_DISPLAY_PARALLEL_35 // Untested. Mine died eventually
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_panel_ili9488
//      codewitch-honey-crisis/htcw_esp_lcd_touch_ft6x36
#include "esp_lcd_panel_ili9488.h"
#include "esp_lcd_touch_ft6x36.h"
#endif
#define LCD_PIN_NUM_CS 37
#define LCD_PIN_NUM_WR 35
#define LCD_PIN_NUM_RD 48
#define LCD_PIN_NUM_RS 36
#define LCD_PIN_NUM_D00 47
#define LCD_PIN_NUM_D01 21
#define LCD_PIN_NUM_D02 14
#define LCD_PIN_NUM_D03 13
#define LCD_PIN_NUM_D04 12
#define LCD_PIN_NUM_D05 11
#define LCD_PIN_NUM_D06 10
#define LCD_PIN_NUM_D07 9
#define LCD_PIN_NUM_D08 3
#define LCD_PIN_NUM_D09 8
#define LCD_PIN_NUM_D10 16
#define LCD_PIN_NUM_D11 15
#define LCD_PIN_NUM_D12 7
#define LCD_PIN_NUM_D13 6
#define LCD_PIN_NUM_D14 5
#define LCD_PIN_NUM_D15 4
#define LCD_PIN_NUM_BCKL 45
#define LCD_INIT esp_lcd_new_panel_ili9488
#define LCD_HRES 320
#define LCD_VRES 480
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_CLOCK_HZ (20 * 1000 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_MIRROR_X 1
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 0
#define LCD_SWAP_XY 1
#define LCD_SWAP_COLOR_BYTES 1
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SCL 39
#define TOUCH_PIN_NUM_SDA 38
#define TOUCH_INIT esp_lcd_touch_new_i2c_ft6x36
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_FT6x36_ADDRESS
#define TOUCH_UPDATE_LIMIT_MS 13
#define TOUCH_CMD_BITS 8
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define TOUCH_CLOCK_HZ (200*1000)
#define TOUCH_PIN_NUM_RST -1
#endif // MATOUCH_ESP_DISPLAY_PARALLEL_35

#ifdef MATOUCH_ESP_DISPLAY_PARALLEL_4 // Untested (I have one but it's in use right now, and inside something so I can't test touch)
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_touch_gt911
#include "esp_lcd_touch_gt911.h"
#endif
#define LCD_PIN_NUM_CS 1
#define LCD_PIN_NUM_SCL 12
#define LCD_PIN_NUM_SDA 11 
#define LCD_PIN_NUM_DE 45
#define LCD_PIN_NUM_VSYNC 4
#define LCD_PIN_NUM_HSYNC 5
#define LCD_PIN_NUM_CLK 21
#define LCD_PIN_NUM_D00 6
#define LCD_PIN_NUM_D01 7
#define LCD_PIN_NUM_D02 15
#define LCD_PIN_NUM_D03 16
#define LCD_PIN_NUM_D04 8
#define LCD_PIN_NUM_D05 0
#define LCD_PIN_NUM_D06 9
#define LCD_PIN_NUM_D07 14
#define LCD_PIN_NUM_D08 47
#define LCD_PIN_NUM_D09 48
#define LCD_PIN_NUM_D10 3
#define LCD_PIN_NUM_D11 39
#define LCD_PIN_NUM_D12 40
#define LCD_PIN_NUM_D13 41
#define LCD_PIN_NUM_D14 42
#define LCD_PIN_NUM_D15 2
#define LCD_PIN_NUM_BCKL -1
#define LCD_HSYNC_POLARITY 1
#define LCD_HSYNC_FRONT_PORCH 10
#define LCD_HSYNC_PULSE_WIDTH 8
#define LCD_HSYNC_BACK_PORCH 50
#define LCD_VSYNC_POLARITY 1
#define LCD_VSYNC_FRONT_PORCH 10
#define LCD_VSYNC_PULSE_WIDTH 8
#define LCD_VSYNC_BACK_PORCH 10
#define LCD_CLK_ON_LEVEL 1
#define LCD_DE_ON_LEVEL 1
#define LCD_BIT_DEPTH 16
#define LCD_INIT esp_lcd_new_panel_st7701
#define LCD_HRES 480
#define LCD_VRES 480
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_SWAP_COLOR_BYTES 1
#ifdef CONFIG_SPIRAM_MODE_QUAD
    #define LCD_CLOCK_HZ (6 * 1000 * 1000)
#else
    #define LCD_CLOCK_HZ (10 * 1000 * 1000)
#endif
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SDA 17
#define TOUCH_PIN_NUM_SCL 18
#define TOUCH_PIN_NUM_INT -1
#define TOUCH_PIN_NUM_RST 38
#define TOUCH_INIT esp_lcd_touch_new_i2c_gt911
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS
#define TOUCH_CLOCK_HZ (400*1000)
#define TOUCH_RST_ON_LEVEL 0
#define TOUCH_INT_ON_LEVEL 0
#define TOUCH_DC_BIT_OFFSET 0
#define TOUCH_CMD_BITS 16
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define SD_PIN_NUM_CLK 12
#define SD_PIN_NUM_MISO 13
#define SD_PIN_NUM_MOSI 11
#define SD_PIN_NUM_CS 10

#endif // MATOUCH_ESP_DISPLAY_PARALLEL_4

#ifdef MATOUCH_ESP_DISPLAY_PARALLEL_43 // Works
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_touch_gt911
#include "esp_lcd_touch_gt911.h"
#endif
#define LCD_PIN_NUM_DE 40
#define LCD_PIN_NUM_VSYNC 41
#define LCD_PIN_NUM_HSYNC 39
#define LCD_PIN_NUM_CLK 42
#define LCD_PIN_NUM_D00 8
#define LCD_PIN_NUM_D01 2
#define LCD_PIN_NUM_D02 46
#define LCD_PIN_NUM_D03 9
#define LCD_PIN_NUM_D04 1
#define LCD_PIN_NUM_D05 5
#define LCD_PIN_NUM_D06 6
#define LCD_PIN_NUM_D07 7
#define LCD_PIN_NUM_D08 15
#define LCD_PIN_NUM_D09 16
#define LCD_PIN_NUM_D10 4
#define LCD_PIN_NUM_D11 45
#define LCD_PIN_NUM_D12 48
#define LCD_PIN_NUM_D13 47
#define LCD_PIN_NUM_D14 21
#define LCD_PIN_NUM_D15 14
#define LCD_PIN_NUM_BCKL -1
#define LCD_HSYNC_POLARITY 0
#define LCD_HSYNC_FRONT_PORCH 8
#define LCD_HSYNC_PULSE_WIDTH 4
#define LCD_HSYNC_BACK_PORCH 8
#define LCD_VSYNC_POLARITY 0
#define LCD_VSYNC_FRONT_PORCH 8
#define LCD_VSYNC_PULSE_WIDTH 4
#define LCD_VSYNC_BACK_PORCH 8
#define LCD_CLK_ON_LEVEL 1
#define LCD_DE_ON_LEVEL 0
#define LCD_BIT_DEPTH 16
#define LCD_HRES 800
#define LCD_VRES 480
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_SWAP_COLOR_BYTES 1
#define LCD_TRANSFER_IN_SPIRAM 1
#ifdef CONFIG_SPIRAM_MODE_QUAD
    #define LCD_CLOCK_HZ (6 * 1000 * 1000)
#else
    #define LCD_CLOCK_HZ (16 * 1000 * 1000)
#endif
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SCL 18
#define TOUCH_PIN_NUM_SDA 17
#define TOUCH_INIT esp_lcd_touch_new_i2c_gt911
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS
#define TOUCH_CLOCK_HZ (400*1000)
#define TOUCH_PIN_NUM_RST 38
#define TOUCH_HRES 480
#define TOUCH_VRES 272
#define TOUCH_RST_ON_LEVEL 0
#define TOUCH_INT_ON_LEVEL 0
#define TOUCH_DC_BIT_OFFSET 0
#define TOUCH_CMD_BITS 16
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define SD_SPI_HOST SPI_3
#define SD_PIN_NUM_CLK 12
#define SD_PIN_NUM_MISO 13
#define SD_PIN_NUM_MOSI 11
#define SD_PIN_NUM_CS 10
#endif // MATOUCH_ESP_DISPLAY_PARALLEL_43

#ifdef MATOUCH_ESP_DISPLAY_PARALLEL_7 // untested 1024x600 only (I gave mine away)
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_touch_gt911
#define LCD_BCKL_ON_LEVEL 0
#define LCD_PIN_NUM_DE 40
#define LCD_PIN_NUM_VSYNC 41
#define LCD_PIN_NUM_HSYNC 39
#define LCD_PIN_NUM_CLK 42
#define LCD_PIN_NUM_D00 8
#define LCD_PIN_NUM_D01 3
#define LCD_PIN_NUM_D02 46
#define LCD_PIN_NUM_D03 9
#define LCD_PIN_NUM_D04 1
#define LCD_PIN_NUM_D05 5
#define LCD_PIN_NUM_D06 6
#define LCD_PIN_NUM_D07 7
#define LCD_PIN_NUM_D08 15
#define LCD_PIN_NUM_D09 16
#define LCD_PIN_NUM_D10 4
#define LCD_PIN_NUM_D11 45
#define LCD_PIN_NUM_D12 48
#define LCD_PIN_NUM_D13 47
#define LCD_PIN_NUM_D14 21
#define LCD_PIN_NUM_D15 14
#define LCD_PIN_NUM_BCKL 10
#define LCD_HSYNC_POLARITY 1
#define LCD_HSYNC_FRONT_PORCH 210
#define LCD_HSYNC_PULSE_WIDTH 30
#define LCD_HSYNC_BACK_PORCH 16
#define LCD_VSYNC_POLARITY 0
#define LCD_VSYNC_FRONT_PORCH 22
#define LCD_VSYNC_PULSE_WIDTH 13
#define LCD_VSYNC_BACK_PORCH 10
#define LCD_CLK_ON_LEVEL 0
#define LCD_DE_ON_LEVEL 0
#define LCD_BIT_DEPTH 16
#define LCD_HRES 1024
#define LCD_VRES 600
#define LCD_TRANSFER_IN_SPIRAM 1
#define LCD_COLOR_SPACE LCD_COLOR_BGR
#define LCD_SWAP_COLOR_BYTES 0
#ifdef CONFIG_SPIRAM_MODE_QUAD
    #define LCD_CLOCK_HZ (6 * 1000 * 1000)
#else
    #define LCD_CLOCK_HZ (10 * 1000 * 1000)
#endif
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SCL 18
#define TOUCH_PIN_NUM_SDA 17
#define TOUCH_INIT esp_lcd_touch_new_i2c_gt911
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS
#define TOUCH_CLOCK_HZ (400*1000)
#define TOUCH_PIN_NUM_RST 38
#define TOUCH_HRES 480
#define TOUCH_VRES 272
#define TOUCH_RST_ON_LEVEL 0
#define TOUCH_INT_ON_LEVEL 0
#define TOUCH_DC_BIT_OFFSET 0
#define TOUCH_CMD_BITS 16
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define SD_PIN_NUM_CMD 11
#define SD_PIN_NUM_CLK 12
#define SD_PIN_NUM_D00 13
#define SD_CLOCK_HZ (40*1000*1000)
#endif // MATOUCH_ESP_DISPLAY_PARALLEL_7

#ifdef WAVESHARE_S3_43_DEVKIT // works, sometimes reboots on initting the touch controller
#ifdef PANEL_DEPENDENCIES
#include "esp_lcd_touch_gt911.h"
#endif
#define LCD_PIN_NUM_DE 5
#define LCD_PIN_NUM_VSYNC 3
#define LCD_PIN_NUM_HSYNC 46
#define LCD_PIN_NUM_CLK 7
#define LCD_PIN_NUM_D00 14
#define LCD_PIN_NUM_D01 38
#define LCD_PIN_NUM_D02 18
#define LCD_PIN_NUM_D03 17
#define LCD_PIN_NUM_D04 10
#define LCD_PIN_NUM_D05 39
#define LCD_PIN_NUM_D06 0
#define LCD_PIN_NUM_D07 45
#define LCD_PIN_NUM_D08 48
#define LCD_PIN_NUM_D09 47
#define LCD_PIN_NUM_D10 21
#define LCD_PIN_NUM_D11 1
#define LCD_PIN_NUM_D12 2
#define LCD_PIN_NUM_D13 42
#define LCD_PIN_NUM_D14 41
#define LCD_PIN_NUM_D15 40
#define LCD_PIN_NUM_BCKL -1
#define LCD_HSYNC_POLARITY 0
#define LCD_HSYNC_FRONT_PORCH 20
#define LCD_HSYNC_PULSE_WIDTH 10
#define LCD_HSYNC_BACK_PORCH 10
#define LCD_VSYNC_POLARITY 0
#define LCD_VSYNC_FRONT_PORCH 10
#define LCD_VSYNC_PULSE_WIDTH 10
#define LCD_VSYNC_BACK_PORCH 10
#define LCD_CLK_ON_LEVEL 1
#define LCD_DE_ON_LEVEL 0
#define LCD_BIT_DEPTH 16
#define LCD_HRES 800
#define LCD_VRES 480
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_SWAP_COLOR_BYTES 1
#define LCD_TRANSFER_IN_SPIRAM
#ifdef CONFIG_SPIRAM_MODE_QUAD
    #define LCD_CLOCK_HZ (6 * 1000 * 1000)
#else
    #define LCD_CLOCK_HZ (14 * 1000 * 1000)
#endif
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SCL           9
#define TOUCH_PIN_NUM_SDA           8
#define TOUCH_I2C_PULLUP 1
#define TOUCH_INIT esp_lcd_touch_new_i2c_gt911
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS
#define TOUCH_CLOCK_HZ (400*1000)
#define TOUCH_HRES LCD_HRES
#define TOUCH_VRES LCD_VRES
#define TOUCH_RST_ON_LEVEL 0
#define TOUCH_INT_ON_LEVEL 0
#define TOUCH_DC_BIT_OFFSET 0
#define TOUCH_CMD_BITS 16
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#ifndef LEGACY_I2C
#define TOUCH_RESET \
    i2c_master_bus_handle_t bus; \
    ESP_ERROR_CHECK(i2c_master_get_bus_handle((i2c_port_num_t)TOUCH_I2C_HOST,&bus)); \
    i2c_master_dev_handle_t i2c=NULL; \
    i2c_device_config_t dev_cfg; \
    memset(&dev_cfg,0,sizeof(dev_cfg)); \
    dev_cfg.scl_speed_hz = 200*1000; \
    dev_cfg.device_address = 0x24; \
    dev_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7; \
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &dev_cfg,&i2c)); \
    uint8_t write_buf = 0x01; \
    ESP_ERROR_CHECK(i2c_master_transmit(i2c,&write_buf,1,1000 )); \
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(i2c)); \
    dev_cfg.device_address = 0x38; \
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus, &dev_cfg,&i2c)); \
    write_buf = 0x2C; \
    ESP_ERROR_CHECK(i2c_master_transmit(i2c,&write_buf,1,1000 )); \
    esp_rom_delay_us(100 * 1000); \
    gpio_set_level((gpio_num_t)4, 0); \
    esp_rom_delay_us(100 * 1000); \
    write_buf = 0x2E; \
    ESP_ERROR_CHECK(i2c_master_transmit(i2c,&write_buf,1,1000 )); \
    esp_rom_delay_us(200 * 1000); \
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(i2c))
#else
#define TOUCH_RESET \
    uint8_t write_buf = 0x01;\
    ESP_ERROR_CHECK(i2c_master_write_to_device((i2c_port_t)TOUCH_I2C_HOST,0x24,&write_buf,1,portMAX_DELAY));\
    write_buf = 0x2c;\
    ESP_ERROR_CHECK(i2c_master_write_to_device((i2c_port_t)TOUCH_I2C_HOST,0x38,&write_buf,1,portMAX_DELAY));\
    esp_rom_delay_us(100 * 1000);\
    gpio_set_level((gpio_num_t)4, 0);\
    write_buf = 0x2E;\
    ESP_ERROR_CHECK(i2c_master_write_to_device((i2c_port_t)TOUCH_I2C_HOST,0x38,&write_buf,1,portMAX_DELAY));\
    esp_rom_delay_us(200 * 1000)
#endif
#define SD_SPI_HOST SPI_3
#define SD_PIN_NUM_CS 4 
#define SD_PIN_NUM_MOSI 11    // SD card master output slave input pin
#define SD_PIN_NUM_CLK  12    // SD card clock pin
#define SD_PIN_NUM_MISO 13    // SD card master input slave output pin
#endif // WAVESHARE_S3_43_DEVKIT

#ifdef WAVESHARE_P4_SMART86BOX // Works
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_st7703
//    codewitch-honey-crisis/htcw_esp_lcd_touch_gt911
#include "esp_lcd_st7703.h"
#include "esp_lcd_touch_gt911.h"
#endif
#define LCD_TRANSFER_IN_SPIRAM
#define LCD_BCKL_ON_LEVEL 0
#define LCD_HRES 720
#define LCD_VRES 720
#define LCD_HSYNC_FRONT_PORCH 80
#define LCD_HSYNC_PULSE_WIDTH 20
#define LCD_HSYNC_BACK_PORCH 80
#define LCD_VSYNC_FRONT_PORCH 30
#define LCD_VSYNC_PULSE_WIDTH 4
#define LCD_VSYNC_BACK_PORCH 12
#define LCD_BIT_DEPTH 16
#define LCD_MIPI_DMA2D 1
#define LCD_INIT esp_lcd_new_panel_st7703
#define LCD_DATA_ENDIAN_LITTLE 1
#define LCD_MIPI_DSI_PHY_PWR_LDO_CHAN (3)
#define LCD_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV (2500)
#define LCD_VENDOR_CONFIG \
st7703_vendor_config_t vendor_config = { \
    .mipi_config = { \
        .dsi_bus = mipi_dsi_bus, \
        .dpi_config = &dpi_config, \
    }, \
    .flags = { \
        .use_mipi_interface = 1, \
    }, \
}
#define LCD_PIN_NUM_RST 27
#define LCD_PIN_NUM_BCKL 26
#define LCD_CLOCK_HZ (46 * 1000 * 1000)
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_PIN_NUM_SDA 7
#define TOUCH_PIN_NUM_SCL 8
#define TOUCH_CLOCK_HZ (100*1000)
#define TOUCH_INIT esp_lcd_touch_new_i2c_gt911
#define TOUCH_RST_ON_LEVEL 0
#define TOUCH_INT_ON_LEVEL 0
#define TOUCH_DC_BIT_OFFSET 0
#define TOUCH_CMD_BITS 16
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_GT911_ADDRESS
#endif // WAVESHARE_P4_SMART86BOX

#ifdef HELTEC_WIFI_LORA_KIT_V2 // should work. the display broke on mine though
#define LCD_I2C_HOST    I2C_1
#define LCD_I2C_ADDRESS 0x3C
#define LCD_CONTROL_PHASE_BYTES 1
#define LCD_DC_BIT_OFFSET 6
#define LCD_BIT_DEPTH 1
#define LCD_PIN_NUM_SCL 15
#define LCD_PIN_NUM_SDA 4
#define LCD_PIN_NUM_RST 16
#define LCD_INIT esp_lcd_new_panel_ssd1306
#define LCD_HRES 128
#define LCD_VRES 64
#define LCD_COLOR_SPACE LCD_COLOR_GSC
#define LCD_CLOCK_HZ (400 * 1000)
#define LCD_GAP_X 0
#define LCD_GAP_Y 0
#define LCD_INVERT_COLOR 0
#define LCD_SWAP_XY 0
#define LCD_MIRROR_X 1
#define LCD_MIRROR_Y 1
#define LCD_INVERT_COLOR 0
#define LCD_SWAP_XY 0
#define LCD_DIVISOR 1
#define LCD_Y_ALIGN 8
#define LCD_TRANSFER_PALETTE 1
#define LCD_VENDOR_CONFIG esp_lcd_panel_ssd1306_config_t vendor_config = {\
    .height = LCD_VRES,\
};
#define LCD_TRANSLATE static uint8_t ssd1306_buffer[LCD_TRANSFER_SIZE];\
    bitmap = ((uint8_t*)bitmap)+LCD_TRANSFER_PALETTE_SIZE; \
     int src_width = x2 - x1 + 1;\
     int dst_width = src_width;\
     int dst_height_pages = (y2 - y1 + 1) >> 3;  /* Height in pages (8-pixel groups) */\
     \
     for (int page = 0; page < dst_height_pages; page++) {\
        for (int x = 0; x < dst_width; x++) {\
            uint8_t dst_byte = 0;\
            for (int bit = 0; bit < 8; bit++) {\
                /* Calculate source bit position */\
                int src_y = page * 8 + bit;\
                int total_bit_offset = src_y * src_width + x;\
                int src_byte_index = total_bit_offset >> 3;\
                int src_bit = 7 - (total_bit_offset & 7);\
                \
                if (((uint8_t*)bitmap)[src_byte_index] & (1 << src_bit)) {\
                    dst_byte |= (1 << bit);\
                }\
            }\
            ssd1306_buffer[page * dst_width + x] = dst_byte;\
        }\
     }\
     bitmap = ssd1306_buffer;
#endif // HELTEC_WIFI_LORA_KIT_V2

#ifdef FREENOVE_S3_DEVKIT // Works (This is their full development kit with the integrated display, not just the s3 devkit)
#ifdef PANEL_DEPENDENCIES
// lib_deps = codewitch-honey-crisis/htcw_esp_lcd_touch_ft6x36
#include "esp_lcd_touch_ft6x36.h"
#endif
#define LCD_SPI_HOST SPI_3
#define LCD_PIN_NUM_DC 0
#define LCD_PIN_NUM_CS 47
#define LCD_PIN_NUM_MOSI 20
#define LCD_PIN_NUM_CLK 21
#define LCD_HRES 240
#define LCD_VRES 320
#define LCD_COLOR_SPACE LCD_COLOR_RGB
#define LCD_CLOCK_HZ (80 * 1000 * 1000)
#define LCD_MIRROR_X 1
#define LCD_MIRROR_Y 0
#define LCD_INIT esp_lcd_new_panel_st7789
#define LCD_INVERT_COLOR 0
#define LCD_SWAP_XY 1
#define TOUCH_I2C_HOST I2C_1
#define TOUCH_I2C_PULLUP 1
#define TOUCH_PIN_NUM_SCL 1
#define TOUCH_PIN_NUM_SDA 2
#define TOUCH_INIT esp_lcd_touch_new_i2c_ft6x36
#define TOUCH_I2C_ADDRESS ESP_LCD_TOUCH_IO_I2C_FT6x36_ADDRESS
#define TOUCH_UPDATE_LIMIT_MS 13
#define TOUCH_CMD_BITS 8
#define TOUCH_PARAM_BITS 0
#define TOUCH_DISABLE_CONTROL_PHASE 1
#define TOUCH_CLOCK_HZ (200*1000)
#define TOUCH_PIN_NUM_RST -1
#define SD_PIN_NUM_D00 40
#define SD_PIN_NUM_CMD 38
#define SD_PIN_NUM_CLK 39
#define SD_CLOCK_HZ (40*1000*1000)
#endif // FREENOVE_S3_DEVKIT

// END devices

#endif // PANELS_H