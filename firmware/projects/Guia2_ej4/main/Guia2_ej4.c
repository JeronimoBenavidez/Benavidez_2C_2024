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
 * | 3/10/2024 | Document creation		                         |
 *
 * @author Jeronimo Benavidez (jeronimo.benavidez@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "analog_io_mcu.h"
#include "uart_mcu.h"
#include "timer_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


/*==================[macros and definitions]=================================*/
uint8_t valorPote;
TaskHandle_t mandar_valores_pote_handle = NULL;
TaskHandle_t leer_valores_pote_handle = NULL;
/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/

void leerValoresPote (void *pvParameter)
{
	while(1)
	{
		AnalogInputReadSingle(CH1, *valorPote);
		ulTaskNotifyTake(pdTrue , portMAX_DELAY);
	}
}



void mandarValoresPote (void *pvParameter)
{
	while (1)
	{
	UartSendByte( UART_PC , (char*)valorPote ) ;
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}
void mandar_pote(void *param)
{
	vTaskNotifyGiveFromISR(mandar_valores_pote_handle, pdFALSE);
}

void leer_pote(void *param)
{
	vTaskNotifyGiveFromISR(leer_valores_pote_handle, pdFALSE))
}

/*==================[external functions definition]==========================*/
void app_main(void){
	printf("Hello world!\n");

	serial_config_t my_uart = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL

	};
	UartInit(&my_uart);

	timer_config_t timer_mandar = {
		.timer = TIMER_A,
		.period = 2000,
		.func_p = mandar_pote,
		.param_p = NULL};
	TimerInit(&timer_mandar);

	timer_config_t timer_leer = {
		.timer = TIMER_B,
		.period = 2000,
		.func_p = leer_pote,
		.param_p = NULL};
	TimerInit(&timer_leer);

		AnalogInputInit();


xTaskCreate(&leerValoresPote, "leyendo valores", 5121 NULL, 5, leer_valores_pote_handle );
xTaskCreate(&mandarValoresPote, "midiendo valores del pote", 512, NULL, 5, mandar_valores_pote_handle) ;
TimerStart(timer_mandar.timer);
TimerStart(timer_leer.timer);
}
/*==================[end of file]============================================*/