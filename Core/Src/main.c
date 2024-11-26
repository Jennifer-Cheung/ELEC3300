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

#include <stdio.h>
#include "lcd.h"
#include "images.h"
#include "kirby.h"

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
//  struct Kirby kirby = {
//		  50, 140,
//		  {KIRBY_IDLE_SPRITE_0, KIRBY_IDLE_SPRITE_1, KIRBY_IDLE_SPRITE_2},
//		  {
//				  KIRBY_WALKING_SPRITE_0, KIRBY_WALKING_SPRITE_1, KIRBY_WALKING_SPRITE_2, KIRBY_WALKING_SPRITE_3, KIRBY_WALKING_SPRITE_4,
//				  KIRBY_WALKING_SPRITE_5, KIRBY_WALKING_SPRITE_6, KIRBY_WALKING_SPRITE_7, KIRBY_WALKING_SPRITE_8, KIRBY_WALKING_SPRITE_9,
//		  }
//  };
//
//  uint8_t currentFrame = 0;
//
//  Kirby_displayKirbyFacingRight(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//  HAL_Delay(1000);

  /* GPIO definitions */
  // Key 1 (Left Button)
  GPIO_TypeDef* LEFT_GPIO_PORT = GPIOA;
  uint16_t LEFT_GPIO_PIN = GPIO_PIN_0;
  uint8_t inputLeft;

  // Key 2 (Right Button)
  GPIO_TypeDef* RIGHT_GPIO_PORT = GPIOC;
  uint16_t RIGHT_GPIO_PIN = GPIO_PIN_13;
  uint8_t inputRight;

  // A Button
  GPIO_TypeDef* A_BUTTON_GPIO_PORT = GPIOB;
  uint16_t A_BUTTON_GPIO_PIN = GPIO_PIN_10;
  uint8_t inputA;

  // B Button
  GPIO_TypeDef* B_BUTTON_GPIO_PORT = GPIOB;
  uint16_t B_BUTTON_GPIO_PIN = GPIO_PIN_11;
  uint8_t inputB;

  // Keeps track of state change: idle - 0, walking right - 1, walking left - 2
