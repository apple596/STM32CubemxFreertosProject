# STM32CubemxFreertosProject
create a project and use a led blink,I use add more ability later

## 1.material image

![image-20230608141221726](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images01.png)

![image-20230608141225690](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images02.png)

## 2.printf函数的重定向到串口输出

**printf与fputc** 都是定义在<stdio.h>中的
核心思想：fputc 函数写入字符 ch 到给定输出流 stream，所以printf函数在调用fputs时，会向stream参数传入`stdout`从而打印数据到标准输出，对于这两个函数的更加具体操作可度娘。

```
int printf(const char *format, ...);
int fputc(int ch, FILE *stream);
```

### 1.简单粗暴的方法HAL

Project--Options for Target--Target--Use MicroLIB(勾选)

```
int fputc (int ch, FILE *f)
{
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
    return ch;
}
```

### 2.提升效率使用寄存器发送

Project--Options for Target--Target--Use MicroLIB(勾选)
找到Docs文件夹下--Reference manual
![image-20230609170719461](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images03.png)

![image-20230609171017188](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images04.png)

![image-20230609171242688](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images05.png)

```
#if 1
#include <stdio.h>

int fputc(int ch, FILE *stream)
{
    while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
```

不勾选 Use MicroLIB

```
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
```

## 3.CPU利用率统计

定义一个

```
uint32_t g_osRuntimeCounter;
```

主要函数：

```
osThreadList()//获取 CPU 利用率与任务相关信息并通过串口打印出来
vTaskGetRunTimeStats()
```

计时函数：

```
void configureTimerForRunTimeStats(void)
{
    g_osRuntimeCounter = 0;
}

unsigned long getRunTimeCounterValue(void)
{
    return g_osRuntimeCounter;
}

//放FreeRTOSConfig.h
extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS configureTimerForRunTimeStats
#define portGET_RUN_TIME_COUNTER_VALUE getRunTimeCounterValue
```

![image-20230610095859338](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images06.png)

配置TIM6时钟，根据**Tclk** 即内部时钟CK_INT，经过APB1预分频器后分频提供，如果APB1预分频系数等于1，则频率不变，否则频率乘以2，库函数中APB1预分频的系数是2，即PCLK1=45MHz，如图所以定时器时钟Tclk=45*2=90M。
![image-20230610101247635](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images07.png)

计算定时器溢出时间(Tout):

```
Tout = 1 / (Tclk / (psc + 1)) ∗ (arr + 1)

定时器时钟Tclk：90MHz
预分频器psc：89
自动重装载寄存器arr：49
Tout = 1/(90MHz/(89+1))∗(49+1) = 50us

Prescaler（时钟预分频数）：90-1 则驱动计数器的时钟 CK_CNT = CK_INT(即90MHz)/(89+1) = 1MHz
Counter Mode（计数模式）：Up（向上计数模式） 基本定时器只能是向上计数
Counter Period（自动重装载值）：50-1 则定时时间 1/CK_CLK*(49+1) = 50us
auto-reload-preload（自动重装载）：Enable（使能）
TRGO Parameters（触发输出）：不使能 在定时器的定时时间到达的时候输出一个信号(如：定时器更新产生TRGO信号来触发ADC的同步转换)
```

![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/images08.png)

利用率：

```
uint8_t CPU_RunInfo[400];
memset(CPU_RunInfo,0,400);              //信息缓冲区清零

osThreadList(CPU_RunInfo);              //获取任务运行时间信息

printf("---------------------------------------------\r\n");
printf("Task      Task_Status Priority  Remaining_Stack Task_No\r\n");
printf("%s", CPU_RunInfo);
printf("---------------------------------------------\r\n");

memset(CPU_RunInfo,0,400);              //信息缓冲区清零

vTaskGetRunTimeStats((char *)&CPU_RunInfo);

printf("Task       Running_Count        Utilization\r\n");
printf("%s", CPU_RunInfo);
printf("---------------------------------------------\r\n\n");
```

# 4.添加SPI点亮ST7789

这边添加的话选择了SPI1,并未添加中断和DMA,基础配置如下，需要特别注意的就是CPOL和CPLA
![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/SPI01.png)

使用逻辑分析仪采集信号时需要配置的SPI1参数如下：

![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/SPI02.png)
![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/SPI03.png)

CPOL和CPLA这里需要特别注意的是采集时是从高电平开始还是电平开始
需要注意的就是采集的频率：

逻辑分析仪的最大深度要高于SPI 时钟(APB2分频) 5-10倍才可比较好的还原信号
此时SPI频率为(APB2)90/8=11,所以我们逻辑分析仪的深度至少
是要11×5=55(MHZ).

![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/SPI04.png)

### 添加DMA功能

![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/DMA01.png)

![image-20230610103116038](https://github.com/apple596/STM32CubemxFreertosProject/blob/main/images/DMA02.png)