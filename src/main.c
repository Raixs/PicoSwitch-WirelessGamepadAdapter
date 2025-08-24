#include <btstack_run_loop.h>
#include <pico/cyw43_arch.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/async_context.h>
#include <uni.h>

#include "sdkconfig.h"
#include "usb.h"

// Sanity check
#ifndef CONFIG_BLUEPAD32_PLATFORM_CUSTOM
#error "Pico W must use BLUEPAD32_PLATFORM_CUSTOM"
#endif

// Defined in my_platform.c
struct uni_platform *get_my_platform(void);

// WiFi credentials; replace with real values
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

static repeating_timer_t wifi_timer;

static bool wifi_reconnect_callback(repeating_timer_t *rt) {
        int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        if (status != CYW43_LINK_UP) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                logi("WiFi reconnecting...\n");
                cyw43_arch_wifi_connect_async(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
        } else {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        }
        return true;
}

void bluepad_core_task()
{
	// initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
        if (cyw43_arch_init()) {
                loge("failed to initialise cyw43_arch\n");
                return;
        }

        cyw43_arch_enable_sta_mode();
        cyw43_arch_wifi_connect_async(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK);
        add_repeating_timer_ms(5000, wifi_reconnect_callback, NULL, &wifi_timer);

        // Must be called before uni_main()
        uni_platform_set_custom(get_my_platform());

	// Initialize BP32
	uni_init(0, NULL);

	// Does not return.
	btstack_run_loop_execute();
}

int
main()
{
	stdio_init_all();

	multicore_launch_core1(bluepad_core_task);
	usb_core_task();

	return 0;
}
