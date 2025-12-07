//
// Created by 艾伦 on 2025/11/17.
//

#ifndef HELLO_FREERTOS_STADY_H
#define HELLO_FREERTOS_STADY_H
#include "main.h"
#include "cmsis_os.h"
//全部的函数
/******************************任务****************************************/
#if 1
BaseType_t  xTaskCreate       ( TaskFunction_t pxTaskCode, // 函数指针, 任务函数
                                const char * const pcName, // 任务的名字
                                const configSTACK_DEPTH_TYPE usStackDepth, // 栈大小,单位为word,10表示40字节
                                void * const pvParameters, // 调用任务函数时传入的参数
                                UBaseType_t uxPriority,    // 优先级
                                TaskHandle_t * const pxCreatedTask ); // 任务句柄, 以后使用它来操作这个任务

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,
                                const char * const pcName,		/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                UBaseType_t uxPriority,
                                StackType_t * const puxStackBuffer,
                                StaticTask_t * const pxTaskBuffer )

void        vTaskDelete         ( TaskHandle_t xTaskToDelete );
UBaseType_t uxTaskPriorityGet   ( const TaskHandle_t xTask );  //获取任务的优先级
void        vTaskPrioritySet    ( TaskHandle_t xTask,           //设置任务的优先级
                                UBaseType_t uxNewPriority );



/*************************队列*******************************/


QueueHandle_t   xQueueCreate      ( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
QueueHandle_t   xQueueCreateStatic(
                                   UBaseType_t uxQueueLength,
                                   UBaseType_t uxItemSize,
                                   uint8_t *pucQueueStorageBuffer,
                                   StaticQueue_t *pxQueueBuffer
                               );

BaseType_t  xQueueReset            ( QueueHandle_t pxQueue);
void        vQueueDelete           ( QueueHandle_t xQueue );

BaseType_t  xQueueSend             (
                                    QueueHandle_t    xQueue,
                                    const void       *pvItemToQueue,
                                    TickType_t       xTicksToWait
                                     );
BaseType_t xQueueSendToBack         (
                                    QueueHandle_t    xQueue,
                                    const void       *pvItemToQueue,
                                    TickType_t       xTicksToWait
                                     );

BaseType_t xQueueSendFromISR         (
                                      QueueHandle_t xQueue,
                                      const void *pvItemToQueue,
                                      BaseType_t *pxHigherPriorityTaskWoken
                                       );

BaseType_t xQueueSendToBackFromISR     (
                                      QueueHandle_t xQueue,
                                      const void *pvItemToQueue,
                                      BaseType_t *pxHigherPriorityTaskWoken
                                       );

BaseType_t xQueueSendToFront        (
                                        QueueHandle_t    xQueue,
                                        const void       *pvItemToQueue,
                                        TickType_t       xTicksToWait
                                  );

BaseType_t xQueueSendToFrontFromISR(
                                      QueueHandle_t xQueue,
                                      const void *pvItemToQueue,
                                      BaseType_t *pxHigherPriorityTaskWoken
                                   );

BaseType_t xQueueReceive            ( QueueHandle_t xQueue,
                                      void * const pvBuffer,
                                      TickType_t xTicksToWait );

BaseType_t xQueueReceiveFromISR     (
                                    QueueHandle_t    xQueue,
                                    void             *pvBuffer,
                                    BaseType_t       *pxTaskWoken
                                  );

/*************************信号量*******************************/
SemaphoreHandle_t xSemaphoreCreateBinary( void );

//创建一个二进制信号量，返回它的句柄。
 //* 静态分配内存，所以需要先有一个StaticSemaphore_t结构体，并传入它的指针
 //* 返回值: 返回句柄，非NULL表示成功
SemaphoreHandle_t xSemaphoreCreateBinaryStatic( StaticSemaphore_t *pxSemaphoreBuffer );
/* 创建一个计数型信号量，返回它的句柄。
 * 此函数内部会分配信号量结构体
 * uxMaxCount: 最大计数值
 * uxInitialCount: 初始计数值
 * 返回值: 返回句柄，非NULL表示成功
 */
SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount);

