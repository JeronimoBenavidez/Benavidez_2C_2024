/*! @mainpage Ejercicio 4 de la guia de trabajo: proyecto 1
 *
 * @section convierto de BDC a Array
 *
 * El programa recibe un dato BCD de 32 bits y lo convierto en un arreglo de n dígitos
 * 
 * Por ejemplo: si recibo el número 5867990, creo un array [5,8,6,7,9,9,0]
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
 * | 15/08/2024 | Modificaciones de la guia sobre el codigo original |
 * | 28/08/2024 | Documentación		                                 |
 *
 * @author Benavidez Jerónimo (Benavidez.Jeronimo@ingenieria.uner.edu.ar)
 *
 */
/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>

/*==================[macros and definitions]=================================*/


/*==================[internal data definition]===============================*/

/*==================[internal functions declaration]=========================*/
int8_t convertToBcdArray(uint32_t data, uint8_t digits, uint8_t *bcd_number) /* creo la función*/
{
	for (size_t j = 0; j < digits; ++j)						/* Hago un ciclo que depende de la cantidad*/
	{														/*de dígitos.                              */
		bcd_number[j] = data % 10;							/* Utilizo la operación resto por 10 para  */
		data = data / 10;									/*para extraer cada dígito del valor base10*/
															/*y luego modifico data con el siguiente digito.*/
	}
	return(1);
}
/*==================[external functions definition]==========================*/
void app_main(void)											/* prueba de la función  */
{
	uint32_t dato = 12123455;
	uint8_t digitos = 8 ;
	uint8_t bcd_numero[8] = {0,0,0,0,1,1,1,1};
	
	for (size_t i = 0; i<8 ; ++i)
	{
		printf("valor: %d", bcd_numero[i]);
	}
	printf(" ");
	convertToBcdArray(dato, digitos, bcd_numero);
	for (size_t i = 0; i<8 ; ++i)
	{
		printf("valor: %d", bcd_numero[i]);
	}
	printf(" ");



	printf("Hola mundo!\n");
}
/*==================[end of file]============================================*/