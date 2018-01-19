#ifndef _Formula_H_
#define _Formula_H_

#include "HAL_BSP.h"

#define _TM_   1
#define _Bike_ 2

DWORD Formula_Calories(UCHAR by_Type,DWORD by_Cal_Speed,DWORD by_Cal_Incline);
USHORT Formula_HourCalories(DWORD by_Cal_Weight,DWORD by_Bu);
USHORT Formula_SecCalories(DWORD by_Cal_Weight,DWORD by_Bu);
USHORT Formula_BMI(UCHAR by_Type,UCHAR _Unit,USHORT _Weight,USHORT _Height);
USHORT Formula_VO2(UCHAR by_Type,USHORT _ACCtime,USHORT _BMI);
USHORT Formula_METs(UCHAR by_Type,UCHAR _Unit,USHORT _Speed,USHORT _Incline);


#endif
