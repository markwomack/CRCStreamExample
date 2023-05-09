//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <DebugMsgs.h>

#include <WiFiServer.h>
#include <WiFiClient.h>

#include "CRCStream.h"

class GetBytesTask : public Task {
  public:
    void setTCPServer(WiFiServer* tcpServer) {
      _tcpServer = tcpServer;
    }
    
    void start(void) { 
      if (_tcpServer == 0) {
        DebugMsgs.debug().println("TCP Server not specified!");
      }
    }
    
    void update(void) {
      WiFiClient tcpClient = _tcpServer->available();
      if (!tcpClient) {
        return;
      }

      DebugMsgs.debug().println("reading data");

      uint32_t expectedSize = 0;
      uint32_t expectedCRC = 0;
      
      uint8_t buffer[1024];
      int count = 0;
      while (tcpClient.available()) {
        int data = tcpClient.read();
        if (data != -1) {
          if (data == '!') {
            buffer[count] = 0;
            expectedSize = strtoul((const char*)buffer, 0, 10);
            break;
          } else {
            buffer[count++] = (uint8_t)data;
          }
        }
      }

      if (expectedSize == 0) {
        DebugMsgs.error().println("Error reading the expected size");
        tcpClient.stop();
        return;
      }

      count = 0;
      while (tcpClient.available()) {
        count += tcpClient.readBytes(buffer + count, 9 - count);
        if (count == 9) {
          if (buffer[8] == '!') {
            buffer[8] = 0;
            expectedCRC = strtoul((const char*)buffer, 0, 16);
          }
          break;
        }
      }

      if (expectedCRC == 0) {
        DebugMsgs.error().println("Error reading the expected crc");
        tcpClient.stop();
        return;
      }
      
      CRCStream* crcStream = new CRCStream(&tcpClient, expectedSize, expectedCRC);

      // Read in the rest of the data
      while (crcStream->available()) {
        crcStream->readBytes(buffer, 1024);
      }

      if (crcStream->sizeAndCRCMatch()) {
        DebugMsgs.debug().println("Size and CRC match");
      } else {
        DebugMsgs.debug().println("Size and CRC do NOT match");
      }
      
      DebugMsgs.debug().printfln("Expected size: %d, actual size: %d", 
        crcStream->getExpectedSize(), crcStream->getCurrentSize());
      DebugMsgs.debug().printfln("Expected CRC: %x, actual CRC: %x", 
        crcStream->getExpectedCRC(), crcStream->getCurrentCRC());

      tcpClient.stop();
    }

  private:
    WiFiServer* _tcpServer = 0;
};
