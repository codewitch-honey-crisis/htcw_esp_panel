#include "panel.h"
#include <memory.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#if LCD_BUS == PANEL_BUS_SPI || TOUCH_BUS == PANEL_BUS_SPI || POWER_BUS == PANEL_BUS_SPI
#include "driver/spi_master.h"
#endif
#if LCD_BUS == PANEL_BUS_I2C || TOUCH_BUS == PANEL_BUS_I2C || POWER_BUS == PANEL_BUS_I2C
#ifndef LEGACY_I2C
#include "driver/i2c_master.h"
#else
#include "driver/i2c.h"
#endif
#endif
#ifdef LCD_PHY_PWR_LDO_CHAN
#include "esp_ldo_regulator.h"
#endif
#if LCD_BUS == PANEL_BUS_MIPI
#include "esp_dma_utils.h"
#include "esp_lcd_mipi_dsi.h"
#endif
#if LCD_BUS == PANEL_BUS_RGB
#include "esp_lcd_panel_rgb.h"
#endif
static const char* TAG = "lcd";
static esp_lcd_panel_handle_t lcd_handle = NULL;
#if LCD_BUS != PANEL_BUS_RGB
static esp_lcd_panel_io_handle_t lcd_io_handle = NULL;
#endif
#if LCD_BUS == PANEL_BUS_MIPI
static esp_ldo_channel_handle_t ldo_mipi_phy = NULL;
static esp_lcd_dsi_bus_handle_t mipi_dsi_bus = NULL;
#endif
#if LCD_TRANSFER_SIZE > 0
static void* draw_buffer = NULL;
#ifndef LCD_NO_DMA
static void* draw_buffer2 = NULL;
#endif
#endif
#ifdef LCD_PIN_NUM_VSYNC
static volatile bool vsync_count = 0;
#endif

#if (defined(LCD_BUS) && LCD_BUS == PANEL_BUS_SPI) || (defined(TOUCH_BUS) && TOUCH_BUS == PANEL_BUS_SPI) || (defined(POWER_BUS) && POWER_BUS == PANEL_BUS_SPI)
static bool spi_initialized[4] = { false };
static void spi_init() {
#ifdef LCD_BUS
#if LCD_BUS == PANEL_BUS_SPI
    if(!spi_initialized[LCD_SPI_HOST]) {    
        spi_bus_config_t spi_cfg;
        memset(&spi_cfg,0,sizeof(spi_cfg));
        #if LCD_TRANSFER_SIZE > 0
        uint32_t spi_sz = LCD_TRANSFER_SIZE+8;
        #else
        uint32_t spi_sz = 32*1024;
        #endif
        if(spi_sz>32*1024) {
            ESP_LOGW(TAG,"SPI transfer size is limited to 32KB, but draw buffer demands more. Increase the LCD_DIVISOR");
            spi_sz = 32*1024;
        }
        spi_cfg.max_transfer_sz = spi_sz;
        spi_cfg.data0_io_num = -1;
        spi_cfg.data1_io_num = -1;
        spi_cfg.data2_io_num = -1;
        spi_cfg.data3_io_num = -1;
        spi_cfg.data4_io_num = -1;
        spi_cfg.data5_io_num = -1;
        spi_cfg.data6_io_num = -1;
        spi_cfg.data7_io_num = -1;
        spi_cfg.quadhd_io_num = -1;
        spi_cfg.quadwp_io_num = -1;
        spi_cfg.mosi_io_num = LCD_PIN_NUM_MOSI;
        spi_cfg.sclk_io_num = LCD_PIN_NUM_CLK;
#ifdef LCD_PIN_NUM_MISO
        spi_cfg.miso_io_num = LCD_PIN_NUM_MISO;
#else    
        spi_cfg.miso_io_num = -1;
#endif
        ESP_ERROR_CHECK(spi_bus_initialize((spi_host_device_t)LCD_SPI_HOST,&spi_cfg,SPI_DMA_CH_AUTO));
        spi_initialized[LCD_SPI_HOST] = true;
    }
#endif

#ifdef TOUCH_BUS
#if TOUCH_BUS == PANEL_BUS_SPI
    if(!spi_initialized[TOUCH_SPI_HOST]) {    
        spi_bus_config_t spi_cfg;
        memset(&spi_cfg,0,sizeof(spi_cfg));
        uint32_t spi_sz = TOUCH_TRANSFER_SIZE+8;
        if(spi_sz>32*1024) {
            ESP_LOGW(TAG,"SPI transfer size is limited to 32KB, but touch device is set to more. Decrease TOUCH_TRANSFER_SIZE");
            spi_sz = 32*1024;
        }
        spi_cfg.max_transfer_sz = spi_sz;
        spi_cfg.data0_io_num = -1;
        spi_cfg.data1_io_num = -1;
        spi_cfg.data2_io_num = -1;
        spi_cfg.data3_io_num = -1;
        spi_cfg.data4_io_num = -1;
        spi_cfg.data5_io_num = -1;
        spi_cfg.data6_io_num = -1;
        spi_cfg.data7_io_num = -1;
        spi_cfg.quadhd_io_num = -1;
        spi_cfg.quadwp_io_num = -1;
#ifdef TOUCH_PIN_NUM_MOSI
        spi_cfg.mosi_io_num = TOUCH_PIN_NUM_MOSI;
#endif
        spi_cfg.sclk_io_num = TOUCH_PIN_NUM_CLK;
        spi_cfg.miso_io_num = TOUCH_PIN_NUM_MISO;
        ESP_ERROR_CHECK(spi_bus_initialize((spi_host_device_t)TOUCH_SPI_HOST,&spi_cfg,SPI_DMA_CH_AUTO));
        spi_initialized[TOUCH_SPI_HOST] = true;
    }
#endif
#endif

#ifdef POWER_BUS
#if POWER_BUS == PANEL_BUS_SPI
    if(!spi_initialized[POWER_SPI_HOST]) {    
        spi_bus_config_t spi_cfg;
        memset(&spi_cfg,0,sizeof(spi_cfg));
        uint32_t spi_sz = POWER_TRANSFER_SIZE+8;
        if(spi_sz>32*1024) {
            ESP_LOGW(TAG,"SPI transfer size is limited to 32KB, but power device is set to more. Decrease POWER_TRANSFER_SIZE");
            spi_sz = 32*1024;
        }
        spi_cfg.max_transfer_sz = spi_sz;
        spi_cfg.data0_io_num = -1;
        spi_cfg.data1_io_num = -1;
        spi_cfg.data2_io_num = -1;
        spi_cfg.data3_io_num = -1;
        spi_cfg.data4_io_num = -1;
        spi_cfg.data5_io_num = -1;
        spi_cfg.data6_io_num = -1;
        spi_cfg.data7_io_num = -1;
        spi_cfg.quadhd_io_num = -1;
        spi_cfg.quadwp_io_num = -1;
#ifdef POWER_PIN_NUM_MOSI
        spi_cfg.mosi_io_num = POWER_PIN_NUM_MOSI;
#endif
        spi_cfg.sclk_io_num = POWER_PIN_NUM_CLK;
#ifdef POWER_PIN_NUM_MISO
        spi_cfg.miso_io_num = POWER_PIN_NUM_MISO;
#endif
        ESP_ERROR_CHECK(spi_bus_initialize((spi_host_device_t)POWER_SPI_HOST,&spi_cfg,SPI_DMA_CH_AUTO));
        spi_initialized[POWER_SPI_HOST] = true;
    }
#endif
#endif
}
#endif
#endif

