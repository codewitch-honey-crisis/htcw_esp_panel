#ifndef PANEL_H
#define PANEL_H
#include "panels.h"
#if __has_include("custom_panel.h")
#include "custom_panel.h"
#endif
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

// BEGIN fill ins for defaults (don't mess!)

#define BUTTON_PIN(x) ((uint64_t)((((uint64_t)1)<<((uint64_t)x))))
#ifdef BUTTON_MASK
#ifndef BUTTON
#define BUTTON
#endif
#ifndef BUTTON_ON_LEVEL
#define BUTTON_ON_LEVEL 1
#endif
#ifndef BUTTON_OFF_LEVEL
#define BUTTON_OFF_LEVEL (!BUTTON_ON_LEVEL)
#endif
#endif

#ifndef LCD_WIDTH
#ifdef LCD_SWAP_XY
#if LCD_SWAP_XY
#define LCD_WIDTH LCD_VRES
#define LCD_HEIGHT LCD_HRES
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif
#else
#define LCD_WIDTH LCD_HRES
#define LCD_HEIGHT LCD_VRES
#endif
#endif
#ifndef LCD_BIT_DEPTH
#define LCD_BIT_DEPTH 16
#endif
#ifndef LCD_X_ALIGN
#define LCD_X_ALIGN 1
#endif
#ifndef LCD_Y_ALIGN
#define LCD_Y_ALIGN 1
#endif
#ifndef LCD_DC_BIT_OFFSET
#define LCD_DC_BIT_OFFSET 0
#endif
#ifndef LCD_CONTROL_PHASE_BYTES
#define LCD_CONTROL_PHASE_BYTES 1
#endif
#ifndef LCD_CMD_BITS
#define LCD_CMD_BITS 8
#endif
#ifndef LCD_PARAM_BITS
#define LCD_PARAM_BITS 8
#endif
#ifndef LCD_DIVISOR
#define LCD_DIVISOR 10
#endif
#ifdef LCD_PIN_NUM_BCKL
#if LCD_PIN_NUM_BCKL > -1
#ifndef LCD_BCKL_ON_LEVEL
#define LCD_BCKL_ON_LEVEL 1
#endif
#ifndef LCD_BCKL_OFF_LEVEL
#define LCD_BCKL_OFF_LEVEL !LCD_BCKL_ON_LEVEL
#endif
#endif
#endif
#ifndef LCD_TRANSFER_SIZE
#define LCD_TRANSFER_SIZE ((((LCD_WIDTH*(LCD_HEIGHT/LCD_DIVISOR)*LCD_BIT_DEPTH))+7)/8)
#endif

#ifndef LCD_BUS
#if defined(LCD_HSYNC_FRONT_PORCH) && defined(LCD_PIN_NUM_D00)
#define LCD_BUS PANEL_BUS_RGB
#elif defined(LCD_HSYNC_FRONT_PORCH) 
#define LCD_BUS PANEL_BUS_MIPI
#elif defined(LCD_PIN_NUM_D00)
#define LCD_BUS PANEL_BUS_I8080
#elif defined(LCD_SPI_HOST)
#define LCD_BUS PANEL_BUS_SPI
#elif defined(LCD_I2C_HOST)
#define LCD_BUS PANEL_BUS_I2C
#endif
#endif

#ifndef TOUCH_BUS
#if defined(TOUCH_SPI_HOST)
#define TOUCH_BUS PANEL_BUS_SPI
#elif defined(TOUCH_I2C_HOST) 
#define TOUCH_BUS PANEL_BUS_I2C
#endif
#endif
#ifdef TOUCH_BUS
#ifndef TOUCH_DC_BIT_OFFSET
#define TOUCH_DC_BIT_OFFSET 0
#endif
#ifndef TOUCH_CONTROL_PHASE_BYTES
#define TOUCH_CONTROL_PHASE_BYTES 1
#endif
#ifndef TOUCH_CMD_BITS
#define TOUCH_CMD_BITS 8
#endif
#ifndef TOUCH_PARAM_BITS
#define TOUCH_PARAM_BITS 8
#endif
#ifndef TOUCH_HRES
#define TOUCH_HRES LCD_HRES
#endif
#ifndef TOUCH_VRES
#define TOUCH_VRES LCD_VRES
#endif
#ifndef TOUCH_MIRROR_X
#ifdef LCD_MIRROR_X
#define TOUCH_MIRROR_X LCD_MIRROR_X
#endif
#endif
#ifndef TOUCH_MIRROR_Y
#ifdef LCD_MIRROR_Y
#define TOUCH_MIRROR_Y LCD_MIRROR_Y
#endif
#endif
#ifndef TOUCH_SWAP_XY
#ifdef LCD_SWAP_XY
#define TOUCH_SWAP_XY LCD_SWAP_XY
#endif
#endif
#ifndef TOUCH_TOP_OVERHANG
#define TOUCH_TOP_OVERHANG 0
#endif
#ifndef TOUCH_BOTTOM_OVERHANG
#define TOUCH_BOTTOM_OVERHANG 0
#endif

