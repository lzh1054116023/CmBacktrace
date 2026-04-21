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
 * Function: It is the configure head file for this library.
 * Created on: 2016-12-15
 */

#ifndef _CMB_CFG_H_
#define _CMB_CFG_H_

    #include <stddef.h>
    #include <stdint.h>
    #include <stdio.h>
    #include <stdlib.h>

    #include "FreeRTOS.h"
    #include "logger.h"

    #define CMB_OS_PLATFORM_RTT         0
    #define CMB_OS_PLATFORM_UCOSII      1
    #define CMB_OS_PLATFORM_UCOSIII     2
    #define CMB_OS_PLATFORM_FREERTOS    3
    #define CMB_OS_PLATFORM_RTX5        4
    #define CMB_OS_PLATFORM_THREADX     5

    #define CMB_CPU_ARM_CORTEX_M0       0
    #define CMB_CPU_ARM_CORTEX_M3       1
    #define CMB_CPU_ARM_CORTEX_M4       2
    #define CMB_CPU_ARM_CORTEX_M7       3
    #define CMB_CPU_ARM_CORTEX_M33      4

    #define CMB_PRINT_LANGUAGE_ENGLISH          0
    #define CMB_PRINT_LANGUAGE_CHINESE          1
    #define CMB_PRINT_LANGUAGE_CHINESE_UTF8     2
    #define CMB_PRINT_LANGUAGE_CUSTOM           3

    #define CMB_ASSERT(cond)                    //

    /* print line, must config by user */
    #define cmb_println(...)                    cmb_printf(__VA_ARGS__);

    /* enable OS platform */
    #define CMB_USING_OS_PLATFORM
    /* OS platform type, must config when CMB_USING_OS_PLATFORM is enable */
    #define CMB_OS_PLATFORM_TYPE                CMB_OS_PLATFORM_FREERTOS

    #define CMB_ELF_FILE_EXTENSION_NAME         ".elf"

    /* cpu platform type, must config by user */
    #define CMB_CPU_PLATFORM_TYPE               CMB_CPU_ARM_CORTEX_M4
    /* enable dump stack information */
    #define CMB_USING_DUMP_STACK_INFO
    /* language of print information */
    #define CMB_PRINT_LANGUAGE                  CMB_PRINT_LANGUAGE_ENGLISH

    #define CMB_NAME_MAX                        64

    #define CMB_CALL_STACK_MAX_DEPTH            16

    #define CMB_DUMP_STACK_DEPTH_SIZE           128

    #define CMB_LOG_BUFFER_SIZE                 256

#endif /* _CMB_CFG_H_ */
