# htcw_esp_panel

Provides out of the box support for several devkits, including display, input, and power management

Supports up to 16 different devices (some untested) out of the box

You can also easily add your own

This hides the complications of using the ESP LCD Panel API, and ESP LCD Touch API, plus any power management necessities required for the board to work properly.

It provides support for GPIO buttons, touch panels, power management (minimal) and displays

The display API is designed with LVGL or htcw_uix partial updates in mind

As far as display interconnects it supports I2C, SPI, I8080, RGB and MIPI

For touch it supports I2C or SPI

Buttons are presently GPIO

If you need more display or touch controller support, you can do what i did, download the ESP LCD Panel API component or related touch component and create a platformIO library out of it. You can then use it in your project. Of course, if you're just using idf.py you can use the component directly.

Documentation is presently somewhat lacking, but it is fairly easy to copy from an existing device and use its configuration as a template.

PlatformIO lib is `codewitch-honey-crisis/htcw_esp_panel`

Use the example code therein to get started

## Configuration

### LCD SPI, I2C RGB, MIPI and I8080 Display bus detection

Depending on the values you set, the `LCD_BUS` will be set to `PANEL_BUS_I2C`, `PANEL_BUS_SPI`, `PANEL_BUS_I8080` `PANEL_BUS_RGB`, or `PANEL_BUS_MIPI`

For example, if you define `LCD_PIN_NUM_D00` you'll either end up with I8080 or RGB, depending on whether `LCD_HSYNC_FRONT_PORCH` is defined.

If you define `LCD_HSYNC_FRONT_PORCH` but not `LCD_PIN_NUM_D00` you'll end up with MIPI.

If you define `LCD_SPI_HOST` the bus ends up as SPI, otherwise I2C for `LCD_I2C_HOST`

These internal determinations may change in the future if more bus types are added, the above is just to clarify some of the automagic happening. 

### LCD_SPI_HOST
The SPI host for the display connected via SPI

Example setting: `SPI3_HOST`

Requires `LCD_PIN_NUM_MOSI`,`LCD_PIN_NUM_CLK`,`LCD_PIN_NUM_DC`

Can also have `LCD_PIN_NUM_CS`,`LCD_PIN_NUM_RST`, `LCD_PIN_NUM_BCKL`, `LCD_BCKL_ON_LEVEL`, often `LCD_CLOCK_HZ` and sometimes `LCD_PIN_NUM_MISO`

Note that if you're sharing the SPI bus with something that needs input like a touch device or SD reader, you should provide a MISO pin definition here.

### LCD_I2C_HOST
The I2C host for the display connected via I2C

Example setting: `I2C_NUM_0`

Requires `LCD_PIN_NUM_SDA`, and `LCD_PIN_NUM_SCL`, `LCD_I2C_ADDR`

Can also have `LCD_I2C_PICKUP`, `LCD_CLOCK_HZ`, `LCD_DC_BIT_OFFSET`, `LCD_CMD_BITS`, `LCD_PARAM_BITS`, and `LCD_DISABLE_CONTROL_PHASE`

Notes: Some devices do not like the newer ESP32 I2C facilities. If you run into trouble, try defining `LEGACY_I2C` - note that this impacts all the code in the system.

If you have multiple devices connected to the same I2C bus, and `LEGACY_I2C` is defined (or you want your config to be able to work with it), use the lowest frequency setting supported by any device on that bus, because the old I2C does not have per device speed settings.

### LCD_PANEL
The LCD panel call to instantiate the panel 

Example setting: `esp_lcd_new_panel_st7789`

Note: The ST7789 is built in, but if you need to use an external LCD driver component, you can add it to the project `./components` folder or to the `./platformio.ini` as a `lib_deps` entry, and add the `#include` for the component here. 

### LCD_HRES, LCD_VRES

The native horizontal and vertical resolutions. This is pre-swapped or adjusted - just what the controller natively registers as.

Example setting: `320` 

### LCD_COLOR_SPACE 

The color space the LCD uses.

Example setting: `LCD_COLOR_RGB` 

Can be `LCD_COLOR_RGB` for RGB, `LCD_COLOR_BGR` for BGR or `LCD_COLOR_GSC` for grayscale or monochrome

### LCD_GAP_X, LCD_GAP_Y

The offset in pixels from the top or left of the screen.

Example setting: `40`

### LCD_MIRROR_X, LCD_MIRROR_Y

