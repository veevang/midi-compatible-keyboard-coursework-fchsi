/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "typedef.h"
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
 UART_HandleTypeDef hlpuart1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim8;

/* USER CODE BEGIN PV */
enum Mode mode = PIANO;
enum Song song_to_play = NO_SONG;

_Bool key_status[7][8] = {{1}};
int key_status_time_index = 0;
_Bool debounced_key_status[7] = {1};
_Bool note_off[7] = {1};

// songs
const enum Tune note_tune[3][512]={{
		RE, RE, RE, MI, MI, SOL, SOL, DO, DO, DO, DO, DO, DO, DO, SOL, LA, DO, RE, RE, RE, MI, MI, SOL, SOL, SOL, SOL, FA, MI, FA, FA, FA, FA, MI,
		RE, RE, RE, MI, MI, SOL, SOL, DO, DO, DO, DO, DO, DO, DO, SOL, LA, DO, RE, RE, RE, MI, MI, SOL, SOL, SOL, SOL, FA, MI, FA, FA,
		REST, REST, FA, FA, MI, RE, DO, RE, MI, MI, REST, REST, REST, FA, FA, MI, RE, DO, RE, DO, DO, REST, DO, MI, SOL,
		SOL, DO, DO, LA, SOL, SOL, MI, SOL, DO, DO, LA, SOL, SOL, MI, MI, RE, RE, LA, SOL, SOL, MI, MI, RE, RE, REST,
		REST, REST, FA, FA, MI, RE, DO, RE, MI, MI, REST, REST, REST, FA, FA, MI, RE, DO, RE, DO, DO, REST, DO, MI, SI,
		SI, DO, DO, DO, RE, DO, SI, SI, DO, DO, DO, RE, DO, RE, MI, RE, DO, DO, RE, DO, RE, MI, FA, MI, RE, RE, SOL, SOL, RE,
		RE, DO, DO, DO, MI, MI, RE, SI, LA, LA, SOL, SOL, SOL, REST, SOL, SOL, DO, DO, FA, DO, DO, SI, DO, RE, RE, MI, MI, MI, REST, SOL, SOL, RE,
		RE, DO, DO, DO, MI, MI, RE, SI, LA, LA, SOL, SOL, MI, RE, RE, DO, REST, SOL, SOL, LA, DO, DO, DO, FA, MI, RE, DO, DO, DO, REST
},{
		SOL, LA, SOL,
		SOL, MI, RE, DO, SOL, MI, MI, REST, REST, REST, MI, MI, MI, LA, LA, DO, MI,
		REST, REST, REST, LA, MI, RE, REST, DO, LA, REST, MI, RE, REST, RE, MI, SOL,
		REST, SOL, SOL, MI, REST, SOL, SOL, MI, REST, RE, DO, MI,
		REST, REST, REST, REST, SOL, SOL, RE, MI, RE, MI, MI,
		REST, REST, REST, REST, RE, MI, MI, MI, LA, DO, RE, DO, DO,
		REST, REST, REST, LA, FA, RE, MI, LA, REST, SOL, DO, RE, RE, REST, REST, REST, SOL, SOL, MI, MI, REST, SOL, SOL, REST, MI, RE, MI,
		REST, REST, REST, REST, MI, LA, REST, DO, LA, LA, REST, REST, LA, SOL, SI, MI, REST, REST, SOL, MI, MI,
		REST, REST, REST, REST, DO, MI, RE, DO, DO, DO, DO, LA, LA, DO, SI, SOL, MI, SOL, MI, RE, MI,
		REST, REST, SOL, LA, RE, RE, RE, DO, RE,
		REST, REST, REST, DO, SOL, REST, MI, RE, MI, SOL, SOL,
		REST, REST, SOL, DO, RE, RE, SOL, RE, RE, REST, SOL, MI, RE, MI, DO,
		REST, REST, REST, REST, REST, REST, REST, MI, RE,
		REST, MI, LA, REST, DO, LA, LA, REST, REST, LA, SOL, MI, SOL, MI, SOL, MI, MI,
		REST, REST, REST, REST, DO, MI, RE, DO, DO, DO, DO, LA, LA, DO, SI, SOL, MI, SOL, MI, RE, MI,
		REST, REST, SOL, LA, RE, RE, RE, DO, RE,
		REST, REST, REST, DO, SOL, REST, MI, RE, MI, SOL, SOL,
		REST, REST, SOL, DO, RE, RE, SOL, RE, RE, REST, SOL, MI, RE, MI, DO,
		REST, REST, REST, REST, REST, REST, SOL, DO, RE, RE, SOL, RE, RE, REST, SOL, MI, RE, MI, DO
},{
		DO, DO, MI, SOL, SOL, LA, DO, LA, SOL, SOL, MI, MI, SOL, MI, DO, LA, DO, RE, SOL,
		LA, LA, LA, DO, SOL, MI, RE, MI, RE, DO, RE, SOL, SOL, FA, SOL, LA, LA, SI, LA, SOL,
		DO, DO, LA, DO, SOL, LA, SOL, LA, LA, SOL, MI, RE, RE, MI, SOL,
		DO, DO, DO, MI, RE, MI, RE, DO, LA, LA, SOL, MI, RE, MI, RE, DO,
		DO, DO, REST, LA, LA, REST, SOL, SOL, LA, SOL, RE, MI, SOL,
		DO, DO, REST, LA, LA, REST, SOL, SOL, LA, SOL, RE, MI, RE, DO
}};

