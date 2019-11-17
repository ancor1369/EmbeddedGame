# An implementation of Free RTOS

To show up the basics of how the an RTOS works, we are using the platform LPCExpresso 1796 that features an ARM cortex M3 LPC1769FBD100 processor and some IO to use it as the core of any applicaiton where it might be useful.

The idea is to test the implementation of a very simple application to feature the FreeRTOS.

The application is implemented on the code exposed on the file src/BlinkLights.c.

The most important part is how a task is described:

```C
/*This is the Task for LED 1*/
static void LED1_Task(void *pvParameters)
{
	bool LedState = true;
	while (1)
	{

		LedState = (bool)!LedState;
		Board_LED_Set(0, LedState);
		vTaskDelay(configTICK_RATE_HZ);
		LedState = (bool)!LedState;
		Board_LED_Set(0, LedState);
		vTaskDelay(3*configTICK_RATE_HZ);
		LedState = true;
	}
}
```

And how it is later initialized, inside the main function it is necesary to call the method:

```C
int main(void)
{
.
.
.
xTaskCreate(LED1_Task, (signed char *) "LED1_Task",
					configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
          (xTaskHandle *) NULL);
.
.
.
}
```

The objective of the implementation is to make the LEDs on the board to blik in sequence as it is stated on the following task operation graph.


![image](https://user-images.githubusercontent.com/25968721/66720815-92e4eb80-edcf-11e9-94c4-8baa7a54e124.png)