/* 创建一个计数型信号量，返回它的句柄。
 * 此函数无需动态分配内存，所以需要先有一个StaticSemaphore_t结构体，并传入它的指针
 * uxMaxCount: 最大计数值
 * uxInitialCount: 初始计数值
 * pxSemaphoreBuffer: StaticSemaphore_t结构体指针
 * 返回值: 返回句柄，非NULL表示成功
 */
SemaphoreHandle_t xSemaphoreCreateCountingStatic( UBaseType_t uxMaxCount,
                                                 UBaseType_t uxInitialCount,
                                                 StaticSemaphore_t *pxSemaphoreBuffer );
/*
 * xSemaphore: 信号量句柄，你要删除哪个信号量
 */
void vSemaphoreDelete               ( SemaphoreHandle_t xSemaphore );

BaseType_t xSemaphoreGive           ( SemaphoreHandle_t xSemaphore );

BaseType_t xSemaphoreGiveFromISR    (
                                    SemaphoreHandle_t xSemaphore,
                                    BaseType_t *pxHigherPriorityTaskWoken
                                    );

BaseType_t xSemaphoreTake           (
                                   SemaphoreHandle_t xSemaphore,
                                   TickType_t xTicksToWait
                                   );

BaseType_t xSemaphoreTakeFromISR      (
                                    SemaphoreHandle_t xSemaphore,
                                    BaseType_t *pxHigherPriorityTaskWoken
                                     );

/*************************互斥量*******************************/

#define configUSE_MUTEXES 1

/* 创建一个互斥量，返回它的句柄。
 * 此函数内部会分配互斥量结构体
 * 返回值: 返回句柄，非NULL表示成功
 */
SemaphoreHandle_t xSemaphoreCreateMutex( void );

/* 创建一个互斥量，返回它的句柄。
 * 此函数无需动态分配内存，所以需要先有一个StaticSemaphore_t结构体，并传入它的指针
 * 返回值: 返回句柄，非NULL表示成功
 */
SemaphoreHandle_t xSemaphoreCreateMutexStatic( StaticSemaphore_t *pxMutexBuffer );
/*
 * xSemaphore: 信号量句柄，你要删除哪个信号量, 互斥量也是一种信号量
 */
void vSemaphoreDelete( SemaphoreHandle_t xSemaphore );

/* 释放 */
BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );

/* 释放(ISR版本) */
BaseType_t xSemaphoreGiveFromISR(
                       SemaphoreHandle_t xSemaphore,
                       BaseType_t *pxHigherPriorityTaskWoken
                   );

/* 获得 */
BaseType_t xSemaphoreTake(
                   SemaphoreHandle_t xSemaphore,
                   TickType_t xTicksToWait
               );
/* 获得(ISR版本) */
xSemaphoreGiveFromISR(
                       SemaphoreHandle_t xSemaphore,
                       BaseType_t *pxHigherPriorityTaskWoken
                   );

/* 创建一个递归锁，返回它的句柄。
 * 此函数内部会分配互斥量结构体
 * 返回值: 返回句柄，非NULL表示成功
 */
SemaphoreHandle_t xSemaphoreCreateRecursiveMutex( void );


/* 释放 */
BaseType_t xSemaphoreGiveRecursive( SemaphoreHandle_t xSemaphore );

/* 获得 */
BaseType_t xSemaphoreTakeRecursive(
                   SemaphoreHandle_t xSemaphore,
                   TickType_t xTicksToWait
               );




/*************************事件组*******************************/
EventGroupHandle_t xEventGroupCreate( void );

/* 创建一个事件组，返回它的句柄。
 * 此函数无需动态分配内存，所以需要先有一个StaticEventGroup_t结构体，并传入它的指针
 * 返回值: 返回句柄，非NULL表示成功
 */
EventGroupHandle_t xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer );

void vEventGroupDelete( EventGroupHandle_t xEventGroup );

