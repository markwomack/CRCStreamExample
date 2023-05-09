//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// Arduino includes
#include <Arduino.h>
#include <WiFiServer.h>
#include <WiFiClient.h>

// My includes
#include <DebugMsgs.h>   // https://github.com/markwomack/ArduinoLogging
#include <TaskManager.h> // https://github.com/markwomack/TaskManager

// Local includes
#include "pin_assignments.h"
#include "constants.h"
#include "MyNetworkHub.h"
#include "GetBytesTask.h"

MyNetworkHub networkHub;

GetBytesTask getBytesTask;

void setup() {
  Serial.begin(115200);
  delay(1000);

  DebugMsgs.enableLevel(ALL);

  // Connect to WiFi network, create a TCP port to monitor
  if (networkHub.start() == 0) {
    WiFiServer* tcpServer = networkHub.getTCPServer(TCP_SERVER_PORT);

    // Set the tcp server into the task that will monitor it
    getBytesTask.setTCPServer(tcpServer);
  } else {
    DebugMsgs.debug().println("Unable to connect to WiFi!");
    while (true) {;}
  }
  
  taskManager.addBlinkTask(LED_STATUS_PIN, 500);
  taskManager.addTask(&getBytesTask, 1000);
  taskManager.start();
}

void loop() {
  taskManager.update();
}
