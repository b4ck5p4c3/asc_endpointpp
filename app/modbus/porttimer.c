#include <port.h>
#include <mbport.h>
#include <periph.h>

static TIM_HandleTypeDef    htim14;
static uint16_t timeout     = 0;
static uint16_t downcounter = 0;

BOOL
xMBPortTimersInit(USHORT usTim1Timerout50us) {
	/*
	htim14.Instance               = MODBUS_TIMER;
	htim14.Init.Prescaler         = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;
	htim14.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim14.Init.Period            = 50 - 1;
	htim14.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	timeout = usTim1Timerout50us;

	if (!HAL_TIM_Base_Init(&htim14) == HAL_OK) {
		return FALSE;
	}

	HAL_NVIC_SetPriority(TIM14_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);
	*/

	return TRUE;
}

inline void
vMBPortTimersEnable()
{
	downcounter = timeout;
	HAL_TIM_Base_Start_IT(&htim14);
}

inline void
vMBPortTimersDisable() {
	HAL_TIM_Base_Stop_IT(&htim14);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance != MODBUS_TIMER) {
		return;
	}

	if (!--downcounter) {
		pxMBPortCBTimerExpired();
	}
}

void TIM14_IRQHandler(void) {
	HAL_TIM_IRQHandler(&htim14);
}
