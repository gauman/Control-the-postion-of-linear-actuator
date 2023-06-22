/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "system_file.h"         // clock configuration header file



# define sw_extend GPIO_PIN_5   // Push button used to activate extend in the actuator.
# define sw_shrink GPIO_PIN_6   // Push button used to activate shrink in  the actuator.
# define sw_stop GPIO_PIN_4     // Push button used to stop the actuator from extend and shrink.
#define  set_home GPIO_PIN_7    // Push button used to set the homing function to the actuator.

# define out_extend GPIO_PIN_8   //output given to the actuator to extend.
# define out_shrink GPIO_PIN_9   //output given to the actuator to shrink.




ADC_HandleTypeDef  hadc1;       // initialize the ADC to get position feedback from PA-14P actuator



uint16_t current_location = 0; // store the current position from the actuator

//STM32 has 12 bit ADC 2^12 = range (0 - 4095).
//Setting the mid range reduce the jitter.


const uint16_t lower_limit = 2042; // set the mid band lower range of 2042
const uint16_t upper_limit = 2052; // set the mid band upper range of 2052.



void SystemClock_Config(void);  // declaration of system clock function
static void MX_GPIO_Init(void); // declaration of GPIO pin function.
static void MX_ADC1_Init(void); // declaration of ADC function to get analog position signal from PA-14P

void extend(void);              // declaration of extend function.
void shrink(void);              // declaration of shrink function.
void stop(void);                //declaration of stop function to prevent extension of shrinking.
void homing(void);              // declaration of homing function to set the actuator in middle of 2 end points.



int main(void)
{
  HAL_Init();             // Reset of all peripherals, Initializes the Flash interface and the Systick.

  SystemClock_Config();    // Configure the system clock
  MX_GPIO_Init();          // Initialize all configured peripherals
  MX_ADC1_Init();

  /* Infinite super loop */

  while (1)
  {


	  if(HAL_GPIO_ReadPin(GPIOB, sw_extend)){   //check the sw_extend push button to activate extension function.

		  extend();                             // extend function

	  } else if(HAL_GPIO_ReadPin(GPIOB, sw_shrink)){ //check the sw_shrink push button to activate shrink function.

		  shrink();                                   // shrink function

	  } else if(HAL_GPIO_ReadPin(GPIOB, sw_stop)){  //check the sw_stop push button to activate stop function.

		  stop();                                     // stop funtion

	  }else if(HAL_GPIO_ReadPin(GPIOB, set_home)){  //check the set_home push button to activate homing function.


		  homing();                                   // homing function.

	  }


  }                                                 // end of while loop


}                                                    // end of main






void extend(void){                                      // extend function to expand the actuator.

	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_SET);   // set the extend pin high.
	HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_RESET); // set the shrink pin low.

	HAL_Delay(100);                                       // delay is added to avoid jitter.
}



void shrink(void){                                       // shrink function to shrink the actuator.

	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_RESET);  // set the extend pin low.
	HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_SET);    // set the shrink pin high.

	HAL_Delay(100);                                        // delay is added to avoid jitter.

}


void stop(void){                                          // stop function to stop actuator from extend and shrink.

	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_RESET);  // set the extend pin low.
	HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_RESET);  // set the shrink pin low.

	HAL_Delay(100);                                         // delay is added to avoid jitter.

}



void homing(void){    // homing function is used to set actuator in middle by continuously extending and shrink .


while(1){      // while is used to continuously extend and shrink until it reach the middle point and exits from loop via break.

if(current_location>=lower_limit && current_location<=upper_limit){ // check if position of actuator is in middle range.
	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_RESET);    // stop the actuator from extension
	HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_RESET);   //  stop actuator from shrinking.
	break;                                                  // exit the loop after the actuator is in middle position.
}

else if(current_location <= lower_limit){     // check it current location is lesser than lower middle range.

	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_SET);    // set the extend pin high to reach lower middle range.
    HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_RESET);  // set the shrink pin low.

}

else if(current_location >= upper_limit){     // check it current location is higher than upper middle range.

	HAL_GPIO_WritePin(GPIOA, out_extend, GPIO_PIN_RESET);  // set the extent pin low.
    HAL_GPIO_WritePin(GPIOA, out_shrink, GPIO_PIN_SET);   // set the shrink pin high to reach upper middle range.

}


HAL_Delay(100);                            // delay is added to avoid jitter.

}                                          // end while loop

}                                          // end of function







// initialization code to set ADC

static void MX_ADC1_Init(void)
{

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

 //Configure Regular Channel

  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

// GPIO Initialization Function

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  //Configure GPIO pins : PB4 PB5 PB6 PB7
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //Configure GPIO pins : PB8 PB9 //
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}




