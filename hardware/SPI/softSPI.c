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
   // SoftSPI_DelayUs(SOFTSPI_DELAY_US);  // 精准微秒延迟
}

static inline void MOSI_Set(uint8_t state)
{
    state ? SPI_MOSI_SET : SPI_MOSI_CLR;
  // SoftSPI_DelayUs(SOFTSPI_DELAY_US);  // 精准微秒延迟
}

// ==================== 修复：写字节函数（标准LCD SPI时序） ====================
// 时序：SCK低电平 → 设置MOSI → SCK高电平（LCD上升沿采样）→ 循环8位
void SPI_WriteByte(uint8_t Byte)
{
    uint8_t i;
    SCK_Set(0); // 初始SCK拉低
    for (i = 0; i < 8; i++)
    {
        // 1. SCK先拉低（准备输出MOSI）
        SCK_Set(0);
        // 2. 设置MOSI（最高位先行）
        MOSI_Set((Byte & 0x80) ? 1 : 0);
        Byte <<= 1;
        // 3. SCK拉高（LCD上升沿采样MOSI）
        SCK_Set(1);
    }
    SCK_Set(0); // 结束后SCK拉低，避免残留电平
}

// ==================== 修复：读字节函数（同步MISO采样） ====================
// 时序：SCK拉高 → 采样MISO → SCK拉低 → 循环8位
uint8_t SPI_ReadByte(void)
{
    uint8_t value = 0, i;
    SCK_Set(0); // 初始SCK拉低
    for (i = 0; i < 8; i++)
    {
        value <<= 1; // 左移，准备接收下一位
        // 1. SCK拉高（外设输出MISO）
        SCK_Set(1);
        // 2. 采样MISO（此时MISO电平稳定）
        if (SPI_MISO_READ == GPIO_PIN_SET)
        {
            value |= 0x01;
        }
        // 3. SCK拉低，完成一个周期
        SCK_Set(0);
    }
    return value;
}


uint8_t SPI_WriteReadByte(uint8_t tx_byte)
{
    uint8_t rxdata = 0,value=0;
    SCK_Set(0);
    for (uint8_t i=0;i<8;i++)
    {
        value=(tx_byte<<i) & 0x80;
        MOSI_Set(value ? 1 :0);
        SCK_Set(1);
        if (SPI_MISO_READ==1)
        {
            rxdata |= 0x80>>i;
        }
        SCK_Set(0); // 结束后拉低
    }

    return rxdata;
}
// ==================== SPI GPIO初始化（适配你的引脚+优化配置） ====================
void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 1. 启用GPIO时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();  // SCK（PA5）、MISO（PA6）
    __HAL_RCC_GPIOB_CLK_ENABLE();  // MOSI（PB5）

    /* 2. 初始电平：SCK/L、MOSI/L（避免上电毛刺） */
    HAL_GPIO_WritePin(SPI1_SCK_GPIO_Port, SPI1_SCK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPI1_MOSI_GPIO_Port, SPI1_MOSI_Pin, GPIO_PIN_RESET);

    /* 3. 配置SCK/MOSI为推挽输出（高速+无上下拉） */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = SPI1_SCK_Pin;
    HAL_GPIO_Init(SPI1_SCK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = SPI1_MOSI_Pin;
    HAL_GPIO_Init(SPI1_MOSI_GPIO_Port, &GPIO_InitStruct);

    /* 4. 配置MISO为上拉输入（避免浮空误采样） */
    GPIO_InitStruct.Pin = SPI1_MISO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SPI1_MISO_GPIO_Port, &GPIO_InitStruct);
}