/*! @mainpage Examen individual
 *
 * @section genDesc General Description
 *
 * Protección para una Mafalda ciclista. Una bocina y leds que se prendan según la distancia detras de ella. Además,
 * implementación de un acelerometro analógico.
 *
 * <a href="https://drive.google.com/...">Operation Example</a>
 *
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 4/11/2024  | rendida del examen                             |
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
#include "led.h"
#include "buzzer.h"
#include "uart_mcu.h"
#include "analog_io_mcu.h"

/*==================[macros and definitions]=================================*/
/** @def Alarma 1
*	@brief Debe sonar cada 1s
*/
#define CONFIG_BLINK_PERIOD_Alarma1 1000
/** @def Alarma 2
*	@brief Debe sonar cada 0.5s
*/
#define CONFIG_BLINK_PERIOD_Alarma2 500
/*==================[internal data definition]===============================*/
TaskHandle_t medir_distancia_handle = NULL;
TaskHandle_t mostrar_distancia_handle = NULL;
TaskHandle_t medir_aceleracion_handle = NULL;
analog_input_config_t eje_X, eje_Y, eje_Z;
uint32_t distancia;
uint16_t ac_x, ac_y, ac_z, variableAuxiliar;
bool alarmaLarga = false;
bool alarmaCorta = false;

/*==================[internal functions declaration]=========================*/

/**
 * @fn medirAceleración
 * @brief Función para medir con el acelerómetro del casco
 *
 */
void medirAceleracion(void *pvParameter)
{
	AnalogInputReadSingle(CH1, &ac_x);
	AnalogInputReadSingle(CH2, &ac_y);
	AnalogInputReadSingle(CH3, &ac_z);
	variableAuxiliar = ac_x + ac_y + ac_z;
	if (variableAuxiliar > 2850)
	{
		UartSendString(UART_PC, "caida detectada\n");
	}

	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

/**
 * @fn sonarAlarma
 * @brief Función para manejar la alarma de buzzer
 *
 */
void sonarAlarma(void *pvParameter)
{
	if (alarmaCorta == true && alarmaLarga == false)
	{
		BuzzerOn();
		vTaskDelay(CONFIG_BLINK_PERIOD_Alarma1 / portTICK_PERIOD_MS);
		BuzzerOff();
		vTaskDelay(CONFIG_BLINK_PERIOD_Alarma1 / portTICK_PERIOD_MS);
	}
	else if (alarmaCorta == false && alarmaLarga == true)
	{
		BuzzerOn();
		vTaskDelay(CONFIG_BLINK_PERIOD_Alarma2 / portTICK_PERIOD_MS);
		BuzzerOff();
		vTaskDelay(CONFIG_BLINK_PERIOD_Alarma2 / portTICK_PERIOD_MS);
	}
	else if (alarmaCorta == true && alarmaLarga == true)
	{
		BuzzerOn();
		vTaskDelay(10000 / portTICK_PERIOD_MS); // 10 segundos de alarma para mostrar error
		BuzzerOff();
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
	else if (alarmaCorta == false && alarmaLarga == false)
		BuzzerOff(); // por las dudas apago el buffer para evitar sonidos por error
}

/**
 * @fn medirDistancia
 * @brief Función de tarea que mide la distancia utilizando el sensor ultrasónico.
 *
 */
void medirDistancia(void *pvParameter)
{

	// printf("inicio ciclo while del sensor \n \r");
	while (1)
	{

		// printf("asigno medicion a distancia \n \r");
		distancia = HcSr04ReadDistanceInCentimeters();

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/**
 * @fn mostrarDistancia
 * @brief
 *
 */
void mostrarDistancia(void *pvParameter)
{
	while (1)
	{

		if (distancia > 5000)
		{

			LedOn(LED_1);
			LedOff(LED_2);
			LedOff(LED_3);
			alarmaCorta = false;
			alarmaLarga = false;
		}
		else if (distancia > 3000)
		{

			LedOn(LED_1);
			LedOn(LED_2);
			LedOff(LED_3);
			alarmaCorta = false;
			alarmaLarga = true;
			UartSendString(UART_PC, "Precaución, vehículo cerca \n");
		}
		else
		{
			LedOn(LED_1);
			LedOn(LED_2);
			LedOn(LED_3);
			alarmaLarga = false;
			alarmaCorta = true;
			UartSendString(UART_PC, "PELIGRO! vehículo cerca \n");
		}

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/**
 * @fn Func_medir
 * @brief Función que activa la tarea de medir distancia desde el temporizador.
 *
 */
void Func_medir(void *param)
{
	vTaskNotifyGiveFromISR(medir_distancia_handle, pdFALSE);
}

/**
 * @fn Func_leds
 * @brief Función que activa la tarea de mostrar distancia desde el temporizador.
 *
 */
void Func_leds(void *param)
{
	vTaskNotifyGiveFromISR(mostrar_distancia_handle, pdFALSE);
}

/**
 * @fn Func_acel
 * @brief Función que activa la tarea de medir con el acelerómetro mediante temporizador.
 *
 */
void Func_acel(void *param)
{
	vTaskNotifyGiveFromISR(medir_aceleracion_handle, pdFALSE);
}

/*==================[external functions definition]==========================*/
/**
 * @fn app_main
 * @brief Función principal de la aplicación. Configura los periféricos, tareas y temporizadores.
 */
void app_main(void)
{
	timer_config_t timer_medir = {
		.timer = TIMER_A,
		.period = 500000,
		.func_p = Func_medir,
		.param_p = NULL};
	TimerInit(&timer_medir);

	timer_config_t timer_leds = {
		.timer = TIMER_B,
		.period = 250000,
		.func_p = Func_leds,
		.param_p = NULL};
	TimerInit(&timer_leds);

	timer_config_t timer_acel = {
		.timer = TIMER_C,
		.period = 10000,
		.func_p = Func_acel,
		.param_p = NULL};
	TimerInit(&timer_acel);

	UartInit(UART_PC);
	serial_config_t my_uart = {
		.port = UART_PC,
		.baud_rate = 115200,
		.func_p = NULL,
		.param_p = NULL};

	HcSr04Init(GPIO_3, GPIO_2);
	LedsInit();
	BuzzerInit(GPIO_9);
	AnalogInputInit(&eje_X);
	AnalogInputInit(&eje_Y);
	AnalogInputInit(&eje_Z);
	AnalogOutputInit();
	xTaskCreate(&medirDistancia, "midiendo distancia", 2048, NULL, 5, &medir_distancia_handle);
	xTaskCreate(&mostrarDistancia, "mostrando distancia", 512, NULL, 5, &mostrar_distancia_handle);
	xTaskCreate(&sonarAlarma, "beep beep", 512, NULL, 5, NULL);
	xTaskCreate(&medirAceleracion, "midiendo aceleracion", 2048, NULL, 5, &medir_aceleracion_handle);
	TimerStart(timer_medir.timer);
	TimerStart(timer_leds.timer);
	TimerStart(timer_acel.timer);
}
/*==================[end of file]============================================*/