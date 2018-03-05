1.IAR 7.2编译启动文件startup_stm32f10x_cl.s的警告可以这样排除，把启动文件中的“REORDER”
全部替换为“REORDER:NOROOT”.
2.由于Cyclops硬体给出的外部晶振是8M，而手册要求的默认外部晶振为25M，
详情可参见system_stm32f10x.c第33行的注释。
  * 4. The default value of HSE crystal is set to 8 MHz (or 25 MHz, depedning on
  *    the product used), refer to "HSE_VALUE" define in "stm32f10x.h" file. 
  *    When HSE is used as system clock source, directly or through PLL, and you
  *    are using different crystal you have to adapt the HSE value to your own
  *    configuration.
因此修改了stm32f10x.h中HSE_VALUE的默认值。
//  #define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
   #define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */

另外还需修改system_stm32f10x.c中1037行左右把
//    RCC->CFGR2 |= (uint32_t)(RCC_CFGR2_PREDIV2_DIV5 | RCC_CFGR2_PLL2MUL8 |
//                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);
修改为：
    RCC->CFGR2 |= (uint32_t)( RCC_CFGR2_PREDIV2_DIV2 |RCC_CFGR2_PLL2MUL10 |
                             RCC_CFGR2_PREDIV1SRC_PLL2 | RCC_CFGR2_PREDIV1_DIV5);							
因为注释中说 默认外部晶振25M，即HSE=25M，要使得PREDIV1CLK = 8M，
现在外部晶振是8M，即HSE=8M，(HSE / 2) * 10 = 40MHz,即可。
   /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
    /* PREDIV1 configuration: PREDIV1CLK = PLL2 / 5 = 8 MHz */	
2018/1/5 Empty project build, The STM32 minimum MCU system complete with IAR 7.2.
2018/1/29 Cyclops K1 software finished