#if (defined(LCD_BUS) && LCD_BUS == PANEL_BUS_I2C) || (defined(TOUCH_BUS) && TOUCH_BUS == PANEL_BUS_I2C) || (defined(POWER_BUS) && POWER_BUS == PANEL_BUS_I2C)
static bool i2c_initialized[2] = { false };
static void i2c_init() {
#ifdef LCD_BUS
#if LCD_BUS == PANEL_BUS_I2C
    if(!i2c_initialized[LCD_I2C_HOST]) {    
#ifndef LEGACY_I2C
        i2c_master_bus_config_t i2c_cfg;
        i2c_master_bus_handle_t i2c_bus_handle;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
        i2c_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
        i2c_cfg.glitch_ignore_cnt = 7;
        i2c_cfg.i2c_port = (i2c_port_num_t)LCD_I2C_HOST;
        // Doesn't actually work, which is why it's commented out.
// #ifndef LCD_NO_DMA
//      i2c_cfg.trans_queue_depth = 10;
// #endif
        i2c_cfg.sda_io_num = (gpio_num_t)LCD_PIN_NUM_SDA;
        i2c_cfg.scl_io_num = (gpio_num_t)LCD_PIN_NUM_SCL;
        // TODO: make configurable
#if defined(LCD_I2C_PULLUP)
        i2c_cfg.flags.enable_internal_pullup = 1;
#else
        i2c_cfg.flags.enable_internal_pullup = 0;
#endif

        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_cfg,&i2c_bus_handle));
#else
        i2c_config_t i2c_cfg;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
#ifdef LCD_CLOCK_HZ
        i2c_cfg.master.clk_speed = LCD_CLOCK_HZ;
#else
        i2c_cfg.master.clk_speed = 100 * 1000;
#endif
        i2c_cfg.mode = I2C_MODE_MASTER;
        i2c_cfg.sda_io_num = LCD_PIN_NUM_SDA;
#if defined(LCD_I2C_PULLUP)
        i2c_cfg.sda_pullup_en = 1;
#else
        i2c_cfg.sda_pullup_en = 0;
#endif

        i2c_cfg.scl_io_num = LCD_PIN_NUM_SCL;
#if defined(LCD_I2C_PULLUP)
        i2c_cfg.scl_pullup_en = 1;
#else
        i2c_cfg.scl_pullup_en = 0;
#endif
        ESP_ERROR_CHECK(i2c_param_config((i2c_port_t)LCD_I2C_HOST,&i2c_cfg));
        ESP_ERROR_CHECK(i2c_driver_install((i2c_port_t)LCD_I2C_HOST,I2C_MODE_MASTER,0,0,0));
#endif    
        i2c_initialized[LCD_I2C_HOST] = true;
    }
#endif

#ifdef TOUCH_BUS
#if TOUCH_BUS == PANEL_BUS_I2C
    if(!i2c_initialized[TOUCH_I2C_HOST]) {    
#ifndef LEGACY_I2C
        i2c_master_bus_config_t i2c_cfg;
        i2c_master_bus_handle_t i2c_bus_handle;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
        i2c_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
        i2c_cfg.glitch_ignore_cnt = 7;
        i2c_cfg.i2c_port = (i2c_port_num_t)TOUCH_I2C_HOST;
        i2c_cfg.sda_io_num = (gpio_num_t)TOUCH_PIN_NUM_SDA;
        i2c_cfg.scl_io_num = (gpio_num_t)TOUCH_PIN_NUM_SCL;
        // TODO: make configurable
#if defined(TOUCH_I2C_PULLUP)
        i2c_cfg.flags.enable_internal_pullup = 1;
#else
        i2c_cfg.flags.enable_internal_pullup = 0;
#endif


        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_cfg,&i2c_bus_handle));
#else
        i2c_config_t i2c_cfg;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
#ifdef TOUCH_CLOCK_HZ
        i2c_cfg.master.clk_speed = TOUCH_CLOCK_HZ;
#else
        i2c_cfg.master.clk_speed = 100 * 1000;
#endif
        i2c_cfg.mode = I2C_MODE_MASTER;
        i2c_cfg.sda_io_num = TOUCH_PIN_NUM_SDA;
#if defined(TOUCH_I2C_PULLUP)
        i2c_cfg.sda_pullup_en = 1;
#else
        i2c_cfg.sda_pullup_en = 0;
#endif
        i2c_cfg.scl_io_num = TOUCH_PIN_NUM_SCL;
#if defined(TOUCH_I2C_PULLUP)
        i2c_cfg.scl_pullup_en = 1;
#else
        i2c_cfg.scl_pullup_en = 0;
#endif

        ESP_ERROR_CHECK(i2c_driver_install((i2c_port_t)TOUCH_I2C_HOST,I2C_MODE_MASTER,0,0,0));
        ESP_ERROR_CHECK(i2c_param_config((i2c_port_t)TOUCH_I2C_HOST,&i2c_cfg));
#endif    
        i2c_initialized[TOUCH_I2C_HOST] = true;
    }
#endif
#endif