const enum Rhythm note_rhythm[3][512]={{
		EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF,
		QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF, QUARTER, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		DOTTED_EIGHTH, SIXTEENTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_EIGHTH, SIXTEENTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_EIGHTH, SIXTEENTH, HALF, QUARTER,
		QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF, QUARTER, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_EIGHTH, SIXTEENTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, SIXTEENTH, SIXTEENTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, SIXTEENTH, SIXTEENTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, SIXTEENTH, DOTTED_EIGHTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, DOTTED_EIGHTH, SIXTEENTH, EIGHTH, EIGHTH, EIGHTH, SIXTEENTH, SIXTEENTH, HALF, HALF
},{
		EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER,
		EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH,
		EIGHTH, DOTTED_QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, QUARTER,
		EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, QUARTER,
		EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_QUARTER,
		QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, HALF,
		EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, DOTTED_QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, QUARTER,
		EIGHTH, QUARTER, QUARTER, EIGHTH, SIXTEENTH, SIXTEENTH, DOTTED_QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, HALF, EIGHTH, EIGHTH, EIGHTH, QUARTER,
		EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, DOTTED_QUARTER, QUARTER, EIGHTH, DOTTED_QUARTER,
		EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
		QUARTER, EIGHTH, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH,
		QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, QUARTER, QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, EIGHTH, DOTTED_QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, EIGHTH, SIXTEENTH, SIXTEENTH, DOTTED_QUARTER, EIGHTH, EIGHTH, EIGHTH, EIGHTH, EIGHTH, HALF, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, HALF, EIGHTH, EIGHTH, EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, QUARTER,
		EIGHTH, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH,
		QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH, QUARTER, EIGHTH, EIGHTH, DOTTED_EIGHTH, DOTTED_EIGHTH, EIGHTH
},{
		QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, EIGHTH, EIGHTH, HALF,
		QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, QUARTER,
		DOTTED_QUARTER, EIGHTH, QUARTER, QUARTER, DOTTED_QUARTER, EIGHTH, HALF, DOTTED_QUARTER, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, HALF,
		DOTTED_QUARTER, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, HALF, DOTTED_QUARTER, EIGHTH, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, HALF,
		QUARTER, HALF, QUARTER, QUARTER, HALF, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, HALF,
		QUARTER, HALF, QUARTER, QUARTER, HALF, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, EIGHTH, EIGHTH, HALF
}};

