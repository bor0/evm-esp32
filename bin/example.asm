6005 ; V[0] = 5 (LED_BUILTIN for ESP32)
E2A1 ; pinMode(V[0], 2); (OUTPUT for ESP32)

6101 ; V[1] = 1
E0A3 ; digitalWrite(5, 1);

62FA ; V[2] = 250
72FA ; V[2] += 250
72FA ; V[2] += 250
72FA ; V[2] = 1000
E0A4 ; delay(1000);

6100 ; V[1] = 0
E0A3 ; digitalWrite(5, 0);

E0A4 ; delay(1000);

1200 ; jump to start