#ifdef POWER_BUS
#if POWER_BUS == PANEL_BUS_I2C
    if(!i2c_initialized[POWER_I2C_HOST]) {    
#ifndef LEGACY_I2C
        i2c_master_bus_config_t i2c_cfg;
        i2c_master_bus_handle_t i2c_bus_handle;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
        i2c_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
        i2c_cfg.glitch_ignore_cnt = 7;
        i2c_cfg.i2c_port = (i2c_port_num_t)POWER_I2C_HOST;
        i2c_cfg.sda_io_num = (gpio_num_t)POWER_PIN_NUM_SDA;
        i2c_cfg.scl_io_num = (gpio_num_t)POWER_PIN_NUM_SCL;
        // TODO: make configurable
#ifdef POWER_I2C_PULLUP
        i2c_cfg.flags.enable_internal_pullup = 1;
#else
        i2c_cfg.flags.enable_internal_pullup = 0;
#endif


        ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_cfg,&i2c_bus_handle));
#else
        i2c_config_t i2c_cfg;
        memset(&i2c_cfg,0,sizeof(i2c_cfg));
#ifdef POWER_CLOCK_HZ
        i2c_cfg.master.clk_speed = POWER_CLOCK_HZ;
#else
        i2c_cfg.master.clk_speed = 100 * 1000;
#endif
        i2c_cfg.mode = I2C_MODE_MASTER;
        i2c_cfg.sda_io_num = POWER_PIN_NUM_SDA;
#ifdef POWER_I2C_PULLUP
        i2c_cfg.sda_pullup_en = 1;
#else
        i2c_cfg.sda_pullup_en = 0;
#endif
        i2c_cfg.scl_io_num = POWER_PIN_NUM_SCL;
#if defined(POWER_I2C_PULLUP)
        i2c_cfg.scl_pullup_en = 1;
#else
        i2c_cfg.scl_pullup_en = 0;
#endif

        ESP_ERROR_CHECK(i2c_driver_install((i2c_port_t)POWER_I2C_HOST,I2C_MODE_MASTER,0,0,0));
        ESP_ERROR_CHECK(i2c_param_config((i2c_port_t)POWER_I2C_HOST,&i2c_cfg));
#endif    
        i2c_initialized[POWER_I2C_HOST] = true;
    }
#endif
#endif
}
#endif
#endif

#ifdef LCD_BUS
size_t lcd_vsync_flush_count(void) { 
#ifdef LCD_PIN_NUM_VSYNC
    return vsync_count; 
#else
    return 0;
#endif
}

void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void *bitmap)
{
    if(lcd_handle==NULL) {
        ESP_LOGE(TAG,"lcd_flush() was invoked but lcd_init() was never called.");
        return;
    }
#ifdef LCD_TRANSLATE
    LCD_TRANSLATE;
#endif
    // pass the draw buffer to the driver
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(lcd_handle, x1, y1, x2 + 1, y2 + 1, bitmap));
#if LCD_PIN_NUM_VSYNC
    vsync_count = vsync_count + 1;
#endif
}

#if !defined(LCD_NO_DMA) && LCD_BUS == PANEL_BUS_RGB
// LCD Panel API calls this
static IRAM_ATTR bool on_flush_complete(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx) {
    // let the display know the flush has finished
    lcd_flush_complete();
    return true;
}
// LCD Panel API calls this
static IRAM_ATTR bool on_vsync(esp_lcd_panel_handle_t panel, const esp_lcd_rgb_panel_event_data_t *edata, void *user_ctx) {
    vsync_count = 0;
    return true;
}
#endif
#if !defined(LCD_NO_DMA) && (LCD_BUS==PANEL_BUS_SPI || LCD_BUS==PANEL_BUS_I2C || LCD_BUS==PANEL_BUS_I8080)
static IRAM_ATTR bool on_flush_complete(esp_lcd_panel_io_handle_t lcd_io, esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
    // let the display know the flush has finished
    lcd_flush_complete();
    return true;
}
#endif
#if !defined(LCD_NO_DMA) && LCD_BUS == PANEL_BUS_MIPI
static IRAM_ATTR bool on_flush_complete(esp_lcd_panel_handle_t panel, esp_lcd_dpi_panel_event_data_t *edata, void *user_ctx) {
    // let the display know the flush has finished
    lcd_flush_complete();
    return true;
}
#endif
void lcd_init(void) {
    if(lcd_handle!=NULL) {
        ESP_LOGW(TAG,"lcd_init() was already called");
        return; // already initialized
    }
#ifdef LCD_RESET
    LCD_RESET;
#endif
#ifdef LCD_PIN_NUM_BCKL
#if LCD_PIN_NUM_BCKL >= 0
    gpio_config_t bk_gpio_config;
    memset(&bk_gpio_config,0,sizeof(gpio_config_t));
    bk_gpio_config.mode = GPIO_MODE_OUTPUT;
    bk_gpio_config.pin_bit_mask = 1ULL << LCD_PIN_NUM_BCKL;
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level((gpio_num_t)LCD_PIN_NUM_BCKL, LCD_BCKL_OFF_LEVEL);
#endif
#endif
    // Turn on the power for MIPI DSI PHY, so it can go from "No Power" state to "Shutdown" state
#ifdef LCD_PHY_PWR_LDO_CHAN
    esp_ldo_channel_config_t ldo_mipi_phy_config = {
        .chan_id = LCD_MIPI_DSI_PHY_PWR_LDO_CHAN,
        .voltage_mv = LCD_MIPI_DSI_PHY_PWR_LDO_VOLTAGE_MV,
    };
    ESP_ERROR_CHECK(esp_ldo_acquire_channel(&ldo_mipi_phy_config, &ldo_mipi_phy));
#endif
#if LCD_BUS == PANEL_BUS_I8080
    gpio_set_direction((gpio_num_t)LCD_PIN_NUM_RD, GPIO_MODE_OUTPUT);
    gpio_set_level((gpio_num_t)LCD_PIN_NUM_RD, 1);
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t i80_cfg;
    memset(&i80_cfg, 0, sizeof(i80_cfg));
    i80_cfg.clk_src = LCD_CLK_SRC_PLL160M;
    i80_cfg.dc_gpio_num = LCD_PIN_NUM_RS;
    i80_cfg.wr_gpio_num = LCD_PIN_NUM_WR;
#ifndef LCD_PIN_NUM_D15    
    i80_cfg.data_gpio_nums[0] = LCD_PIN_NUM_D00;
    i80_cfg.data_gpio_nums[1] = LCD_PIN_NUM_D01;
    i80_cfg.data_gpio_nums[2] = LCD_PIN_NUM_D02;
    i80_cfg.data_gpio_nums[3] = LCD_PIN_NUM_D03;
    i80_cfg.data_gpio_nums[4] = LCD_PIN_NUM_D04;
    i80_cfg.data_gpio_nums[5] = LCD_PIN_NUM_D05;
    i80_cfg.data_gpio_nums[6] = LCD_PIN_NUM_D06;
    i80_cfg.data_gpio_nums[7] = LCD_PIN_NUM_D07;
    bus_config.bus_width = 8;
#else
    i80_cfg.data_gpio_nums[0] = LCD_PIN_NUM_D00;
    i80_cfg.data_gpio_nums[1] = LCD_PIN_NUM_D01;
    i80_cfg.data_gpio_nums[2] = LCD_PIN_NUM_D02;
    i80_cfg.data_gpio_nums[3] = LCD_PIN_NUM_D03;
    i80_cfg.data_gpio_nums[4] = LCD_PIN_NUM_D04;
    i80_cfg.data_gpio_nums[5] = LCD_PIN_NUM_D05;
    i80_cfg.data_gpio_nums[6] = LCD_PIN_NUM_D06;
    i80_cfg.data_gpio_nums[7] = LCD_PIN_NUM_D07;
    i80_cfg.data_gpio_nums[8] = LCD_PIN_NUM_D08;
    i80_cfg.data_gpio_nums[9] = LCD_PIN_NUM_D09;
    i80_cfg.data_gpio_nums[10] = LCD_PIN_NUM_D10;
    i80_cfg.data_gpio_nums[11] = LCD_PIN_NUM_D11;
    i80_cfg.data_gpio_nums[12] = LCD_PIN_NUM_D12;
    i80_cfg.data_gpio_nums[13] = LCD_PIN_NUM_D13;
    i80_cfg.data_gpio_nums[14] = LCD_PIN_NUM_D14;
    i80_cfg.data_gpio_nums[15] = LCD_PIN_NUM_D15;
    i80_cfg.bus_width = 16;
#endif  // LCD_PIN_NUM_D15
    i80_cfg.max_transfer_bytes = LCD_TRANSFER_SIZE;
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&i80_cfg, &i80_bus));
    esp_lcd_panel_io_i80_config_t i80_io_cfg;
    memset(&i80_io_cfg, 0, sizeof(i80_io_cfg));
    i80_io_cfg.cs_gpio_num = LCD_PIN_NUM_CS;
    i80_io_cfg.pclk_hz = LCD_CLOCK_HZ;
    i80_io_cfg.trans_queue_depth = 20;
    i80_io_cfg.dc_levels.dc_idle_level = 0;
    i80_io_cfg.dc_levels.dc_idle_level = 0;
    i80_io_cfg.dc_levels.dc_cmd_level = 0;
    i80_io_cfg.dc_levels.dc_dummy_level = 0;
    i80_io_cfg.dc_levels.dc_data_level = 1;
    i80_io_cfg.lcd_cmd_bits = LCD_CMD_BITS;
    i80_io_cfg.lcd_param_bits = LCD_PARAM_BITS;
