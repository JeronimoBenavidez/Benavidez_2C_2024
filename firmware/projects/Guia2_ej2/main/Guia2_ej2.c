/*! @mainpage Ejercicio 1 Guia de trabajo: proyecto 2
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
#include "hc_sr04.h"
#include "lcditse0803.h"
#include "led.h"
#include "switch.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data definition]===============================*/
TaskHandle_t medir_distancia = NULL;       
TaskHandle_t leer_teclas = NULL;       
TaskHandle_t mostrar_distancia = NULL;
uint16_t distancia;
int8_t tecla;
bool midiendo= true;
bool congelarPantalla = false;
/*==================[internal functions declaration]=========================*/
static void medirDistancia(void *pvParameter) {
	
	printf("inicio ciclo while del sensor \n \r" );
	while(1){
	//	printf("mido \n \r" );
		if (midiendo){
			//printf("asigno medicion a distancia \n \r");
		distancia=HcSr04ReadDistanceInCentimeters();
		 
	} else {};
	vTaskDelay(1000/ portTICK_PERIOD_MS);}
}

static void leerTeclas(void *pvParameter){
	
	printf("inicio ciclo while de teclas \n \r" );
	while(1){
	//printf("inicializo el switch \n \r" );
	            
	printf("me meto en switch de teclas \n \r" );     
	  	switch(SwitchesRead()){									
    		case SWITCH_1:
			//printf("cambio variable midiendo \n \r" );
			midiendo = !midiendo;
    		break;
    		case SWITCH_2:
			//printf("cambio variable congelarPantalla \n \r" );
    		congelarPantalla = !congelarPantalla;	
    		break;
			
}
			vTaskDelay(100/ portTICK_PERIOD_MS);
}}

static void mostrarDistancia(void *pvParameter){
	
	//printf("entro en el while de distancia \n \r" );
	while(1){
		//printf("entro en el id de leds si midiendo = true \n \r" );
		if(midiendo){
		//printf("entro en el if de led si distancia <10 \n \r" );	
			if(distancia<10){
				//printf("entré en el if de led si distancia <10 \n \r" );
				LedOff(LED_1);
				LedOff(LED_2);
				LedOff(LED_3);
			} else if(distancia < 20){
				//printf("entré en el if de led si distancia <10 \n \r" );
					LedOn(LED_1);
					LedOff(LED_2);
					LedOff(LED_3);
				} else if (distancia < 30){
				//	printf("entré en el if de led si distancia <10 \n \r" );
					LedOn(LED_1);
					LedOn(LED_2);
					LedOff(LED_3);
					} else {LedOn(LED_1);LedOn(LED_2);LedOn(LED_3);}
		}
		//printf("entro en el if del display \n \r");
		if(!congelarPantalla){
			//printf("entré en el if del display porque congelarPantalla = false \n \r" );
		//	printf("inicializo LCD \n \r ");
			LcdItsE0803Write(distancia);
		}
	vTaskDelay(100/ portTICK_PERIOD_MS); }
}
/*==================[external functions definition]==========================*/
void app_main(void){
	printf("inicializo sensor HcSr04 \n \r" );
	HcSr04Init(GPIO_3, GPIO_2);
	printf("inicializo el switch \n \r");
	SwitchesInit();
	printf("inicializo pantalla \n \r" );
	LcdItsE0803Init();
	printf("inicializo leds \n \r" );
	LedsInit();
	xTaskCreate(&medirDistancia, "midiendo distancia", 2048, NULL, 5, NULL);   
    xTaskCreate(&leerTeclas, "leyendo teclas", 2048, NULL, 5,  NULL );  
    xTaskCreate(&mostrarDistancia, "mostrando distancia", 512, NULL, 5, NULL);   
}
/*==================[end of file]============================================*/