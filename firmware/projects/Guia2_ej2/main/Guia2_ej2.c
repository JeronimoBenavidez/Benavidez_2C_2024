/*! @mainpage Ejercicio 2 Guia de trabajo: proyecto 2
 *
 * @section genDesc General Description
 *
 * Este programa mide la distancia utilizando un sensor ultrasónico HC-SR04 y controla la visualización de la distancia en un display LCD.
 * También enciende LEDs en función de la distancia medida y permite pausar la medición y congelar la pantalla mediante botones.
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
TaskHandle_t informar_tecla_handle = NULL;
uint16_t distancia;
bool midiendo = true;
bool congelarPantalla = false;
/*==================[internal functions declaration]=========================*/

/**
 * @brief Función que mide la distancia con el sensor HC-SR04.
 * 
 * La función realiza una medición de la distancia en centímetros utilizando el
 * sensor ultrasónico HC-SR04 y actualiza la variable global `distancia`.
 * 
 */
static void medirDistancia(void *pvParameter)
{

	printf("inicio ciclo while del sensor \n \r");
	while (1)
	{

			if (midiendo){
			//printf("asigno medicion a distancia \n \r");
		distancia=HcSr04ReadDistanceInCentimeters();
			}
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/**
 * @brief Función que muestra la distancia y controla los LEDs.
 * 
 * Esta función enciende los LEDs de acuerdo a la distancia medida. Además,
 * muestra el valor de la distancia en el display LCD, a menos que la pantalla esté congelada.
 * 
 */
static void mostrarDistancia(void *pvParameter)
{

	// printf("entro en el while de distancia \n \r" );
	while (1)
	{

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
		if(!congelarPantalla){
			LcdItsE0803Write(distancia);
		}
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/**
 * @brief Función de interrupción para el timer de medición.
 * 
 * Activa la tarea que mide la distancia cada vez que se cumple el tiempo configurado.
 * 
 */
void Func_medir(void *param)
{
	vTaskNotifyGiveFromISR(medir_distancia_handle, pdFALSE);
}

/**
 * @brief Función de interrupción para el timer de visualización.
 * 
 * Activa la tarea que muestra la distancia cada vez que se cumple el tiempo configurado.
 * 
 */
void Func_congelar(void *param)
{
	vTaskNotifyGiveFromISR(mostrar_distancia_handle, pdFALSE);
}


/**
 * @brief Cambia el estado de la variable `midiendo`.
 * 
 * Esta función invierte el valor de la variable `midiendo` cuando se presiona el botón correspondiente.
 */
void Tecla_midiendo(){
	
	midiendo = !midiendo;
}

/**
 * @brief Cambia el estado de la variable `congelarPantalla`.
 * 
 * Esta función invierte el valor de la variable `congelarPantalla` cuando se presiona el botón correspondiente.
 */
void Tecla_congelarPantalla(){

	congelarPantalla = !congelarPantalla;
}

/*==================[external functions definition]==========================*/
/**
 * @brief Función principal del programa.
 * 
 * Inicializa los periféricos (sensor, switches, display, LEDs) y crea las tareas
 * para medir la distancia y mostrar los resultados. También configura e inicia
 * dos timers para activar las tareas correspondientes.
 */
void app_main(void)
{
	timer_config_t timer_medir = {
		.timer = TIMER_A,
		.period = 1000000,
		.func_p = Func_medir,
		.param_p = NULL
	};
	TimerInit(&timer_medir);

timer_config_t timer_mostrar = {
		.timer = TIMER_B,
		.period = 1000,
		.func_p = Func_congelar,
		.param_p = NULL
	};
	TimerInit(&timer_mostrar);


	printf("inicializo sensor HcSr04 \n \r");
	HcSr04Init(GPIO_3, GPIO_2);
	printf("inicializo el switch \n \r");
	SwitchesInit();
	printf("inicializo pantalla \n \r");
	LcdItsE0803Init();
	printf("inicializo leds \n \r");
	LedsInit();
	SwitchActivInt(SWITCH_1, Tecla_midiendo, NULL);
	SwitchActivInt(SWITCH_2, Tecla_congelarPantalla, NULL);


	xTaskCreate(&medirDistancia, "midiendo distancia", 2048, NULL, 5, &medir_distancia_handle);
	xTaskCreate(&mostrarDistancia, "mostrando distancia", 512, NULL, 5, &mostrar_distancia_handle);
    
	TimerStart(timer_medir.timer);
    TimerStart(timer_mostrar.timer);

}
/*==================[end of file]============================================*/