#ifndef TOUCH_LEFT_OVERHANG
#define TOUCH_LEFT_OVERHANG 0
#endif
#ifndef TOUCH_RIGHT_OVERHANG
#define TOUCH_RIGHT_OVERHANG 0
#endif
#ifndef TOUCH_WIDTH
#ifdef TOUCH_SWAP_XY
#if TOUCH_SWAP_XY
#define TOUCH_WIDTH TOUCH_VRES
#define TOUCH_HEIGHT TOUCH_HRES
#else
#define TOUCH_WIDTH TOUCH_HRES
#define TOUCH_HEIGHT TOUCH_VRES
#endif
#else
#define TOUCH_WIDTH TOUCH_HRES
#define TOUCH_HEIGHT TOUCH_VRES
#endif
#endif
#endif

#ifndef POWER_BUS
#if defined(POWER_SPI_HOST)
#define POWER_BUS PANEL_BUS_SPI
#elif defined(POWER_I2C_HOST) 
#define POWER_BUS PANEL_BUS_I2C
#endif
#endif
#if defined(POWER_BUS) || defined(POWER_INIT)
#define POWER
#endif
#if defined(POWER_BUS) && (POWER_BUS == PANEL_BUS_SPI)
#ifndef POWER_TRANSFER_SIZE
#define POWER_TRANSFER_SIZE 1024
#endif
#endif
#define LCD_COLOR_RGB 1
#define LCD_COLOR_BGR 2
#define LCD_COLOR_GSC 3
#define PANEL_BUS_SPI 1
#define PANEL_BUS_I2C 2
#define PANEL_BUS_I8080 3
#define PANEL_BUS_RGB 4
#define PANEL_BUS_MIPI 5

#if LCD_BUS == PANEL_BUS_MIPI
#if LCD_BIT_DEPTH == 24
#define MIPI_DPI_PX_FORMAT (LCD_COLOR_PIXEL_FORMAT_RGB888)
#elif LCD_BIT_DEPTH == 18
// not supported
#define MIPI_DPI_PX_FORMAT (LCD_COLOR_PIXEL_FORMAT_RGB666)
#elif LCD_BIT_DEPTH == 16
#define MIPI_DPI_PX_FORMAT (LCD_COLOR_PIXEL_FORMAT_RGB565)
#endif
#ifndef LCD_MIPI_BUS_ID
#define LCD_MIPI_BUS_ID 0
#endif
#ifndef LCD_MIPI_LANES
#define LCD_MIPI_LANES 2
#endif
#ifndef LCD_MIPI_LANE_MBPS
#define LCD_MIPI_LANE_MBPS 1000
#endif
#ifndef LCD_MIPI_CLK_SRC
#define LCD_MIPI_CLK_SRC MIPI_DSI_PHY_CLK_SRC_DEFAULT
#endif
#ifndef LCD_MIPI_CHANNEL
#define LCD_MIPI_CHANNEL 0
#endif
#endif

// END fill ins for defauls (don't mess!)
#ifdef LCD_BUS
/// @brief Initializes the display
void lcd_init(void);
/// @brief Sends a bitmap to the display
/// @param x1 The x1 coord
/// @param y1 The y1 coord
/// @param x2 The x2 coord
/// @param y2 The y2 coord
/// @param bitmap The bitmap data
void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, void* bitmap);
#if LCD_TRANSFER_SIZE > 0
/// @brief Returns the transfer buffer 
/// @return A pointer to the transfer buffer, of size LCD_TRANSFER_SIZE
void* lcd_transfer_buffer(void);
#ifndef LCD_NO_DMA
/// @brief Returns the secondary transfer buffer 
/// @return A pointer to the secondary transfer buffer, of size LCD_TRANSFER_SIZE
void* lcd_transfer_buffer2(void);
#endif
#endif
#ifndef LCD_NO_DMA
/// @brief Called when a transfer is complete. To be implemented by the lcd_init() caller
void lcd_flush_complete(void);
#endif
/// @brief Indicates how many flushes have occurred since the vblanking period was active
/// @return The number of draws that have occurred, or 0 if not supported
size_t lcd_vsync_flush_count(void);
#endif
#ifdef TOUCH_BUS
/// @brief Initializes the touch panel
void touch_init(void);
/// @brief Updated the touch panel data
void touch_update(void);
/// @brief Reads the touch scaled to the LCD (if present, otherwise, same as touch_read_raw)
/// @param in_out_count The touch count
/// @param out_x The x array
/// @param out_y The y array
/// @param out_strength The strength array
void touch_read(size_t* in_out_count,uint16_t* out_x,uint16_t* out_y, uint16_t* out_strength);
/// @brief Reads the touch unscaled
/// @param in_out_count The touch count
/// @param out_x The x array
/// @param out_y The y array
/// @param out_strength The strength array
void touch_read_raw(size_t* in_out_count,uint16_t* out_x,uint16_t* out_y, uint16_t* out_strength);
#endif
#ifdef BUTTON
/// @brief Initializes the buttons
void button_init(void);
/// @brief Indicates which buttons are pressed
/// @param pin The pin to read
/// @return True if pressed, otherwise false
bool button_read(uint8_t pin);
/// @brief Reads all the buttons and returns a mask
/// @return A mask indicating the pins of the pressed buttons
uint64_t button_read_all(void);
#endif
#ifdef POWER
/// @brief Initializes the power subsystem
void power_init(void);
#endif
#ifdef __cplusplus
}
#endif
#endif // LCD_H