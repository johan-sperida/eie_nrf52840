#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void) {
    int ret;

    //looks like we are checking if it's ready
    if(!gpio_is_ready_dt(&led0)) {
        return -1;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    
    //error catching?
    if (ret < 0) {
        return ret;
    }

    while (1) {

    }

    return 0;
}