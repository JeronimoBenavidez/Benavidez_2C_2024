/*! @mainpage Ejercicio 5 de la guia de trabajo:proyecto 1
 *
 * @section muestro dígito en display
 *
 * Creo una función para mostrar dígitos en un display utilizando estructuras
 * del tipo gpioConf_t
 * 
 * Por ejemplo: si recibo el dígito 2, muestro en display el número "2".
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
 * | 22/08/2024 | Modificaciones de la guia sobre el codigo original |
 * | 28/08/2024 | Documentación		                                 |
 *
 * @author Benavidez Jerónimo (Benavidez.Jeronimo@ingenieria.uner.edu.ar)
 *
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include "gpio_mcu.h"    /* incluyo la libreria gpio_mcu para usar las salidas de la placa*/

/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct
{
	gpio_t pin; /*!< GPIO pin number */
	io_t dir;	/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;

/** @def vec_pines
*	@brief Creo un vector de pines para posteriormente asignar cada dígito del binario que corresponde
*/
gpioConf_t vec_pines[] = {{GPIO_20, GPIO_OUTPUT},				
						{GPIO_21, GPIO_OUTPUT},					
						{GPIO_22, GPIO_OUTPUT},					
						{GPIO_23, GPIO_OUTPUT}};  

/*==================[internal functions declaration]=========================*/
void convertBCDToPINs(uint8_t digit, gpioConf_t *gpioPinConfig)	/* Creo la funcion para la conversion*/
{
	
	uint8_t mask =1;	/* creo una máscara para comparar y decidir si pongo en alto o bajo la salida*/
	for (size_t j = 0; j < 4; ++j)
	{
		GPIOInit(gpioPinConfig[j].pin, gpioPinConfig[j].dir);	/* Inicializo las salidas GPIO */
	}
	for (size_t i = 0; i<4; ++i)
	{
		if((mask & digit) != 0 ){						/*Comparo y decido si mandar 1 o 0*/
			GPIOOn(gpioPinConfig[i].pin);
		} else {
			GPIOOff(gpioPinConfig[i].pin);
		}
		mask = mask << 1;
	}
}
/*==================[external functions definition]==========================*/
void app_main(void)						/* pruebo la función*/
{

	convertBCDToPINs (5, vec_pines);
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/