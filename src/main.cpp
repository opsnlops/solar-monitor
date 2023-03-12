

#include <FreeRTOS.h>
#include <task.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "network/network.h"
#include "logging/logging.h"

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )


void main_task(__unused void *params) {


    start_networking();


    // Delete this task
    vTaskDelete(nullptr);

}


void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, nullptr, TEST_TASK_PRIORITY, &task);

#if NO_SYS && configUSE_CORE_AFFINITY && configNUM_CORES > 1
    // we must bind the main task to one core (well at least while the init is called)
    // (note we only do this in NO_SYS mode, because cyw43_arch_freertos
    // takes care of it otherwise)
    vTaskCoreAffinitySet(task, 1);
#endif

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}



int main()
{
    stdio_init_all();
    logger_init();


    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
#if ( portSUPPORT_SMP == 1 )
    rtos_name = "FreeRTOS SMP";
#else
    rtos_name = "FreeRTOS";
#endif

#if ( portSUPPORT_SMP == 1 ) && ( configNUM_CORES == 2 )
    info("Starting %s on both cores", rtos_name);
    vLaunch();
#elif ( RUN_FREERTOS_ON_CORE == 1 )
    info("Starting %s on core 1", rtos_name);
    multicore_launch_core1(vLaunch);
    while (true);
#else
    info("Starting %s on core 0", rtos_name);
    vLaunch();
#endif
    return 0;
}