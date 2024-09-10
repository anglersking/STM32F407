欢迎使用野牛物联网产品
本目录为代码例程，有如下注意事项
1、每个工程均支持UDP/TCP/MQTT/onenet/阿里云例程,通过修改main.c中的protocolType变量来进入不同的例程
2、UDP/TCP/MQTT的例程访问野牛自建的服务器
3、硬件环境
	STM32F1平台：STM32F103ZET6芯片,正点原子ELITE精英版开发板   
4、串口引脚
	STM32F1平台：核心板通信串口的TX接MCU端的PA3引脚
				 核心板通信串口的RX接MCU端的PA2引脚
				 DEBUG 串口打印的引脚为 PA9 PA10