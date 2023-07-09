/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "src\misc\lv_color.h"
/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */
#define    LCD_PWR(n)   (n?\
                        HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_SET):\
                        HAL_GPIO_WritePin(LCD_PWR_GPIO_Port,LCD_PWR_Pin,GPIO_PIN_RESET))
#define    LCD_WR_RS(n) (n?\
                        HAL_GPIO_WritePin(LCD_WR_GPIO_Port,LCD_WR_Pin,GPIO_PIN_SET):\
                        HAL_GPIO_WritePin(LCD_WR_GPIO_Port,LCD_WR_Pin,GPIO_PIN_RESET))
#define    LCD_RST(n)   (n?\
                        HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_SET):\
                        HAL_GPIO_WritePin(LCD_RST_GPIO_Port,LCD_RST_Pin,GPIO_PIN_RESET))
												

#define LCD_Width   240
#define LCD_Height  240

#define WHITE   0xFFFF    
#define YELLOW  0xFFE0    
#define BRRED   0XFC07    
#define PINK    0XF81F   
#define RED     0xF800    
#define BROWN   0XBC40    
#define GRAY    0X8430    
#define GBLUE   0X07FF   
#define GREEN   0x07E0    
#define BLUE    0x001F    
#define BLACK   0x0000  

#define LCD_TOTAL_BUF_SIZE    (240*240*2)
#define LCD_Buf_Size 1152
static uint8_t lcd_buf[LCD_Buf_Size];

void LCD_Init(void);
static void LCD_GPIO_Init(void);
void LCD_Clear(uint16_t color);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DisplayOff(void);
void LCD_DisplayOn(void);
static void LCD_Write_Data(uint8_t dat);
static void LCD_Write_Cmd(uint8_t cmd);
uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size);
/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */

void TFT_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
void LCD_Fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);
void LCD_Show_Image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

