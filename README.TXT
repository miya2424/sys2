器材:
	精英STM32F103开发板ZET6
	
目的:
	MMA7361三轴振动传感器+GYMCU680(环境温湿度+气压+空气质量)+ATK-BD-GPS+PT100油温+WSC电流传感器+ESP8266本地服务器+rs485+modbus
	
硬件资源:
	1,ESP8266使用串口4通信，接线方式：
	vcc-5v；gnd-gnd；TXD-PC11;RXD-PC10;RST-PA12;I0_0-PA15.
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)用于打印数据至串口助手。
	3,PT100串口通信方式为SPI通信，接线方式：
	VIN-5V;GND-GND;SDO-PA6;CS-PA11;SCLK-PA8;SDI-PA7;DRDY-PA5.
	4,GYMCU680使用串口2通信，接线方式：
	VCC-5V;GND-GND;RX-PA2;TX-PA3.
	5,ATK-BD-GPS使用串口3通信，接线方式：
	PPS-KEY;RXD-RXD;TXD-TXD;GND-GND;VCC-5V.
	6.MMA7361使用ADC通信，接线方式：x-PC1;3V3-3.3V;GND-GND;SLEEPPIN-PC5;SELFTEST-PC4;GSELECT-PC0.(STM32内部ADC1,通道16,即:ADC1_CH16).
	7.RS-485接线方式：RS485_A-T/R+;RS485_B-T/R-;GND-GND.
	
现象:
	本项目通过串口5进行485通信，ADC1读取三轴振动信号，串口2读取环境温湿度等，串口3连接BD监测经纬度，SPI检测PT100通道的电机绕组温度，通过串口4连接ESP8266将传感器数据上传至本地EMQ服务器。可实现传感数据实时上传至EMQ服务器，通过Node-red保存至Influxdb时序数据库，Grafana数据看板展示功能。同时通过本地PLC系统实现现场控制。
	通信协议为MQTT协议，本地使用MODBUS+RS485通信。





