//  uint8_t state = 0;
//
//  // Keeps track of direction: right - 0, left - 1
//  uint8_t direction = 0;
//
//  const uint16_t KIRBY_FLOATING_SPRITE_0[1056] = {
//  0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x3A8B,0x3104,0x40C4,0x4906,0x4906,0x3883,0x3186,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3229,0x3946,0x48E5,0x4906,0x40C5,0x39A7,0x42EC,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x3A8B,0x3104,0x9147,0xE332,0xF3F6,0xF3F6,0xD28F,0x68C5,0x4083,0x48E5,0x4947,0x4967,0x4988,0x4988,0x4967,0x4967,0x4926,0x40C5,0x2842,0x1820,0x99CA,0xEB94,0xF3F6,0xEB73,0x81CA,0x39C8,0x42EC,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x78E5,0xDB11,0xF4B9,0xFD1B,0xFD1B,0xF457,0xD28F,0xD28F,0xEBB4,0xF478,0xFCB9,0xFCFA,0xFD1B,0xFCD9,0xFCB9,0xF437,0xEB73,0xAA4C,0x89A8,0xCA8E,0xF416,0xF4B9,0xFCFA,0xF457,0x5167,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x89AA,0xEC37,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xF457,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xEB73,0xEB73,0xEBB4,0xF478,0xFCB8,0x59A8,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x9A4C,0xF478,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCD9,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xE311,0xE331,0xEBF5,0x5988,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x920B,0xF437,0xFCFA,0xFD1B,0xFD1B,0xFCFA,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCFA,0xFCDA,0xFCFA,0xFD1B,0xFCFA,0xFCB8,0xF416,0xD28E,0xC24D,0x40E5,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x8127,0xE352,0xF498,0xFCDA,0xFCDA,0xFCB9,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF437,0xD372,0xE416,0xF4DA,0xF437,0xD372,0xE3D4,0xF416,0xAA0B,0x3062,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x3A8B,0x10C3,0x6083,0xCA4D,0xF3D5,0xF477,0xFC98,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xEB93,0xAACC,0xC2CE,0xEC37,0xEB93,0xAACC,0xC2CE,0xEC37,0xEBB4,0x6126,0x2966,0x42EC,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x2021,0x6905,0xC26D,0xEBB4,0xF478,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xC4D5,0x9A6B,0xD393,0xF457,0xC4D5,0x9A6B,0xD393,0xFCD9,0xA20B,0x2841,0x2A09,0x4B4E,0x4B4E,
//  0x4B4E,0x3A8B,0x20E3,0x80E6,0xDAF0,0xF457,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCD9,0x69C9,0x3041,0xB351,0xFCD9,0x69C9,0x3041,0xB351,0xFD1B,0xD2AF,0x5083,0x2966,0x42EC,0x4B4E,
//  0x4B4E,0x1925,0x4862,0xB9EB,0xEBF5,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xA22C,0x2841,0x9B31,0xFD1B,0xA22C,0x2841,0x9B31,0xFD1B,0xEBB4,0x9988,0x2841,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x78E5,0xDAF0,0xF499,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF4B9,0xF3F8,0xFBB7,0xFC99,0xD2F0,0x40A4,0x9B31,0xFD1B,0xD2F0,0x40A4,0x9A4F,0xF438,0xF4B8,0xD28E,0x4083,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8969,0xEBB4,0xFCD9,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xF2D4,0xFA13,0xFBF8,0xF457,0x81EA,0xB372,0xFD1B,0xF457,0x81EA,0xB18E,0xF355,0xF4B9,0xE352,0x48C5,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8969,0xEBB4,0xFCD9,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xEBD5,0xEC16,0xF4DA,0xF4B9,0xEBB4,0xF457,0xFD1B,0xFD1B,0xE352,0x48C5,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8969,0xEBB4,0xFCD9,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xEBD5,0xE393,0xDB11,0xEC37,0xFD1B,0xFD1B,0xFD1B,0xE352,0x48C5,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8969,0xEBB4,0xFCD9,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xEB93,0xD26E,0xC1EB,0xE3D5,0xFD1B,0xFD1B,0xFD1B,0xE352,0x48C5,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8969,0xEB93,0xF4B9,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xF457,0xF499,0xF457,0xF499,0xFD1B,0xFD1B,0xFCFA,0xE332,0x48C5,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x8127,0xE2F1,0xF457,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCD9,0xFCFA,0xFCD9,0xFCFA,0xFD1B,0xFD1B,0xFCB9,0xDAD0,0x48A4,0x2A09,0x4B4E,
//  0x4B4E,0x1925,0x6083,0xCA4D,0xF3D5,0xF498,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCFA,0xF436,0xB1EB,0x3042,0x2A09,0x4B4E,
//  0x4B4E,0x29C7,0x3082,0x9948,0xE2F1,0xF3F5,0xF498,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCFA,0xF477,0xE2F1,0x7905,0x20C3,0x326A,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x6083,0xCA4D,0xEB53,0xF3F5,0xF498,0xFCDA,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCFA,0xFCDA,0xF477,0xF3D5,0xDA6E,0x48A4,0x2A09,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x29C7,0x3082,0x9948,0xE2F1,0xF394,0xF3F5,0xF436,0xF498,0xFCDA,0xFCFA,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFD1B,0xFCFA,0xFCDA,0xF477,0xF436,0xF3D5,0xEB52,0xA189,0x38E4,0x326A,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x6083,0xCA4D,0xEB53,0xF394,0xF394,0xF3F5,0xF436,0xF478,0xF4B9,0xFCDA,0xFCDA,0xFCDA,0xFCDA,0xFCDA,0xFCDA,0xFCDA,0xF4B9,0xF498,0xF457,0xF436,0xF3D5,0xF394,0xEB52,0xC22D,0x40A3,0x2A09,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x1925,0x5022,0xB149,0xDAAF,0xEB53,0xF394,0xF394,0xF394,0xF394,0xF3F5,0xF436,0xF436,0xF436,0xF436,0xF436,0xF436,0xF436,0xF3D5,0xF394,0xF394,0xF394,0xF394,0xEB52,0xD24E,0x70E5,0x20C3,0x326A,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x3A8B,0x20E3,0x9827,0xC089,0xB149,0xCA4D,0xE2F1,0xEB53,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xEB52,0xE2D0,0xC20C,0x8906,0x38E4,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x4821,0xC88B,0xE06D,0xA047,0xA085,0xB148,0xCA4D,0xE2F1,0xEB12,0xEB53,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xF394,0xEB52,0xEB12,0xE2D0,0xC20C,0x8906,0x5062,0x20C3,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x7004,0xE1EF,0xF991,0xE06D,0xC849,0x9044,0x9885,0xB148,0xB1AA,0xCA4D,0xDAAF,0xDAAF,0xDAAF,0xDAAF,0xDAAF,0xDAAF,0xDAAF,0xD22D,0xB9CA,0x8906,0x5062,0x20C3,0x1104,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x8006,0xEB94,0xFA73,0xF8D0,0xF8D0,0xD04A,0xB806,0x8043,0x4861,0x6083,0x78A4,0x88C5,0xA0C6,0xB0A6,0xB0A6,0xB0A6,0xB0A6,0xB0A6,0x6863,0x20C3,0x1104,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x7805,0xE06D,0xF8D0,0xF8D0,0xF08E,0xD82B,0xB808,0x6843,0x20C3,0x1104,0x1104,0x3082,0x7823,0xA805,0xB806,0xB806,0xB005,0x8804,0x38A4,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x1925,0x6002,0xC069,0xE88D,0xE88E,0xD02A,0xB007,0x6843,0x20C3,0x326A,0x4B4E,0x4B4E,0x29C7,0x3082,0x7024,0x8806,0x8806,0x8004,0x38A4,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x29C7,0x3082,0x6022,0x8005,0x8806,0x8004,0x5022,0x20C3,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x29C7,0x1104,0x1104,0x1104,0x1104,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,
//  0x4B4E,0x4B4E,0x4B4E,0x29C7,0x1104,0x1104,0x1104,0x1104,0x1104,0x326A,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E,0x4B4E
//  };
//
//  const uint8_t FLOATING_X_OFFSET = 2;
//  const uint8_t FLOATING_WIDTH = 33;
//  const uint8_t FLOATING_HEIGHT = 32;
//
//  const uint16_t xPos = 10;
//  const uint16_t yPos = 110;

  struct Kirby kirby = {
		  10,		// uint16_t xPos
		  110,		// uint16_t yPos
		  0,		// uint8_t hasSwallowed
		  0,		// uint8_t isFloating
		  0,		// uint8_t enableUp
		  1,		// uint8_t enableX
		  RIGHT,	// enum Direction direction
		  IDLE,		// enum State state
		  0,		// uint8_t currentFrame
		  2500,		// uint16_t remainingTicks
		  IDLE,		// enum State previousState
		  1			// uint8_t enableStateChange
  };

  /**
   * Draw sprite parameters
   * uint8_t spriteWidth
   * uint8_t spriteHeight
   * uint8_t spriteOffset
   * uint16_t* frame
   * uint16_t xPos
   * uint16_t yPos
   */

