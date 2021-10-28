// Please see the "examples/LICENSE" file for
// the copyright notice and license text.

#include <stdio.h>
#include "client.h"

int main(void)
{
    printf("udpclient starting program\n");
    SOCK_STARTUP();
    std::shared_ptr<feal::Actor> tcpclient1 = std::make_shared<Client>();
    tcpclient1.get()->init();
    tcpclient1.get()->start();
    tcpclient1.get()->wait_for_shutdown();
    feal::EventBus::getInstance().destroyInstance();
    SOCK_CLEANUP();
    printf("udpclient terminating program\n");
    return 0;
}


