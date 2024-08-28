/*! @mainpage Blinking
 *
 * \section Parpadeo del led
 *
 * Hacemos parpadear el led 1
 *
 * @section modificaciones
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 12/09/2023 | Document creation		                         |
 * | 28/08/2024 | Documentación	                         |
 *
 * @author Benavidez Jerónimo (Benavidez.Jeronimo@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 1000  /* defino el periodo de parpadeo */
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
    LedsInit();      /* inicializo el led*/
    while(true){     /* creo un ciclo while que siempre resive true para buclear*/
        printf("LED ON\n");  
        LedOn(LED_1);   /* prendo el led*/
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);  /* espero un tiempo*/
        printf("LED OFF\n"); 
        LedOff(LED_1);  /* apago el led*/
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS); /* espero otro tiempo*/
    }
}
/*==================[end of file]============================================*/
