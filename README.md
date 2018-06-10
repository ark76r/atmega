# atmega

C examples for atmega328p microcontroller.

Repo contains several libraries and simple examples of their usage.

Library contains:

* uart - UART library - may use interrupt and cyclic buffer for data reading
* timer0 - used to setup CPU clock frequency and timer, to perform delays with sleep
* dallas1w - simple implementation of dallas 1w protocol. Now works with only one device on the bus
* i2c - simple implementation of TWI master
* ds18b20 - reading the temperature from the sensor
* mpl3115a2 - reading the temperature fomr the sensor
* lcd - handles communication with HD44780U LCD display
* console - adds support for stdout (printf sends data to uart)