#ifndef LCD_NO_DMA
    i80_io_cfg.on_color_trans_done = on_flush_complete;
#endif
    i80_io_cfg.user_ctx = NULL;
#ifdef LCD_SWAP_COLOR_BYTES
    i80_io_cfg.flags.swap_color_bytes = LCD_SWAP_COLOR_BYTES;
#else
    io_config.flags.swap_color_bytes = false;
#endif  // LCD_SWAP_COLOR_BYTES
    // TODO: make the following configurable
    i80_io_cfg.flags.cs_active_high = false;
    i80_io_cfg.flags.reverse_color_bits = false;
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &i80_io_cfg, &lcd_io_handle));
#endif
#if LCD_BUS == PANEL_BUS_RGB
    esp_lcd_rgb_panel_config_t rgb_panel_cfg;
    memset(&rgb_panel_cfg,0,sizeof(esp_lcd_rgb_panel_config_t));
    rgb_panel_cfg.data_width = 16; // RGB565 in parallel mode, thus 16bit in width
    //rgb_panel_cfg.dma_burst_size = 64;
    rgb_panel_cfg.num_fbs = 1,
    rgb_panel_cfg.clk_src = LCD_CLK_SRC_DEFAULT,
    rgb_panel_cfg.disp_gpio_num = -1,
    rgb_panel_cfg.pclk_gpio_num = LCD_PIN_NUM_CLK,
    rgb_panel_cfg.vsync_gpio_num = LCD_PIN_NUM_VSYNC,
    rgb_panel_cfg.hsync_gpio_num = LCD_PIN_NUM_HSYNC,
    rgb_panel_cfg.de_gpio_num = LCD_PIN_NUM_DE,
#if LCD_BIT_DEPTH == 8
    rgb_panel_cfg.data_gpio_nums[8]=LCD_PIN_NUM_D08;
    rgb_panel_cfg.data_gpio_nums[9]=LCD_PIN_NUM_D09;
    rgb_panel_cfg.data_gpio_nums[10]=LCD_PIN_NUM_D10;
    rgb_panel_cfg.data_gpio_nums[11]=LCD_PIN_NUM_D11;
    rgb_panel_cfg.data_gpio_nums[12]=LCD_PIN_NUM_D12;
    rgb_panel_cfg.data_gpio_nums[13]=LCD_PIN_NUM_D13;
    rgb_panel_cfg.data_gpio_nums[14]=LCD_PIN_NUM_D14;
    rgb_panel_cfg.data_gpio_nums[15]=LCD_PIN_NUM_D15;
#elif !defined(LCD_SWAP_COLOR_BYTES) || LCD_SWAP_COLOR_BYTES == false
#if LCD_COLOR_SPACE != LCD_COLOR_BGR
    rgb_panel_cfg.data_gpio_nums[0]=LCD_PIN_NUM_D00;
    rgb_panel_cfg.data_gpio_nums[1]=LCD_PIN_NUM_D01;
    rgb_panel_cfg.data_gpio_nums[2]=LCD_PIN_NUM_D02;
    rgb_panel_cfg.data_gpio_nums[3]=LCD_PIN_NUM_D03;
    rgb_panel_cfg.data_gpio_nums[4]=LCD_PIN_NUM_D04;
    rgb_panel_cfg.data_gpio_nums[5]=LCD_PIN_NUM_D05;
    rgb_panel_cfg.data_gpio_nums[6]=LCD_PIN_NUM_D06;
    rgb_panel_cfg.data_gpio_nums[7]=LCD_PIN_NUM_D07;
    rgb_panel_cfg.data_gpio_nums[8]=LCD_PIN_NUM_D08;
    rgb_panel_cfg.data_gpio_nums[9]=LCD_PIN_NUM_D09;
    rgb_panel_cfg.data_gpio_nums[10]=LCD_PIN_NUM_D10;
    rgb_panel_cfg.data_gpio_nums[11]=LCD_PIN_NUM_D11;
    rgb_panel_cfg.data_gpio_nums[12]=LCD_PIN_NUM_D12;
    rgb_panel_cfg.data_gpio_nums[13]=LCD_PIN_NUM_D13;
    rgb_panel_cfg.data_gpio_nums[14]=LCD_PIN_NUM_D14;
    rgb_panel_cfg.data_gpio_nums[15]=LCD_PIN_NUM_D15;
