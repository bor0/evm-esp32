/*
This file is part of evm-esp32.

evm-esp32 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

evm-esp32 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with evm-esp32. If not, see <http://www.gnu.org/licenses/>.

*/
#include <HTTPClient.h>
#include <WiFi.h>

#include <stdio.h>

#include "include/vm.h"

// Sometimes downloading from GitHub causes the device to reboot. Best store on a local web server without https: https://raw.githubusercontent.com/bor0/evm-esp32/main/bin/example.bin
// E.g. http://192.168.0.108:8080/example.bin
char binary_url[255];

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println("Enter WiFi SSID (newline terminated): ");
    while (!Serial.available()) delay(10);
    String ssid = Serial.readStringUntil('\n');

    Serial.println("Enter WiFi password (newline terminated): ");
    while (!Serial.available()) delay(10);
    String password = Serial.readStringUntil('\n');

    Serial.println("Enter URL for binary (newline terminated): ");
    while (!Serial.available()) delay(10);
    String url = Serial.readStringUntil('\n');

    memcpy(binary_url, url.c_str(), sizeof(binary_url));

    Serial.print("Connecting to WiFi");

    WiFi.begin(ssid.c_str(), password.c_str());

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected to WiFi");
}

void loop()
{
    HTTPClient client;
    struct vm VM;

    printf("Requesting (extended) CHIP-8 binary\n");

    client.begin(binary_url);
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
