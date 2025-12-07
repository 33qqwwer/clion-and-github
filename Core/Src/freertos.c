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
#include "event_groups.h"
#include "string.h"
#include "font.h"
#include "usart.h"
#include "music.h"
#include "queue.h"
#include "tim.h"
#include "lvgl.h"
#include "lvgl_demo.h"
#include "lcd.h"
#include "touch.h"
#include "lvgl_private.h"
#include "lv_global.h"
#include "event_groups.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
EventGroupHandle_t EventGroup_hadle;
BaseType_t lvglTask_state;
BaseType_t musicTask_state;
TaskHandle_t* musictaskhander=NULL;
UBaseType_t pri_music=osPriorityNormal;

TaskHandle_t* PWM_LEDhander=NULL;
TaskHandle_t* lvgl_hander=NULL;

TaskHandle_t* task1hander=NULL;
TaskHandle_t* task2hander=NULL;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for my_UARTTask */
osThreadId_t my_UARTTaskHandle;
const osThreadAttr_t my_UARTTask_attributes = {
  .name = "my_UARTTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void musicTask(void* );
void PWMLED_TasK(void*);
void task1(void* priam);
void task2(void* priam);
void lvgl_task(void *argument);
/* USER CODE END FunctionPrototypes */

void Start_LEDTask(void *argument);
void Start_UARTTask(void *argument);

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
  LEDTaskHandle = osThreadNew(Start_LEDTask, NULL, &LEDTask_attributes);

  /* creation of my_UARTTask */
  my_UARTTaskHandle = osThreadNew(Start_UARTTask, NULL, &my_UARTTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  //创建LVGL的DEMO任务
   lvglTask_state=xTaskCreate(lvgl_demo_task,"lvgl_demo",4096,NULL,osPriorityNormal+1,lvgl_hander);
 // if (lvglTask_state!=pdPASS)
 // {
 //   printf("lvgl创建任务失败！！");
 // }

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Start_LEDTask */
/**
  * @brief  Function implementing the LEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_LEDTask */
void Start_LEDTask(void *argument)
{
  /* USER CODE BEGIN Start_LEDTask */
  /* Infinite loop */


  for(;;)
  {
  //  FT6336_Scan();
    HAL_GPIO_TogglePin(yellow_LED_GPIO_Port, yellow_LED_Pin);
    HAL_GPIO_WritePin(green_LED_GPIO_Port, green_LED_Pin, GPIO_PIN_SET);

     vTaskDelay(500);
    osDelay(1);
  }
  /* USER CODE END Start_LEDTask */
}

/* USER CODE BEGIN Header_Start_UARTTask */
/**
* @brief Function implementing the my_UARTTask thread.
* @param argument: Not used
* @retval None
*/

extern uint8_t Rxdata[30];

/* USER CODE END Header_Start_UARTTask */
void Start_UARTTask(void *argument)
{
  /* USER CODE BEGIN Start_UARTTask */
  // 添加短暂延迟，确保串口硬件稳定后再发送数据
  osDelay(100);
  uint8_t Txxxxdata[]="发送__DMA\r\n";
  /*创建队列*/
  //QueueHandle_t uartQueue=xQueueCreate(10,sizeof(ID));
  /* Infinite loop */
  extern DMA_HandleTypeDef hdma_usart3_rx;    //接收DMA通道的指针地址
  HAL_UARTEx_ReceiveToIdle_DMA(&huart3,Rxdata,sizeof(Rxdata));
  __HAL_DMA_DISABLE_IT(&hdma_usart3_rx,DMA_IT_HT);
  for(;;)
  {
    //HAL_GPIO_WritePin(SPI1_MOSI_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_SET);
    //HAL_UART_Transmit_DMA(&huart3,Txxxxdata,sizeof(Txxxxdata));
    // printf("__io_putchar!!!!\r\n");

    // printf("Hello World!\r\n");


    osDelay(50);  // 延迟可保留，此时不会再导致冲突
  }

  /* USER CODE END Start_UARTTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

