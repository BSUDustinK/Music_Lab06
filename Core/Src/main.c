/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c  LAB6
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_host.h"
#include "seg7.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
int DelayValue = 50;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
//void Play_Note(int note,int size,int tempo,int space);
//extern void Seven_Segment_Digit (unsigned char digit, unsigned char hex_char, unsigned char dot);
//extern void Seven_Segment(unsigned int HexValue);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char ramp = 0;
char RED_BRT = 0;
char GREEN_BRT = 0;
char BLUE_BRT = 0;
char RED_STEP = 1;
char GREEN_STEP = 2;
char BLUE_STEP = 3;
char DIM_Enable = 0;
char Music_ON = 0;
int TONE = 0;
int COUNT = 0;
int INDEX = 0;
int Note = 0;
int Save_Note = 0;
int Vibrato_Depth = 1;
int Vibrato_Rate = 40;
int Vibrato_Count = 0;
char Animate_On = 0;
char Message_Length = 0;
char *Message_Pointer;
char *Save_Pointer;
int Delay_msec = 0;
int Delay_counter = 0;

/* HELLO ECE-330L */

  //Modified For Lab 06 THROUGH THE FIRE AND THE FLAMES
char Message[] =
		{SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,
		CHAR_T,CHAR_H,CHAR_R,CHAR_O,CHAR_U,CHAR_G,CHAR_H,
		SPACE,CHAR_T,CHAR_H,CHAR_E,SPACE,CHAR_F,CHAR_I,CHAR_R,CHAR_E,SPACE,
		CHAR_A,CHAR_N,CHAR_D,SPACE,
		CHAR_T,CHAR_H,CHAR_E,SPACE,
		CHAR_F,CHAR_L,CHAR_A,CHAR_M,CHAR_E,CHAR_S,
		SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE,SPACE};

/* Declare array for Song */
Music Song[500]; //Song[INDEX]

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
  //MX_I2C1_Init();
  //MX_I2S3_Init();
  //MX_SPI1_Init();
  //MX_USB_HOST_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

  /*** Configure GPIOs ***/
  GPIOD->MODER = 0x55555555; // set all Port D pins to outputs
  GPIOA->MODER |= 0x000000FF; // Port A mode register - make A0 to A3 analog pins
  GPIOE->MODER |= 0x55555555; // Port E mode register - make E8 to E15 outputs
  GPIOC->MODER |= 0x0; // Port C mode register - all inputs
  GPIOE->ODR = 0xFFFF; // Set all Port E pins high

  /*** Configure ADC1 ***/
  RCC->APB2ENR |= 1<<8;  // Turn on ADC1 clock by forcing bit 8 to 1 while keeping other bits unchanged
  ADC1->SMPR2 |= 1; // 15 clock cycles per sample
  ADC1->CR2 |= 1;        // Turn on ADC1 by forcing bit 0 to 1 while keeping other bits unchanged

  /*****************************************************************************************************
  These commands are handled as part of the MX_TIM7_Init() function and don't need to be enabled
  RCC->AHB1ENR |= 1<<5; // Enable clock for timer 7
  __enable_irq(); // Enable interrupts
  NVIC_EnableIRQ(TIM7_IRQn); // Enable Timer 7 Interrupt in the NVIC controller
  *******************************************************************************************************/

  TIM7->PSC = 199; //250Khz timer clock prescaler value, 250Khz = 50Mhz / 200
  TIM7->ARR = 1; // Count to 1 then generate interrupt (divide by 2), 125Khz interrupt rate to increment byte counter for 78Hz PWM
  TIM7->DIER |= 1; // Enable timer 7 interrupt
  TIM7->CR1 |= 1; // Enable timer counting

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  /* Jeopardy Song

   */
  int noteCount = 0;

  int tempoForSong = 1200; 

 Song[noteCount].note =rest; 
 Song[noteCount].size =whole; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;

 Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;

 Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
   
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D4; 
 Song[noteCount].size =_8th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =B4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As3_Bb3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As4_Bb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =B4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =B4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =B4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As4_Bb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =D5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =As4_Bb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds4_Eb4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C3; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs2_Ab2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F2; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs4_Ab4; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =F5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Ds5_Eb5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =C5; 
 Song[noteCount].size =_16th; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =G5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 
Song[noteCount].note =Gs5_Ab5; 
 Song[noteCount].size =_32nd; 
 Song[noteCount].tempo = tempoForSong; 
 Song[noteCount].space = 10; 
 Song[noteCount].end = 0; 
 noteCount++;
 


//Verse 1A

 Song[noteCount].note = E5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = A5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = half;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;


 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;


 Song[noteCount].note = D5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = D5;
 Song[noteCount].size = half;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = rest;    //I use this to differenetiate the dot after the note
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = rest;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = A5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = G5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = F5;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

 Song[noteCount].note = D5;
 Song[noteCount].size = _8th;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;
 Song[noteCount].note = D5;
 Song[noteCount].size = half;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;

//Verse 1B
 Song[noteCount].note = rest;
 Song[noteCount].size = quarter;
 Song[noteCount].tempo = tempoForSong;
 Song[noteCount].space = 10;
 Song[noteCount].end = 0;
 noteCount++;


















  Save_Note = Song[0].note;  // Needed for vibrato effect
  INDEX = 0;




  while (1)
  {
	  int i,j;

	  Message_Pointer = &Message[0];
	  Save_Pointer = &Message[0];
	  Message_Length = sizeof(Message)/sizeof(Message[0]);
	  Delay_msec = 200;
	  Animate_On = 1;




	  while ((GPIOC->IDR & 1 << 10))
	  {
	  for (i=0;i<16;i++)
	  {
	  GPIOD->ODR |= 1 << i;
	  if (i>0)GPIOD->ODR &= ~(1 << (i-1));
	  HAL_Delay(50);
	  }
	  for (i=15;i>=0;i--)
	  	  {
		  GPIOD->ODR |= 1 << i;
	  	  if (i<15)GPIOD->ODR &= ~(1 << (i+1));
	  	  HAL_Delay(50);
	  	  }

	  }
	  INDEX = 0;
	  Music_ON = 1;


	  DIM_Enable = 1;
	  // Green
	  GREEN_BRT = 255;
	  BLUE_BRT = 0;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // Blue
	  GREEN_BRT = 0;
	  BLUE_BRT = 255;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // Red
	  GREEN_BRT = 0;
	  BLUE_BRT = 0;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Magenta
	  GREEN_BRT = 0;
	  BLUE_BRT = 255;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Yellow
	  GREEN_BRT = 255;
	  BLUE_BRT = 0;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  // Cyan
	  GREEN_BRT = 255;
	  BLUE_BRT = 255;
	  RED_BRT = 0;
	  HAL_Delay(1000);

	  // White
	  GREEN_BRT = 255;
	  BLUE_BRT = 255;
	  RED_BRT = 255;
	  HAL_Delay(1000);

	  DIM_Enable = 0;

	  HAL_Delay(20000);  // Delay to allow song to finish




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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */


/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 0;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; 
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                           Audio_RST_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin
                          |Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

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
