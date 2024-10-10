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
#include <stdbool.h>
#include "analog_io_mcu.h"
#include "uart_mcu.h"
#include "timer_mcu.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*==================[macros and definitions]=================================*/
#define BUFFER_SIZE 231
uint16_t valorPote = 0;
analog_input_config_t entradaPote;
TaskHandle_t mandar_valores_pote_handle = NULL;
TaskHandle_t leer_valores_pote_handle = NULL;
TaskHandle_t main_task_handle = NULL;
int i=0;

/*==================[internal data definition]===============================*/

const char ecg[BUFFER_SIZE] = {
    76, 77, 78, 77, 79, 86, 81, 76, 84, 93, 85, 80,
    89, 95, 89, 85, 93, 98, 94, 88, 98, 105, 96, 91,
    99, 105, 101, 96, 102, 106, 101, 96, 100, 107, 101,
    94, 100, 104, 100, 91, 99, 103, 98, 91, 96, 105, 95,
    88, 95, 100, 94, 85, 93, 99, 92, 84, 91, 96, 87, 80,
    83, 92, 86, 78, 84, 89, 79, 73, 81, 83, 78, 70, 80, 82,
    79, 69, 80, 82, 81, 70, 75, 81, 77, 74, 79, 83, 82, 72,
    80, 87, 79, 76, 85, 95, 87, 81, 88, 93, 88, 84, 87, 94,
    86, 82, 85, 94, 85, 82, 85, 95, 86, 83, 92, 99, 91, 88,
    94, 98, 95, 90, 97, 105, 104, 94, 98, 114, 117, 124, 144,
    180, 210, 236, 253, 227, 171, 99, 49, 34, 29, 43, 69, 89,
    89, 90, 98, 107, 104, 98, 104, 110, 102, 98, 103, 111, 101,
    94, 103, 108, 102, 95, 97, 106, 100, 92, 101, 103, 100, 94, 98,
    103, 96, 90, 98, 103, 97, 90, 99, 104, 95, 90, 99, 104, 100, 93,
    100, 106, 101, 93, 101, 105, 103, 96, 105, 112, 105, 99, 103, 108,
    99, 96, 102, 106, 99, 90, 92, 100, 87, 80, 82, 88, 77, 69, 75, 79,
    74, 67, 71, 78, 72, 67, 73, 81, 77, 71, 75, 84, 79, 77, 77, 76, 76,
};
/*==================[internal functions declaration]=========================*/

void leerValoresPote(void *pvParameter)
{
	while (1)
	{
		AnalogInputReadSingle(CH1, &valorPote);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		
	}
}

void mandarValoresECG(void *pvParameter)
{
	while (1)
	{
		UartSendString(UART_PC, ">valor:");
		//UartSendString(UART_PC, (char *)UartItoa(valorPote, 10));
		UartSendString(UART_PC, (char*)UartItoa(ecg[i], 10));
		UartSendString(UART_PC, "\r\n");
		i = i+1;
		if (i==BUFFER_SIZE){
			i=0;
		};
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}
void mandar_ECG(void *param)
{
	vTaskNotifyGiveFromISR(mandar_valores_pote_handle, pdFALSE);
}

void leer_pote(void *param)
{
	vTaskNotifyGiveFromISR(leer_valores_pote_handle, pdFALSE);
}

/*==================[external functions definition]==========================*/
void app_main(void)
{

	serial_config_t my_uart = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL
	};
	UartInit(&my_uart);
	UartSendString(UART_PC, "arranca el programa \n");
	UartSendString(UART_PC, "Serial configurado \n");
	timer_config_t timer_mandar = {
		.timer = TIMER_A,
		.period = 4000,
		.func_p = mandar_ECG,
		.param_p = NULL};

	TimerInit(&timer_mandar);
	UartSendString(UART_PC, "timer de mandar configurado \n");
	timer_config_t timer_leer = {
		.timer = TIMER_B,
		.period = 2000,
		.func_p = leer_pote,
		.param_p = NULL};
	TimerInit(&timer_leer);
	UartSendString(UART_PC, "timer de leer configurado \n");
	AnalogInputInit(&entradaPote);
	AnalogOutputInit();
	xTaskCreate(&leerValoresPote, "leyendo valores", 512, NULL, 5, &leer_valores_pote_handle);
	xTaskCreate(&mandarValoresECG, "mandar valores del pote", 512, NULL, 5, &mandar_valores_pote_handle);
	UartSendString(UART_PC, "Tareas creadas \n");
	TimerStart(timer_mandar.timer);
	TimerStart(timer_leer.timer);
	UartSendString(UART_PC, "Timer iniciados \n");
}
/*==================[end of file]============================================*/