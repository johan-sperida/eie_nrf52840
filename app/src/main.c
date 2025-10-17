#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

//--------------------------------------------
#define LED1_NODE DT_ALIAS(led1)

static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
//--------------------------------------------

#define LED2_NODE DT_ALIAS(led2)

static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
//--------------------------------------------

#define LED3_NODE DT_ALIAS(led3)

static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
//--------------------------------------------

int main(void) {
    int ret;

    // Configure led0
    if (!gpio_is_ready_dt(&led0)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return ret;
    }


    // Configure led1
    if (!gpio_is_ready_dt(&led1)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ret;
    }

    // Configure led2
    if (!gpio_is_ready_dt(&led2)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ret;
    }

    // Configure led3
    if (!gpio_is_ready_dt(&led3)) {
        return -1;
    }
    ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ret;
    }


    while (1) {
        k_msleep(1000);
        gpio_pin_toggle_dt(&led0);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led1);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led2);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led3);

        k_msleep(1000);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led3);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led2);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led1);

        k_msleep(1000);
        gpio_pin_toggle_dt(&led0);

        k_msleep(1000);
    }

    return 0;
}