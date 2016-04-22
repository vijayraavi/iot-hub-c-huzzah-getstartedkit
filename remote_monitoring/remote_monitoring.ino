// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

// Please use an Arduino IDE 1.6.8 or greater

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>

#include <AzureIoTHub.h>

#include "remote_monitoring.h"

char ssid[] = "[Your WiFi network SSID or name]";
char pass[] = "[Your WiFi network WPA password or WEP key]";

WiFiClientSecure sslClient;

AzureIoTHubClient iotHubClient(sslClient);

void setup() {
    initSerial();
    initWifi();
    initTime();

    iotHubClient.begin();
}

void loop() {
    // Run the Remote Monitoring from the Azure IoT Hub C SDK
    // You must set the device id, device key, IoT Hub name and IotHub suffix in
    // remote_monitoring.c
    remote_monitoring_run();
}

void initSerial() {
    // Start serial and initialize stdout
    Serial.begin(115200);
    Serial.setDebugOutput(true);
}

void initWifi() {
    // Attempt to connect to Wifi network:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      // unsuccessful, retry in 4 seconds
      Serial.print("failed ... ");
      delay(4000);
      Serial.print("retrying ... ");
    }

    Serial.println("Connected to wifi");
}

void initTime() {  
   time_t epochTime;

   configTime(0, 0, "pool.ntp.org", "time.nist.gov");

   while (true) {
       epochTime = time(NULL);

       if (epochTime == 0) {
           Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
           delay(2000);
       } else {
           Serial.print("Fetched NTP epoch time is: ");
           Serial.println(epochTime);
           break;
       }
   }
}
