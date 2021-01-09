/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define user1 "ALLA"
#define user2 "KOMP"
#define user3 "STD+"
#define END 3
#define MIDEND 2
#define BLOCK 0
#define rxALARM 239
#define Set_Rx_Mode_ALARM 73
#define Base_State_ALARM 0
#define Alarm_signal_count 33
#define value_delay_Alarm 850 //delay after alarm
#define delay_IDLE_value 475
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

RTC_TimeTypeDef sTime = { 0 };
volatile uint16_t adc = 0;
volatile uint16_t adc_Filtered = 0;
volatile uint16_t adc_Filtered_Result = 0;
volatile uint16_t rxPL = 0;
volatile uint8_t ConfigPereph = 0;
volatile uint8_t ConfigPSU = 0;
volatile uint8_t ALARM = 0;
volatile uint8_t Boot_end = 0;
volatile uint32_t adc_counter = 0;
volatile int32_t delay_Alarm = 0;
volatile uint32_t delay_IDLE_counter = 0;
volatile uint8_t name = UID;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		adc = HAL_ADC_GetValue(&hadc1);
		adc_counter++;
		adc_Filtered = adc_Filtered + adc;
		if (adc_counter == 5) {
			adc_Filtered_Result = 0;
			adc_Filtered_Result = adc_Filtered / 5;
			adc_Filtered = 0;
			adc_counter = 0;
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (Boot_end == END) {

		
		
		
		if (GPIO_Pin == GPIO_PIN_0) {
			Boot_end = BLOCK;
			ALARM = rxALARM;



		}

		
		
		if (GPIO_Pin == GPIO_PIN_12) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			sTime.Hours = sTime.Hours;
			if (sTime.Minutes == 0x59) {
				sTime.Hours = sTime.Hours--;
			}

			sTime.Minutes = sTime.Minutes++;
			sTime.Seconds = 0x01;
			if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
				Error_Handler();
			}
		}

		if (GPIO_Pin == GPIO_PIN_13) {
			HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
			sTime.Hours = sTime.Hours++;
			sTime.Minutes = sTime.Minutes;
			sTime.Seconds = 0x01;

			if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
				Error_Handler();
			}
		}
	}

	
	
	if (Boot_end >= MIDEND) {
		if (GPIO_Pin == GPIO_PIN_2) {
			rxPL = nrfRXWHILE_START();
		}
	}

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_IWDG_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	nRF24_CSN_H();
	nRF24_RX_OFF()
//------------------------------------------------------------------ Config PIN
	GPIO_PinState S1 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	GPIO_PinState S2 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	GPIO_PinState S3 = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2);
	ConfigPereph = S3;
	ConfigPSU = S2;
//------------------------------------------------------------------ OLED
	ssd1306_Init();
	ssd1306_FlipScreenVertically();
	ssd1306_Clear();
	ssd1306_SetColor(White);
	HAL_Delay(750);



//------------------------------------------------------------------ StartUP
		ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("ALARM BUTTON PRJ", Font_7x10);
		ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("HW V1.1", Font_7x10);
		ssd1306_SetCursor(0, 20);
		ssd1306_WriteString("SW V.84", Font_7x10);
	
			ssd1306_UpdateScreen();
			HAL_Delay(800);
			ssd1306_Clear();
	
	ssd1306_SetCursor(0, 20);
	ssd1306_WriteString("nRF24L01 chk:", Font_7x10);

		if(S1 == 0) {
	if (ConfigPereph == 1) {
		ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("BOSCH BMx280:", Font_7x10);
	}
	if (ConfigPereph == 0) {
		ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("ST HTS221chk:", Font_7x10);
	}
} else {		
	ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("NO SENSOR", Font_7x10);
}
	
	
	
	ssd1306_SetCursor(0, 0);
	ssd1306_WriteString("POWER SYSTEM:", Font_7x10);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);  
	ssd1306_SetCursor(92, 0);
	if (ConfigPSU == 0) {
	ssd1306_WriteString(" AC ", Font_7x10);
	}
		if (ConfigPSU == 1) {
	ssd1306_WriteString("ACCU", Font_7x10);
	}
	ssd1306_UpdateScreen();
	HAL_Delay(800);
//------------------------------------------------------------------ MEMS StartUP
	if(S1 == 0) {
	if (ConfigPereph == 1) {
		BME280_Begin();
	}
	if (ConfigPereph == 0) {
		hts221_ini();
	}
	}
	if (ConfigPSU == 1) {
//------------------------------------------------------------------ ADC StartUP
	HAL_ADCEx_Calibration_Start(&hadc1);
	HAL_ADC_Start_IT(&hadc1);
	HAL_TIM_Base_Start(&htim3); }
//------------------------------------------------------------------ nRF24l01_1 StartUP
	nRF24_ChecktoOLED();
	nRF24_Init();
	nrfRXMULTI_START();