/* 设置事件组中的位
 * xEventGroup: 哪个事件组
 * uxBitsToSet: 设置哪些位?
 *              如果uxBitsToSet的bitX, bitY为1, 那么事件组中的bitX, bitY被设置为1
 *               可以用来设置多个位，比如 0x15 就表示设置bit4, bit2, bit0
 * 返回值: 返回原来的事件值(没什么意义, 因为很可能已经被其他任务修改了)
 */
EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    const EventBits_t uxBitsToSet );
/* 设置事件组中的位
 * xEventGroup: 哪个事件组
 * uxBitsToSet: 设置哪些位?
 *              如果uxBitsToSet的bitX, bitY为1, 那么事件组中的bitX, bitY被设置为1
 *               可以用来设置多个位，比如 0x15 就表示设置bit4, bit2, bit0
 * pxHigherPriorityTaskWoken: 有没有导致更高优先级的任务进入就绪态? pdTRUE-有, pdFALSE-没有
 * 返回值: pdPASS-成功, pdFALSE-失败
 */
BaseType_t xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                      const EventBits_t uxBitsToSet,
                                      BaseType_t * pxHigherPriorityTaskWoken );


EventBits_t xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                 const EventBits_t uxBitsToWaitFor,
                                 const BaseType_t xClearOnExit,
                                 const BaseType_t xWaitForAllBits,
                                 TickType_t xTicksToWait );

EventBits_t xEventGroupSync(    EventGroupHandle_t xEventGroup,
                                const EventBits_t uxBitsToSet,
                                const EventBits_t uxBitsToWaitFor,
                                TickType_t xTicksToWait );



/*************************任务通知*******************************/

/*不能在ISR函数里获取任务通知值，因为中断里不含有TCB*/
BaseType_t xTaskNotifyGive( TaskHandle_t xTaskToNotify );

void vTaskNotifyGiveFromISR( TaskHandle_t xTaskHandle, BaseType_t *pxHigherPriorityTaskWoken );

uint32_t ulTaskNotifyTake( BaseType_t xClearCountOnExit, TickType_t xTicksToWait );


BaseType_t xTaskNotify( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction );

BaseType_t xTaskNotifyFromISR( TaskHandle_t xTaskToNotify,
                               uint32_t ulValue,
                               eNotifyAction eAction,
                               BaseType_t *pxHigherPriorityTaskWoken );

BaseType_t xTaskNotifyWait( uint32_t ulBitsToClearOnEntry,
                            uint32_t ulBitsToClearOnExit,
                            uint32_t *pulNotificationValue,
                            TickType_t xTicksToWait );



/*************************软件定时器*******************************/



//定时器的回调函数的原型如下：
void ATimerCallback( TimerHandle_t xTimer );
/* 使用动态分配内存的方法创建定时器
 * pcTimerName:定时器名字, 用处不大, 尽在调试时用到
 * xTimerPeriodInTicks: 周期, 以Tick为单位
 * uxAutoReload: 类型, pdTRUE表示自动加载, pdFALSE表示一次性
 * pvTimerID: 回调函数可以使用此参数, 比如分辨是哪个定时器
 * pxCallbackFunction: 回调函数
 * 返回值: 成功则返回TimerHandle_t, 否则返回NULL
 */
TimerHandle_t xTimerCreate( const char * const pcTimerName,
                            const TickType_t xTimerPeriodInTicks,
                            const UBaseType_t uxAutoReload,
                            void * const pvTimerID,
                            TimerCallbackFunction_t pxCallbackFunction );

/* 使用静态分配内存的方法创建定时器
 * pcTimerName:定时器名字, 用处不大, 尽在调试时用到
 * xTimerPeriodInTicks: 周期, 以Tick为单位
 * uxAutoReload: 类型, pdTRUE表示自动加载, pdFALSE表示一次性
 * pvTimerID: 回调函数可以使用此参数, 比如分辨是哪个定时器
 * pxCallbackFunction: 回调函数
 * pxTimerBuffer: 传入一个StaticTimer_t结构体, 将在上面构造定时器
 * 返回值: 成功则返回TimerHandle_t, 否则返回NULL
 */
