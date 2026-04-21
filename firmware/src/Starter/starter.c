#define _EXPOSURE_C

#include "definitions.h"         
#include "starter.h"
#include "application.h"

#define LOW_SPEED_ACTIVATION_TMO_MS  5000 // % second maximum activation time
#define HIGH_SPEED_ACTIVATION_TMO_MS 5000 // % second maximum activation time

// Gets the counts for a ms timer. Based on the STARTER_xx_Loop periodic time
#define  LOOP_PERIOD 15
#define  MS_COUNTS(x) ((x/LOOP_PERIOD)+1) // 15ms Periodic  

#define STARTER_COMMAND_OFF                 0
#define STARTER_COMMAND_ACTIVATING_LOW      1
#define STARTER_COMMAND_ACTIVATED_LOW       2
#define STARTER_COMMAND_ACTIVATING_HIGH     3
#define STARTER_COMMAND_ACTIVATED_HIGH      4
#define STARTER_COMMAND_RESET_FAULT         5


static int current_starter_command;
static int current_sequence;
static int current_sequence_timer;

void STARTER_Initialization(void){
    current_starter_command = STARTER_COMMAND_OFF;
    current_sequence = 0;
    
    STARTER_Off();
    
}
void STARTER_Activating_Low_Procedure(){
    switch(current_sequence){
        // Reset Fault Procedure
        case 0:
            STARTER_RUN_Clear();
            STARTER_FREERUN_Clear();
            STARTER_SPEED_Clear(); // Low Speed Setting    
            
            // Reset Fault condition 
            if(STARTER_FAULT_Get()){                
                current_sequence_timer = MS_COUNTS(100);                
                current_sequence++;
                break;
            }
            
            // No error
            current_sequence = 3; // Activates the sequence
            break;
            
        case 1: // Reset Fault procedure
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            STARTER_FREERUN_Set();
            current_sequence_timer = MS_COUNTS(100);                
            current_sequence++;
            break;
            
        case 2: // Verify Reset Fault Condition
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            // The Reset fault condition failed:
            // End Sequence in Fault condition
            if(STARTER_FAULT_Get()){
                STARTER_Off();
                return;
            }
            
            STARTER_FREERUN_Clear();
            current_sequence++;
            break;
            
        case 3: // Activate the sequence
            STARTER_RUN_Set();
            current_sequence_timer = MS_COUNTS(100);  
            current_sequence++;
            break;
            
        case 4: // Wait a minimum time before to monitor the activation
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            current_sequence_timer = MS_COUNTS(LOW_SPEED_ACTIVATION_TMO_MS);  
            current_sequence++;
            break;
        
        case 5:
            if(current_sequence_timer){
                current_sequence_timer--;
                
                // Timeout Activation
                if(!current_sequence_timer){
                    STARTER_Off();
                    return;
                }                
            }
            
            // Wait for the Speed OK or Fault Condition
            if(STARTER_FAULT_Get()){
                STARTER_Off();
                return;
            }
            
            // Fine attivazione con successo
            if(STARTER_SPEEDOK_Get()){
                current_sequence = 0;
                current_starter_command = STARTER_COMMAND_ACTIVATED_LOW;
                return;
            }
                        
            break;               
    }
    
    return;
}

void STARTER_Activating_High_Procedure(){
    switch(current_sequence){
        // Reset Fault Procedure
        case 0:
            // Reset Fault condition 
            if(STARTER_FAULT_Get()){                
                STARTER_RUN_Clear();
                STARTER_FREERUN_Clear();
                current_sequence_timer = MS_COUNTS(100);                
                current_sequence++;
                break;
            }
            
            // No error
            STARTER_FREERUN_Clear();
            STARTER_SPEED_Set(); // High Speed Setting    
            current_sequence = 3; // Activates the sequence
            break;
            
        case 1: // Reset Fault procedure
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            STARTER_FREERUN_Set();
            current_sequence_timer = MS_COUNTS(100);                
            current_sequence++;
            break;
            
        case 2: // Verify Reset Fault Condition
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            // The Reset fault condition failed:
            // End Sequence in Fault condition
            if(STARTER_FAULT_Get()){
                STARTER_Off();
                return;
            }
            
            STARTER_FREERUN_Clear();
            current_sequence++;
            break;
            
        case 3: // Activate the sequence
            STARTER_RUN_Set();
            current_sequence_timer = MS_COUNTS(100);  
            current_sequence++;
            break;
            
        case 4: // Wait a minimum time before to monitor the activation
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            current_sequence_timer = MS_COUNTS(HIGH_SPEED_ACTIVATION_TMO_MS);  
            current_sequence++;
            break;
        
        case 5:
            if(current_sequence_timer){
                current_sequence_timer--;
                
                // Timeout Activation
                if(!current_sequence_timer){
                    STARTER_Off();
                    return;
                }                
            }
            
            // Wait for the Speed OK or Fault Condition
            if(STARTER_FAULT_Get()){
                STARTER_Off();
                return;
            }
            
            // Fine attivazione con successo
            if(STARTER_SPEEDOK_Get()){
                current_sequence = 0;
                current_starter_command = STARTER_COMMAND_ACTIVATED_HIGH;
                return;
            }
                        
            break;               
    }
    
    return;

}

