/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
BaseType_t musicTaskbase;
TaskHandle_t* musictaskhander;
UBaseType_t pri=osPriorityNormal;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for LEDTask */
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for my_UARTTask */
osThreadId_t my_UARTTaskHandle;
const osThreadAttr_t my_UARTTask_attributes = {
  .name = "my_UARTTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for my_screenTask */
osThreadId_t my_screenTaskHandle;
const osThreadAttr_t my_screenTask_attributes = {
  .name = "my_screenTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ldle_Task */
osThreadId_t ldle_TaskHandle;
const osThreadAttr_t ldle_Task_attributes = {
  .name = "ldle_Task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void musicTask(void);

/* USER CODE END FunctionPrototypes */

void StartLEDTask(void *argument);
void Start_UARTTask(void *argument);
void Start_screenTask(void *argument);
void Startldle_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of LEDTask */
  LEDTaskHandle = osThreadNew(StartLEDTask, NULL, &LEDTask_attributes);

  /* creation of my_UARTTask */
  my_UARTTaskHandle = osThreadNew(Start_UARTTask, NULL, &my_UARTTask_attributes);

  /* creation of my_screenTask */
  my_screenTaskHandle = osThreadNew(Start_screenTask, NULL, &my_screenTask_attributes);

  /* creation of ldle_Task */
  ldle_TaskHandle = osThreadNew(Startldle_Task, NULL, &ldle_Task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  musicTaskbase= xTaskCreate(musicTask,"music",128*4,NULL,pri,musictaskhander);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartLEDTask */
/**
  * @brief  Function implementing the LEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void *argument)
{
  /* USER CODE BEGIN StartLEDTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port,YELLOW_LED_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(red_LED_GPIO_Port,red_LED_Pin,GPIO_PIN_RESET);
    osDelay(500);
    HAL_GPIO_WritePin(YELLOW_LED_GPIO_Port,YELLOW_LED_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(red_LED_GPIO_Port,red_LED_Pin,GPIO_PIN_SET);
    osDelay(500);
  }
  /* USER CODE END StartLEDTask */
}

/* USER CODE BEGIN Header_Start_UARTTask */
/**
* @brief Function implementing the my_UARTTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_UARTTask */
void Start_UARTTask(void *argument)
{
  /* USER CODE BEGIN Start_UARTTask */
  uint8_t  massage[]="helloworld\r\n";
  extern uint8_t receiveData[2];
  HAL_UART_Receive_IT(&huart3,receiveData,2);
  /* Infinite loop */
  for(;;)
  {
   // HAL_UART_Transmit(&huart3, massage, strlen(massage), 100);
   osDelay(10);
  }
  /* USER CODE END Start_UARTTask */
}

/* USER CODE BEGIN Header_Start_screenTask */
/**
* @brief Function implementing the my_screenTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_screenTask */
void Start_screenTask(void *argument)
{
  /* USER CODE BEGIN Start_screenTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_screenTask */
}

/* USER CODE BEGIN Header_Startldle_Task */
/**
* @brief Function implementing the ldle_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Startldle_Task */
void Startldle_Task(void *argument)
{
  /* USER CODE BEGIN Startldle_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Startldle_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void musicTask(void)
{
  for(;;)
  {
    osDelay(1);
  }
}

/* USER CODE END Application */

