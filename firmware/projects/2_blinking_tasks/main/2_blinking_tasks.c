/*! @mainpage Blinking
 *
 * \section Parpadeo tasks
 *
 * Hacemos parpadear los leds 1 2 y 3 a diferentes frecuencias 
 * 
 * @section Modificaciones
 *
 * |   Date	    | Description                                        |
 * |:----------:|:---------------------------------------------------|
 * | 12/09/2023 | Document creation		                             |
 * | 01/08/2024 | Modificaciones de la guia sobre el codigo original |
 * | 28/08/2024 | Documentación		                                 |
 *
 * @author Benavidez Jerónimo (Benavidez.Jeronimo@ingenieria.uner.edu.ar)
 *
 */
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD_LED_1 1730      /* defino los periodos de parpadeo */
#define CONFIG_BLINK_PERIOD_LED_2 1570      /* de los 3 leds                   */
#define CONFIG_BLINK_PERIOD_LED_3 550
/*==================[internal data definition]===============================*/
TaskHandle_t led1_task_handle = NULL;       /* creo 3 variables para manejar con */
TaskHandle_t led2_task_handle = NULL;       /* con tasks y así conciderarlas como*/
TaskHandle_t led3_task_handle = NULL;       /* si se ejecutacen a la vez ( en realidad */
                                            /* se pone en juego el freeRTOS )     */
/*==================[internal functions declaration]=========================*/
static void Led1Task(void *pvParameter){            /* Defino el parpadeo del  LED1*/
    while(true){
        printf("LED_1 ON\n");
        LedOn(LED_1);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_1 / portTICK_PERIOD_MS);
        printf("LED_1 OFF\n");
        LedOff(LED_1);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_1 / portTICK_PERIOD_MS);
    }
}

static void Led2Task(void *pvParameter){            /* Defino el parpadeo del  LED2*/
    while(true){
        printf("LED_2 ON\n");
        LedOn(LED_2);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_2 / portTICK_PERIOD_MS);
        printf("LED_2 OFF\n");
        LedOff(LED_2);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_2 / portTICK_PERIOD_MS);
    }
}

static void Led3Task(void *pvParameter){            /* Defino el parpadeo del  LED3*/
    while(true){
        printf("LED_3 ON\n");
        LedOn(LED_3);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_3 / portTICK_PERIOD_MS);
        printf("LED_3 OFF\n");
        LedOff(LED_3);
        vTaskDelay(CONFIG_BLINK_PERIOD_LED_3 / portTICK_PERIOD_MS);
    }
}
/*==================[external functions definition]==========================*/
void app_main(void){                                
    LedsInit();                                    /* inicializo los leds*/
    xTaskCreate(&Led1Task, "LED_1", 512, NULL, 5, &led1_task_handle);   /*uso xtaskCreate para*/
    xTaskCreate(&Led2Task, "LED_2", 512, NULL, 5, &led2_task_handle);   /* asignar las tareas */
    xTaskCreate(&Led3Task, "LED_3", 512, NULL, 5, &led3_task_handle);   /* de parpadeo creadas*/
}
