//Suoritus on pistemäärän 2 arvoinen, sillä liikennevalot toimivat sarjassa, mutta nappeja ei ole vielä toteutettu.

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

int led_state = 0; // 0: red, 1: yellow, 2: green

// Led pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec gre = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
// static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
// static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

// Red led thread initialization
#define STACKSIZE 500
#define PRIORITY 5
void red_led_task(void *, void *, void*);
K_THREAD_DEFINE(red_thread,STACKSIZE,red_led_task,NULL,NULL,NULL,PRIORITY,0,0);

void yellow_led_task(void *, void *, void*);
K_THREAD_DEFINE(yellow_thread,STACKSIZE,yellow_led_task,NULL,NULL,NULL,PRIORITY,0,0);

void green_led_task(void *, void *, void*);
K_THREAD_DEFINE(green_thread,STACKSIZE,green_led_task,NULL,NULL,NULL,PRIORITY,0,0);


// Initialize leds
int  init_led() {

	// Led pin initialization
	int ret_red = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	int ret_gre = gpio_pin_configure_dt(&gre, GPIO_OUTPUT_ACTIVE);

	if (ret_red < 0) {
		printk("Error: Red led configure failed\n");		
		return ret_red;
	}

	if (ret_gre < 0) {
		printk("Error: Green led configure failed\n");		
		return ret_gre;
	}
	// set led off
	gpio_pin_set_dt(&red,0);
	gpio_pin_set_dt(&gre,0);
	

	printk("Led initialized ok\n");
	
	return 0;
}

// Task to handle red led
void red_led_task(void *, void *, void*) {
	
	printk("Red led thread started\n");
	while (true) {
		if (led_state == 0) {
			gpio_pin_set_dt(&red,1);
			printk("Red on\n");
			k_sleep(K_SECONDS(1));
			gpio_pin_set_dt(&red,0);
			printk("Red off\n");
			led_state = 1;
		}
	k_sleep(K_SECONDS(2));
	}
}

// Task to handle yellow led
void yellow_led_task(void *, void *, void*) {

	printk("Yellow led thread started\n");

	while (true) {
		if (led_state == 1) {
			gpio_pin_set_dt(&red,1);
			gpio_pin_set_dt(&gre,1);
			printk("Yellow on\n");
			k_sleep(K_SECONDS(1));
			gpio_pin_set_dt(&red,0);
			gpio_pin_set_dt(&gre,0);
			printk("Yellow off\n");
			led_state = 2;
		}
	k_sleep(K_SECONDS(2));
	}
} 

// Task to handle green led
void green_led_task(void *, void *, void*) {

	printk("Green led thread started\n");

	while (true) {
		if (led_state == 2) {
			gpio_pin_set_dt(&gre,1);
			printk("Green on\n");
			k_sleep(K_SECONDS(1));
			gpio_pin_set_dt(&gre,0);
			printk("Green off\n");
			led_state = 0;
		}
	k_sleep(K_SECONDS(2));
	}
}

// Main program
int main(void)
{
	init_led();

	return 0;
}