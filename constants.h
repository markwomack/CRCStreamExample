//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

//#define FIXED_HOST_IP

#ifdef FIXED_HOST_IP
const IPAddress HOST_IP_ADDRESS(IPAddress(192, 168, 86, 102));   // Fixed IP address
#endif

// TCP server constants
const uint32_t TCP_SERVER_PORT(50005); // Receive bytes on this port

#endif // CONSTANTS_H