const int note_octave[3][512]={{
		0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0,	0, 0, -1, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0,	0, 0, 0, 0,	0, 0, 0, 0, 0, 0, -1, -1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
},{
		-1, -1, -1,
		0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1, 0, 0,
		0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
		0, 0, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0
},{
		0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0,
		0, 0, 0, 1, 0, 	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
}};
const int length[3] = {229, 277, 97};
const int bpm[3] = {67,82,90};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM8_Init(void);
/* USER CODE BEGIN PFP */
void set_tune (enum Tune t, int octave);
void play_song(enum Song song, const int bpm, const int length, const enum Tune* note_tune, const enum Rhythm* note_rhythm, const int* note_ocative);
void display(char c);
void control_midi(int command, enum Tune tune, int octave);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  mode = PIANO;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim8);// enable it
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  switch(mode){
	  case MIDI:
		  for(int i = 0; i < 7; i++){
			  if (note_off[i] != debounced_key_status[i]){// modify note status
				  if (debounced_key_status[i]){// send note off signal
					  control_midi(MIDI_NOTE_OFF, i + DO, 0);
					  note_off[i] = 1;
				  }
				  else{// send note on signal
					  control_midi(MIDI_NOTE_ON, i + DO, 0);
					  note_off[i] = 0;
				  }
			  }
		  }
		  break;
	  case PIANO:
		  break;
	  case MUSIC:
		  switch(song_to_play){
		  case NO_SONG:
			  break;
		  case SONG_1:
			  display('1');
			  play_song(SONG_1, bpm[0], length[0], note_tune[0], note_rhythm[0], note_octave[0]);
			  display('^');
			  break;
		  case SONG_2:	// song2
			  display('2');
			  play_song(SONG_2, bpm[1], length[1], note_tune[1], note_rhythm[1], note_octave[1]);
			  display('^');
			  break;
		  case SONG_3:	// song3
			  display('3');
			  play_song(SONG_3, bpm[2], length[2], note_tune[2], note_rhythm[2], note_octave[2]);
			  display('^');
			  break;
		  }
		  break;
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 31250;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV256;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

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

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 170 - 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2272;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 2200;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 170-1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 1999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */

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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_10, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 PA8 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC5 PC6 PC7 PC8
                           PC9 PC10 PC11 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB3 PB4 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void  HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    // TIM INTERRUPT
{
	if(htim->Instance==TIM8)   // IF INTERRUPT GENERATED BY TIM8
	{
		// key_status_time_index
		key_status[0][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5);
		key_status[1][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
		key_status[2][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7);
		key_status[3][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8);
		key_status[4][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9);
		key_status[5][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
		key_status[6][key_status_time_index] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
		key_status_time_index++;
		key_status_time_index %= 8;	// key_status_time_index in [0, 7)
		for (int i = 0; i < 7; i++){
			int pressed_count = 0;
			for (int j = 0; j < 8; j++){// 8 time (16ms) the same
				if(key_status[i][j] == 0){
					pressed_count++;
				}
			}
			if (pressed_count == 8){// 8 time (16ms) pressed
				debounced_key_status[i] = 0;
			}
			else if(pressed_count == 0){// 8 time (16ms) released
				debounced_key_status[i] = 1;
			}
		}
	}

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// 消抖,8ci
	int octave;
	enum Tune tune;
	int control_key = 0;
	char key;

	switch(mode){
	case PIANO:// play piano

		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) &&
					!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){// neither K8 nor K pressed!
			octave = 0;
		}
		else if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) &&
				!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){// K8 pressed! higher
			octave = 1;
		}
		else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) &&
				HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){// K pressed! lower
			octave = -1;
		}
		else{// K8 and K pressed! switch mode!
			mode = MUSIC;
			return;
		}

		switch(GPIO_Pin){
		case GPIO_PIN_5:tune = DO; key = '1'; break;
		case GPIO_PIN_6:tune = RE; key = '2'; break;
		case GPIO_PIN_7:tune = MI; key = '3'; break;
		case GPIO_PIN_8:tune = FA; key = '4'; break;
		case GPIO_PIN_9:tune = SOL;	key = '5'; break;
		case GPIO_PIN_10:tune = LA; key = '6'; break;
		case GPIO_PIN_11:tune = SI; key = '7'; break;
		default:control_key = 1; break;
		}
		if (!control_key){
			if(!HAL_GPIO_ReadPin(GPIOC, GPIO_Pin)){// 低电平表示按下！
				set_tune(tune, octave);
				HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
				display(key);
			}
			else{
				HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
				display('^');
			}
		}
		break;
	case MUSIC:
		if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) &&
				HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){// K and K8 pressed!
			mode = MIDI;
			return;
		}
		// play song
		switch(GPIO_Pin){
		case GPIO_PIN_5:	// song1
			if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5)){// 按下
				song_to_play = SONG_1;
			}
			break;
		case GPIO_PIN_6:	// song2
			if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6)){// 按下
				song_to_play = SONG_2;
			}
			break;
		case GPIO_PIN_7:	// song3
			if (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7)){
				song_to_play = SONG_3;
			}
			break;
		default:	// STOP PLAY
			song_to_play = NO_SONG;
			break;
		}
		break;
	case MIDI:
		if(!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12) &&
				HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)){// K and K8 pressed!
			mode = PIANO;
			return;
		}
		break;
	}

}

