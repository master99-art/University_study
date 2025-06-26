/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define joint5_Pin GPIO_PIN_0
#define joint5_GPIO_Port GPIOA
#define joint6_Pin GPIO_PIN_4
#define joint6_GPIO_Port GPIOA
#define joint2_Pin GPIO_PIN_5
#define joint2_GPIO_Port GPIOA
#define joint3_Pin GPIO_PIN_6
#define joint3_GPIO_Port GPIOA
#define hand_Pin GPIO_PIN_7
#define hand_GPIO_Port GPIOA
#define joint1_Pin GPIO_PIN_9
#define joint1_GPIO_Port GPIOE
#define joint4_Pin GPIO_PIN_12
#define joint4_GPIO_Port GPIOD
#define K_hand_Pin GPIO_PIN_4
#define K_hand_GPIO_Port GPIOD
#define DIR6_Pin GPIO_PIN_5
#define DIR6_GPIO_Port GPIOD
#define DIR5_Pin GPIO_PIN_6
#define DIR5_GPIO_Port GPIOD
#define K6_Pin GPIO_PIN_7
#define K6_GPIO_Port GPIOD
#define K5_Pin GPIO_PIN_9
#define K5_GPIO_Port GPIOG
#define K4_Pin GPIO_PIN_10
#define K4_GPIO_Port GPIOG
#define K3_Pin GPIO_PIN_11
#define K3_GPIO_Port GPIOG
#define K2_Pin GPIO_PIN_12
#define K2_GPIO_Port GPIOG
#define K1_Pin GPIO_PIN_13
#define K1_GPIO_Port GPIOG
#define DIR1_Pin GPIO_PIN_3
#define DIR1_GPIO_Port GPIOB
#define DIR2_Pin GPIO_PIN_4
#define DIR2_GPIO_Port GPIOB
#define DIR3_Pin GPIO_PIN_5
#define DIR3_GPIO_Port GPIOB
#define DIR4_Pin GPIO_PIN_8
#define DIR4_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_9
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_0
#define LED2_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
