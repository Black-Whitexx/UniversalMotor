#include "stdarg.h"
#include "stdio.h"
#include "main.h"
#include "printf.h"

#define TX_BUF_SIZE 32

uint8_t send_buf[TX_BUF_SIZE];

int usart_printf(const char *format, ...)
{
    va_list args;
    uint32_t length;
    va_start(args, format);
    length = vsnprintf((char *) send_buf, TX_BUF_SIZE, (const char *) format, args);
    va_end(args);
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *) send_buf, length);

}