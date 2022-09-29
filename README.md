# WS2812B
WS2812B 驱动

## STM32

使用SPI1+DMA1_channel3 2.25Mbits/s

```cpp
// PB3 ------> SPI1_SCK  ----> NC
// PB5 ------> SPI1_MOSI ----> WS2812B
```
