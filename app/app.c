#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "modbus.h"
#include "registers.h"

extern UART_HandleTypeDef huart1;

// pin init helper
typedef enum {
    GpioModeInput,
    GpioModeOutput,
} GpioMode;

typedef struct {
    GPIO_TypeDef* port;
    uint32_t pin;
} GpioPin;

typedef struct {
    GpioPin gpio;
    GpioPin pull;
} IoLine;

void app_gpio_init(GpioPin gpio, GpioMode mode) {
    if(gpio.pin != 0) {
        GPIO_InitTypeDef GPIO_InitStruct;

        GPIO_InitStruct.Pin = gpio.pin;
        GPIO_InitStruct.Pull = GPIO_NOPULL;

        switch(mode) {
            case GpioModeInput:
                GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            break;

            case GpioModeOutput: 
                GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
                GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
            break;
        }

        HAL_GPIO_Init(gpio.port, &GPIO_InitStruct);
    }
}

void app_gpio_write(GpioPin gpio, bool state) {
    if(gpio.pin != 0) {
        HAL_GPIO_WritePin(gpio.port, gpio.pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

IoLine BOARD_PINS[] = {
    // dummy IO0
    {.gpio = {.pin = 0, .port = (GPIO_TypeDef*)0}, .pull = {.pin = 0, .port = (GPIO_TypeDef*)0}},

    {.gpio = {.port = GPIOA, .pin = GPIO_PIN_0}, .pull = {.port = GPIOA, .pin = GPIO_PIN_1}},
    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_2}, .pull = {.port = GPIOC, .pin = GPIO_PIN_3}},
    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_0}, .pull = {.port = GPIOC, .pin = GPIO_PIN_1}},
    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_14}, .pull = {.port = GPIOC, .pin = GPIO_PIN_15}},

    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_9}, .pull = {.port = GPIOC, .pin = GPIO_PIN_13}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_7}, .pull = {.port = GPIOB, .pin = GPIO_PIN_8}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_5}, .pull = {.port = GPIOB, .pin = GPIO_PIN_6}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_3}, .pull = {.port = GPIOB, .pin = GPIO_PIN_4}},

    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_12}, .pull = {.port = GPIOD, .pin = GPIO_PIN_2}},
    {.gpio = {.pin = 0, .port = (GPIO_TypeDef*)0}, .pull = {.pin = 0, .port = (GPIO_TypeDef*)0}},
    {.gpio = {.port = GPIOA, .pin = GPIO_PIN_11}, .pull = {.port = GPIOA, .pin = GPIO_PIN_15}},
    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_9}, .pull = {.port = GPIOA, .pin = GPIO_PIN_8}},

    {.gpio = {.port = GPIOC, .pin = GPIO_PIN_7}, .pull = {.port = GPIOC, .pin = GPIO_PIN_8}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_15}, .pull = {.port = GPIOC, .pin = GPIO_PIN_6}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_13}, .pull = {.port = GPIOB, .pin = GPIO_PIN_14}},
    {.gpio = {.port = GPIOB, .pin = GPIO_PIN_1}, .pull = {.port = GPIOB, .pin = GPIO_PIN_12}},
};

osThreadId ledTaskHandle;
void led_task(void const * argument);

extern uint8_t receive_buf[1];

void app() {
    printf("=== Endpoint ASC B4CKSP4CE ===\n");

    osThreadDef(ledTask, led_task, osPriorityNormal, 0, 128);
    ledTaskHandle = osThreadCreate(osThread(ledTask), NULL);

    modbus_init();

    while(1) {
        modbus_poll();
        osDelay(1);
    }
}

extern uint32_t test_var;

void led_task(void const* argument) {

    for(size_t i = 1; i < sizeof(BOARD_PINS)/sizeof(BOARD_PINS[0]); i++) {
        app_gpio_init(BOARD_PINS[i].gpio, GpioModeOutput);
    }

    while(1) {
        for(size_t i = 1; i < sizeof(BOARD_PINS)/sizeof(BOARD_PINS[0]); i++) {
            app_gpio_write(BOARD_PINS[i].gpio, true);
            osDelay(30);
        }

        for(size_t i = 1; i < sizeof(BOARD_PINS)/sizeof(BOARD_PINS[0]); i++) {
            app_gpio_write(BOARD_PINS[i].gpio, false);
            osDelay(30);
        }

        // printf("test: %d\n", test_var);

        /*
        vMBPortSerialEnable(0, 1);
        xMBPortSerialPutByte('a');
        osDelay(2);
        vMBPortSerialEnable(1, 0);
        */
    }
}