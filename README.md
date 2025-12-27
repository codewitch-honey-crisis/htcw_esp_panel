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