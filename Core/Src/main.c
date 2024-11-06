/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "lcd.h"
#include "images.h"
#include "kirby.h"
#include "kirbySprites.h"
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 DMA_HandleTypeDef hdma_memtomem_dma1_channel1;
SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void XferCpltCallback(DMA_HandleTypeDef* hdma);

uint16_t currentPos = 0;
/*
 * Moves the platform image one pixel off from the original position.
 *
 * uint16_t xPos: the updated x Position of the first column
 * */
void movePlatform(uint16_t kirbyX) {
	uint16_t xPos = kirbyX - 160;
	if (currentPos < xPos) {
		// Scrolling to the right
		uint16_t targetCol = (xPos - 1) % 320;
		for (uint16_t row = 0; row < 209; row++) {
			LCD_DrawDot(targetCol, row, platformImg[882 * row + xPos + 320 - 1]);
		}
	} else if (currentPos > xPos) {
		// Scrolling to the left
		uint16_t targetCol = xPos % 320;
		for (uint16_t row = 0; row < 209; row++) {
			LCD_DrawDot(targetCol, row, platformImg[882 * row + xPos]);
		}
	} else {
		return;
	}
	currentPos = xPos;


	// Vertical scroll, starting from column = xPos
	  LCD_Write_Cmd(0x33);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x01);
	  LCD_Write_Data(0x40);
	  LCD_Write_Data(0x00);
	  LCD_Write_Data(0x00);

	  LCD_Write_Cmd(0x37);
	  LCD_Write_Data((xPos % 320) >> 8);
	  LCD_Write_Data((xPos % 320) & 0xff);
}

void displayKirbyFacingRight(const uint16_t* frame, uint16_t xPos, uint16_t yPos) {
	for (int x = 0; x < 26; x++) {
		uint16_t displayXPos = (xPos + x) % 320;
		for (int y = 0; y < 23; y++) {
			uint16_t displayYPos = yPos + y;
			uint16_t color = frame[23 * x + y];
			// Replacing color
			if (color == 0x4B4E) {
				uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + x)];
				color = newColor;
			}
			LCD_DrawDot(displayXPos, displayYPos, color);
		}
	}
}

void displayKirbyFacingLeft(const uint16_t* frame, uint16_t xPos, uint16_t yPos) {
	for (int x = 0; x < 26; x++) {
		uint16_t displayXPos = (xPos + 25 - x) % 320;
		for (int y = 0; y < 23; y++) {
			uint16_t displayYPos = yPos + y;
			uint16_t color = frame[23 * x + y];
			// Replacing color
			if (color == 0x4B4E) {
				uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + 25 - x)];
				color = newColor;
			}
			LCD_DrawDot(displayXPos, displayYPos, color);
		}
	}
}

void redrawColumnBefore(struct Kirby* kirby) {
	// Redraw the first column
	for (int y = 0; y < 23; y++) {
	  uint16_t displayX = kirby->xPos % 320;
	  uint16_t displayY = kirby->yPos + y;
	  uint16_t color = platformImg[882 * (displayY) + (kirby->xPos)];
	  LCD_DrawDot(displayX, displayY, color);
	}
}

