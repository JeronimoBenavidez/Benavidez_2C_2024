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


gpioConf_t vec_pines_1[] = {{GPIO_20, GPIO_OUTPUT},{GPIO_21, GPIO_OUTPUT},{GPIO_22, GPIO_OUTPUT},{GPIO_23, GPIO_OUTPUT}};
gpioConf_t vec_pines_2[] = {{GPIO_19, GPIO_OUTPUT},{GPIO_18, GPIO_OUTPUT},{GPIO_9, GPIO_OUTPUT}};

/*==================[internal functions declaration]=========================*/
int8_t convertToBcdArray(uint32_t data, uint8_t digits, uint8_t *bcd_number)
{
	for (size_t j = 0; j < digits; ++j)
	{
		bcd_number[j] = data % 10;
		data = data / 10;
		
	}
	return(1);
}

void convertBCDToPINs(uint8_t digit, gpioConf_t *gpioPinConfig)
{
	
	uint8_t mask = 1;
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

void mostrarEnDisplay (uint32_t data, uint8_t digits, gpioConf_t *gpioPinConfig_1, gpioConf_t *gpioPinConfig_2){
	
	uint8_t vecAux[3];
	convertToBcdArray(data, digits, vecAux);
		for (size_t j = 0; j < 3; ++j)
	{
		GPIOInit(gpioPinConfig_2[j].pin, gpioPinConfig_2[j].dir);
	}
	for (size_t i = 0; i < 3; ++i)
	{
		convertBCDToPINs(vecAux[2-i], gpioPinConfig_1);
	
		GPIOOn(gpioPinConfig_2[i].pin);
		GPIOOff(gpioPinConfig_2[i].pin);
	}

}


/*==================[external functions definition]==========================*/
void app_main(void){
	uint32_t datos = 420;
	uint8_t digitos = 3;
	
	mostrarEnDisplay (datos, digitos,vec_pines_1, vec_pines_2);
	printf("Hello world!\n");
}
/*==================[end of file]============================================*/