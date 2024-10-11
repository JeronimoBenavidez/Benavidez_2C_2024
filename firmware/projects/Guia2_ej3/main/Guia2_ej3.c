/*! @mainpage Ejercicio 3 Guia de trabajo: proyecto 2
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
#include "uart_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
TaskHandle_t medir_distancia_handle = NULL;
TaskHandle_t mostrar_distancia_handle = NULL;
TaskHandle_t leer_teclas_handle = NULL;
uint32_t distancia;
uint8_t tecla;

bool midiendo = true;
bool congelarPantalla = false;
/*==================[internal functions declaration]=========================*/
/**
 * @brief Función de tarea que mide la distancia utilizando el sensor ultrasónico.
 *
 */
static void medirDistancia(void *pvParameter)
{

	// printf("inicio ciclo while del sensor \n \r");
	while (1)
	{

		if (midiendo)
		{
			// printf("asigno medicion a distancia \n \r");
			distancia = HcSr04ReadDistanceInCentimeters();
		}
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/**
 * @brief Función que maneja la lectura de teclas por UART.
 * 
 * Cambia el estado de las banderas `midiendo` y `congelarPantalla` según la tecla ingresada.
 */
void teclasUART()
{
	while (1)
	{
		UartReadByte(UART_PC, &tecla);

		if (tecla == 79)
		{

			midiendo = !midiendo;
		}

		if (tecla == 72)
		{
			congelarPantalla = !congelarPantalla;
		}

		tecla = NULL;
	}
}

/**
 * @brief Función de tarea que muestra la distancia en el LCD y la transmite por UART.
 *
 */
static void mostrarDistancia(void *pvParameter)
{ // printf("entro en el while de distancia \n \r" );
	while (1)
	{
		if (midiendo == 1)
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

			if (!congelarPantalla)
			{
				LcdItsE0803Write(distancia);
			}
			UartSendString(UART_PC, "La distancia es: \n");

			UartSendString(UART_PC, (char *)UartItoa(distancia, 10));
			UartSendString(UART_PC, " cm \r \n");
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		}
	}
}

/**
 * @brief Función que activa la tarea de mostrar distancia desde el temporizador.
 *
 */
void Func_medir(void *param)
{
	vTaskNotifyGiveFromISR(medir_distancia_handle, pdFALSE);
}

/**
 * @brief Función que activa la tarea de mostrar distancia desde el temporizador.
 *
 */
void Func_congelar(void *param)
{
	vTaskNotifyGiveFromISR(mostrar_distancia_handle, pdFALSE);
}

/**
 * @brief Función que activa la tarea de mostrar distancia desde el temporizador.
 *
 **/
void leer_teclas(void *param)
{
	vTaskNotifyGiveFromISR(leer_teclas_handle, pdFALSE);
}

/**
 * @brief Cambia el estado de la variable `midiendo`.
 * 
 * Esta función invierte el valor de la variable `midiendo` cuando se presiona el botón correspondiente.
 */
void Tecla_midiendo()
{

	midiendo = !midiendo;
}

/**
 * @brief Cambia el estado de la variable `congelarPantalla`.
 * 
 * Esta función invierte el valor de la variable `congelarPantalla` cuando se presiona el botón correspondiente.
 */
void Tecla_congelarPantalla()
{

	congelarPantalla = !congelarPantalla;
}

/*==================[external functions definition]==========================*/
/**
 * @brief Función principal de la aplicación. Configura los periféricos, tareas y temporizadores.
 */
void app_main(void)
{
	serial_config_t my_uart = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = teclasUART,
		.param_p = NULL

	};
	UartInit(&my_uart);

	timer_config_t timer_medir = {
		.timer = TIMER_A,
		.period = 1000000,
		.func_p = Func_medir,
		.param_p = NULL};
	TimerInit(&timer_medir);

	timer_config_t timer_mostrar = {
		.timer = TIMER_B,
		.period = 500000,
		.func_p = Func_congelar,
		.param_p = NULL};
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