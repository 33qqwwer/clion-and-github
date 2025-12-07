//
// Created by 艾伦 on 2025/11/23.
//
#include "lvgl_demo.h"
#include "lvgl.h"
#include "lvgl_private.h"
#include "lv_demo_stress.h"
#include "stdio.h"
#include "touch.h"
#include "lcd.h"
#include "lv_port_disp.h"
#include "lv_port_indev_template.h"
void lvgl_demo_task(void *pvParameter)
{
    printf("LVGL测试开始!!!!\r\n");
    osDelay(400);
   // printf("LVGL测试开始!!!!\r\n");
    lv_init();
    // LCD_Init();
    // TP_Init();
  //  printf("准备初始化显示适配层...\r\n");
    lv_port_disp_init();
  //  printf("显示适配层初始化完成！\r\n");
  //  printf("准备初始化输入适配层...\r\n");
    lv_port_indev_init();
  //  printf("输入适配层初始化完成！\r\n");

    printf("LVGL初始化完成!!!!\r\n");
    (void)pvParameter;
 lv_demo_stress();
    while (1)
    {
        lv_timer_handler();
        osDelay(5);
    }

}

