/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "string.h"
#include "ssd1306.h"
#include "nrf24spi1.h"
#include "unrf24.h"
#include "bme280.h"
#include "mems.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define SPON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
#define SPOFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define LDON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET)
#define LDOFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET)
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AL_BUT_Pin GPIO_PIN_0
#define AL_BUT_GPIO_Port GPIOA
#define AL_BUT_EXTI_IRQn EXTI0_IRQn
#define ADC_INP_Pin GPIO_PIN_1
#define ADC_INP_GPIO_Port GPIOA
#define NRF_IRQ_Pin GPIO_PIN_2
#define NRF_IRQ_GPIO_Port GPIOA
#define NRF_IRQ_EXTI_IRQn EXTI2_IRQn
#define NRF_CE_Pin GPIO_PIN_3
#define NRF_CE_GPIO_Port GPIOA
#define NRF_CSN_Pin GPIO_PIN_4
#define NRF_CSN_GPIO_Port GPIOA
#define S1_Pin GPIO_PIN_0
#define S1_GPIO_Port GPIOB
#define S2_Pin GPIO_PIN_1
#define S2_GPIO_Port GPIOB
#define S3_Pin GPIO_PIN_2
#define S3_GPIO_Port GPIOB
#define Alarm_Output_Pin GPIO_PIN_11
#define Alarm_Output_GPIO_Port GPIOB
#define MIN_BUT_Pin GPIO_PIN_12
#define MIN_BUT_GPIO_Port GPIOB
#define MIN_BUT_EXTI_IRQn EXTI15_10_IRQn
#define HOUR_BUT_Pin GPIO_PIN_13
#define HOUR_BUT_GPIO_Port GPIOB
#define HOUR_BUT_EXTI_IRQn EXTI15_10_IRQn
#define LD1_Pin GPIO_PIN_3
#define LD1_GPIO_Port GPIOB
#define LD1_EXTI_IRQn EXTI3_IRQn
#define LD2_Pin GPIO_PIN_4
#define LD2_GPIO_Port GPIOB
#define LD2_EXTI_IRQn EXTI4_IRQn
#define BUZZ_Pin GPIO_PIN_8
#define BUZZ_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
