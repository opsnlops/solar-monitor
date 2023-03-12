

#include <FreeRTOS.h>
#include <task.h>


#include "tasks.h"
#include "pico/stdlib.h"
#include "hardware/watchdog.h"
#include "pico/cyw43_arch.h"
#include "pico/stdio.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "network/network.h"
#include "logging/logging.h"
#include "network/mdns.h"
#include "network/mqtt.h"


extern TaskHandle_t heartbeat_checker_task_handle;

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 10UL )

volatile uint32_t heartbeat = 0;

[[noreturn]] void main_task(__unused void *params) {

    logger_init();
    start_networking();

    //start_mdns();
    start_mqtt();
    do_mqtt_publish();

    for(EVER) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("squeak (%u)\n", heartbeat);
    }

}


void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task,
                "main_task",
                configMINIMAL_STACK_SIZE,
                NULL,
                TEST_TASK_PRIORITY,
                &task);


    xTaskCreate(heartbeat_checker_task,
        "heartbeat_task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TEST_TASK_PRIORITY,
        &heartbeat_checker_task_handle);


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


portTASK_FUNCTION(heartbeat_checker_task, pvParameters) {

    uint32_t last_heartbeat = heartbeat;

    for(EVER) {

        vTaskDelay(pdMS_TO_TICKS(30000));

        if(heartbeat == last_heartbeat)
        {
            printf("hummmm we're not getting heatbeats anymore, byeeee!\n");
            watchdog_enable(1, 1);
            while(1);
        }

        last_heartbeat = heartbeat;

    }


}