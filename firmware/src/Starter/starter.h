#ifndef _STARTER_H    
#define _STARTER_H



#undef ext
#undef ext_static

#ifdef _STARTER_C
    #define ext
    #define ext_static static 
#else
    #define ext extern
    #define ext_static extern
#endif



ext void STARTER_Initialization(void);
ext void STARTER_Off(void);
ext void STARTER_ActivateLow(void);
ext void STARTER_ActivateHigh(void);
ext void STARTER_SetFreeRun(void);
ext void STARTER_SetSpeedControl(void);
ext bool STARTER_isSpeedOk(void);
ext bool STARTER_isFault(void);
ext void STARTER_resetFault(void);



#endif  

