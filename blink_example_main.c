#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_system.h"
#include "freertos/queue.h"

xQueueHandle q;

#define BLINK_GPIO 2
 #define INPUT_GPIO_NO 34
 unsigned int GPIOlevel; 
 
void blink_task(void *pvParameter)
{
 
	while(1)
	{
	    printf("Hello world!\n");
        xQueueReceive(q,&GPIOlevel,(TickType_t )10); 
        if(xQueueReceive(q,&GPIOlevel,(TickType_t )10) ==pdTRUE)
        {
        gpio_set_level(BLINK_GPIO,GPIOlevel);
	    vTaskDelay(100 / portTICK_RATE_MS);
        }
        else
        {
         printf("not recieved");
        }
	}
}
 
void button_task(void *pvParameter)
{
 ////////////////led//////////////
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    //////////////button///////////////
    gpio_pad_select_gpio(INPUT_GPIO_NO);
    gpio_pullup_dis(INPUT_GPIO_NO);
    gpio_set_direction(INPUT_GPIO_NO, GPIO_MODE_INPUT);
    
    while(1) {
        GPIOlevel = gpio_get_level(INPUT_GPIO_NO);
       if(GPIOlevel==1)
       {
         xQueueSend(q,(void *)&GPIOlevel,(TickType_t )10); 
       }
       else
       {
        gpio_set_level(BLINK_GPIO, 0);
       }
         vTaskDelay(100 / portTICK_RATE_MS);
    }
}
 
 
void app_main()
{
    q = xQueueCreate( 1, sizeof(int));
    xTaskCreate(&blink_task, "blinky", 2048, NULL, 5, NULL);
    xTaskCreate(&button_task, "button", 512,NULL,5,NULL );
}
