/*! @mainpage Blinking switch
 *
 * \section Parpadeo con switches
 *
 * Prendemos y apagamos los led 1 y 2 dependiendo si apretamos el switch que les corresponden
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
#include "switch.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD 250      /* defino un tiempo para el periodo de parpadeo */
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

/*==================[external functions definition]==========================*/
void app_main(void){
	uint8_t teclas; 
	LedsInit();          /* inicializo los leds*/
	SwitchesInit();      /* inicializo los switches*/
    while(1)    {
    	teclas  = SwitchesRead();                          /* asigno a una variable teclas el valor */
		printf("se apreta tecla %d \r \n" , teclas);}      /* de un switch para saber cual aprieto  */
    	switch(teclas){									/* dependiendo de qué tecla apriente selecciono*/
    		case SWITCH_1:
    			LedToggle(LED_1);						/*cambio el estado del led en cuestión*/
    		break;
    		case SWITCH_2:
    			LedToggle(LED_2);
    		break;
			case SWITCH_1 | SWITCH_2:   				/* si me apretan los dos a la vez, cambio el estado del 3*/
				LedToggle(LED_3);
			break;
			case SWITCH_1 & SWITCH_2:                  /* si no apretan, me aseguro de tener apagados todos los led*/
				LedOff(LED_1);
				LedOff(LED_2);
				LedOff(LED_3);
			break;
	    	}
	    
		
	
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS); /* el delay */
	}
}