void redrawColumnAfter(struct Kirby* kirby) {
	// Redraw the last column
	for (int y = 0; y < 23; y++) {
		uint16_t displayX = (kirby->xPos + 25) % 320;
		uint16_t displayY = kirby->yPos + y;
		uint16_t color = platformImg[882 * (displayY) + kirby->xPos + 25];
		LCD_DrawDot(displayX, displayY, color);
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
  MX_DMA_Init();
  MX_FSMC_Init();
  /* USER CODE BEGIN 2 */

  // Initialize DMA
  hdma_memtomem_dma1_channel1.XferCpltCallback = &XferCpltCallback;

  LCD_INIT();

  // Change render direction to horizontal
  LCD_Write_Cmd(0x36);
  LCD_Write_Data(0x68);

  LCD_OpenWindow(0, 0, 320, 240);
  LCD_Write_Cmd ( CMD_SetPixel );

  // Rendering platform Image
  for (int y = 0; y < 209; y++) {
	  HAL_DMA_Start_IT(&hdma_memtomem_dma1_channel1, (uint32_t)(&platformImg[882 * y]), (uint32_t)0x60020000, 320);
	  HAL_Delay(1);
  }

  // Initializing Kirby
  struct Kirby kirby = {
		  50, 140,
		  {KIRBY_IDLE_SPRITE_0, KIRBY_IDLE_SPRITE_1, KIRBY_IDLE_SPRITE_2},
		  {
				  KIRBY_WALKING_SPRITE_0, KIRBY_WALKING_SPRITE_1, KIRBY_WALKING_SPRITE_2, KIRBY_WALKING_SPRITE_3, KIRBY_WALKING_SPRITE_4,
				  KIRBY_WALKING_SPRITE_5, KIRBY_WALKING_SPRITE_6, KIRBY_WALKING_SPRITE_7, KIRBY_WALKING_SPRITE_8, KIRBY_WALKING_SPRITE_9,
		  }
  };

  // Looping through idle sprites
  uint8_t currentFrame = 0;
//  while (1) {
//	  displayKirby(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//	  currentFrame = (currentFrame + 1) % 3;
//	  HAL_Delay(50);
//	  if (currentFrame == 1) {
//		  HAL_Delay(2500); // Stay longer at the 0th frame
//	  }
//  }

  displayKirbyFacingRight(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
  HAL_Delay(1000);
//  // Moving Kirby to the right
//  // Redraw the first column
//  for (int y = 0; y < 23; y++) {
//	  uint16_t displayX = kirby.xPos;
//	  uint16_t displayY = kirby.yPos + y;
//	  uint16_t color = platformImg[882 * (displayY) + (displayX)];
//	  LCD_DrawDot(displayX, displayY + y, color);
//  }
//
//  // Draw Kirby 1 pixel to the right
//  kirby.xPos++;
//  displayKirby(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);

//  // Moves the platform to the right
//  for (uint16_t i = 0; i < 50; i++) {
//	  movePlatform(i);
//  }
//
//  HAL_Delay(2000);
//
//  // Moves the platform to the left
//  for (uint16_t i = 49; i > 0; i--) {
//	  movePlatform(i);
//  }


  /* GPIO definitions */
  // Key 1
  GPIO_TypeDef* KEY1_GPIO_PORT = GPIOA;
  uint16_t KEY1_GPIO_PIN = GPIO_PIN_0;
  uint8_t key1State;

  // Key 2
  GPIO_TypeDef* KEY2_GPIO_PORT = GPIOC;
  uint16_t KEY2_GPIO_PIN = GPIO_PIN_13;
  uint8_t key2State;

  // Keeps track of state change: idle - 0, walking right - 1, walking left - 2
  uint8_t state = 0;

  // Keeps track of direction: right - 0, left - 1
  uint8_t direction = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // Displays xPos on LCD
	  LCD_Clear(10, 210, 100, 20, 0xFFFF);
	  char buffer[6];
	  sprintf(buffer, "%d", kirby.xPos);
	  LCD_DrawString(10, 210, buffer);

	  key1State = HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
	  key2State = HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
	  if (key1State) {
		  /* Walking left */
		  if (state != 2) {
			  state = 2;
			  currentFrame = 0;
			  direction = 1;
		  }
		  redrawColumnAfter(&kirby);
		  if (kirby.xPos > 0) {
			  if (kirby.xPos > 160 && kirby.xPos < 696) {
				  movePlatform(kirby.xPos);
			  }
			  kirby.xPos--;
		  }
		  displayKirbyFacingLeft(kirby.walkingKeyframes[currentFrame], kirby.xPos, kirby.yPos);
		  currentFrame = (currentFrame + 1) % 10;
		  HAL_Delay(20);
	  } else if (key2State) {
		  /* Walking right */
		  // Set state to walking, reset frame
		  if (state != 1) {
			  state = 1;
			  currentFrame = 0;
			  direction = 0;
		  }
		  redrawColumnBefore(&kirby);
		  if (kirby.xPos < 829) {
			  if (kirby.xPos > 160 && kirby.xPos < 696) {
				  movePlatform(kirby.xPos);
			  }
			  kirby.xPos++;
		  }
		  displayKirbyFacingRight(kirby.walkingKeyframes[currentFrame], kirby.xPos, kirby.yPos);
		  currentFrame = (currentFrame + 1) % 10;
		  HAL_Delay(20);
	  } else {
		  /* Idle */
		  // Set state to idle, reset frame
		  if (state != 0) {
			  state = 0;
			  currentFrame = 0;
		  }
		  if (direction == 0) {
			  // right
			  displayKirbyFacingRight(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
		  } else {
			  // left
			  displayKirbyFacingLeft(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
		  }
		  currentFrame = (currentFrame + 1) % 3;
		  HAL_Delay(50);
		  if (currentFrame == 1) {
			  //HAL_Delay(2500); // Stay longer at the 0th frame

			  uint32_t tickstart = HAL_GetTick();
			  uint32_t wait = 2500;

			  /* Add a freq to guarantee minimum wait */
			  if (wait < HAL_MAX_DELAY)
			  {
				  wait += (uint32_t)(uwTickFreq);
			  }

			  while ((HAL_GetTick() - tickstart) < wait)
			  {
				  key1State = HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
				  key2State = HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
				  if (key2State || key1State) {
					  break;
				  }
			  }
		  }
	  }
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
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel1
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
  hdma_memtomem_dma1_channel1.Instance = DMA1_Channel1;
  hdma_memtomem_dma1_channel1.Init.Direction = DMA_MEMORY_TO_MEMORY;
  hdma_memtomem_dma1_channel1.Init.PeriphInc = DMA_PINC_ENABLE;
  hdma_memtomem_dma1_channel1.Init.MemInc = DMA_MINC_DISABLE;
  hdma_memtomem_dma1_channel1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_memtomem_dma1_channel1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_memtomem_dma1_channel1.Init.Mode = DMA_NORMAL;
  hdma_memtomem_dma1_channel1.Init.Priority = DMA_PRIORITY_LOW;
  if (HAL_DMA_Init(&hdma_memtomem_dma1_channel1) != HAL_OK)
  {
    Error_Handler( );
  }

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : Key_2_Pin */
  GPIO_InitStruct.Pin = Key_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Key_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Key_1_Pin */
  GPIO_InitStruct.Pin = Key_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Key_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /** Disconnect NADV
  */

  __HAL_AFIO_FSMCNADV_DISCONNECTED();

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */

void XferCpltCallback(DMA_HandleTypeDef* hdma) {
	__NOP();
}

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
