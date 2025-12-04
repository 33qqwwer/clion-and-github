#include "softSPI.h"

// ==================== 核心：100MHz下精准微秒延迟函数 ====================
void SoftSPI_DelayUs(uint32_t us)
{
    // SystemCoreClock需确保为100MHz（在SystemClock_Config中配置）
    uint32_t ticks = us * (SystemCoreClock / 1000000);  // 100MHz → ticks = us * 100
    uint32_t start = SysTick->VAL;

    // SysTick是向下计数器，处理溢出情况
    if (start > ticks)
    {
        while ((start - SysTick->VAL) < ticks);
    }
    else
    {
        while ((0xFFFFFF - SysTick->VAL + start) < ticks);
    }
    __NOP();  // 空操作，避免编译器优化
}

// ==================== 内联引脚控制（消除函数调用开销） ====================
static inline void SCK_Set(uint8_t state)
{
    state ? SPI_SCLK_SET : SPI_SCLK_CLR;
    SoftSPI_DelayUs(SOFTSPI_DELAY_US);  // 精准微秒延迟
}

static inline void MOSI_Set(uint8_t state)
{
    state ? SPI_MOSI_SET : SPI_MOSI_CLR;
    SoftSPI_DelayUs(SOFTSPI_DELAY_US);  // 精准微秒延迟
}

// ==================== 写字节函数（标准SPI时序：MSB先行+上升沿采样） ====================
void SPI_WriteByte(uint8_t Byte)
{
    uint8_t i = 0;
    for (i = 0; i < 8; i++)
    {
        // 1. 先设置MOSI（高位先行）
        MOSI_Set((Byte & 0x80) ? 1 : 0);
        Byte <<= 1;

        // 2. SCK时序：低→高（LCD在SCK上升沿采样MOSI）
        SCK_Set(0);
        SCK_Set(1);
    }
}

// ==================== 读字节函数（修正MISO采样判断，消除歧义） ====================
uint8_t SPI_ReadByte(void)
{
    uint8_t value = 0, i = 0, byte = 0xFF;  // 发送0xFF作为dummy字节
    for (i = 0; i < 8; i++)
    {
        value <<= 1;

        // 1. 发送dummy字节（MOSI）
        MOSI_Set((byte & 0x80) ? 1 : 0);
        byte <<= 1;

        // 2. SCK拉低 → 采样MISO（显式判断GPIO_PIN_SET，避免歧义）
        SCK_Set(0);
        if (SPI_MISO_READ == GPIO_PIN_SET)  // 核心修正：显式判断高电平
        {
            value |= 1;  // 等价于value +=1，更符合位操作习惯
        }

        // 3. SCK拉高，完成一个时钟周期
        SCK_Set(1);
    }
    return value;
}

// ==================== SPI GPIO初始化（适配你的引脚+优化配置） ====================
void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 1. 启用GPIO时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();  // MOSI（PB2）
    __HAL_RCC_GPIOC_CLK_ENABLE();  // SCK（PC10）、MISO（PC11）

    /* 2. 初始电平：SCK/L、MOSI/L（避免上电毛刺） */
    HAL_GPIO_WritePin(SPI3_SCK_GPIO_Port, SPI3_SCK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI3_MOSI_GPIO_Port, SPI3_MOSI_Pin, GPIO_PIN_RESET);

    /* 3. 配置SCK/MOSI为推挽输出（高速+无上下拉） */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = SPI3_SCK_Pin;
    HAL_GPIO_Init(SPI3_SCK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI3_MOSI_Pin;
    HAL_GPIO_Init(SPI3_MOSI_GPIO_Port, &GPIO_InitStruct);

    /* 4. 配置MISO为上拉输入（避免浮空误采样） */
    GPIO_InitStruct.Pin = SPI3_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SPI3_MISO_GPIO_Port, &GPIO_InitStruct);
}