//  for (int x = 0; x < FLOATING_WIDTH; x++) {
//	  uint16_t displayXPos = (xPos + FLOATING_X_OFFSET + x) % SCREEN_WIDTH;
//	  for (int y = FLOATING_HEIGHT - 1; y >= 0; y--) {
//		  uint16_t displayYPos = yPos + y;
//		  uint16_t color = KIRBY_FLOATING_SPRITE_0[y * FLOATING_WIDTH + x];
//		  if (color == 0x4B4E) {
//			  uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + FLOATING_X_OFFSET + x)];
//			  color = newColor;
//		  }
//		  LCD_DrawDot(displayXPos, displayYPos, color);
//	  }
//  }
//
//  for (int x = 0; x < FLOATING_WIDTH; x++) {
//	  uint16_t displayXPos = (xPos + FLOATING_X_OFFSET + FLOATING_WIDTH - x) % SCREEN_WIDTH;
//	  for (int y = FLOATING_HEIGHT - 1; y >= 0; y--) {
//		  uint16_t displayYPos = yPos + y;
//		  uint16_t color = KIRBY_FLOATING_SPRITE_0[y * FLOATING_WIDTH + x];
//		  if (color == 0x4B4E) {
//			  uint16_t newColor = platformImg[882 * (displayYPos) + (xPos + FLOATING_X_OFFSET + FLOATING_WIDTH - x)];
//			  color = newColor;
//		  }
//		  LCD_DrawDot(displayXPos, displayYPos, color);
//	  }
//  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /* Reads inputs */
	  inputLeft = HAL_GPIO_ReadPin(LEFT_GPIO_PORT, LEFT_GPIO_PIN);
	  inputRight = HAL_GPIO_ReadPin(RIGHT_GPIO_PORT, RIGHT_GPIO_PIN);
	  inputA = HAL_GPIO_ReadPin(A_BUTTON_GPIO_PORT, A_BUTTON_GPIO_PIN);
	  inputB = HAL_GPIO_ReadPin(B_BUTTON_GPIO_PORT, B_BUTTON_GPIO_PIN);

	  /* Updates state based on input */
	  Kirby_updateState(&kirby, inputB, inputA, inputLeft, inputRight);

	  /* Checks X displacement */
	  if (inputLeft) {
		  Kirby_moveX(&kirby, LEFT);
	  } else if (inputRight) {
		  Kirby_moveX(&kirby, RIGHT);
	  }

	  /* Checks Y displacement */
	  Kirby_moveY(&kirby);

	  /* Renders sprite */
	  Kirby_renderSprite(&kirby);

	  // Displays xPos on LCD
