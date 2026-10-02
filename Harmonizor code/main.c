/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "fonts.h"

#include "cir_buf.h"

#include "yin_pitch_detect.h"

#include "filter.h"

#include "harmonic_gen.h"

#include "map_key_midi.h"
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

/* USER CODE BEGIN PV */
//Key and midi
uint32_t state = 0;      //KEY CHANGE 0-11 (C to B)

//ADC DAC buffers
uint32_t adc_buf[N];
uint32_t dac_buf[N];

//cir_buf
uint16_t cir_buf[8192];

//Pitch
float freq;
//float last_freq;
//uint32_t error_count = 0;

//flags
volatile uint8_t pitch_detect_flag = 0;

//Knobs
float32_t harm_current;
float32_t diss_current;

uint32_t last_time_harm = 0;
uint32_t last_time_diss = 0;

//Harmonic Volume Control
float32_t altitude_target;
float32_t altitude_current;

//DEBUG TIME
uint32_t start_tim;
uint32_t fin_tim;
uint32_t used_tim;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C2_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
  //SCREEN HELLO
  SSD1306_Init (); // initialise the display
  SSD1306_GotoXY (10,10); // goto 10, 10
  SSD1306_Puts ("Yo", &Font_11x18, 1); // print Hello
  SSD1306_GotoXY (10, 30);
  SSD1306_Puts ("hello!", &Font_11x18, 1);
  SSD1306_UpdateScreen(); // update screen

  //INIT KEY ARRAY as C maj
  update_key(0);

  //INIT FILTER
  filter_init();

  //START ADC and DAC
  HAL_ADC_Start_DMA(&hadc1, adc_buf, N);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, dac_buf, N, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //PITCH DETECT LOOP
	  start_tim = HAL_GetTick(); //DEBUG

	  uint16_t sample_buf[2048];
//	  uint16_t filtered_buf[2048];
	  read_newest_cir_buf(sample_buf, 2048);
//	  apply_filter(sample_buf, filtered_buf, 2048);

//	  uint16_t decimated_buf[1024];
	  uint16_t decimated_buf[512];
//	  decimate(filtered_buf, decimated_buf, 1024, 2);

//	  decimate(sample_buf, decimated_buf, 1024, 2);   //Origin for decimate 2
//      YIN_Result_t result = YIN_DetectPitch(decimated_buf, 1024, SAMPLE_RATE/2);


	  decimate(sample_buf, decimated_buf, 512, 4); //test deci 4
	  YIN_Result_t result = YIN_DetectPitch(decimated_buf, 512, SAMPLE_RATE/4);

      freq = result.freq_hz;

      freq /=2; //IDK WHY lol

      //ADD freq protection prevent 40Hz cls noise
      if (freq < 81.0f || freq > 700.0f){
    	  freq = 0;
      }
//      if(last_freq != 0 && error_count <=4){
//          if(freq < last_freq/2 || freq > freq * 2){
//    	      freq = last_freq;
//    	      error_count ++;
//          }
//      }
//      else {error_count == 0;}
//      last_freq = freq;
      //    freq protection end

      uint8_t is_maj3 = is_it_maj3(freq);    // 1 or 0

      add_wave_to_buffer(freq, is_maj3);

      if(freq == 0){altitude_target = 0.0f; altitude_current = 0.0f;}
      else         {altitude_target = harm_current;}

	  fin_tim = HAL_GetTick(); //DEBUG
	  used_tim = fin_tim - start_tim; //DEBUG


	  //KNOB KNOB KNOB
	  if(HAL_GetTick() - last_time_harm >= 100){    //actually as the fre detect alr 115ms so these knobs are more than 100ms
	      HAL_ADC_Start(&hadc2);
	  	  if(HAL_ADC_PollForConversion(&hadc2, 10) == HAL_OK){

	  		  float32_t harm_knob = (float32_t)HAL_ADC_GetValue(&hadc2);
	  		  float32_t harm_target = (harm_knob/4096.0f);
	  		  smooth(harm_target, &harm_current, 0.2f);

	  	  	  last_time_harm = HAL_GetTick();
	  	  }
	  }
	  if(HAL_GetTick() - last_time_diss >= 100){
	  	  HAL_ADC_Start(&hadc3);
	  	  if(HAL_ADC_PollForConversion(&hadc3, 10) == HAL_OK){

	  		  float32_t diss_knob = (float32_t)HAL_ADC_GetValue(&hadc3);
	  		  float32_t diss_target = (diss_knob/4096.0f);
	  		  smooth(diss_target, &diss_current, 0.2f);

	  	  	  last_time_diss = HAL_GetTick();
	  	  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
#ifdef USE_FULL_ASSERT
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
