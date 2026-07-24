# STM32F407

基于 STM32F407VET6 的嵌入式外设驱动学习例程合集，所有工程由 STM32CubeMX 生成，使用 HAL 库 + Keil MDK-ARM。

## 硬件

| 项 | 参数 |
|---|------|
| **MCU** | STM32F407VET6 (ARM Cortex-M4, 168MHz) |
| **开发环境** | STM32CubeMX + Keil MDK-ARM |
| **HAL 库** | STM32F4xx_HAL_Driver |
| **调试器** | ST-Link / J-Link |

## 例程列表

### 基础外设 (00-01)

| 例程 | 说明 | 关键技术 |
|------|------|----------|
| **USART** | 串口轮询收发 | UART1 初始化，printf 重定向 |
| **UART_Interupt_function_version** | 串口中断方式收发 | UART RXNE 中断，中断处理函数 |
| **USART_Interupt__version_callback** | 串口中断回调方式 | HAL_UART_RxCpltCallback |
| **USART_NVIC_USART_LED_VIZ** | 串口中断控制 LED | NVIC 优先级配置 |

### 输入输出 (02-03)

| 例程 | 说明 | 关键技术 |
|------|------|----------|
| **KEY_NVIC** | 按键 EXTI 外部中断 | GPIO 中断，NVIC 嵌套向量中断 |
| **ADC_single** | 单通道 ADC 采样 | ADC1 轮询采样，模拟电压读取 |
| **ADC_MULTI_POWEER_TEMPRETUE** | 多通道 ADC + UART 输出 | ADC 扫描模式，电压/温度采集 |

### 显示 (04-06)

| 例程 | 说明 | 关键技术 |
|------|------|----------|
| **OLED_IIC** | 0.96" OLED (I2C) 驱动 | I2C 通信，OLED 字库，画点/画线 |
| **OLED_SPI** | 0.96" OLED (SPI) 驱动 | SPI 通信，OLED 字库 |
| **SPI_TFT** | TFT LCD 彩屏驱动 | SPI 高速传输，GUI 绘图，Logo 显示 |

### 存储与通信 (07-09)

| 例程 | 说明 | 关键技术 |
|------|------|----------|
| **SPI_W25Q16** | W25Q16 Flash 存储 | SPI Flash 读写，扇区擦除 |
| **BEI_DOU_GPS_GU_T13** | 北斗+GPS 双模定位 | UART2 中断接收，NMEA 协议解析 |
| **hall_speed** | 霍尔传感器测速 | TIM 编码器模式，转速计算 |

### 4G NB-IoT 通信

| 例程 | 说明 | 关键技术 |
|------|------|----------|
| **4G/YNH-MN316** | 中移物联 MN316 4G 模块 | AT 指令，MQTT，OneNET 云，OpenCPU |

配套资料：硬件原理图、用户手册、云平台对接文档、OpenCPU 源码例程。

### LCD 屏幕

| 目录 | 说明 |
|------|------|
| **1.47_LCD** | 1.47" IPS TFT 规格书、源码、取模教程 |

## 构建

### Keil MDK-ARM

每个例程自带 MDK-ARM 工程：

```bash
# 打开对应 .uvprojx 文件
# 编译 → F7
# 下载 → F8 (需连接 ST-Link/J-Link)
```

### printf 重定向

所有例程使用统一的 printf 重定向到 USART1（PA9/PA10, 115200）：

```c
int fputc(int ch, FILE *p) {
    while(!(USART1->SR & (1<<7)));
    USART1->DR = ch;
    return ch;
}
```

## 常用外设引脚

| 外设 | 引脚 | 说明 |
|------|------|------|
| USART1 | PA9(TX), PA10(RX) | printf 输出 / 串口调试 |
| USART2 | PA2(TX), PA3(RX) | GPS / 辅助串口 |
| I2C1 | PB6(SCL), PB7(SDA) | OLED I2C |
| SPI1 | PA5(SCK), PA7(MOSI), PA6(MISO) | OLED/TFT/W25Q16 |
| ADC1 | PA0-PA7 | 多通道 ADC 输入 |
| KEY | PE0-PE3 | 按键 EXTI 中断 |

## 学习路径建议

```
新手推荐顺序：
1. USART     → 程序跑起来，printf 打印
2. KEY_NVIC  → 按键中断，理解 NVIC
3. ADC_single → 模拟量采集
4. OLED_IIC  → I2C 通信 + 显示
5. hall_speed → TIM 编码器模式
6. BEI_DOU_GPS → NMEA 协议解析
7. SPI_W25Q16 → SPI + Flash 存储
```

## 关联项目

- [ROS_HARDWARE](https://github.com/anglersking/ROS_HARDWARE) — STM32F407 小车驱动板（FreeRTOS + PID + 串口通信）
- [STM32F407 小车驱动板](https://github.com/anglersking/STM32F407) — 同 MCU 的机器人底盘应用
