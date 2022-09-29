STM32

SPI&DMA_2.25Mbits

使用 HAL库 和 cubemx 配置SPI和DMA


    PB3     ------> SPI1_SCK    ---->  NC
    PB5     ------> SPI1_MOSI   ---->  WS2812B
    
    SPI1，DMA1_channel3，

WS2812B
1. >=537us低电平 复位
2. 单线归零码