#else
    rgb_panel_cfg.data_gpio_nums[0]=LCD_PIN_NUM_D11;
    rgb_panel_cfg.data_gpio_nums[1]=LCD_PIN_NUM_D12;
    rgb_panel_cfg.data_gpio_nums[2]=LCD_PIN_NUM_D13;
    rgb_panel_cfg.data_gpio_nums[3]=LCD_PIN_NUM_D14;
    rgb_panel_cfg.data_gpio_nums[4]=LCD_PIN_NUM_D15;
    rgb_panel_cfg.data_gpio_nums[5]=LCD_PIN_NUM_D05;
    rgb_panel_cfg.data_gpio_nums[6]=LCD_PIN_NUM_D06;
    rgb_panel_cfg.data_gpio_nums[7]=LCD_PIN_NUM_D07;
    rgb_panel_cfg.data_gpio_nums[8]=LCD_PIN_NUM_D08;
    rgb_panel_cfg.data_gpio_nums[9]=LCD_PIN_NUM_D09;
    rgb_panel_cfg.data_gpio_nums[10]=LCD_PIN_NUM_D10;
    rgb_panel_cfg.data_gpio_nums[11]=LCD_PIN_NUM_D00;
    rgb_panel_cfg.data_gpio_nums[12]=LCD_PIN_NUM_D01;
    rgb_panel_cfg.data_gpio_nums[13]=LCD_PIN_NUM_D02;
    rgb_panel_cfg.data_gpio_nums[14]=LCD_PIN_NUM_D03;
    rgb_panel_cfg.data_gpio_nums[15]=LCD_PIN_NUM_D04;
#endif
#else
#if LCD_COLOR_SPACE != LCD_COLOR_BGR
    rgb_panel_cfg.data_gpio_nums[8]=LCD_PIN_NUM_D00;
    rgb_panel_cfg.data_gpio_nums[9]=LCD_PIN_NUM_D01;
    rgb_panel_cfg.data_gpio_nums[10]=LCD_PIN_NUM_D02;
    rgb_panel_cfg.data_gpio_nums[11]=LCD_PIN_NUM_D03;
    rgb_panel_cfg.data_gpio_nums[12]=LCD_PIN_NUM_D04;
    rgb_panel_cfg.data_gpio_nums[13]=LCD_PIN_NUM_D05;
    rgb_panel_cfg.data_gpio_nums[14]=LCD_PIN_NUM_D06;
    rgb_panel_cfg.data_gpio_nums[15]=LCD_PIN_NUM_D07;
    rgb_panel_cfg.data_gpio_nums[0]=LCD_PIN_NUM_D08;
    rgb_panel_cfg.data_gpio_nums[1]=LCD_PIN_NUM_D09;
    rgb_panel_cfg.data_gpio_nums[2]=LCD_PIN_NUM_D10;
    rgb_panel_cfg.data_gpio_nums[3]=LCD_PIN_NUM_D11;
    rgb_panel_cfg.data_gpio_nums[4]=LCD_PIN_NUM_D12;
    rgb_panel_cfg.data_gpio_nums[5]=LCD_PIN_NUM_D13;
    rgb_panel_cfg.data_gpio_nums[6]=LCD_PIN_NUM_D14;
    rgb_panel_cfg.data_gpio_nums[7]=LCD_PIN_NUM_D15;
#else
    rgb_panel_cfg.data_gpio_nums[8]=LCD_PIN_NUM_D11;
    rgb_panel_cfg.data_gpio_nums[9]=LCD_PIN_NUM_D12;
    rgb_panel_cfg.data_gpio_nums[10]=LCD_PIN_NUM_D13;
    rgb_panel_cfg.data_gpio_nums[11]=LCD_PIN_NUM_D14;
    rgb_panel_cfg.data_gpio_nums[12]=LCD_PIN_NUM_D15;
    rgb_panel_cfg.data_gpio_nums[13]=LCD_PIN_NUM_D05;
    rgb_panel_cfg.data_gpio_nums[14]=LCD_PIN_NUM_D06;
    rgb_panel_cfg.data_gpio_nums[15]=LCD_PIN_NUM_D07;
    rgb_panel_cfg.data_gpio_nums[0]=LCD_PIN_NUM_D08;
    rgb_panel_cfg.data_gpio_nums[1]=LCD_PIN_NUM_D09;
    rgb_panel_cfg.data_gpio_nums[2]=LCD_PIN_NUM_D10;
    rgb_panel_cfg.data_gpio_nums[3]=LCD_PIN_NUM_D00;
    rgb_panel_cfg.data_gpio_nums[4]=LCD_PIN_NUM_D01;
    rgb_panel_cfg.data_gpio_nums[5]=LCD_PIN_NUM_D02;
    rgb_panel_cfg.data_gpio_nums[6]=LCD_PIN_NUM_D03;
    rgb_panel_cfg.data_gpio_nums[7]=LCD_PIN_NUM_D04;
#endif

