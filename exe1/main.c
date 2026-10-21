#include <stdio.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include "pico/stdlib.h"

SemaphoreHandle_t xPrintfMutex;

void task_a(void *p) {
    while (true) {
        // TODO: obtenha xPrintfMutex antes de usar o printf.
        printf("Task A: inicio da mensagem\n");
        vTaskDelay(pdMS_TO_TICKS(50));
        printf("Task A: fim da mensagem\n");
        // TODO: libere xPrintfMutex apos terminar de usar o printf.

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void task_b(void *p) {
    while (true) {
        // TODO: obtenha xPrintfMutex antes de usar o printf.
        printf("Task B: inicio da mensagem\n");
        vTaskDelay(pdMS_TO_TICKS(50));
        printf("Task B: fim da mensagem\n");
        // TODO: libere xPrintfMutex apos terminar de usar o printf.

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

int main() {
    stdio_init_all();

    // TODO: crie o mutex e armazene-o em xPrintfMutex.

    xTaskCreate(task_a, "Task A", 1024, NULL, 1, NULL);
    xTaskCreate(task_b, "Task B", 1024, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {
    }
}
