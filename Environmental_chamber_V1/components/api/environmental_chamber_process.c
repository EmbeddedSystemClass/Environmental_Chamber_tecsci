
//Standard libraries
#include <stdio.h>
#include <stdbool.h>
//ESP libraries
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "sdkconfig.h" // generated by "make menuconfig"

//Bosch Sensor BME280 librarie
#include "bme280.h"
//Environmetal Chamber libraries
#include "sensor.h"
#include "humidifier.h"
#include "dryer.h"
#include "environmental_chamber_process.h"
#include "environmental_chamber_handlers.h"
#include "environmental_chamber_Control.h"
#include "environmental_chamber_monitor.h"
#include "custom_bool.h"

QueueHandle_t xQueue_Environmental_Chamber_Monitor;

//EMVIROMENTAL CHAMBER INIT FUNCTION

void EnvironmentalChamberInit(environmental_chamber_t *p_echamber){
	//obs: sacar todos los paeamtros que representen estado de retorno, yq que la comunicacion inversa va por otro flujo, "monitor_EC"

	p_echamber->targetRH=TARGET_RH_INIT_VALUE;
	p_echamber->targetTemp=TARGET_TEMP_INIT_VALUE;
	p_echamber->config.rh_activation=CHAMBER_RH_DEACTIVATED;
	p_echamber->config.temp_activation=CHAMBER_TEMP_DEACTIVATED;

//sensors init:
	sensor_RH_init();
//actuators init:
	dryer_init();
	humidifier_init();

//control task init:
	init_control_tasks();

	//Monitor Queue init:
	xQueue_Environmental_Chamber_Monitor = xQueueCreate(2, sizeof(environmental_chamber_monitor_data_t)); // el largo de la cola es 1
	if (xQueue_Environmental_Chamber_Monitor == NULL) {
		printf("No se pudo crear la COLA\n");
		while (1);/* se queda bloqueado el sistema hasta que venga el técnico de mantenmiento   */
	}

}

void EnvironmentalChamberSetRH(environmental_chamber_t *p_echamber,double rh_to_set){
	//check_rh_value(rh_to_set);
	p_echamber->targetRH=rh_to_set;
}

void EnvironmentalChamberSetTemp(environmental_chamber_t *p_echamber,double temp_to_set){
	//check_rh_value(temp_to_set);
	p_echamber->targetTemp=temp_to_set;
}

void EnvironmentalChamberActivateRH(environmental_chamber_t *p_echamber){
	p_echamber->config.rh_activation=CHAMBER_RH_ACTIVATED;
}

void EnvironmentalChamberDeactivateRH(environmental_chamber_t *p_echamber){
	p_echamber->config.rh_activation=CHAMBER_RH_DEACTIVATED;
}

void EnvironmentalChamberActivateTemp(environmental_chamber_t *p_echamber){
	p_echamber->config.temp_activation=CHAMBER_TEMP_ACTIVATED;
}

void EnvironmentalChamberDeactivateTemp(environmental_chamber_t *p_echamber){
	p_echamber->config.temp_activation=CHAMBER_TEMP_DEACTIVATED;
}

void EnvironmentalChamberRun(environmental_chamber_t *p_echamber){

		if(p_echamber->config.rh_activation == CHAMBER_RH_ACTIVATED || p_echamber->config.temp_activation==CHAMBER_TEMP_DEACTIVATED){
					Handler_EnvironmentalChamberRunProcess((cbool_t)p_echamber->config.rh_activation,
																								(cbool_t)p_echamber->config.temp_activation,
																								p_echamber->targetRH,
																								p_echamber->targetTemp);
	}

		return;
}

void EnvironmentalChamberStop(environmental_chamber_t *p_echamber){ //stop the running process and deactivates the activation flags

		// p_echamber->config.rh_activation=CHAMBER_RH_DEACTIVATED;
		// p_echamber->config.temp_activation=CHAMBER_TEMP_DEACTIVATED;

		Handler_EnvironmentalChamberStopProcess();

		return;

}

void EnvironmentalChamberMonitor(environmental_chamber_t *p_echamber){ //stop the running process and deactivates the activation flags

		environmental_chamber_monitor_data_t data;

		Handler_EnvironmentalChamberMonitor(&data.actualRH,&data.actualTemp,&data.RH_control_state,&data.RH_control_state);

		if( xQueueSendToBack(xQueue_Environmental_Chamber_Monitor,&data,10) != pdPASS ){
		    /* Failed to post the message, even after 10 ticks. */
		    printf("Failed to post the message in the xQueue_Environmental_Chamber_Monitor, even after 10 ticks\n");
		}

		return;
}
