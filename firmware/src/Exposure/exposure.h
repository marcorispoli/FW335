#ifndef _EXPOSURE_H    
#define _EXPOSURE_H



#undef ext
#undef ext_static

#ifdef _EXPOSURE_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif

typedef enum{
    EXPMODE_IDLE = 0,
    EXPMODE_TEST_TMO,
}_EXPOSURE_MODES;

ext_static _EXPOSURE_MODES exposure_mode;
ext_static void HVREF_Initialization();
ext_static void HVREF_Set(unsigned short value);


ext void ExposureInit(void);


#endif  

