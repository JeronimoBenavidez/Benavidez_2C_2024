/*! @mainpage Ejercicio 3 de la guia de trabajo: proyecto 1
 *
 * @section Programar según diagrama en bloque
 *
 * Realizo un programa que permita satisfacer el diagrama del ejercicio 3
 * de "guia de trabajo:proyecto 1" 
 *
 * Resultado en la carpeta de imagenes del repositorio
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section Modificaciones
 *
 * |   Date	    | Description                                        |
 * |:----------:|:---------------------------------------------------|
 * | 12/09/2023 | Document creation		                             |
 * | 08/08/2024 | Modificaciones de la guia sobre el codigo original |
 * | 28/08/2024 | Documentación		                                 |
 *
 * @author Benavidez Jerónimo (Benavidez.Jeronimo@ingenieria.uner.edu.ar)
 *
 */
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


/*==================[macros and definitions]=================================*/
struct leds
{
	 /*ON, OFF, TOGGLE                                  */
    uint8_t mode;      

     /*indica el número de led a controlar				 */
	uint8_t n_led;        

	 /*indica la cantidad de ciclos de ncendido/apagado */
	uint8_t n_ciclos;  

	/*indica el tiempo de cada ciclo                   */  
	uint16_t periodo;     
} 
/* creo una variable tipo leds llamada my_leds */
my_leds; 	
/*==================[internal data definition]===============================*/
/** @def ON
*	@brief Le asigno el valor 1 a ON
*/
#define ON 1					

/** @def OFF
*	@brief Le asigno el valor 2 a OFF
*/
#define OFF 2					
/** @def TOGGLE
*	@brief Le asigno el valor 3 a TOGGLE
*/
#define TOGGLE 3

/*==================[internal functions declaration]=========================*/

/** @fn Función Guía 1
* @brief ésta función irá haciendo comparaciones para prender, apagar o titilar el led que corresponda
*/

void funcion_guia1(struct leds* my_leds){


if (my_leds->mode == ON){				/* primer bloque del diagrama */
	
	switch (my_leds->n_led)				/* si se cumple el primer bloque*/
	{
		case 1:
			LedOn(LED_1);
		break;

		case 2:
			LedOn(LED_2);
		break;

		case 3:
			LedOn(LED_3);
		break;
	}	
}
else if(my_leds->mode == OFF)			/* si no se cumple paso al siguiente bloque*/
	{
		switch (my_leds->n_led)
	{
			case 1:
				LedOff(LED_1);
			break;

			case 2:
				LedOff(LED_2);
			break;

			case 3:
				LedOff(LED_3);
			break;
	}	
	}
	else                                /* si éste tampoco se cumple inicio el bucle*/
	
		for(size_t i=0; i<my_leds->n_ciclos; ++i)
		{
			switch (my_leds->n_led)	 	/* cambio el estado del LED que corresponda */
			{
			case 1:{
				LedToggle(LED_1);
				for (size_t j=0; j < (my_leds->periodo/100); ++j)
			{
			vTaskDelay(my_leds->periodo/ portTICK_PERIOD_MS);
			break;
				}
			case 2:
			{
				LedToggle(LED_2);
				for (size_t j=0; j < (my_leds->periodo/100); ++j)
			{
			vTaskDelay(my_leds->periodo/ portTICK_PERIOD_MS);
			break;
			}}
			case 3:
			{
				LedToggle(LED_3);
				for (size_t j=0; j < (my_leds->periodo/100); ++j)
			{
			vTaskDelay(my_leds->periodo/ portTICK_PERIOD_MS);
			break;
			}
			}
			
			
			}
			}
			
			
		}
	



}
/*==================[external functions definition]==========================*/

/** @fn código de prueba
* @brief pruebo la función
*/
void app_main(void){           /* verifico a modo de prueba si funciona */
	LedsInit();

	struct leds ledDePrueba;
	ledDePrueba.mode = TOGGLE ;
	ledDePrueba.n_led = 3;
	ledDePrueba.n_ciclos = 9999;
	ledDePrueba.periodo = 750;
	

	
	funcion_guia1(&ledDePrueba);



}

/*==================[end of file]============================================*/