void set_tune(enum Tune t, int octave){
	if (t == REST){// DON'T PLAY ANYTHING
		TIM1->ARR = 2272;
		TIM1->CCR1 = 2273;
		return;
	}
	switch (octave){
	case 0:// BEGIN WITH 440Hz
		if (t == DO){
			TIM1->ARR=3822;
			TIM1->CCR1=3600;
		}
		else if (t == RE){
			TIM1->ARR=3404;
			TIM1->CCR1=3233;
		}
		else if (t == MI){
			TIM1->ARR=3033;
			TIM1->CCR1=2881;
		}
		else if (t == FA){
			TIM1->ARR=2863;
			TIM1->CCR1=2719;
		}
		else if (t == SOL){
			TIM1->ARR=2550;
			TIM1->CCR1=2422;
		}
		else if (t == LA){
			TIM1->ARR=2272;
			TIM1->CCR1=2023;
		}
		else if (t == SI){
			TIM1->ARR=2024;
			TIM1->CCR1=1923;
		}
		break;
	case -1:// LOW OCTAVE
		if (t == DO){
			TIM1->ARR=7644;
			TIM1->CCR1=7262;
		}
		else if (t == RE){
			TIM1->ARR=6809;
			TIM1->CCR1=6468;
		}
		else if (t == MI){
			TIM1->ARR=6067;
			TIM1->CCR1=5763;
		}
		else if (t == FA){
			TIM1->ARR=5726;
			TIM1->CCR1=5440;
		}
		else if (t == SOL){
			TIM1->ARR=5101;
			TIM1->CCR1=4845;
		}
		else if (t == LA){
			TIM1->ARR=4544;
			TIM1->CCR1=4317;
		}
		else if (t == SI){
			TIM1->ARR=4048;
			TIM1->CCR1=3845;
		}
		break;
	case 1:// HIGH OCTAVE
		if (t == DO){
			TIM1->ARR=1910;
			TIM1->CCR1=1814;
		}
		else if (t == RE){
			TIM1->ARR=1701;
			TIM1->CCR1=1616;
		}
		else if (t == MI){
			TIM1->ARR=1516;
			TIM1->CCR1=1440;
		}
		else if (t == FA){
			TIM1->ARR=1431;
			TIM1->CCR1=1359;
		}
		else if (t == SOL){
			TIM1->ARR=1275;
			TIM1->CCR1=1210;
		}
		else if (t == LA){
			TIM1->ARR=1135;
			TIM1->CCR1=1078;
		}
		else if (t == SI){
			TIM1->ARR=1011;
			TIM1->CCR1=960;
		}
		break;
	}
}

void control_midi(int command, enum Tune t, int octave){
	unsigned char frame[3];
	switch(command){// only single note at a time
	case MIDI_NOTE_OFF:
		frame[0] = 0x80;
		break;
	case MIDI_NOTE_ON:
		frame[0] = 0x90;
		break;
	}

	if (t == DO){
		frame[1] = 60;
	}
	else if (t == RE){
		frame[1] = 62;
	}
	else if (t == MI){
		frame[1] = 64;
	}
	else if (t == FA){
		frame[1] = 65;
	}
	else if (t == SOL){
		frame[1] = 67;
	}
	else if (t == LA){
		frame[1] = 69;
	}
	else if (t == SI){
		frame[1] = 71;
	}
	frame[2] = 0x7f;

	for (int i = 0; i < 3;i++){
		HAL_UART_Transmit(&hlpuart1, &frame[i], 1, 0xffff);
	}
}

void play_song(enum Song song, const int bpm, const int length, const enum Tune* note_tune, const enum Rhythm* note_rhythm, const int* note_ocative){
	long long sixteenth_time = 60*1000/4/bpm;// time of sixteenth note
	for (long long i = 0; i < length; i++){
		if (song_to_play != song){
			return;
		}
		// 考虑节奏
		long long time = 0;	//millisecond
		if (note_rhythm[i] == WHOLE){
			time = 16*sixteenth_time;
		}
		else if(note_rhythm[i] == HALF){
			time = 8*sixteenth_time;
		}
		else if (note_rhythm[i] == DOTTED_QUARTER){
			time = 6*sixteenth_time;
		}
		else if(note_rhythm[i] == QUARTER){
			time = 4*sixteenth_time;
		}
		else if(note_rhythm[i] == DOTTED_EIGHTH){
			time = 3*sixteenth_time;
		}
		else if(note_rhythm[i] == EIGHTH){
			time = 2*sixteenth_time;
		}
		else if(note_rhythm[i] == SIXTEENTH){
			time = 1*sixteenth_time;
		}
		set_tune(note_tune[i], note_ocative[i]);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_Delay(time - 50);	// 防止溢出�??????????
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		HAL_Delay(50);
	}
	// play complete
	song_to_play = NO_SONG;
}

void display(char c){
	switch(c){
	case '^':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		break;
	case '0':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		break;
	case '1':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '2':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '3':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '4':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '5':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '6':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	case '7':
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
		break;
	}
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