void STARTER_Reset_Fault_Procedure(){
    switch(current_sequence){
        // Reset Fault Procedure
        case 0:
            STARTER_RUN_Clear();
            STARTER_FREERUN_Clear();

            current_sequence_timer = MS_COUNTS(100);                
            current_sequence++;
            break;
            
        case 1: // Reset Fault procedure
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            STARTER_FREERUN_Set();
            current_sequence_timer = MS_COUNTS(100);                
            current_sequence++;
            break;
            
        case 2: // Verify Reset Fault Condition
            if(current_sequence_timer){
                current_sequence_timer--;
                break;
            }
            
            STARTER_Off();
            break;
      
    }
    
    return;
}

void STARTER_15ms_Loop(){
    switch(current_starter_command){
        
        case STARTER_COMMAND_OFF:            
            break;
            
        case STARTER_COMMAND_ACTIVATING_LOW:
            STARTER_Activating_Low_Procedure();
            break;
            
        case STARTER_COMMAND_ACTIVATING_HIGH:
            STARTER_Activating_High_Procedure();
            break;
            
        case STARTER_COMMAND_RESET_FAULT:
            STARTER_Reset_Fault_Procedure();
            break;
        
        case STARTER_COMMAND_ACTIVATED_LOW:
        case STARTER_COMMAND_ACTIVATED_HIGH:
            if(STARTER_FAULT_Get()){
                STARTER_Off();                
            }
            break;
        
    }
}

void STARTER_Off(void){
    // Clear the Run and Ready to reset a possible Fault condition
    STARTER_RUN_Clear();
    STARTER_FREERUN_Clear();
    current_starter_command = STARTER_COMMAND_OFF;
    current_sequence_timer = 0;
    current_sequence = 0;
    return;
}

void STARTER_ActivateLow(void){
    
    // If a running procedure is executing, no more action is required
    if(current_starter_command == STARTER_COMMAND_ACTIVATING_LOW) return;
    if(current_starter_command == STARTER_COMMAND_ACTIVATED_LOW)  return;
    if(current_starter_command == STARTER_COMMAND_ACTIVATING_HIGH) return;
    if(current_starter_command == STARTER_COMMAND_ACTIVATED_HIGH)  return;
    
    current_starter_command = STARTER_COMMAND_ACTIVATING_LOW;
    current_sequence = 0;
    
}

void STARTER_ActivateHigh(void){
    // If a running procedure is executing, no more action is required
    if(current_starter_command == STARTER_COMMAND_ACTIVATING_HIGH) return;
    if(current_starter_command == STARTER_COMMAND_ACTIVATED_HIGH)  return;
    
    current_starter_command = STARTER_COMMAND_ACTIVATING_HIGH;
    current_sequence = 0;
}

void STARTER_resetFault(void){
    if(current_starter_command != STARTER_COMMAND_OFF)  return;
    
    if(!STARTER_FAULT_Get()){
        STARTER_Off();
        return;
    }
    
    current_starter_command = STARTER_COMMAND_RESET_FAULT;
    current_sequence = 0;
}

void STARTER_SetFreeRun(bool state){
   if(state) STARTER_FREERUN_Set(); 
   else STARTER_FREERUN_Clear(); 
}
bool STARTER_isSpeedOk(void){
    return STARTER_SPEEDOK_Get();
}
bool STARTER_isFault(void){
    return STARTER_FAULT_Get();
}



