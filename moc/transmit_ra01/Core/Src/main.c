#include "main.h"
#include"Lora.h"
#include "cJSON.h"
#include <string.h>
#include <stdint.h>
#include <math.h> 
#include <stdlib.h>
#include"stdio.h"
SPI_HandleTypeDef hspi1;		
TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
LoRa myLoRa;
#define TRIG_PIN          GPIO_PIN_9
#define TRIG_PORT         GPIOA
volatile uint32_t time_Val1 = 0;
volatile uint32_t time_Val2 = 0;
volatile uint32_t time_Width = 0;
volatile uint8_t flag_Fisrt_Captured = 0;
volatile uint32_t timer_ms = 0;
volatile uint8_t length  = 0;
//uint8_t length1 = 0;
int length1 =0;
int TB = 2;

char JSON[100];
char Str_KC[100];	
char Str_TB[100];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
void delay_us(uint16_t time);
void delay_ms(uint16_t ms);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
//void DataJson(unsigned int KC, unsigned int TB);
void DataJson(unsigned int KC);

/* USER CODE BEGIN PFP */
///
void delay_ms(uint16_t ms)
{
	while(ms--)
	{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < 1000);
	}
}

void delay_us(uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER (&htim1) < time);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if( htim->Instance == TIM1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //interrupt source channel1
	{
		if (flag_Fisrt_Captured==0)
		{
			time_Val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			flag_Fisrt_Captured = 1;  
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (flag_Fisrt_Captured==1)
		{
			time_Val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // read second value
			__HAL_TIM_SET_COUNTER(htim, 0);  // reset the counter

			if (time_Val2 > time_Val1)
			{
				time_Width = time_Val2-time_Val1;
			}

			else if (time_Val1 > time_Val2)
			{
				time_Width = (0xffff - time_Val1) + time_Val2;
			}

			length = (uint8_t)(time_Width * 0.034/2);
			flag_Fisrt_Captured = 0; // set flag false


			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);

			
		}
	}
}
void HCSR04_Read (void)
{
 	flag_Fisrt_Captured = 0;
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);  // pull the TRIG pin HIGH
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  // pull the TRIG pin low	
}

/*void DataJson(unsigned int KC, unsigned int TB)
{
		
	for(int i = 0 ; i < 100; i++)
	{	
		Str_KC[i] = 0;
	  Str_TB[i] = 0;
		JSON[i] = 0;
	}
	
	sprintf(Str_KC, "%d", KC);
	sprintf(Str_TB, "%d", TB);
	
	strcat(JSON,"{\"TB\":\"");
	strcat(JSON,Str_TB);
	strcat(JSON,"\",");

	strcat(JSON,"\"KC\":\"");
	strcat(JSON,Str_KC);
	strcat(JSON,"\"}");
	
	
	strcat(JSON,"\r\n");
	
	//printf("DataJson: %s\n", JSON);
	
}

*/


void DataJson(unsigned int KC)
{
		
	for(int i = 0 ; i < 100; i++)
	{	
		Str_KC[i] = 0;

		JSON[i] = 0;
	}
	
	sprintf(Str_KC, "%d", KC);

	

	strcat(JSON,"{\"TB2\":\"");
	strcat(JSON,Str_KC);
	strcat(JSON,"\"}");
	
	
	strcat(JSON,"\r\n");
	
	//printf("DataJson: %s\n", JSON);
	
}

int main(void)
{

  HAL_Init();

	myLoRa = newLoRa();		
	

	
	myLoRa.CS_port         = NSS_GPIO_Port;
	myLoRa.CS_pin          = NSS_Pin;
	myLoRa.reset_port      = RST_GPIO_Port;
	myLoRa.reset_pin       = RST_Pin;
	myLoRa.DIO0_port       = DIO0_GPIO_Port;
	myLoRa.DIO0_pin        = DIO0_Pin;
	myLoRa.hSPIx           = &hspi1;


  SystemClock_Config();

	myLoRa.frequency             = 433;             // default = 433 MHz
	myLoRa.spredingFactor        = SF_7;            // default = SF_7
	myLoRa.bandWidth             = BW_125KHz;      // default = BW_125KHz
	myLoRa.crcRate               = CR_4_5;          // default = CR_4_5
	myLoRa.power                 = POWER_20db;      // default = 20db
	myLoRa.overCurrentProtection = 130;             // default = 100 mA
	myLoRa.preamble              = 9;              // default = 8;

  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
 
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
	
  if (LoRa_init(&myLoRa) == LORA_OK)
  {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 
  }
  else
  {
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); 
  }

  while (1)
  {

		HCSR04_Read();
 	  length1 = (int)length;
		//length1 = 5;
		DataJson(length1);

    if (length1 > 0) 
		{
      LoRa_transmit(&myLoRa,  (uint8_t*)JSON, strlen(JSON), TRANSMIT_TIMEOUT);


      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			delay_ms(500);
			} 
		}

}
/**
  * @b	rief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
   HAL_TIM_Base_Start_IT(&htim1);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, NSS_Pin|RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SR04_TRIG_GPIO_Port, SR04_TRIG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_Pin RST_Pin */
  GPIO_InitStruct.Pin = NSS_Pin|RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SR04_TRIG_Pin */
  GPIO_InitStruct.Pin = SR04_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SR04_TRIG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_Pin */
  GPIO_InitStruct.Pin = DIO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
