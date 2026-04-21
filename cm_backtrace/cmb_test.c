#include "cmb_test.h"

int cmb_test_func3(int arg1, int arg2, int arg3) {
    int ret = 4;
    // test Example 1
    cmb_backtrace_current_task_lite();
    // test Example 2
    cmb_backtrace_current_task_depth();
    // test Example 3
    cm_backtrace_task_violence(xTaskGetCurrentTaskHandle(), 1);
    // test Example 4
    cm_backtrace_task_violence(xTaskGetCurrentTaskHandle(), 0);
    cmb_println("cm_backtrace test fun3 result=%d", ret);
    return ret;
}

int cmb_test_func2(int arg1, int arg2) {
    unsigned int sp;
    unsigned int lr;
    __asm volatile ("mov %0, sp" : "=r" (sp));
    __asm volatile ("mov %0, lr" : "=r" (lr));
    __asm volatile ("isb");
    cmb_println("cm_backtrace test fun2 sp=0x%08X lr=0x%08X", sp, lr);
    int ret = cmb_test_func3(1, 2, 3);
    cmb_println("cm_backtrace test fun2 result=%d", ret);
    return ret;
}

int cmb_test_func1(int arg1) {
    unsigned int sp;
    unsigned int lr;
    __asm volatile ("mov %0, sp" : "=r" (sp));
    __asm volatile ("mov %0, lr" : "=r" (lr));
    __asm volatile ("isb");
    cmb_println("cm_backtrace test fun1 sp=0x%08X lr=0x%08X", sp, lr);
    int ret = cmb_test_func2(1, 2);
    cmb_println("cm_backtrace test fun1 result=%d", ret);
    return ret;
}

void cmb_test_main(void) {
    unsigned int sp;
    unsigned int lr;
    __asm volatile ("mov %0, sp" : "=r" (sp));
    __asm volatile ("mov %0, lr" : "=r" (lr));
    __asm volatile ("isb");
    cmb_println("cm_backtrace test main sp=0x%08X lr=0x%08X", sp, lr);
    int ret = cmb_test_func1(1);
    cmb_println("cm_backtrace test main result=%d", ret);
}