//------------------------------------------------------------------ Knock-knock-knock
	SPON();
	HAL_Delay(50);
	SPOFF();
	HAL_Delay(50);
	SPON();
	HAL_Delay(45);
	SPOFF();
	HAL_Delay(50);
	SPON();
	HAL_Delay(40);
	SPOFF();
	HAL_Delay(3000); 
	
	ssd1306_Clear();
	ssd1306_UpdateScreen();
	Boot_end = END; // Enable All IRQ
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//------------------------------------------------------------------ WHILE MAIN LOOP
	while (1) {
		ssd1306_Clear();
	HAL_IWDG_Refresh(&hiwdg);

		if (ALARM == rxALARM) {
						SPON();
						HAL_Delay(150);
						SPOFF();
			nrfTXMULTI_START();
			ssd1306_SetCursor(24, 3);
			ssd1306_WriteString("ALARM", Font_16x26);
			ssd1306_UpdateScreen();
			
			for (uint8_t i = 0; i < Alarm_signal_count; i++) {
			HAL_IWDG_Refresh(&hiwdg);
			nrfTXWHILE_START();
			}

			ALARM = Set_Rx_Mode_ALARM;
			Boot_end = END;
		}

		if (ALARM == Set_Rx_Mode_ALARM) {
			ssd1306_Clear();
			ssd1306_UpdateScreen();
			nrfRXMULTI_START();
			ALARM = Base_State_ALARM;
		}

		switch (rxPL) {
		case 0x00:
				//------------------------------------------------------------------ ALARM STATE DELAY
			if (delay_Alarm > 0) {
				delay_Alarm--;
				HAL_Delay(1);
				Boot_end = MIDEND; // Enable only RX IRQ

				if (delay_Alarm == -1) {
					Boot_end = END; // Enable All IRQ
				}

			} else

			{
				//------------------------------------------------------------------ IDLE STATE ST
				delay_IDLE_counter++;
				if (delay_IDLE_counter >= delay_IDLE_value) {
					Time(ConfigPereph);
						if(S1 == 0) {
						if (ConfigPereph == 1) {
						BME280_Read();
					}
				if (ConfigPereph == 0) {
						hts221_Read();
					}		
				} else {
						ssd1306_SetCursor(0, 0);
		ssd1306_WriteString("ALARM BUTTON PRJ", Font_7x10);
			ssd1306_SetCursor(0, 10);
		ssd1306_WriteString("HW V1.1", Font_7x10);
			ssd1306_SetCursor(0, 20);
		ssd1306_WriteString("SW V.84", Font_7x10);
				}
						if (ConfigPSU == 1) {
					ADC_BL(adc_Filtered_Result);
						}
					switch (name) {
					case 0x01:
						ssd1306_SetCursor(90, 10);
						ssd1306_WriteString(user1, Font_7x10);
						break;
					case 0x02:
						ssd1306_SetCursor(90, 10);
						ssd1306_WriteString(user2, Font_7x10);
						break;
					case 0x03:
						ssd1306_SetCursor(90, 10);
						ssd1306_WriteString(user3, Font_7x10);
						break;
					default:
						ssd1306_SetCursor(90, 10);
						ssd1306_WriteString("U0", Font_7x10);
						break;
					}
					delay_IDLE_counter = 0;
					ssd1306_UpdateScreen();
				}
	Boot_end = END; // Enable All IRQ
				//------------------------------------------------------------------ IDLE STATE FIN
				//------------------------------------------------------------------ RX ALARM
			}
			break;
		case 0x01:
			ssd1306_SetCursor(31, 3);
			ssd1306_WriteString(user1, Font_16x26);
			ssd1306_UpdateScreen();
			SPON(); LDON();
			HAL_Delay(500);
			SPOFF(); LDOFF();
			rxPL = 0;
			delay_Alarm = value_delay_Alarm;
			break;
		case 0x02:
			ssd1306_SetCursor(31, 3);
			ssd1306_WriteString(user2, Font_16x26);
			ssd1306_UpdateScreen();
			SPON(); LDON();
			HAL_Delay(500);
			SPOFF(); LDOFF();
			rxPL = 0;
			delay_Alarm = value_delay_Alarm;
			ssd1306_Clear();
			ssd1306_UpdateScreen();
			break;
		case 0x03:
			ssd1306_SetCursor(31, 3);
			ssd1306_WriteString(user3, Font_16x26);
			ssd1306_UpdateScreen();
			SPON(); LDON();
			HAL_Delay(500);
			SPOFF(); LDOFF();
			rxPL = 0;
			delay_Alarm = value_delay_Alarm;
			break;
		default:
			ssd1306_SetCursor(0, 3);
			ssd1306_WriteString("F0", Font_16x26);
			ssd1306_UpdateScreen();
			SPON(); LDON();
			HAL_Delay(500);
			SPOFF(); LDOFF();
			rxPL = 0;
			delay_Alarm = value_delay_Alarm;
			break;
		}
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
