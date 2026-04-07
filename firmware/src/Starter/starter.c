#define _EXPOSURE_C

#include "definitions.h"         
#include "starter.h"
#include "application.h"

void STARTER_Initialization(void){
    STARTER_Off();
    STARTER_SetFreeRun();
    STARTER_resetFault();    
}

void STARTER_Off(void){
    STARTER_RUN_Clear();
}
void STARTER_ActivateLow(void){
    STARTER_SPEED_Clear();
}
void STARTER_ActivateHigh(void){
    STARTER_SPEED_Set();
}
void STARTER_SetFreeRun(void){
   STARTER_FREERUN_Set(); 
}
void STARTER_SetSpeedControl(void){
    STARTER_FREERUN_Clear();
}
bool STARTER_isSpeedOk(void){
    return STARTER_SPEEDOK_Get();
}
bool STARTER_isFault(void){
    return STARTER_FAULT_Get();
}
void STARTER_resetFault(void){
    
}

