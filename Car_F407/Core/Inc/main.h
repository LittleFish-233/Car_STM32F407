/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define Key_Stop_Pin GPIO_PIN_3
#define Key_Stop_GPIO_Port GPIOE
#define Key_Stop_EXTI_IRQn EXTI3_IRQn
#define Motor_1_IN1_Pin GPIO_PIN_2
#define Motor_1_IN1_GPIO_Port GPIOF
#define Motor_1_IN2_Pin GPIO_PIN_3
#define Motor_1_IN2_GPIO_Port GPIOF
#define Motor_2_IN1_Pin GPIO_PIN_4
#define Motor_2_IN1_GPIO_Port GPIOF
#define Motor_2_IN2_Pin GPIO_PIN_5
#define Motor_2_IN2_GPIO_Port GPIOF
#define Motor_3_IN1_Pin GPIO_PIN_6
#define Motor_3_IN1_GPIO_Port GPIOF
#define Motor_3_IN2_Pin GPIO_PIN_7
#define Motor_3_IN2_GPIO_Port GPIOF
#define SIM_SD_Inser_Pin GPIO_PIN_8
#define SIM_SD_Inser_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_9
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_10
#define LED2_GPIO_Port GPIOF
#define LCD_SDA_Pin GPIO_PIN_3
#define LCD_SDA_GPIO_Port GPIOC
#define BatteryLevel_Pin GPIO_PIN_4
#define BatteryLevel_GPIO_Port GPIOA
#define Motor_1_PWM_Pin GPIO_PIN_6
#define Motor_1_PWM_GPIO_Port GPIOA
#define Motor_2_PWM_Pin GPIO_PIN_7
#define Motor_2_PWM_GPIO_Port GPIOA
#define Motor_3_PWM_Pin GPIO_PIN_0
#define Motor_3_PWM_GPIO_Port GPIOB
#define Motor_4_PWM_Pin GPIO_PIN_1
#define Motor_4_PWM_GPIO_Port GPIOB
#define Motor_4_IN1_Pin GPIO_PIN_0
#define Motor_4_IN1_GPIO_Port GPIOG
#define Motor_4_IN2_Pin GPIO_PIN_1
#define Motor_4_IN2_GPIO_Port GPIOG
#define Motor_1_Encoder_A_Pin GPIO_PIN_9
#define Motor_1_Encoder_A_GPIO_Port GPIOE
#define Motor_1_Encoder_B_Pin GPIO_PIN_11
#define Motor_1_Encoder_B_GPIO_Port GPIOE
#define LCD_SCL_Pin GPIO_PIN_10
#define LCD_SCL_GPIO_Port GPIOB
#define Infrared1_Pin GPIO_PIN_12
#define Infrared1_GPIO_Port GPIOB
#define Infrared2_Pin GPIO_PIN_13
#define Infrared2_GPIO_Port GPIOB
#define W25QXX_CS_Pin GPIO_PIN_14
#define W25QXX_CS_GPIO_Port GPIOB
#define Infrared3_Pin GPIO_PIN_8
#define Infrared3_GPIO_Port GPIOD
#define Infrared4_Pin GPIO_PIN_9
#define Infrared4_GPIO_Port GPIOD
#define Motor_4_Encoder_A_Pin GPIO_PIN_12
#define Motor_4_Encoder_A_GPIO_Port GPIOD
#define Motor_4_Encoder_B_Pin GPIO_PIN_13
#define Motor_4_Encoder_B_GPIO_Port GPIOD
#define Motor_F_STBY_Pin GPIO_PIN_2
#define Motor_F_STBY_GPIO_Port GPIOG
#define Motor_B_STBY_Pin GPIO_PIN_3
#define Motor_B_STBY_GPIO_Port GPIOG
#define LCD_PWR_Pin GPIO_PIN_4
#define LCD_PWR_GPIO_Port GPIOG
#define LCD_RST_Pin GPIO_PIN_5
#define LCD_RST_GPIO_Port GPIOG
#define LCD_WR_RS_Pin GPIO_PIN_6
#define LCD_WR_RS_GPIO_Port GPIOG
#define Motor_2_Encoder_A_Pin GPIO_PIN_6
#define Motor_2_Encoder_A_GPIO_Port GPIOC
#define Motor_2_Encoder_B_Pin GPIO_PIN_7
#define Motor_2_Encoder_B_GPIO_Port GPIOC
#define Bluetooth_TX_Pin GPIO_PIN_9
#define Bluetooth_TX_GPIO_Port GPIOA
#define Bluetooth_RX_Pin GPIO_PIN_10
#define Bluetooth_RX_GPIO_Port GPIOA
#define W52QXX_SCK_Pin GPIO_PIN_3
#define W52QXX_SCK_GPIO_Port GPIOB
#define W52QXX_MISO_Pin GPIO_PIN_4
#define W52QXX_MISO_GPIO_Port GPIOB
#define W52QXX_MOSI_Pin GPIO_PIN_5
#define W52QXX_MOSI_GPIO_Port GPIOB
#define MPU_SCL_Pin GPIO_PIN_6
#define MPU_SCL_GPIO_Port GPIOB
#define MPU_SDA_Pin GPIO_PIN_7
#define MPU_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define  u32 uint32_t
#define  u16 uint16_t
#define  u8 uint8_t
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
