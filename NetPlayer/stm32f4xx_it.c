//V1.0.0
#include "stm32f4xx_it.h"

//NMI exception handler
void NMI_Handler(void)
{
}

//Hard Fault exception handler
void HardFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//Memory Manage exception handler
void MemManage_Handler(void)
{
  	while (1)
  	{
  	}
}

//Bus Fault exception handler
void BusFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//Usage Fault exception handler
void UsageFault_Handler(void)
{
  	while (1)
  	{
  	}
}

//SVCall exception handler
void SVC_Handler(void)
{
}

//Debug Monitor exception handler
void DebugMon_Handler(void)
{
}

//PendSVC exception handler
void PendSV_Handler(void)
{
}

//SysTick handler
unsigned int __g_Timer=0;

void SysTick_Handler(void)
{
  __g_Timer++;
}

uint32_t TimeGetTime(void)
{
  return __g_Timer;
}

void TimerInit(void)
{
   if(SysTick_Config(SystemCoreClock / 1000))
   {
    while(1);
   }
}

void sleepms(uint32_t ms)
{
 uint32_t CurrentTime=TimeGetTime();
 while(TimeGetTime()-CurrentTime<ms);
}

