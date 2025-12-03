//
// Created by 艾伦 on 2025/11/23.
//
#include "lvgl_demo.h"
#include "lvgl.h"
#include "lvgl_private.h"
#include "lv_demo_stress.h"
void lvgl_demo_task(void *pvParameter)
{
    (void)pvParameter;
    lv_demo_stress();
    while (1)
    {
        lv_task_handler();
        vTaskDelay(5);
    }

}

