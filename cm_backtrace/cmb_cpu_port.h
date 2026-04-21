
#ifndef __CMBACKTRACE_CPU_PORT_HEAD__
#define __CMBACKTRACE_CPU_PORT_HEAD__

    #include <stdint.h>
    #include <stdbool.h>

    #include "cmb_def.h"   // cm backtrace defined 
    #include "cmsis_os2.h" // CMSIS:RTOS2
    #include "FreeRTOS.h"  // ARM.FreeRTOS::RTOS:Core
    #include "task.h"      // ARM.FreeRTOS::RTOS:Core

    /* Push register to stack order list when fault interrupt happen: total 32 bytes */
    typedef struct fault_push_regs_stack_t {
        uint32_t r0;
        uint32_t r1;
        uint32_t r2;
        uint32_t r3;
        uint32_t r12;
        uint32_t lr;
        uint32_t pc;
        union {
            uint32_t value;
            struct {
                uint32_t c              : 1;
                uint32_t z              : 1;
                uint32_t f              : 1;
                uint32_t ISR_NUMBER     : 8;   // 低8位是中断向量号
                uint32_t Reserved_8_23  : 16;
                uint32_t THUMB          : 1;        // Bit 24 (应为1)
                uint32_t Reserved_25_31 : 7;
            } bits;
        } psr;
    } fault_push_regs_stack_t;

    #if (CMB_CPU_PLATFORM_TYPE == CMB_CPU_ARM_CORTEX_M4)

        // Please define these registers below reference its datasheet core_cm4.h / PM0214.pdf（PM0214 Programming manual）
        #define CMB_NVIC_CFSR   (*(volatile uint32_t *)0xE000ED28)  // Configurable fault status register

        struct cmb_hard_fault_regs {
            fault_push_regs_stack_t saved;
            /** 1 System handler control and state register */
            #define CMB_SYSHND_CTRL (*(volatile uint32_t *)0xE000ED24)
            union {
                uint32_t value;
                struct {
                    uint8_t MEMFAULTACT    : 1; /* bit0  : Memory management fault exception active bit, reads as 1 if exception is active
                                                           若为 1，表示当前正在执行 MemFault 异常处理程序 */
                    uint8_t BUSFAULTACT    : 1; /* bit1  : Bus fault exception active bit, reads as 1 if exception is active
                                                           若为 1，表示当前正在执行 BusFault  异常处理程序 */
                    uint8_t unused_bit2    : 1; /* bit2  : Reserved, must be kept cleared */
                    uint8_t USGFAULTACT    : 1; /* bit3  : Usage fault exception active bit, reads as 1 if exception is active
                                                           若为 1，表示当前正在执行 UsageFault 异常处理程序 */
                    uint8_t unused_bit4    : 1; /* bit4  : Reserved, must be kept cleared */
                    uint8_t unused_bit5    : 1; /* bit5  : Reserved, must be kept cleared */
                    uint8_t unused_bit6    : 1; /* bit6  : Reserved, must be kept cleared */
                    uint8_t SVCALLACT      : 1; /* bit7  : SVC call active bit, reads as 1 if SVC call is active
                                                           若为 1，表示当前正在执行 UsageFault 异常处理程序 */
                    uint8_t MONITORACT     : 1; /* bit8  : Debug monitor active bit, reads as 1 if Debug monitor is active */
                    uint8_t unused_bit9    : 1; /* bit9  : Reserved, must be kept cleared */
                    uint8_t PENDSVACT      : 1; /* bit10 : PendSV exception active bit, reads as 1 if exception is active
                                                           若为 1，表示当前正在执行 SysCall 异常处理程序 */
                    uint8_t SYSTICKACT     : 1; /* bit11 : SysTick exception active bit, reads as 1 if exception is active
                                                           若为 1，表示当前正在执行 SysTick 异常处理程序 */
                    uint8_t USGFAULTPENDED : 1; /* bit12 : Usage fault exception pending bit, reads as 1 if exception is pending
                                                //         设置1 强行触发（软件产生）一个 UsageFault 异常 */
                    uint8_t MEMFAULTPENDED : 1; /* bit13 : Memory management fault exception pending bit, reads as 1 if exception is pending
                                                           设置1 强行触发（软件产生）一个 MemFault 异常 */
                    uint8_t BUSFAULTPENDED : 1; /* bit14 : Bus fault exception pending bit, reads as 1 if exception is pending
                                                           设置1 强行触发（软件产生）一个 BusFault 异常 */
                    uint8_t SVCALLPENDED   : 1; /* bit15 : SVC call pending bit, reads as 1 if exception is pending
                                                           设置1 强行触发（软件产生）一个 SysCall 异常 */
                    uint8_t MEMFAULTENA    : 1; /* bit16 : Memory management fault enable bit, set to 1 to enable
                                                           设 MemFault 为1 则发生 MemFault 后执行 UsageFault_Handler 中断号程序, 否则执行 HardFault_Handler 中断号程序 */
                    uint8_t BUSFAULTENA    : 1; /* bit17 : Bus fault enable bit, set to 1 to enable
                                                           设 BusFault 为1 则发生 BusFault 后执行 BusFault_Handler 中断号程序, 否则执行 HardFault_Handler 中断号程序*/
                    uint8_t USGFAULTENA    : 1; /* bit18 : Usage fault enable bit, set to 1 to enable
                                                           设 UsageFault 为1 则发生 UsageFault 后执行 Usage_Handler 中断号程序, 否则执行 HardFault_Handler 中断号程序*/

                } bits;
            } syshndctrl;

            /** 2 Memory Management Fault Status Register */
            #define CMB_NVIC_MFSR   (*(volatile uint32_t *)0xE000ED28)
            union {
                uint8_t value;
                struct {
                    uint8_t IACCVIOL     : 1;  /* Bit0:Instruction access violation flag
                                                  指令访问违规（Instruction Access Violation）
                                                  即使 MPU 禁用或不存在，访问 XN（Execute Never）区域也会触发此故障
                                                  */
                    uint8_t DACCVIOL     : 1;  /* Bit1:Data access violation flag
                                                  数据访问违规（Data Access Violation）
                                                  典型场景：对只读区域执行写操作，或访问 MPU 禁止访问的地址 */
                    uint8_t unused_bit2  : 1;  /* Bit2:Reserved, must be kept cleared */
                    uint8_t MUNSTKERR    : 1;  /* Bit3:Memory manager fault on unstacking for a return from exception
                                                  异常返回出栈时发生内存管理故障, 典型场景：异常返回时，从堆栈恢复寄存器时访问了非法地址
                                               */
                    uint8_t MSTKERR      : 1;  /* Bit4:Memory manager fault on stacking for exception entry
                                                  异常入栈时（将寄存器压入堆栈）发生内存管理故障,典型场景：堆栈指针指向了 MPU 禁止访问的内存区域
                                               */
                    uint8_t MLSPERR      : 1;  /* Bit5:MemManage fault occurred during floating-point lazy state preservation
                                                  在浮点惰性状态保存期间是否发生了 MemManage 故障 */
                    uint8_t unused_bit6  : 1;  /* Bit6:Reserved, must be kept cleared */
                    uint8_t MMARVALID    : 1;  /* Bit7:Memory Management Fault Address Register (MMAR) valid flag
                                                  改位为1，则 MMAR 有效
                                               */
                } bits;
            } mfsr;

            /** 3 Bus fault status register */
            #define CMB_NVIC_BFSR   (*(volatile uint32_t *)0xE000ED29)
            union {
                uint8_t value;
                struct {
                    uint8_t IBUSERR     : 1;  /* bit0: Instruction bus error 发生了指令总线错误
                                                       典型场景：从无效地址取指执行（如跳转到未映射的内存区域） */
                    uint8_t PRECISERR   : 1;  /* bit1: Precise data bus error 发生了数据总线错误，堆栈中的 PC 指向引发故障的指令
                                                       典型场景：从无效地址读取数据（如读 0xFFFFFFFF 处的值） */
                    uint8_t IMPREISERR  : 1;  /* bit2: Imprecise data bus error 发生了数据总线错误，但堆栈中的返回地址与引发错误的指令无关
                                                       典型场景：写操作被缓冲（write buffer），写入操作已经返回成功，但后续实际写内存时出错。
                                                               CPU 已经执行了后续多条指令，无法精确定位 */
                    uint8_t UNSTKERR    : 1;  /* bit3: Bus fault on unstacking for a return from exception
                                                       异常返回出栈时发生了总线故障
                                                       典型场景：从异常返回时，从堆栈恢复寄存器时访问了无效内存
                                                       */
                    uint8_t STKERR      : 1;  /* bit4: Bus fault on stacking for exception entry
                                                       异常入栈时发生了一个或多个总线故障
                                                       典型场景：堆栈指针指向了无效内存地址（如未映射的地址空间）
                                                       */
                    uint8_t LSPERR      : 1;  /* bit5: Bus fault on floating-point lazy state preservation 浮点惰性状态保存期间发生了总线故障 */
                    uint8_t unused_bit6 : 1;  /* bit6: Reserved, must be kept cleared */
                    uint8_t BFARVALID   : 1;  /* bit7: Bus Fault Address Register (BFAR) valid flag
                                                       发生精确总线故障（PRECISERR=1）时，硬件会将故障地址写入 BFAR 并设置此位
                                                       如果总线故障因优先级问题被上溢为 HardFault，HardFault 处理程序必须将此位清零，否则返回后会覆盖原始 BusFault 处理程序的 BFAR 值
                                                       */
                } bits;
            } bfsr;

            /** 4 Usage fault status register */
            #define CMB_NVIC_UFSR   (*(volatile uint32_t *)0xE000ED2A)
            union {
                uint16_t value;
                struct {
                    uint16_t UNDEFINSTR     : 1; /* bit0 :Undefined instruction usage fault 处理器尝试执行无法解码的指令
                                                         典型场景:
                                                              1: 执行了处理器不支持的指令编码
                                                              2: 代码区包含数据（被当作指令执行）
                                                              3: 跳转到了错误地址
                                                        */
                    uint16_t INVSTATE       : 1; /* bit1 :Invalid state usage fault
                                                         触发场景:
                                                             1: 试图切换到非 Thumb 状态（如 BX 到一个 LSB=0 的地址）
                                                             2: 执行不支持的操作模式 */
                    uint16_t INVPC          : 1; /* bit2 :Invalid PC load usage fault 将无效的 EXC_RETURN 值加载到 PC,
                                                         触发场景:
                                                            尝试用无效值加载 EXC_RETURN（异常返回专用值）
                                                            无效的上下文切换
                                                         */
                    uint16_t NOCP           : 1; /* bit3 :No coprocessor usage fault  Cortex-M 处理器不支持外部协处理器（不像 Cortex-A 系列有 FPU 协处理器）。因此执行任何协处理器指令（如 CDP、LDC、STC、MCR、MRC 等）都会触发此故障 */
                    uint16_t unused_bit4    : 1; //bit4 :Reserved, must be kept cleared
                    uint16_t unused_bit5    : 1; //bit5 :Reserved, must be kept cleared
                    uint16_t unused_bit6    : 1; //bit6 :Reserved, must be kept cleared
                    uint16_t unused_bit7    : 1; //bit7 :Reserved, must be kept cleared
                    uint16_t UNALIGNED      : 1; /* bit8 :Unaligned access usage fault  设置 CCR 寄存器的 UNALIGN_TRP 位为 1 */
                    uint16_t DIVBYZERO      : 1; /* bit9 :Divide by zero usage fault    设置 CCR 寄存器的 DIV_0_TRP 位为 1，否则除零不会触发故障 */
                    uint16_t unused_bit10   : 1; //bit10:Reserved, must be kept cleared
                    uint16_t unused_bit11   : 1; //bit11:Reserved, must be kept cleared
                    uint16_t unused_bit12   : 1; //bit12:Reserved, must be kept cleared
                    uint16_t unused_bit13   : 1; //bit13:Reserved, must be kept cleared
                    uint16_t unused_bit14   : 1; //bit14:Reserved, must be kept cleared
                    uint16_t unused_bit15   : 1; //bit15:Reserved, must be kept cleared
                } bits;
            } ufsr;

            /** 5 hard fault status register */
            #define CMB_NVIC_HFSR   (*(volatile uint32_t *)0xE000ED2C)

            union {
                uint32_t value;
                struct {
                    uint32_t unused_bit0    : 1;  /* Bit0   : Reserved, must be kept cleared */
                    uint32_t VECTBL         : 1;  /* bit1   : Vector table hard fault
                                                              典型场景：
                                                                 中断向量表地址配置错误（如 SCB->VTOR 指向无效内存）
                                                                 向量表所在的 Flash/RAM 区域不可访问
                                                                 向量表地址未对齐
                                                                */
                    uint32_t unused_bit2_29 : 28; /* Bit2_29: Reserved, must be kept cleared */
                    uint32_t FORCED         : 1;  /* Bit30  : Forced hard fault 发生了强制硬故障——由其他可配置故障上溢导致
                                                              触发场景：
                                                                 优先级问题 : MemManage/BusFault/UsageFault 的优先级被设置为低于当前正在执行的异常，导致无法响应该故障
                                                                 故障被禁用 : 对应的 ENA 位为 0（如 USGFAULTENA=0）
                                                                 故障处理程序本身出错 : 故障处理程序执行过程中再次发生故障
                                                              处理方式：当 FORCED = 1 时，硬故障处理程序必须读取其他故障状态寄存器（MMFSR、BFSR、UFSR）来找到真正的故障原因
                                                  */
                    uint32_t DEBUGE_VT      : 1;  /* Bit31  : Debug vector table */
                } bits;
            } hfsr;

            /** 6 Debug fault status register */
            #define CMB_NVIC_DFSR   (*(volatile uint32_t *)0xE000ED30)
            /*
            改寄存器转为 DBG 仿真工具使用的, 如果使用仿真器，非常简单，IDE就实现这些寄存器的配置，如果要在无仿真器下使用，则要配置这些寄存器
            通过 break 点去触发异常
                DFSR 记录的调试事件需要在DEMCR（Debug Exception and Monitor Control Register）中先使能，否则不会被捕获：
                    DEMCR 位: 功能
                        Bit 16 : VC_HARDERR   - 使能硬故障向量捕获
                        Bit 15 : VC_INTERR    - 使能中断服务错误向量捕获
                        Bit 10 : VC_CORERESET - 使能内核复位向量捕获
                        Bit 9  : VC_MMERR     - 使能 MemManage 故障向量捕获
                        Bit 8  : VC_NOCPERR   - 使能协处理器故障向量捕获
                        Bit 7  : VC_CHKERR    - 使能检查故障向量捕获
                        Bit 0  : VC_BUSERR    - 使能总线故障向量捕获
            如果在无仿真器下, 怎么跟踪怀疑关键的代码，通过 在关键代码插入 BKPT 指令，触发一次调式异常，在调试异常里 做你的跟踪信息
                第一步：启用 Debug Monitor
                    // 使能调试监视器 (Debug Monitor)，这一步是关键
                    CoreDebug->DEMCR |= CoreDebug_DEMCR_MON_EN_Msk;
                    // 可选：确保 DebugMonitor 异常有合适的优先级 (NVIC 设置)
                    NVIC_SetPriority(DebugMonitor_IRQn, 0x80); // 设置一个较低的优先级
                    NVIC_EnableIRQ(DebugMonitor_IRQn);
                第二步：在可疑代码处埋入探针
                    #define TRACE_POINT(id) __asm("BKPT #" #id)
                    void Task_Communication(void) {
                        // ... 代码 ...
                        TRACE_POINT(1);  // 插入探针1：标记进入通信任务
                        // ... 代码 ...
                        TRACE_POINT(2);  // 探针2：标记特定函数调用前
                    }
                第三步：在 DebugMon_Handler 中记录日志
                    void DebugMon_Handler(void) {
                        uint32_t return_addr;
                        // 1. 获取触发断点的地址
                        if (__get_IPSR() != 0) {
                            return_addr = __get_MSP();
                        } else {
                            return_addr = __get_PSP();
                        }
                        // 简易日志：通过 RTT 或串口打印地址
                        printf("[Trace] Hit BKPT at address: 0x%08X\n", return_addr);
                        // 2. 处理完后必须清除 Debug Event 标志
                        CoreDebug->DFSR = CoreDebug_DFSR_BKPT_Msk;
                    }

                    uint8_t decode_bkpt_imm(uint16_t instruction) {
                        // --------------------------------------------------------
                        // humb BKPT 指令（16位）布局：
                        //     ┌───────┬────────────┬───────┬────────────┐
                        //     │ 15-12 │   11-8     │ 7-4   │   3-0      │
                        //     ├───────┼────────────┼───────┼────────────┤
                        //     │ 1011  │   imm[7:4] │ 1111  │  imm[3:0]  │
                        //     └───────┴────────────┴───────┴────────────┘
                        // 立即数 = (imm_high << 4) | imm_low
                        // --------------------------------------------------------
                        // 检查是否为 BKPT 指令（高 4 位应为 0xB，中间 4 位应为 0xF）
                        if ((instruction & 0xFF00) != 0xBE00) {
                            return 0xFF;  // 不是有效的 BKPT 指令
                        }
                        // 提取立即数：高 4 位在 bit[11:8]，低 4 位在 bit[3:0]
                        uint8_t imm_high = (instruction >> 8) & 0x0F;  // bits 11-8
                        uint8_t imm_low  = (instruction >> 0) & 0x0F;  // bits 3-0
                        return (imm_high << 4) | imm_low;
                    }

                    void DebugMon_Handler(void) {
                        uint32_t pc;
                        // 判断进入异常时使用的是 MSP 还是 PSP
                        if (__get_IPSR() != 0) {
                            // 异常中，使用 MSP
                            pc = ((uint32_t*)__get_MSP())[6];  // 栈帧中 PC 位于偏移 6
                        } else {
                            // 线程模式，使用 PSP
                            pc = ((uint32_t*)__get_PSP())[6];
                        }
                        // 注意：BKPT 指令本身是 2 字节（Thumb 模式）
                        // PC 指向 BKPT 指令的下一条指令，所以需要回退 2 字节
                        uint16_t bkpt_instruction = *(uint16_t*)(pc - 2);
                        // 解码立即数（见下文）
                        uint8_t imm = decode_bkpt_imm(bkpt_instruction);
                        // 打印或记录这个 ID
                        printf("BKPT ID: %d\n", imm);
                        // 清除断点事件标志
                        CoreDebug->DFSR = CoreDebug_DFSR_BKPT_Msk;
                    }

                特别提醒：在 DebugMon_Handler 中，尽量避免复杂的函数调用（如 printf），因为此时的系统状态可能不太稳定。
                        最稳妥的做法是将必要的信息（如 PC 值）存入一个全局数组，在主循环中再将数据打印出来
            */
            union {
                uint32_t value;
                struct {
                    uint32_t HALTED    : 1;  /* bit0: 停机状态。处理器已进入调试停机状态时，此位置1 */
                    uint32_t BKPT      : 1;  /* bit1: 断点指令。当执行BKPT指令时，此位置1 */
                    uint32_t DWTTRAP   : 1;  /* bit2: 数据观察点匹配。当DWT（数据观察点与跟踪单元）配置的数据观察点被触发时，此位置1 */
                    uint32_t VCATCH    : 1;  /* bit3: 向量捕获。当发生被使能的向量捕获事件时，此位置1（如复位、NMI、硬故障等） */
                    uint32_t EXTERNAL  : 1;  /* bit4: 外部调试请求。当外部调试器发出 halt 请求时，此位置1 */
                    uint32_t Bit5_31  : 27;  /* Bit5_31: Reserved, must be kept cleared */
                } bits;
            } dfsr;

            /** 7 Memory management fault address register */
            #define CMB_NVIC_MMAR   (*(volatile uint32_t *)0xE000ED34)
            uint32_t mmar;

            /** 8 Bus fault address register */
            #define CMB_NVIC_BFAR   (*(volatile uint32_t *)0xE000ED38)  // Bus Fault Address Register
            uint32_t bfar;
            /** 9 Auxiliary Fault Status Register 辅助故障状态寄存器 */
            #define CMB_NVIC_AFSR   (*(volatile uint32_t *)0xE000ED3C)
            uint32_t afsr; // Implementation defined. The AFSR contains additional system fault information. The 
                           // bits map to the AUXFAULT input signals
        };
    #else
        #error "Please add your CPU defined";
    #endif

    uint32_t cmb_get_msp(void);
    uint32_t cmb_get_psp(void);
    uint32_t cmb_get_sp(void);
    int cmb_printf(const char *format, ...);

#endif


