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

/*==================[macros and definitions]=================================*/


/*==================[internal data definition]===============================*/

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
/*==================[external functions definition]==========================*/
void app_main(void)
{
	uint32_t dato = 12123455;
	uint8_t digitos = 8 ;
	uint8_t bcd_numero[8] = {0,0,0,0,1,1,1,1};
	
	for (size_t i = 0; i<8 ; ++i)
	{
		printf(bcd_numero[i]);
	}
	printf("' '");
	convertToBcdArray(dato, digitos, bcd_numero);
	for (size_t i = 0; i<8 ; ++i)
	{
		printf(bcd_numero[i]);
	}
	printf("' '");



	printf("Hola mundo!\n");
}
/*==================[end of file]============================================*/