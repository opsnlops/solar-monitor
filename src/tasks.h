
#pragma once

#include "controller-config.h"

#include <FreeRTOS.h>
#include <task.h>


/*
 * This file is a crude sort of task manager, listing all the tasks
 * we have.
 */

portTASK_FUNCTION_PROTO(log_queue_reader_task, pvParameters);           // used in logging/logging.cpp

portTASK_FUNCTION_PROTO(heartbeat_checker_task, pvParameters);