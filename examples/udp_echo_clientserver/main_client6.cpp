/*
 * Copyright 2021 ruben2020 https://github.com/ruben2020
 *
 * Dual-licensed under the Apache License, Version 2.0, and
 * the GNU General Public License, Version 2.0;
 * you may not use this file except in compliance
 * with either one of these licenses.
 *
 * You can find copies of these licenses in the included
 * LICENSE-APACHE2 and LICENSE-GPL2 files.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these licenses is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the license texts for the specific language governing permissions
 * and limitations under the licenses.
 *
 * SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-only
 *
 */
 
#include <stdio.h>
#include "client6.h"

int main(void)
{
    printf("udpclient6 starting program\n");
    SOCK_STARTUP();
    std::shared_ptr<feal::Actor> tcpclient1 = std::make_shared<Client6>();
    tcpclient1.get()->init();
    tcpclient1.get()->start();
    tcpclient1.get()->wait_for_shutdown();
    feal::EventBus::getInstance().destroyInstance();
    SOCK_CLEANUP();
    printf("udpclient6 terminating program\n");
    return 0;
}


