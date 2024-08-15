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
 * | 12/09/2023 | Document creation		                         |
 *
 * @author Albano Peñalva (albano.penalva@uner.edu.ar)
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


gpioConf_t vec_pines[] = {{GPIO_20, GPIO_OUTPUT},{GPIO_21, GPIO_OUTPUT},{GPIO_22, GPIO_OUTPUT},{GPIO_23, GPIO_OUTPUT}};

/*==================[internal functions declaration]=========================*/
void convertBCDToPINs(uint8_t digit, gpioConf_t *gpioPinConfig)
{
	
	uint8_t mask =1;
	for (size_t j = 0; j < 4; ++j)
	{
		GPIOInit(gpioPinConfig[j].pin, gpioPinConfig[j].dir);
	}
	for (size_t i = 0; i<4; ++i)
	{
		if((mask & digit) != 0 ){
			GPIOOn(gpioPinConfig[i].pin);
		} else {
			GPIOOff(gpioPinConfig[i].pin);
		}
		mask = mask << 1;
	}
}
/*==================[external functions definition]==========================*/
void app_main(void)
{

	convertBCDToPINs (5, vec_pines);
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/