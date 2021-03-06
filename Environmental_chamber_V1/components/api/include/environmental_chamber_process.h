#ifndef _ENVIRONMENTAL_CHAMBER_PROCESS_H_
#define _ENVIRONMENTAL_CHAMBER_PROCESS_H_

#include <stdio.h>
#include <stdint.h>
#include "custom_bool.h"

/*DEFINED VALUES*/
#define _FLAG_ERROR          	1<< 8
#define TARGET_RH_INIT_VALUE 70 //%RH
#define TARGET_TEMP_INIT_VALUE 25 //°C

/*DEFINED STRUCTS*/
typedef enum{CHAMBER_RH_DEACTIVATED=0, CHAMBER_RH_ACTIVATED=1} chamberRHactivation_t;
typedef enum{CHAMBER_TEMP_DEACTIVATED=0, CHAMBER_TEMP_ACTIVATED=1} chamberTEMPactivation_t;

typedef struct {
	chamberRHactivation_t rh_activation;
	chamberTEMPactivation_t temp_activation;
} chamberConfig_t;

typedef struct {
	double targetRH; //o hacer  struct target_t y struct actualState_t  ?
	double targetTemp;
	chamberConfig_t config;
} environmental_chamber_t;


//FUNCTION DECLARATION
void EnvironmentalChamberInit(environmental_chamber_t *p_echamber);
void EnvironmentalChamberSetRH(environmental_chamber_t *p_echamber,double rh_to_set);
void EnvironmentalChamberSetTemp(environmental_chamber_t *p_echamber,double temp_to_set);
void EnvironmentalChamberActivateRH(environmental_chamber_t *p_echamber);
void EnvironmentalChamberDeactivateRH(environmental_chamber_t *p_echamber);
void EnvironmentalChamberActivateTemp(environmental_chamber_t *p_echamber);
void EnvironmentalChamberDeactivateTemp(environmental_chamber_t *p_echamber);
void EnvironmentalChamberRun(environmental_chamber_t *p_echamber);
void EnvironmentalChamberStop(environmental_chamber_t *p_echamber);
void EnvironmentalChamberMonitor(environmental_chamber_t *p_echamber);

#endif
