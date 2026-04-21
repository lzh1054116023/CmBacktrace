/* cmb_cpu_port.c - ARM Cortex-M4 移植文件 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "stm32f4xx_hal.h"
#include "cmb_cpu_port.h"

/** uart for logger */
extern UART_HandleTypeDef huart1;

uint32_t cmb_get_msp(void) {
#if 0
    uint32_t msp;
    __asm volatile ("mrs %0, msp" : "=r" (msp));
    return msp;
#else
    return __get_MSP();
#endif
}

uint32_t cmb_get_psp(void) {
#if 0
    uint32_t psp;
    __asm volatile ("mrs %0, psp" : "=r" (psp));
    return psp;
#else
    return __get_PSP();
#endif
}

uint32_t cmb_get_sp(void) {
    uint32_t sp;
    __asm volatile ("mov %0, sp" : "=r" (sp));
    return sp;
}

static char buffer[CMB_LOG_BUFFER_SIZE];
int cmb_printf(const char *format, ...) {
    int len;
    int send_len = 0;
    va_list args;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    do {
        if (len < 0) {
            break;
        } else if (len >= sizeof(buffer)) {
            buffer[sizeof(buffer) - 1] = '\0';
        }

        if (huart1.gState != HAL_UART_STATE_READY) {
            break;
        }

        if (HAL_UART_Transmit(&huart1, (const uint8_t *)buffer, (uint16_t)len, 1000) == HAL_ERROR) {
            // Has a error in uart internal driver
            break;
        }

        if (huart1.gState == HAL_UART_STATE_READY) {
            send_len = (huart1.TxXferSize - huart1.TxXferCount);
            HAL_UART_Transmit(&huart1, (const uint8_t *)"\r\n", 2, 1000);
        } else {
            // Has a error in uart internal driver
            send_len = 0;
        }
    } while (0);
    return send_len;
}

