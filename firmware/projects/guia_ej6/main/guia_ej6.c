/*! @mainpage ejercicio 6 de la guia de trabajo:protecto 1
 *
 * @section Muestro en display número de 3 dígitos
 *
 * Recibo un número de 3 dígitos y lo muetro en un display
 *
 * Por ejemplo: si recibo el número "421", en el display debe leerse "421"
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
#include "gpio_mcu.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
typedef struct
{
	gpio_t pin; /*!< GPIO pin number */
	io_t dir;	/*!< GPIO direction '0' IN;  '1' OUT*/
} gpioConf_t;


gpioConf_t vec_pines_1[] = {{GPIO_20, GPIO_OUTPUT},{GPIO_21, GPIO_OUTPUT},		/*!<Creo vector de pines para el dígito*/
						{GPIO_22, GPIO_OUTPUT},{GPIO_23, GPIO_OUTPUT}};
gpioConf_t vec_pines_2[] = {{GPIO_19, GPIO_OUTPUT},{GPIO_18, GPIO_OUTPUT},		/*!<Creo vector de pines para asignar la posición del dígito*/
						{GPIO_9, GPIO_OUTPUT}};									

/*==================[internal functions declaration]=========================*/
int8_t convertToBcdArray(uint32_t data, uint8_t digits, uint8_t *bcd_number)	/* creo función para extraer */
{																				/* los dígitos del número	 */
	for (size_t j = 0; j < digits; ++j)								/* Hago un ciclo que depende de la cantidad*/
	{																/*de dígitos.                              */
		bcd_number[j] = data % 10;									/* Utilizo la operación resto por 10 para  */
		data = data / 10;											/*para extraer cada dígito del valor base10*/
																	/*y luego modifico data con el siguiente digito.*/
	}
	return(1);
}

void convertBCDToPINs(uint8_t digit, gpioConf_t *gpioPinConfig)			/* Convierto los valores BCD en salidas*/
																		/* para el display                     */
{
	
	uint8_t mask = 1;			/* creo una máscara para comparar y decidir si pongo en alto o bajo la salida*/
	for (size_t j = 0; j < 4; ++j)		/* Inicializo las salidas GPIO */
	{
		GPIOInit(gpioPinConfig[j].pin, gpioPinConfig[j].dir);
	}
	for (size_t i = 0; i<4; ++i)			/*Comparo y decido si mandar 1 o 0*/
	{
		if((mask & digit) != 0 ){
			GPIOOn(gpioPinConfig[i].pin);
		} else {
			GPIOOff(gpioPinConfig[i].pin);
		}
		mask = mask << 1; 	/* corro la máscara para la siguiente comparación*/
	}
}

void mostrarEnDisplay (uint32_t data, uint8_t digits,             /* Creo una función para mostrar un número*/
 gpioConf_t *gpioPinConfig_1, gpioConf_t *gpioPinConfig_2){		  /* pasado como parámetro en un display    */
	
	uint8_t vecAux[3];			/*Creo un vector auxiliar para asignar los valores del arreglo*/
	convertToBcdArray(data, digits, vecAux);	/*Convierto el valor data en un array*/
		for (size_t j = 0; j < 3; ++j)
	{
		GPIOInit(gpioPinConfig_2[j].pin, gpioPinConfig_2[j].dir);	/* inicilizo las compuertas de selección*/
	}
	for (size_t i = 0; i < 3; ++i)
	{
		convertBCDToPINs(vecAux[2-i], gpioPinConfig_1);	/* convierto el valor BCD en entradas para el display*/
	
		GPIOOn(gpioPinConfig_2[i].pin);	/* mando un pulso a las compuertas de selección para asignar el valor */
		GPIOOff(gpioPinConfig_2[i].pin);/* que corresponde                                                    */
	}

}


/*==================[external functions definition]==========================*/
void app_main(void){				/* Creo un código de prueba*/
	uint32_t datos = 420;			/* Asigno un valor arbitrario, solo para probar*/
	uint8_t digitos = 3;
	
	mostrarEnDisplay (datos, digitos,vec_pines_1, vec_pines_2);
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/