TimerHandle_t xTimerCreateStatic(const char * const pcTimerName,
                                 TickType_t xTimerPeriodInTicks,
                                 UBaseType_t uxAutoReload,
                                 void * pvTimerID,
                                 TimerCallbackFunction_t pxCallbackFunction,
                                 StaticTimer_t *pxTimerBuffer );
/* 删除定时器
 * xTimer: 要删除哪个定时器
 * xTicksToWait: 超时时间
 * 返回值: pdFAIL表示"删除命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerDelete( TimerHandle_t xTimer, TickType_t xTicksToWait );
/* 启动定时器
 * xTimer: 哪个定时器
 * xTicksToWait: 超时时间
 * 返回值: pdFAIL表示"启动命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait );

/* 启动定时器(ISR版本)
 * xTimer: 哪个定时器
 * pxHigherPriorityTaskWoken: 向队列发出命令使得守护任务被唤醒,
 *                            如果守护任务的优先级比当前任务的高,
 *                            则"*pxHigherPriorityTaskWoken = pdTRUE",
 *                            表示需要进行任务调度
 * 返回值: pdFAIL表示"启动命令"无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerStartFromISR(   TimerHandle_t xTimer,
                                 BaseType_t *pxHigherPriorityTaskWoken );

/* 停止定时器
 * xTimer: 哪个定时器
 * xTicksToWait: 超时时间
 * 返回值: pdFAIL表示"停止命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait );

/* 停止定时器(ISR版本)
 * xTimer: 哪个定时器
 * pxHigherPriorityTaskWoken: 向队列发出命令使得守护任务被唤醒,
 *                            如果守护任务的优先级比当前任务的高,
 *                            则"*pxHigherPriorityTaskWoken = pdTRUE",
 *                            表示需要进行任务调度
 * 返回值: pdFAIL表示"停止命令"无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerStopFromISR(    TimerHandle_t xTimer,
                                 BaseType_t *pxHigherPriorityTaskWoken );
/* 复位定时器
 * xTimer: 哪个定时器
 * xTicksToWait: 超时时间
 * 返回值: pdFAIL表示"复位命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait );

/* 复位定时器(ISR版本)
 * xTimer: 哪个定时器
 * pxHigherPriorityTaskWoken: 向队列发出命令使得守护任务被唤醒,
 *                            如果守护任务的优先级比当前任务的高,
 *                            则"*pxHigherPriorityTaskWoken = pdTRUE",
 *                            表示需要进行任务调度
 * 返回值: pdFAIL表示"停止命令"无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerResetFromISR(   TimerHandle_t xTimer,
                                 BaseType_t *pxHigherPriorityTaskWoken );
/* 修改定时器的周期
 * xTimer: 哪个定时器
 * xNewPeriod: 新周期
 * xTicksToWait: 超时时间, 命令写入队列的超时时间
 * 返回值: pdFAIL表示"修改周期命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerChangePeriod(   TimerHandle_t xTimer,
                                 TickType_t xNewPeriod,
                                 TickType_t xTicksToWait );

/* 修改定时器的周期
 * xTimer: 哪个定时器
 * xNewPeriod: 新周期
 * pxHigherPriorityTaskWoken: 向队列发出命令使得守护任务被唤醒,
 *                            如果守护任务的优先级比当前任务的高,
 *                            则"*pxHigherPriorityTaskWoken = pdTRUE",
 *                            表示需要进行任务调度
 * 返回值: pdFAIL表示"修改周期命令"在xTicksToWait个Tick内无法写入队列
 *        pdPASS表示成功
 */
BaseType_t xTimerChangePeriodFromISR( TimerHandle_t xTimer,
                                      TickType_t xNewPeriod,
                                      BaseType_t *pxHigherPriorityTaskWoken );
