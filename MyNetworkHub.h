//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef MY_NETWORK_HUB_H
#define MY_NETWORK_HUB_H

// Third party includes
#include <Udp.h>
#include <WiFiServer.h>

class MyNetworkHub {
  public:
    MyNetworkHub() { /* Nothing to see here, move along. */ }

    int start(void);
    void stop(void);

    UDP* getUdpPort(uint32_t portNum);
    WiFiServer* getTCPServer(uint32_t portNum);
    void printWifiStatus(void);
};

#endif // MY_NETWORK_HUB_H
