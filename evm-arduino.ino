/*
This file is part of evm-arduino.

evm-arduino is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

evm-arduino is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with evm-arduino. If not, see <http://www.gnu.org/licenses/>.

*/
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

#include <HTTPClient.h>
#include <WiFi.h>

#include <stdio.h>

#include "include/vm.h"

void setup()
{
    Serial.begin(115200);
    delay(10);

    printf("Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        printf(".");
    }

    printf("\nConnected to WiFi\n");
}

void loop()
{
    HTTPClient client;
    struct vm VM;

    printf("Requesting (extended) CHIP-8 binary\n");

    // Sometimes downloading from GitHub causes the device to reboot. Best store on a local web server without https
    //client.begin("https://raw.githubusercontent.com/bor0/evm-arduino/main/bin/example.bin");
    client.begin("http://192.168.0.108:8080/example.bin");
    client.GET();

    printf("Initializing VM\n");

    vm_init(&VM, client.getString());

    printf("Entering VM loop\n");

    while (!VM.halt) {
        vm_cycle(&VM);
        delay(1);
    }

    printf("VM halted\n");
}