/* 获得定时器的ID
 * xTimer: 哪个定时器
 * 返回值: 定时器的ID
 */
void *pvTimerGetTimerID( TimerHandle_t xTimer );

/* 设置定时器的ID
 * xTimer: 哪个定时器
 * pvNewID: 新ID
 * 返回值: 无
 */
void vTimerSetTimerID( TimerHandle_t xTimer, void *pvNewID );

/***************************低功耗模式*****************************************/
/*进入睡眠模式： WFI 指令：__WFI
退出睡眠模式： 任何中断或事件都可以唤醒睡眠模式
FreeRTOS 已经提供了 tickless 低功耗代码的实现，本质调用指令 WFI 实现睡眠模式.
可以在本该空闲任务执行的期间，让MCU 进入相应的低功耗模式；当其他任务准备运
行的时候，唤醒MCU退出低功耗模式

，在进入睡眠模式前，可以关闭外设时钟、降低系统主频等，进一步降低系统功耗
2，退出睡眠模式后，开启前面所关闭的外设时钟、恢复系统时钟主频等
调用函数configPRE_SLEEP_RPOCESSING()，需自行实现该函数的内部操作
调用函数configPost_Sleep_ProCESSING()，需自行实现该函数的内部操作*/


/***************************内存管理*************************************/
void * pvPortMalloc( size_t xWantedSize );	// 分配内存，如果分配内存不成功，则返回值为NULL。
void vPortFree( void * pv );	// 释放内存
//当前还有多少空闲内存，这函数可以用来优化内存的使用情况
size_t xPortGetFreeHeapSize( void );
//返回：程序运行过程中，空闲内存容量的最小值。
size_t xPortGetMinimumEverFreeHeapSize( void );


#endif



#if 0

/***************队列************************/
动态创建  QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
        uxQueueLength	队列长度，最多能存放多少个数据(item)
        uxItemSize	    每个数据(item)的大小：以字节为单位
        返回值	        非0：成功，返回句柄，以后使用句柄来操作队列
                         NULL：失败，因为内存不足

静态创建  QueueHandle_t xQueueCreateStatic(
                           UBaseType_t uxQueueLength,
                           UBaseType_t uxItemSize,
                           uint8_t *pucQueueStorageBuffer,
                           StaticQueue_t *pxQueueBuffer
                       );
                    uxQueueLength	队列长度，最多能存放多少个数据(item)
                    uxItemSize	每个数据(item)的大小：以字节为单位
                    pucQueueStorageBuffer	如果uxItemSize非0，pucQueueStorageBuffer必须指向一个uint8_t数组，
                    此数组大小至少为"uxQueueLength * uxItemSize"
                    pxQueueBuffer	必须执行一个StaticQueue_t结构体，用来保存队列的数据结构
                    返回值	非0：成功，返回句柄，以后使用句柄来操作队列
                    NULL：失败，因为pxQueueBuffer为NULL

复位      BaseType_t xQueueReset( QueueHandle_t pxQueue);
删除      void vQueueDelete( QueueHandle_t xQueue );



 * 往队列尾部写入数据，如果没有空间，阻塞时间为xTicksToWait
BaseType_t xQueueSend(QueueHandle_t xQueue,const void *pvItemToQueue,TickType_t xTicksToWait);

（ 等同于xQueueSendToBack）
BaseType_t xQueueSendToBack(
                                QueueHandle_t    xQueue,
                                const void       *pvItemToQueue,
                                TickType_t       xTicksToWait
                            );

* 往队列头部写入数据，如果没有空间，阻塞时间为xTicksToWait
BaseType_t xQueueSendToFront(
                                QueueHandle_t    xQueue,
                                const void       *pvItemToQueue,
                                TickType_t       xTicksToWait
                            );


