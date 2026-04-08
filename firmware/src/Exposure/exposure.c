#define _EXPOSURE_C

#include "definitions.h"         
#include "exposure.h"
#include "application.h"

void HVREF_Initialization(void){
    HVREF_LD_Clear(); // Set to load the data output 
    HVREF_CLR_Set();  // Set to Clear the data output 
    SPI_CS_Set();     // Sets the CS high  
    HVREF_CLR_Clear();  // Set to Clear the data output
    
    HVREF_Set(0);
}


void HVREF_Set(unsigned short value){
    unsigned char spi_data[2];
    
    if(value > 4095) value = 4095;    
    spi_data[1] = value & 0xff;
    spi_data[0] = (value >> 8) & 0xff;
    
    HVREF_LD_Clear();
    SPI_CS_Clear();
    SERCOM4_SPI_Write(spi_data, 2);
    SPI_CS_Set();
    HVREF_LD_Set();
}

void ExposureInit(void){
    HVREF_Initialization();
    exposure_mode = EXPMODE_IDLE;
}

void ExposureExecutionLoop(void){
    
    if(exposure_mode == EXPMODE_IDLE){
        
    }
}