#endif
    memset(&rgb_panel_cfg.timings,0,sizeof(esp_lcd_rgb_timing_t));
    rgb_panel_cfg.timings.pclk_hz = LCD_CLOCK_HZ;
    rgb_panel_cfg.timings.h_res = LCD_HRES;
    rgb_panel_cfg.timings.v_res = LCD_VRES;
    rgb_panel_cfg.timings.hsync_back_porch = LCD_HSYNC_BACK_PORCH;
    rgb_panel_cfg.timings.hsync_front_porch = LCD_HSYNC_FRONT_PORCH;
    rgb_panel_cfg.timings.hsync_pulse_width = LCD_HSYNC_PULSE_WIDTH;
    rgb_panel_cfg.timings.vsync_back_porch = LCD_VSYNC_BACK_PORCH;
    rgb_panel_cfg.timings.vsync_front_porch = LCD_VSYNC_FRONT_PORCH;
    rgb_panel_cfg.timings.vsync_pulse_width = LCD_VSYNC_PULSE_WIDTH;
    rgb_panel_cfg.timings.flags.pclk_active_neg = true;
    rgb_panel_cfg.timings.flags.hsync_idle_low = false;
    rgb_panel_cfg.timings.flags.pclk_idle_high = LCD_CLK_IDLE_HIGH;
    rgb_panel_cfg.timings.flags.de_idle_high = LCD_DE_IDLE_HIGH;
    rgb_panel_cfg.timings.flags.vsync_idle_low = false;
    rgb_panel_cfg.flags.bb_invalidate_cache = true;
    rgb_panel_cfg.flags.disp_active_low = false;
    rgb_panel_cfg.flags.double_fb = false;
    rgb_panel_cfg.flags.no_fb = false;
    rgb_panel_cfg.flags.refresh_on_demand = false;
    rgb_panel_cfg.flags.fb_in_psram = true; // allocate frame buffer in PSRAM
    //rgb_panel_cfg.sram_trans_align = 4;
    //rgb_panel_cfg.psram_trans_align = 64;
    rgb_panel_cfg.num_fbs = 2;
    rgb_panel_cfg.bounce_buffer_size_px = LCD_HRES*(LCD_VRES/LCD_DIVISOR);
#endif
#if LCD_BUS == PANEL_BUS_SPI
    spi_init();
    esp_lcd_panel_io_spi_config_t lcd_spi_cfg;
    memset(&lcd_spi_cfg,0,sizeof(lcd_spi_cfg));
    lcd_spi_cfg.cs_gpio_num = LCD_PIN_NUM_CS;
    lcd_spi_cfg.dc_gpio_num = LCD_PIN_NUM_DC;
    lcd_spi_cfg.lcd_cmd_bits = LCD_CMD_BITS;
    lcd_spi_cfg.lcd_param_bits = LCD_PARAM_BITS;        
#ifdef LCD_CLOCK_HZ
    lcd_spi_cfg.pclk_hz = LCD_CLOCK_HZ;
#else
    lcd_spi_cfg.pclk_hz = 20 * 1000 * 1000;
#endif
    lcd_spi_cfg.trans_queue_depth = 10;
#ifndef LCD_NO_DMA
    lcd_spi_cfg.on_color_trans_done = on_flush_complete;
#endif
#ifdef LCD_SPI_MODE
    lcd_spi_cfg.spi_mode = LCD_SPI_MODE;
#else
    lcd_spi_cfg.spi_mode = 0;
#endif
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_SPI_HOST, &lcd_spi_cfg, &lcd_io_handle));
#endif
#if LCD_BUS == PANEL_BUS_I2C
    i2c_init();
    esp_lcd_panel_io_i2c_config_t lcd_i2c_cfg;
    memset(&lcd_i2c_cfg,0,sizeof(lcd_i2c_cfg));
    lcd_i2c_cfg.control_phase_bytes = LCD_CONTROL_PHASE_BYTES;
    lcd_i2c_cfg.dc_bit_offset = LCD_DC_BIT_OFFSET;
    lcd_i2c_cfg.dev_addr = LCD_I2C_ADDR;
#ifdef LCD_DISABLE_CONTROL_PHASE
    lcd_i2c_cfg.flags.disable_control_phase = true;
#endif
    lcd_i2c_cfg.lcd_cmd_bits = LCD_CMD_BITS;
    lcd_i2c_cfg.lcd_param_bits = LCD_PARAM_BITS;
#ifndef LCD_NO_DMA
    lcd_i2c_cfg.on_color_trans_done = on_flush_complete;
#else
    lcd_i2c_cfg.on_color_trans_done = NULL;
#endif
#ifndef LEGACY_I2C
#ifdef LCD_CLOCK_HZ
    lcd_i2c_cfg.scl_speed_hz = LCD_CLOCK_HZ;
#else
    lcd_i2c_cfg.scl_speed_hz = 100 * 1000;
#endif
#else
    lcd_i2c_cfg.scl_speed_hz = 0;
#endif
#ifndef LEGACY_I2C
    i2c_master_bus_handle_t i2c_bus_handle;
    ESP_ERROR_CHECK(i2c_master_get_bus_handle((i2c_port_num_t)LCD_I2C_HOST,&i2c_bus_handle));
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v2((i2c_master_bus_handle_t)i2c_bus_handle, &lcd_i2c_cfg, &lcd_io_handle));
#else
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v1((uint32_t)LCD_I2C_HOST, &lcd_i2c_cfg, &lcd_io_handle));
#endif
#endif
#if LCD_BUS == PANEL_BUS_MIPI
    esp_lcd_dsi_bus_config_t dsi_bus_config = {                                                    \
        .bus_id = LCD_MIPI_BUS_ID,                                     
        .num_data_lanes = LCD_MIPI_LANES,                             
        .phy_clk_src = LCD_MIPI_CLK_SRC,     
        .lane_bit_rate_mbps = LCD_MIPI_LANE_MBPS,                      
    };
    ESP_ERROR_CHECK(esp_lcd_new_dsi_bus(&dsi_bus_config, &mipi_dsi_bus));
    
    esp_lcd_dbi_io_config_t dbi_config = {                                 
        .virtual_channel = LCD_MIPI_CHANNEL,         
        .lcd_cmd_bits = LCD_CMD_BITS,            
        .lcd_param_bits = LCD_PARAM_BITS,          
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_dbi(mipi_dsi_bus, &dbi_config, &lcd_io_handle));

    esp_lcd_dpi_panel_config_t dpi_config=// = ST7703_720_720_PANEL_60HZ_DPI_CONFIG(MIPI_DPI_PX_FORMAT);
    {   
        .virtual_channel = LCD_MIPI_CHANNEL,                                                                             
        .dpi_clk_src = MIPI_DSI_DPI_CLK_SRC_DEFAULT,     
        .dpi_clock_freq_mhz = LCD_CLOCK_HZ/(1000 * 1000),                        
        .pixel_format = MIPI_DPI_PX_FORMAT,         
        .num_fbs = 1,                                    
        .video_timing = {                                
            .h_size = LCD_HRES,                               
            .v_size = LCD_VRES,                               
            .hsync_pulse_width = LCD_HSYNC_PULSE_WIDTH,                     
            .hsync_back_porch = LCD_HSYNC_BACK_PORCH,                     
            .hsync_front_porch = LCD_HSYNC_FRONT_PORCH,                    
            .vsync_pulse_width = LCD_VSYNC_PULSE_WIDTH,                     
            .vsync_back_porch = LCD_VSYNC_BACK_PORCH,                      
            .vsync_front_porch = LCD_VSYNC_FRONT_PORCH,                     
        },           
#ifdef LCD_MIPI_DMA2D                                    
        .flags.use_dma2d = 1,
#endif
    };