//中断函数里写入  （xQueueSendToBackFromISR==xQueueSendFromISR）
BaseType_t xQueueSendToBackFromISR(
                                      QueueHandle_t xQueue,
                                      const void *pvItemToQueue,
                                      BaseType_t *pxHigherPriorityTaskWoken
                                      //表示这个队列有没有唤醒比当前运行任务优先级更高的任务
                                      若其值被设为 pdTRUE：表示当前队列操作导致了一个比当前运行任务优先级更高的任务进入就绪状态。
                                      此时，中断服务程序（ISR）应在退出前调用 portYIELD_FROM_ISR() 触发一次任务切换，
                                      让高优先级任务立即执行（保证实时性）。
                                      若其值为 pdFALSE：表示没有高优先级任务被唤醒，无需额外调度。
                                   );
*/
BaseType_t xQueueSendToFrontFromISR(
                                     QueueHandle_t xQueue,
                                     const void *pvItemToQueue,
                                     BaseType_t *pxHigherPriorityTaskWoken
                                  );


//读队列
BaseType_t xQueueReceive( QueueHandle_t xQueue,
                          void * const pvBuffer,
                          TickType_t xTicksToWait );

BaseType_t xQueueReceiveFromISR(
                                    QueueHandle_t    xQueue,
                                    void             *pvBuffer,
                                    BaseType_t       *pxTaskWoken
                                );

//查询
 * 返回队列中可用数据的个数
UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );
 * 返回队列中可用空间的个数
UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue );

/覆盖/
 /* 覆盖队列
 * xQueue: 写哪个队列
 * pvItemToQueue: 数据地址
 * 返回值: pdTRUE表示成功, pdFALSE表示失败
 */
BaseType_t xQueueOverwrite(
                           QueueHandle_t xQueue,
                           const void * pvItemToQueue
                      );

BaseType_t xQueueOverwriteFromISR(
                           QueueHandle_t xQueue,
                           const void * pvItemToQueue,
                           BaseType_t *pxHigherPriorityTaskWoken
                      );

//偷看
当队列长度为1时，可以使用xQueueOverwrite()或xQueueOverwriteFromISR()来覆盖数据。
注意，队列长度必须为1。当队列满时，这些函数会覆盖里面的数据，这也以为着这些函数不会被阻塞。
函数原型如下：
/* 偷看队列
 * 如果想让队列中的数据供多方读取，也就是说读取时不要移除数据，要留给后来人。那么可以使用"窥视"，
 * 也就是xQueuePeek()或xQueuePeekFromISR()。这些函数会从队列中复制出数据，但是不移除数据。
 * 这也意味着，如果队列中没有数据，那么"偷看"时会导致阻塞；
 * 一旦队列中有数据，以后每次"偷看"都会成功。 函数原型如下：
 * xQueue: 偷看哪个队列
 * pvItemToQueue: 数据地址, 用来保存复制出来的数据
 * xTicksToWait: 没有数据的话阻塞一会
 * 返回值: pdTRUE表示成功, pdFALSE表示失败
 */
BaseType_t xQueuePeek(
                          QueueHandle_t xQueue,
                          void * const pvBuffer,
                          TickType_t xTicksToWait
                      );

BaseType_t xQueuePeekFromISR(
                                 QueueHandle_t xQueue,
                                 void *pvBuffer,
                             );


 /***********************事件组的函数*************************/
事件组的句柄 EventGroup_hadle  =  xEventGroupCreate();
设置事件组的某一位，高8为不用所以是（0-24）：xEventGroupSetBits(EventGroup_hadle,1<<2);    //bit2被置位
等待事件组的某一位或者多位，用按位或表示要等待多个事件发送；
xEventGroupWaitBits(EventGroup_hadle,1<<2 | 1<<3,pdTRUE,pdTRUE,osWaitForever);
同步点，
EventBits_t xEventGroupSync(    EventGroupHandle_t xEventGroup, //事件组
                                const EventBits_t uxBitsToSet,  //我完成的事件我给置1
                                const EventBits_t uxBitsToWaitFor,  //还需要等待哪些位
                                TickType_t xTicksToWait );  //等待时间


/****************任务通知函数*******************/






#endif



#endif //HELLO_FREERTOS_STADY_H