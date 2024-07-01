/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

#define dbmsg(fmt, args...) printf("%s[%d]: " fmt "  \r\n", __FUNCTION__, __LINE__, ##args) //__FILE__,
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void set_brightness_value(void *obj, int32_t value);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SYSLED_Pin GPIO_PIN_13
#define SYSLED_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOA
#define GREEN_LED_Pin GPIO_PIN_14
#define GREEN_LED_GPIO_Port GPIOB
#define RED_LED_Pin GPIO_PIN_15
#define RED_LED_GPIO_Port GPIOB
#define BLUE_LED_Pin GPIO_PIN_8
#define BLUE_LED_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_9
#define LCD_RST_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_8
#define LCD_DC_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
