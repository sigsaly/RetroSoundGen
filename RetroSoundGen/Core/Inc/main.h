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
#include <stdbool.h>
#include "uart.h"
#include "psg.h"
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define FCLK	    18000000 //1789772.5
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OPN_A0_Pin GPIO_PIN_13
#define OPN_A0_GPIO_Port GPIOC
#define OPN_A1_Pin GPIO_PIN_14
#define OPN_A1_GPIO_Port GPIOC
#define OPN_RD_Pin GPIO_PIN_15
#define OPN_RD_GPIO_Port GPIOC
#define RESETN_Pin GPIO_PIN_0
#define RESETN_GPIO_Port GPIOC
#define OPN_A0C1_Pin GPIO_PIN_1
#define OPN_A0C1_GPIO_Port GPIOC
#define OPN_WR_Pin GPIO_PIN_2
#define OPN_WR_GPIO_Port GPIOC
#define BC1_8_Pin GPIO_PIN_3
#define BC1_8_GPIO_Port GPIOC
#define CLK4M_Pin GPIO_PIN_0
#define CLK4M_GPIO_Port GPIOA
#define Tx2_Pin GPIO_PIN_2
#define Tx2_GPIO_Port GPIOA
#define Rx2_Pin GPIO_PIN_3
#define Rx2_GPIO_Port GPIOA
#define CLK8M_Pin GPIO_PIN_6
#define CLK8M_GPIO_Port GPIOA
#define BDIR_5_Pin GPIO_PIN_4
#define BDIR_5_GPIO_Port GPIOC
#define BC1_5_Pin GPIO_PIN_5
#define BC1_5_GPIO_Port GPIOC
#define DA0_Pin GPIO_PIN_0
#define DA0_GPIO_Port GPIOB
#define DA1_Pin GPIO_PIN_1
#define DA1_GPIO_Port GPIOB
#define DA2_Pin GPIO_PIN_2
#define DA2_GPIO_Port GPIOB
#define BDIR_2_Pin GPIO_PIN_12
#define BDIR_2_GPIO_Port GPIOB
#define BC1_2_Pin GPIO_PIN_13
#define BC1_2_GPIO_Port GPIOB
#define BDIR_3_Pin GPIO_PIN_14
#define BDIR_3_GPIO_Port GPIOB
#define BC1_3_Pin GPIO_PIN_15
#define BC1_3_GPIO_Port GPIOB
#define BDIR_4_Pin GPIO_PIN_6
#define BDIR_4_GPIO_Port GPIOC
#define BC1_4_Pin GPIO_PIN_7
#define BC1_4_GPIO_Port GPIOC
#define BDIR_6_Pin GPIO_PIN_8
#define BDIR_6_GPIO_Port GPIOC
#define BC1_6_Pin GPIO_PIN_9
#define BC1_6_GPIO_Port GPIOC
#define Tx1_Pin GPIO_PIN_9
#define Tx1_GPIO_Port GPIOA
#define Rx1_Pin GPIO_PIN_10
#define Rx1_GPIO_Port GPIOA
#define BDIR_7_Pin GPIO_PIN_10
#define BDIR_7_GPIO_Port GPIOC
#define BC1_7_Pin GPIO_PIN_11
#define BC1_7_GPIO_Port GPIOC
#define BDIR_8_Pin GPIO_PIN_12
#define BDIR_8_GPIO_Port GPIOC
#define DA3_Pin GPIO_PIN_3
#define DA3_GPIO_Port GPIOB
#define DA4_Pin GPIO_PIN_4
#define DA4_GPIO_Port GPIOB
#define DA5_Pin GPIO_PIN_5
#define DA5_GPIO_Port GPIOB
#define DA6_Pin GPIO_PIN_6
#define DA6_GPIO_Port GPIOB
#define DA7_Pin GPIO_PIN_7
#define DA7_GPIO_Port GPIOB
#define BC1_1_Pin GPIO_PIN_8
#define BC1_1_GPIO_Port GPIOB
#define BDIR_1_Pin GPIO_PIN_9
#define BDIR_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
