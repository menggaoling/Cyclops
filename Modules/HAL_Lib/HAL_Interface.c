#include "HAL_Interface.h"

void Hal_System_Initial(void)
{
  Hal_BSP_Initial();
  HAL_Set_ERP_Power(ON);
}
