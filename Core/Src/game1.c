//
// Created by 艾伦 on 2025/11/13.
#include "game1.h"
#include "cmsis_os.h"
#include "queue.h"
#include "semphr.h"


SemaphoreHandle_t carsemaphpre;


static const uint8_t carImgData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0x50, 0x30, 0x10, 0x18, 0xf8, 0xf8, 0x18, 0x30, 0x50, 0xe0, 0x40, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xf8, 0xd8, 0xe4, 0x27, 0x33, 0x33, 0xe2, 0xe6, 0x8e, 0x8c, 0xff, 0x83, 0x80, 0xe2, 0x62, 0x32, 0x33, 0x23,
    0xe4, 0xcc, 0xf8, 0x80, 0x00, 0x00, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf1, 0xf3, 0xf2, 0xf2, 0xf3, 0xf1, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0, 0xf1, 0xf3,
    0xf2, 0xf2, 0xf3, 0xf1, 0xf0, 0xf0, 0xf0, 0xf0, 0xf0,
    };
const Image carImg = {27, 20, carImgData};
static const uint8_t roadMarking[] ={
    0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,


};

#if 1
static const uint8_t clearImg[81]={0};
#endif

typedef  struct  car{
    uint8_t x;
    uint8_t y;
    char key[2];
}   car_typedef;

car_typedef g_cars[3]={
{0,0,{'R','1'}},
{0, 22,{'R','2'}},
{0,44,{'R','3'}},
};

static void carTask(void* Parameters);

/*隐藏汽车*/
static void Hide_car(car_typedef* car)
{
    OLED_SetBlock(car->x,car->y,clearImg,27,20,OLED_COLOR_NORMAL);
    OLED_ShowFrame();
}

/*显示汽车*/
static void showcar(car_typedef* car)
{
    OLED_SetBlock(car->x,car->y,carImgData,27,20,OLED_COLOR_NORMAL);
    OLED_ShowFrame();
}

/*游戏项目*/
void car_game(void )
{
    OLED_Init();
    /*画小车*/
    for(uint8_t i=0;i<3;i++)
    {
       OLED_SetBlock(g_cars[i].x,g_cars[i].y,carImgData,27,20,OLED_COLOR_NORMAL);
    }
    /*画路标*/
    for(uint8_t i=0;i<3;i++)
    {
        for (uint8_t j=0;j<16;j++)
        {
            OLED_SetBlock(j*8,21+22*i,roadMarking,128,1,OLED_COLOR_NORMAL);
        }

    }
    OLED_ShowFrame();

    /*创建任务*/
    xTaskCreate(carTask,"carTask1",128,&g_cars[0],osPriorityNormal,NULL);
    xTaskCreate(carTask,"carTask2",128,&g_cars[1],osPriorityNormal,NULL);
    xTaskCreate(carTask,"carTask3",128,&g_cars[2],osPriorityNormal,NULL);

   carsemaphpre= xSemaphoreCreateCounting(3,1);

}

/*游戏小车任务*/
static void carTask(void* Parameters)
{
    car_typedef* car=Parameters;
        uint8_t rxData[20];

    /*获取信号量*/
    xSemaphoreTake( carsemaphpre, portMAX_DELAY);


   //  /*创建自己的队列*/
   //  QueueHandle_t  carQueue=xQueueCreate(10,sizeof(car_typedef));
   //  /*注册自己的队列*/
   // extern void Register_queueHandle(void* QueueHandle);
   //  Register_queueHandle(carQueue);
    while(1)
    {
        // /*读取UART的值,读队列*/
        // xQueueReceive(carQueue,rxData,HAL_MAX_DELAY);

        /*处理读出的队列数据 */
        #if 1
        /*小车移动*/
        // if (rxData[0]==car->key[0] && rxData[1]==car->key[1])
        // {

            if (car->x<128-27)
            {
                /*隐藏之前的小车*/
                Hide_car(car);
                car->x +=3;
                if (car->x>128-27)
                {
                    car->x=128-27;
                }
                /*显示新的小车*/
                showcar(car);
                 osDelay(50);
                if (car->x==128-27)
                {
                    xSemaphoreGive(carsemaphpre);
                    vTaskDelete(NULL);
                }
            }
        //}
    }
        #endif
}
