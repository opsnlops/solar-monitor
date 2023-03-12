
#include "network.h"

#include "secrets.h"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "logging/logging.h"


void start_networking() {

    info("starting networking");


    if (cyw43_arch_init()) {
        fatal("failed to initialise cyw43");
        return;
    }

    cyw43_arch_enable_sta_mode();

    debug("connecting to Wifi...");
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        warning("failed to connect.");
        exit(1);
    } else {
        debug("Wifi connected");
    }

    info("My IP is %s", ip4addr_ntoa(netif_ip4_addr(netif_list)));

}

void stop_networking() {

    info("stopping networking");
    cyw43_arch_deinit();

}