/*
 * This file is part of the CmBacktrace Library.
 *
 * Copyright (c) 2016, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: It is an head file for this library. You can see all be called functions.
 * Created on: 2016-12-15
 */

#ifndef _CORTEXM_BACKTRACE_H_
#define _CORTEXM_BACKTRACE_H_

#include "cmb_cpu_port.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Backtrace initialize */
void cm_backtrace_init(const char *firmware_name, const char *hardware_ver, const char *software_ver);
/* Backtrace version information */
void cm_backtrace_firmware_info(void);
/* Backtrace stack called history from sp starting */
size_t cm_backtrace_call_stack(uint32_t *buffer, size_t size, uint32_t sp);
/* Backtrace stack information from start to end */
void cm_backtrace_assert(uint32_t sp);
/* Backtrace fault stack information */
void cm_backtrace_fault(uint32_t fault_handler_lr, uint32_t fault_handler_sp);
#if 1
/* Backtrace stack information according to stack data */
void cm_backtrace_violence_base(const char *name, uint32_t *stack_addr_start, uint32_t *stack_addr_end);
/* Backtrace stack information of a task */
void cm_backtrace_task_violence(TaskHandle_t TaskHandle, bool is_full);
/* Backtrace stack information of current task */
void cm_backtrace_current_task_violence(bool is_full);
/* Backtrace stack information of current task which print minimum information */
#define cmb_backtrace_current_task_lite()   do {                                                                        \
                                                unsigned int sp;                                                        \
                                                unsigned int lr;                                                        \
                                                TaskStatus_t TaskStatus;                                                \
                                                __asm volatile ("isb");                                                 \
                                                __asm volatile ("mov %0, sp" : "=r" (sp));                              \
                                                __asm volatile ("mov %0, lr" : "=r" (lr));                              \
                                                vTaskGetInfo(xTaskGetCurrentTaskHandle(), &TaskStatus, 1, eInvalid);    \
                                                __asm volatile ("isb");                                                 \
                                                cmb_println("cm_backtrace sp=0x%08X lr=0x%08X", sp, lr);                \
                                                cm_backtrace_violence_base(TaskStatus.pcTaskName,                       \
                                                                (uint32_t *)sp,                                         \
                                                                (uint32_t *)TaskStatus.pxEndOfStack);                   \
                                            } while (0);

#define cmb_backtrace_current_task_depth()   do {                                                                       \
                                                unsigned int sp;                                                        \
                                                int depth;                                                              \
                                                unsigned int lr;                                                        \
                                                uint32_t buffer[CMB_CALL_STACK_MAX_DEPTH];                              \
                                                __asm volatile ("isb");                                                 \
                                                __asm volatile ("mov %0, sp" : "=r" (sp));                              \
                                                __asm volatile ("mov %0, lr" : "=r" (lr));                              \
                                                __asm volatile ("isb");                                                 \
                                                cmb_println("backtrace depth=%02d ------->", CMB_CALL_STACK_MAX_DEPTH); \
                                                cmb_println("sp=0x%08X lr=0x%08X", sp, lr);                             \
                                                depth = cm_backtrace_call_stack(buffer                                  \
                                                                              , CMB_CALL_STACK_MAX_DEPTH                \
                                                                              , (uint32_t)sp);                          \
                                                for (int i = 0; i < depth; i++) {                                       \
                                                    cmb_println("#%02d callback Call Site: 0x%08X", i, (int)buffer[i]);\
                                                }                                                                       \
                                                cmb_println("\r\n");                                                    \
                                            } while (0);




#endif

#ifdef __cplusplus
}
#endif

#endif /* _CORTEXM_BACKTRACE_H_ */