Can be `false` or `true`. Defaults to `false`. Indicates whether to mirror the X or Y axes.

### LCD_SWAP_XY 

Can be `false` or `true`. Defaults to `false`. Indicates whether or not the X and Y axes are flipped.

### LCD_INVERT_COLOR 

Can be `false` or `true`. Defaults to `false`. Indicates whether the colors are inverted (common for IPS panels)

### LCD_BIT_DEPTH

Indicates the color depth of the frame buffer

### LCD_TRANSFER_SIZE

Can be any number of bytes. `0` disables provisioning of transfer buffers. The default is computed based on a fractional part of the display resolution, as indicated by `LCD_DIVISOR`

### LCD_TRANSFER_IN_SPIRAM

Indicates that the LCD transfer buffer(s) should be created in attached PSRAM instead of intrinsic SRAM. This is not supported on regular ESP32s, but it is on S3s and P4s, and is often desirable with larger resolution displays.

### LCD_DIVISOR

Defaults to `10`. Indicates the fractional part of the display to use for computing `LCD_TRANSFER_SIZE`. Typically 10 is fine, but for something like an SSD1306 it's so small, that it doesn't make sense to subdivide it further, so `1` is a good value for it there. Similarly for the TTGO its display is smaller but not quite as small as the SSD1306, so making it `2` here strikes a good balance between memory consumption and performance. Generally lower is faster but requires more memory. However "faster" here quickly runs into diminishing returns after some magic point to be determined by you, dear user.

### LCD_DATA_ENDIAN_LITTLE

This indicates that the display operates on little endian data, instead of the more common and default big endian data ordering

### LCD_VENDOR_CONFIG

Some panels have a custom structure you must fill in with extra details about the display. You can define this macro to use it. See the existing panels.h for examples

#### LCD_TRANSLATE

Some panels have a funny memory buffer, and the flushing doesn't consume regular bitmaps. Defining code in this macro will allow insert it into the flushing pipeline to do pre-flush translation. See the existing definitions in panels.h for examples.

### TOUCH_SPI_HOST
The SPI host for the touch panel connected via SPI

Example setting: `SPI_3`

Requires `TOUCH_PIN_NUM_MISO`,`TOUCH_PIN_NUM_CLK`

Can also have `LCD_PIN_NUM_CS`,`LCD_PIN_NUM_RST`, `LCD_PIN_NUM_INT`, `LCD_INT_ON_LEVEL`, `TOUCH_RST_ON_LEVEL`, often `TOUCH_CLOCK_HZ` and sometimes `TOUCH_PIN_NUM_MOSI`

Note that if you're sharing the SPI bus with something that needs output like an SD reader, you should provide a MOSI pin definition here.

### TOUCH_I2C_HOST
The I2C host for the touch panel connected via I2C

Example setting: `I2C_1`

Requires `TOUCH_PIN_NUM_SDA`, and `TOUCH_PIN_NUM_SCL`, `TOUCH_I2C_ADDR`

Can also have `TOUCH_I2C_PICKUP`, `TOUCH_CLOCK_HZ`, `TOUCH_DC_BIT_OFFSET`, `TOUCH_CMD_BITS`, `TOUCH_PARAM_BITS`, and `TOUCH_DISABLE_CONTROL_PHASE`

Note: Some devices do not like the newer ESP32 I2C facilities. If you run into trouble, try defining `LEGACY_I2C` - note that this impacts all the code in the system.

### TOUCH_PANEL
The touch panel call to instantiate the panel 

Example setting: `esp_lcd_touch_new_i2c_ft6x36`

Note: You can add any necessary touch component to the project `./components` folder or to the `./platformio.ini` as a `lib_deps` entry, and add the `#include` for the component here. 

### TOUCH_HRES, TOUCH_VRES

The native horizontal and vertical resolutions. This is pre-swapped or adjusted - just what the controller natively registers as. If not specified these will be the same as the LCD, if defined.

Example setting: `320` 

### TOUCH_LEFT_OVERHANG,TOUCH_RIGHT_OVERHANG,TOUCH_TOP_OVERHANG,TOUCH_BOTTOM_OVERHANG

These arguments are for indicating the amount that the touch panel hangs over or past the display boundary.

For example, the M5 Stack Core2's touch panel is 40 pixels taller than the display, extending off the bottom so it has a `TOUCH_BOTTOM_OVERHANG` of `40`

These default to `0`

Example setting: `40`