#endif
#if LCD_BUS != PANEL_BUS_RGB
    esp_lcd_panel_dev_config_t panel_config;
    memset(&panel_config,0,sizeof(panel_config));
#ifdef LCD_VENDOR_CONFIG
    LCD_VENDOR_CONFIG;
#endif
#ifdef LCD_PIN_NUM_RST
    panel_config.reset_gpio_num = LCD_PIN_NUM_RST;
#else
    panel_config.reset_gpio_num = -1;
#endif
#if LCD_COLOR_SPACE == LCD_COLOR_RGB
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
#elif LCD_COLOR_SPACE == LCD_COLOR_BGR
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR;
#elif LCD_COLOR_SPACE == LCD_COLOR_GSC
    // seems to work
    panel_config.rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB;
#endif
#ifdef LCD_DATA_ENDIAN_LITTLE
    panel_config.data_endian = LCD_RGB_DATA_ENDIAN_LITTLE;
#else
    panel_config.data_endian = LCD_RGB_DATA_ENDIAN_BIG;
#endif
    panel_config.bits_per_pixel = LCD_BIT_DEPTH;
#ifdef LCD_VENDOR_CONFIG
    panel_config.vendor_config = &vendor_config;
#else
    panel_config.vendor_config = NULL;
#endif
    ESP_ERROR_CHECK(LCD_PANEL(lcd_io_handle, &panel_config, &lcd_handle));
#else
    ESP_ERROR_CHECK(esp_lcd_new_rgb_panel(&rgb_panel_cfg, &lcd_handle));   
    esp_lcd_rgb_panel_event_callbacks_t rgb_cbs;
    memset(&rgb_cbs,0,sizeof(rgb_cbs));
#ifndef LCD_NO_DMA
    rgb_cbs.on_color_trans_done = on_flush_complete;
#endif
    rgb_cbs.on_vsync = on_vsync;
    esp_lcd_rgb_panel_register_event_callbacks(lcd_handle,&rgb_cbs,NULL);
    
#endif
    ESP_ERROR_CHECK(esp_lcd_panel_reset(lcd_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(lcd_handle));
#if LCD_BUS == PANEL_BUS_MIPI && !defined(LCD_NO_DMA)
    esp_lcd_dpi_panel_event_callbacks_t mipi_cbs = {
        .on_color_trans_done = on_flush_complete,
    };
    ESP_ERROR_CHECK(esp_lcd_dpi_panel_register_event_callbacks(lcd_handle, &mipi_cbs, NULL));
#endif

    int gap_x = 0, gap_y = 0;
#ifdef LCD_GAP_X
    gap_x = LCD_GAP_X;
#endif
#ifdef LCD_GAP_Y
    gap_y = LCD_GAP_Y;
#endif
    esp_lcd_panel_set_gap(lcd_handle,gap_x,gap_y);
    
#ifdef LCD_SWAP_XY
#if LCD_SWAP_XY
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(lcd_handle,true));
#endif
#endif
    bool mirror_x = false, mirror_y = false;
#ifdef LCD_MIRROR_X
#if LCD_MIRROR_X
    mirror_x = LCD_MIRROR_X;
#endif
#endif
#ifdef LCD_MIRROR_Y
#if LCD_MIRROR_Y
    mirror_y = LCD_MIRROR_Y;
#endif
#endif
    esp_lcd_panel_mirror(lcd_handle,mirror_x,mirror_y);
    
#ifdef LCD_INVERT_COLOR
#if LCD_INVERT_COLOR
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(lcd_handle,true));
#endif
#endif
#if LCD_BUS != PANEL_BUS_RGB
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(lcd_handle, true));
#endif
#ifdef LCD_PIN_NUM_BCKL
#if LCD_PIN_NUM_BCKL >= 0
    gpio_set_level((gpio_num_t)LCD_PIN_NUM_BCKL, LCD_BCKL_ON_LEVEL);
#endif
#endif
#if LCD_TRANSFER_SIZE > 0
    uint32_t heap_caps = MALLOC_CAP_8BIT;
#ifdef LCD_TRANSFER_IN_SPIRAM
    heap_caps |= MALLOC_CAP_SPIRAM;
#else
    heap_caps |= MALLOC_CAP_INTERNAL;
#endif
    // it's recommended to allocate the draw buffer from internal memory, for better performance
    draw_buffer = heap_caps_malloc(LCD_TRANSFER_SIZE, heap_caps);
    if(draw_buffer==NULL) {
        ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
    }
#ifndef LCD_NO_DMA
    draw_buffer2 = heap_caps_malloc(LCD_TRANSFER_SIZE, heap_caps);
    if(draw_buffer2==NULL) {
        ESP_ERROR_CHECK(ESP_ERR_NO_MEM);
    }
