

#pragma once

extern "C" {

#include "lwip/apps/mdns.h"
#include "lwip/opt.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

void start_mdns();
static void srv_txt(struct mdns_service *service, void *txt_userdata);
static void mdns_example_report(struct netif* netif, u8_t result);
void mdns_example_init(void);




} // extern C