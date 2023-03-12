

#include "logging/logging.h"

#include "network/mdns.h"

extern "C" {

void start_mdns() {

    info("init-ing mDNS...");
    mdns_resp_init();
    mdns_example_init();
}


static void srv_txt(struct mdns_service *service, void *txt_userdata)
{
    debug("srv_txt() called");

    err_t res;
    LWIP_UNUSED_ARG(txt_userdata);

    res = mdns_resp_add_service_txtitem(service, "path=/", 6);
    error("mdns add service txt failed");
}


static void mdns_example_report(struct netif* netif, u8_t result)
{
    debug("mdns status[netif %d]: %d", netif->num, result);
}

void mdns_example_init(void)
{
    debug("mdns_example_init() called");

    //mdns_resp_register_name_result_cb(mdns_example_report);
    mdns_resp_add_netif(netif_default, "solar-monitor");
    debug("mdns_resp_add_netif() done");
    mdns_resp_add_service(netif_default, "solar-monitor", "_http", DNSSD_PROTO_TCP, 80, srv_txt, NULL);
    debug("mdns_resp_add_service() done");
    mdns_resp_announce(netif_default);
    debug("mdns_resp_announce() done");
}


} // extern C
