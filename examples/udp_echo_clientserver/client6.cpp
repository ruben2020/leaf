#include <cstdio>
#include <cstring>
#include "client6.h"

void Client6::send_to_server(void)
{
    feal::sockerrenum se;
    serveraddr.family = feal::ipaddr::INET6;
    serveraddr.port = 12001;
    strcpy(serveraddr.addr, "::1");
    se = dgram.create_sock((feal::family_t) serveraddr.family);
    if (se != feal::S_OK) printf("create sock: %d\n", se);
    timers.startTimer<EvtDelayTimer>(std::chrono::seconds(1));

}

