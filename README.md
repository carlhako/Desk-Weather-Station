# Desk-Weather-Station
ESP8266 with a BME280 and Nokia 5110. Stats sent to [Thingspeak](https://thingspeak.com/channels/351359).

![alt text](https://raw.githubusercontent.com/carlhako/Desk-Weather-Station/master/Station.png "Desk Weather Station")

Parts List
* NodeMCU - I chose to use NodeMCU as there are are multiple 3.3v and GND outputs. Also note there are 2 versions of the NodeMCU one is wider than the other, you want the narrow one. V3 is typically the wider one however I purchased the newer integrated nodemcu where the ESP8266 chip is now part of the board and not a ESP-12 attached to the top, it was labeled as V3. [Heres the one I bought](https://www.aliexpress.com/item/1pcs-NodeMCU-V3-Lua-WIFI-module-integration-of-ESP8266-extra-memory-32M-Flash-USB-serial-CH340G/32837858093.html)
* Nokia 5110 display
* BME280 - temp, air pressure and humidity all in one sensor
* 12x 10cm female to female dupond cables

---

Instructions

1. Solder pins onto BME280 and 5110 display. You want to put the pins sticking out the back of the display like the following picture. If your display has pins at the top and bottom it doesnt matter the wires a long enough to reach top and bottom inside the little enclosure.
![alt text](https://raw.githubusercontent.com/carlhako/Desk-Weather-Station/master/Wiring.png "Wiring")
2. Wire up display and BME280, dont worry about the enclosure yet just connect everything up. I used the following pin mapping. 

NodeMCU Pin | Device | Device Pin
--- | --- | ---
D0 | Display | LIGHT
D1 | Display | CE
D2 | Display | RST
D3 | BME280  | SCL
D4 | BME280  | SDA
D5 | Display | DC
D6 | Display | DIN
D7 | Display | CLK
3.3v | Display | VCC
GND  | Display | GND
3.3v | BME280 | VIN
GND  | BME280 | GND
