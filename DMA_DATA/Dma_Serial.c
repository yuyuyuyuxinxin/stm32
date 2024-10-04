#include "Dma_Serial.h"
// 初始化串口结构体
xUart_typedef xUart1 = {0};
xUart_typedef xUart2 = {0};
xUart_typedef xUart3 = {0};
xUart_typedef xUart4 = {0};
xUart_typedef xUart5 = {0};
xUart_typedef xUart6 = {0};

// 初始化姿态角度变量
float Roll = 0;   // 横滚角
float Pitch = 0;  // 俯仰角
float Yaw = 0;    // 航向角
//Z轴角度归零指令
uint8_t yaw_cmd[3] = {0xFF,0xAA,0x52};

//Z轴角度归零 
void Yaw_zero(void)
{
	//UART_DMA_Transmit(yaw_cmd,3);
    printf("%.*s", 3, yaw_cmd);

}

// UART接收事件回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart == &huart1)  // 判断事件是否来源于 UART1
    {
        __HAL_UNLOCK(huart);  // 解锁 UART 句柄，确保可以执行接下来的操作

        xUart1.ReceiveNum = Size;  // 保存接收到的数据大小
        memset(xUart1.ReceiveData, 0, sizeof(xUart1.ReceiveData));  // 清空接收缓冲区
        memcpy(xUart1.ReceiveData, xUart1.BuffTemp, Size);  // 将接收到的数据从临时缓冲区拷贝到接收缓冲区
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, xUart1.BuffTemp, sizeof(xUart1.BuffTemp));  // 重新开始 DMA 接收，接收下一批数据
    }
}

// 定义状态机的状态枚举
typedef enum {
    WAITING_FOR_55,  // 等待数据起始字节0x55
    WAITING_FOR_53,  // 等待数据类型字节0x53
    RECEIVING_DATA,  // 接收数据中
    DATA_RECEIVED    // 数据接收完成
} State;

// 初始化状态机的初始状态
State currentState = WAITING_FOR_55;
uint8_t receivedData[6];  // 用于存储接收到的数据
uint8_t dataIndex = 0;    // 数据接收索引

// 数据处理函数，根据状态机的状态处理传入的数据
void processData(uint8_t data) 
{
    switch (currentState) 
    {
        case WAITING_FOR_55:
            // 等待接收 0x55 作为起始字节
            if (data == 0x55) {
                currentState = WAITING_FOR_53;  // 接收到0x55后，进入下一个状态
            }
            break;

        case WAITING_FOR_53:
            // 等待接收 0x53 作为数据类型字节
            if (data == 0x53) {
                currentState = RECEIVING_DATA;  // 接收到0x53后，开始接收实际数据
                dataIndex = 0;  // 重置数据接收索引
            } else {
                currentState = WAITING_FOR_55;  // 如果不是0x53，重新等待0x55
            }
            break;

        case RECEIVING_DATA:
            // 接收数据，并存储在 `receivedData` 数组中
            receivedData[dataIndex++] = data;  
            if (dataIndex >= 6) {
                currentState = DATA_RECEIVED;  // 如果接收到的字节数达到6个，则表示数据接收完成
            }
            break;

        case DATA_RECEIVED:
            // 数据接收完成后，处理数据
            data_deal();  
            currentState = WAITING_FOR_55;  // 处理完后，状态机重新回到等待0x55的状态
            break;
    }
}

// 数据处理函数，将接收到的数据转换为姿态角度
void data_deal(void)
{
    // 计算Roll角度 (横滚角)
    Roll = ((short)(receivedData[1] << 8) | receivedData[0]) / 32768.0 * 180;

    // 计算Pitch角度 (俯仰角)
    Pitch = ((short)(receivedData[3] << 8) | receivedData[2]) / 32768.0 * 180;

    // 计算Yaw角度 (航向角)
    Yaw = ((short)(receivedData[5] << 8) | receivedData[4]) / 32768.0 * 180;

    // 打印调试信息，显示接收到的数据和计算得到的Yaw角度
   
}