//	  LCD_Clear(10, 210, 100, 20, 0xFFFF);
//	  char buffer[6];
//	  sprintf(buffer, "%d", kirby.xPos);
//	  LCD_DrawString(10, 210, buffer);
//
//	  key1State = HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
//	  key2State = HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
//	  if (key1State) {
//		  /* Walking left */
//		  if (state != 2) {
//			  state = 2;
//			  currentFrame = 0;
//			  direction = 1;
//		  }
//		  Kirby_redrawColumnAfter(&kirby);
//		  if (kirby.xPos > 0) {
//			  if (kirby.xPos > 160 && kirby.xPos < 696) {
//				  Kirby_movePlatform(kirby.xPos);
//			  }
//			  kirby.xPos--;
//		  }
//		  Kirby_displayKirbyFacingLeft(kirby.walkingKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//		  currentFrame = (currentFrame + 1) % 10;
//		  HAL_Delay(20);
//	  } else if (key2State) {
//		  /* Walking right */
//		  // Set state to walking, reset frame
//		  if (state != 1) {
//			  state = 1;
//			  currentFrame = 0;
//			  direction = 0;
//		  }
//		  Kirby_redrawColumnBefore(&kirby);
//		  if (kirby.xPos < 829) {
//			  if (kirby.xPos > 160 && kirby.xPos < 696) {
//				  Kirby_movePlatform(kirby.xPos);
//			  }
//			  kirby.xPos++;
//		  }
//		  Kirby_displayKirbyFacingRight(kirby.walkingKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//		  currentFrame = (currentFrame + 1) % 10;
//		  HAL_Delay(20);
//	  } else {
//		  /* Idle */
//		  // Set state to idle, reset frame
//		  if (state != 0) {
//			  state = 0;
//			  currentFrame = 0;
//		  }
//		  if (direction == 0) {
//			  // right
//			  Kirby_displayKirbyFacingRight(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//		  } else {
//			  // left
//			  Kirby_displayKirbyFacingLeft(kirby.idleKeyframes[currentFrame], kirby.xPos, kirby.yPos);
//		  }
//		  currentFrame = (currentFrame + 1) % 3;
//		  HAL_Delay(50);
//		  if (currentFrame == 1) {
//			  //HAL_Delay(2500); // Stay longer at the 0th frame
//
//			  uint32_t tickstart = HAL_GetTick();
//			  uint32_t wait = 2500;
//
//			  /* Add a freq to guarantee minimum wait */
//			  if (wait < HAL_MAX_DELAY)
//			  {
//				  wait += (uint32_t)(uwTickFreq);
//			  }
//
//			  while ((HAL_GetTick() - tickstart) < wait)
//			  {
//				  key1State = HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
//				  key2State = HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
//				  if (key2State || key1State) {
//					  break;
//				  }
//			  }
//		  }
//	  }
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
  __HAL_RCC_GPIOB_CLK_ENABLE();
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

  /*Configure GPIO pins : A_Button_Pin B_Button_Pin */
  GPIO_InitStruct.Pin = A_Button_Pin|B_Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
