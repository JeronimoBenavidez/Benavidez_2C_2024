/*! @mainpage Ejercicio 1 Guia de trabajo: proyecto 2
 *
 * @section genDesc General Description
 *
 * This section describes how the program works.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 * @section hardConn Hardware Connection
 *
 * |  EDU-CIAA-NXP  |   ESP32   	|
 * |:--------------:|:--------------|
 * | 	GND 	 	| 	GND 		|
 * | 	+5V	 	    | 	 +5V	    |
 * | 	TRIGGER 	| 	GPIO_2		|
 * | 	ECHO	 	| 	GPIO_3		|
 *
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 29/08/2024 | Creación en  clase                             |
 *
 * @author Benavidez Jerónimo (jeronimo.benavidez@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "timer_mcu.h"
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "led.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
TaskHandle_t medir_distancia_handle = NULL;
TaskHandle_t mostrar_distancia_handle = NULL;

uint16_t distancia;
switch_t tecla;
bool midiendo = true;
bool congelarPantalla = false;
/*==================[internal functions declaration]=========================*/

static void medirDistancia(void *pvParameter)
{

	printf("inicio ciclo while del sensor \n \r");
	while (1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);   
		distancia = HcSr04ReadDistanceInCentimeters();

	}
}

static void leerTeclas(void *pvParameter){
	while(1){
	//printf("inicializo el switch \n \r" );
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);               
 
	  	switch(SwitchesRead()){									
    		case SWITCH_2:
			//printf("cambio variable congelarPantalla \n \r" );
    		congelarPantalla = !congelarPantalla;	
    		break;

}}}

static void mostrarDistancia(void *pvParameter)
{

	// printf("entro en el while de distancia \n \r" );
	while (1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (distancia < 10)
		{
			// printf("entré en el if de led si distancia <10 \n \r" );
			LedOff(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
		}
		else if (distancia < 20)
		{
			// printf("entré en el if de led si distancia <10 \n \r" );
			LedOn(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
		}
		else if (distancia < 30)
		{
			//	printf("entré en el if de led si distancia <10 \n \r" );
			LedOn(LED_1);
			LedOn(LED_2);
			LedOff(LED_3);
		}
		else
		{
			LedOn(LED_1);
			LedOn(LED_2);
			LedOn(LED_3);
		}
		
		LcdItsE0803Write(distancia);
	}
}

void Func_medir(void *param)
{
	vTaskNotifyGiveFromISR(medir_distancia_handle, pdFALSE);
}
void Func_congelar(void *param)
{
	vTaskNotifyGiveFromISR(mostrar_distancia_handle, pdFALSE);
}

/*==================[external functions definition]==========================*/
void app_main(void)
{
	timer_config_t timer_medir = {
		.timer = TIMER_A,
		.period = 1000,
		.func_p = Func_medir,
		.param_p = NULL
	};
	TimerInit(&medir_distancia_handle);

timer_config_t timer_mostrar = {
		.timer = TIMER_B,
		.period = 100,
		.func_p = Func_congelar,
		.param_p = NULL
	};
	TimerInit(&mostrar_distancia_handle);


	printf("inicializo sensor HcSr04 \n \r");
	HcSr04Init(GPIO_3, GPIO_2);
	printf("inicializo el switch \n \r");
	SwitchesInit();
	printf("inicializo pantalla \n \r");
	LcdItsE0803Init();
	printf("inicializo leds \n \r");
	LedsInit();
	xTaskCreate(&medirDistancia, "midiendo distancia", 2048, NULL, 5, &medir_distancia_handle);
	xTaskCreate(&mostrarDistancia, "mostrando distancia", 512, NULL, 5, &mostrar_distancia_handle);
    xTaskCreate(&leerTeclas, "leyendo teclas", 2048, NULL, 5,  NULL );  
	TimerStart(timer_medir.timer);
    TimerStart(timer_mostrar.timer);
}
/*==================[end of file]============================================*/