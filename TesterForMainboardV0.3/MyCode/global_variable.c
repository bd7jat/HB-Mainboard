/* exact-width signed integer types 
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __INT64 int64_t;
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __INT64 uint64_t;*/

#include "stdint.h"
#include "..\MyCode\global_variable.h"

//variable for uart communication


uint32_t AdcTank[32];
uint8_t BoardReveivedData[50];
uint8_t LoadTestReference;				
PeripheralState LedBeeper;							
AD_parameter    AdcQueue;
flag Signal;
TestState TestStatus;
BoardContent BoardControl;

