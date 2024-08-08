/*! @mainpage Template
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |    Peripheral  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	PIN_X	 	| 	GPIO_X		|
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 8/08/2024 | Document creation		                         |
 *
 * @author Jerónimo Benavidez (jeronimo.benavidez@ingenieria.uner.edu.ar)
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
    uint8_t mode;         //ON, OFF, TOGGLE
	uint8_t n_led;        //indica el número de led a controlar
	uint8_t n_ciclos;     //indica la cantidad de ciclos de ncendido/apagado
	uint16_t periodo;     //indica el tiempo de cada ciclo
} my_leds; 	
/*==================[internal data definition]===============================*/

#define ON 1
#define OFF 2
#define TOGGLE 3

/*==================[internal functions declaration]=========================*/
void funcion_guia1(struct leds* my_leds){


if (my_leds->mode == ON){
	
	switch (my_leds->n_led)
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
else if(my_leds->mode == OFF)
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
	else 
	
		for(size_t i=0; i<my_leds->n_ciclos; ++i)
		{
			switch (my_leds->n_led)
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


void app_main(void){
	LedsInit();

	struct leds ledDePrueba;
	ledDePrueba.mode = TOGGLE ;
	ledDePrueba.n_led = 3;
	ledDePrueba.n_ciclos = 9999;
	ledDePrueba.periodo = 750;
	

	
	funcion_guia1(&ledDePrueba);



}

/*==================[end of file]============================================*/