#endif
#endif
}
#if LCD_TRANSFER_SIZE > 0
void* lcd_transfer_buffer(void) { return draw_buffer; }
#ifndef LCD_NO_DMA
void* lcd_transfer_buffer2(void) { return draw_buffer2; }
#endif
#endif
#endif
#ifdef TOUCH_BUS
static esp_lcd_panel_io_handle_t touch_io_handle = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;
#ifdef LCD_TOUCH_PIN_NUM_INT
static void on_touch(esp_lcd_touch_handle_t tp) {
    // TODO: do something here
}
#endif
void touch_init(void) {
    if(touch_handle!=NULL) {
        ESP_LOGW(TAG,"lcd_touch_init() was already called");
        return; // already initialized
    }
#if TOUCH_BUS == PANEL_BUS_SPI
    spi_init();
#endif    
#if TOUCH_BUS == PANEL_BUS_I2C
    i2c_init();
#endif
#ifdef TOUCH_RESET
    TOUCH_RESET;
#endif
    esp_lcd_touch_config_t touch_cfg;
    memset(&touch_cfg,0,sizeof(touch_cfg));
    #ifdef TOUCH_DRIVER_DATA
    TOUCH_DRIVER_DATA;
    touch_cfg.driver_data = &driver_data;
#endif
#ifdef TOUCH_PIN_NUM_INT
    touch_cfg.int_gpio_num = (gpio_num_t)TOUCH_PIN_NUM_INT;
    touch_cfg.interrupt_callback = on_touch;
#else
    touch_cfg.int_gpio_num = (gpio_num_t)-1;
#endif
#ifdef TOUCH_INT_ON_LEVEL
    touch_cfg.levels.reset = TOUCH_INT_ON_LEVEL;
#else 
    touch_cfg.levels.reset = 1;
#endif
#ifdef TOUCH_PIN_NUM_RST
    touch_cfg.rst_gpio_num = (gpio_num_t)TOUCH_PIN_NUM_RST;
#else
    touch_cfg.rst_gpio_num = (gpio_num_t)-1;
#endif
#ifdef TOUCH_RST_ON_LEVEL
    touch_cfg.levels.reset = TOUCH_RST_ON_LEVEL;
#else
    touch_cfg.levels.reset = 1;
#endif

    touch_cfg.x_max = TOUCH_HRES;
    touch_cfg.y_max = TOUCH_VRES;
#ifdef TOUCH_SWAP_XY
#if TOUCH_SWAP_XY
    touch_cfg.flags.swap_xy = TOUCH_SWAP_XY;
#endif
#endif
#ifdef TOUCH_MIRROR_X
#if TOUCH_MIRROR_X
    touch_cfg.flags.mirror_x = TOUCH_MIRROR_X;
#endif
#endif
#ifdef TOUCH_MIRROR_Y
#if TOUCH_MIRROR_Y
    touch_cfg.flags.mirror_y = TOUCH_MIRROR_X;
#endif
#endif
#if TOUCH_BUS == PANEL_BUS_I2C
    esp_lcd_panel_io_i2c_config_t touch_i2c_cfg;
    touch_i2c_cfg.control_phase_bytes = TOUCH_CONTROL_PHASE_BYTES;
    touch_i2c_cfg.dc_bit_offset = TOUCH_DC_BIT_OFFSET;
    touch_i2c_cfg.dev_addr = TOUCH_I2C_ADDR;
#ifdef TOUCH_DISABLE_CONTROL_PHASE
    touch_i2c_cfg.flags.disable_control_phase = true;
#endif
    touch_i2c_cfg.lcd_cmd_bits = TOUCH_CMD_BITS;
    touch_i2c_cfg.lcd_param_bits = TOUCH_PARAM_BITS;
#ifndef LEGACY_I2C
    touch_i2c_cfg.scl_speed_hz = TOUCH_CLOCK_HZ;
#else
    touch_i2c_cfg.scl_speed_hz = 0;
#endif
#ifndef LEGACY_I2C
    i2c_master_bus_handle_t i2c_bus_handle;
    ESP_ERROR_CHECK(i2c_master_get_bus_handle((i2c_port_num_t)TOUCH_I2C_HOST,&i2c_bus_handle));
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v2((i2c_master_bus_handle_t)i2c_bus_handle, &touch_i2c_cfg, &touch_io_handle));
#else
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v1((uint32_t)TOUCH_I2C_HOST, &touch_i2c_cfg, &touch_io_handle));
#endif
#endif
    ESP_ERROR_CHECK(TOUCH_PANEL(touch_io_handle,&touch_cfg,&touch_handle));
}
void touch_read_raw(size_t* in_out_count,uint16_t* out_x,uint16_t* out_y,uint16_t* out_strength) {
    if(ESP_OK!=esp_lcd_touch_read_data(touch_handle)) {
        ESP_LOGE(TAG,"touch read error");
        *in_out_count = 0;
        return;
    }
    uint8_t count=*in_out_count;
    if(!esp_lcd_touch_get_coordinates(touch_handle,out_x,out_y,out_strength,&count,count)) {
        *in_out_count = 0;
        return;
    }
    *in_out_count = count;
}
void touch_read(size_t* in_out_count,uint16_t* out_x,uint16_t* out_y,uint16_t* out_strength) {
    touch_read_raw(in_out_count,out_x,out_y,out_strength);
#if defined(LCD_BUS) && (TOUCH_WIDTH!=LCD_WIDTH||TOUCH_HEIGHT!=LCD_HEIGHT || TOUCH_LEFT_OVERHANG!=0 ||TOUCH_RIGHT_OVERHANG!=0||TOUCH_TOP_OVERHANG!=0||TOUCH_BOTTOM_OVERHANG!=0)
    for(size_t i = 0;i<*in_out_count;++i) {
        // the panel may have a different res than the screen
        out_x[i]=(out_x[i]*(TOUCH_WIDTH-TOUCH_LEFT_OVERHANG-TOUCH_RIGHT_OVERHANG))/(LCD_WIDTH);
        out_y[i]=(out_y[i]*(TOUCH_HEIGHT-TOUCH_TOP_OVERHANG-TOUCH_BOTTOM_OVERHANG))/LCD_HEIGHT;
    }
#endif
}
void touch_update(void) {
    esp_lcd_touch_read_data(touch_handle);
}
#endif

#ifdef BUTTON
void button_init(void) {
    gpio_config_t cfg;
    memset(&cfg,0,sizeof(cfg));
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pin_bit_mask = BUTTON_MASK;
#if BUTTON_ON_LEVEL == 0
    cfg.pull_down_en = 0;
    cfg.pull_up_en = 1;
#else
    cfg.pull_down_en = 1;
    cfg.pull_up_en = 0;
#endif
    ESP_ERROR_CHECK(gpio_config(&cfg));
}
bool button_read(uint8_t pin) {
#if BUTTON_ON_LEVEL == 1
    return gpio_get_level((gpio_num_t)pin);
#else
    return !gpio_get_level((gpio_num_t)pin);
#endif
}
uint64_t button_read_all(void) {
    uint64_t result = 0;
    for(int i = 0;i<GPIO_NUM_MAX;++i) {
        uint64_t mask = ((uint64_t)1)<<i;
        if(mask & BUTTON_MASK) {
            if(button_read(i)) {
                result |= mask;
            }
        }
    }
    return result;
}
#endif
#ifdef POWER
void power_init(void) {
#if defined(POWER_BUS) && (POWER_BUS == PANEL_BUS_SPI)
    spi_init();
#endif
#if defined(POWER_BUS) && (POWER_BUS == PANEL_BUS_I2C)
    i2c_init();
#endif
#ifdef POWER_INIT
    POWER_INIT;
#endif
}
#endif