# Step1 Enable configRECORD_STACK_HIGH_ADDRESS
    Add macro defined in the file mcu/Inc/FreeRTOSConfig.h
    #define configRECORD_STACK_HIGH_ADDRESS          1

# Step2 : add member uxStackDepth to TCB 
- in the typedef struct tskTaskControlBlock{} Add the line 
    StackType_t * pxStack;     /**< Points to the start of the stack. */
    StackType_t uxStackDepth;  /**<  Stack size depth */
- assigne value , in the API prvInitialiseNewTask()
    /* Avoid dependency on memset() if it is not required. */
    #if ( tskSET_NEW_STACKS_TO_KNOWN_VALUE == 1 )
    {
        /* Fill the stack with a known value to assist debugging. */
        ( void ) memset( pxNewTCB->pxStack, ( int ) tskSTACK_FILL_BYTE, ( size_t ) uxStackDepth * sizeof( StackType_t ) );
    }
    #endif /* tskSET_NEW_STACKS_TO_KNOWN_VALUE */
    // For cm_backtrace
    pxNewTCB->uxStackDepth = uxStackDepth;

# Step3: Add some API to get stack data of freeRTOS
- declare API in the file mcu/STM32Cube_FW_F4/Middlewares/Third_Party/FreeRTOS/Source/include/task.h
    uint32_t *vTaskStackAddr();
    uint32_t *vTaskTopOfStack();
    uint32_t *vTaskEndOfStack();
    uint32_t *vTaskStartOfStack();
    uint32_t vTaskStackSize();
    char *vTaskName();
- define API in the file mcu/STM32Cube_FW_F4/Middlewares/Third_Party/FreeRTOS/Source/include/task.c
    uint32_t *vTaskStackAddr() {
        return pxCurrentTCB->pxStack;
    }

    uint32_t *vTaskTopOfStack() {
        uint32_t *pxTopOfStack = (uint32_t *)pxCurrentTCB->pxTopOfStack;
        return pxTopOfStack;
    }

    uint32_t *vTaskEndOfStack() {
        return pxCurrentTCB->pxEndOfStack;
    }

    uint32_t *vTaskStartOfStack() {
        return pxCurrentTCB->pxStack;
    }

    uint32_t vTaskStackSize() {
        #if ( portSTACK_GROWTH > 0 )
        return (pxNewTCB->pxEndOfStack - pxNewTCB->pxStack + 1);
        #else /* ( portSTACK_GROWTH > 0 )*/
        return pxCurrentTCB->uxStackDepth;
        #endif /* ( portSTACK_GROWTH > 0 )*/
    }

    char *vTaskName() {
        return pxCurrentTCB->pcTaskName;
    }
    
# step4: Modify ld script file mcu/STM32F405OEYx_FLASH.ld b/src/mcu/STM32F405OEYx_FLASH.ld
    _Min_Heap_Size  = 0x800; /* required amount of heap  */
    _Min_Stack_Size = 0x800;  /* required amount of stack */

    /* ========== cm_backtrace ========== */
    CMB_CSTACK_BLOCK_START = _estack - _Min_Stack_Size;
    CMB_CSTACK_BLOCK_END   = _estack;
    
    CMB_CODE_SECTION_START = ORIGIN(FLASH);
    CMB_CODE_SECTION_END   = ORIGIN(FLASH) + LENGTH(FLASH);
    /* ========================================= */

# step5 : test examples
    test source int the file cmb_test.h/c : call the API cmb_test_main() where test 
    get call site function from PC value : arm-none-eabi-addr2line -e build/ulu_cmm2hc_ss_firmware.out -f 0x0802CC16  0x080245F8 0x0802CCFC 0x0802CD56 0x0802CD56 0x0802CDAA 0x0802CDAA
