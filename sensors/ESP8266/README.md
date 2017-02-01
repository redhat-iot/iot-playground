# Have fun with the ESP8266

This is a quick how to get running the Wemos D1 mini (v1) and the DHT11 sensor shield with Arduino.

## Getting started with Arduino

First, install the latest version of Arduino [here](https://www.arduino.cc/en/Main/Software) (the packaged versions shipped with distributions are often out of date.)

Launch the IDE, then close it.
Clone the library directory into your Sketchbook directory (usually `~/Arduino/` but this can be changed in the IDE settings.)
```
cd ~/Arduino
mkdir -p hardware/esp8266com
cd hardware/esp8266com
git clone https://github.com/esp8266/Arduino.git esp8266
```

Download the binary tools:
```
cd esp8266/tools
python get.py
```

*Tip:* If not using Linux, you'll need to install the USB driver, [see here.](https://www.wemos.cc/tutorial/get-started-arduino.html) (almost all the info displayed here comes from this page actually)

Then, you can restart the Arduino IDE and select the Wemos Board in `Tools -> Board`

*Tip*: The nice board handed to the fellow messaging Red Hatters are the Wemos D1 mini *v1* model. So in the Board menu you should select *Wemos D1 (Retired)*


### Install the libraries
To use the mqtt_dht.imo sketch, you will need to install some extra libraries.

In the Arduino Editor go to `Sketch -> Include library -> Manage Libraries` and search for :
 - DHT : Install the 'DHT Sensor Library' by adafruit
 - MQTT : Install the 'Adafruit MQTT Library'

 Then, you can upload the ino sketch provided in this repo.
 Please adapt the variables to match your name and your wi-fi credentials !

## Topics and variables used

### MQTT Topics
The provided `.ino` sketch use some pre-defined MQTT topics to post the data:
- `/MQTT_USERNAME/temperature/celcius` : Raw measured temperature, in degrees celcius.
- `/MQTT_USERNAME/temperature/farenheit` : Raw measured temperature, in degrees farenheit.
- `/MQTT_USERNAME/humidity` : Raw measured humidity, a percentage of humidity in the air.

### Variables

These variables must be adapted acording to your local network and personal credentials :
```
WLAN_SSID
WLAN_PASS
MQTT_USERNAME
MQTT_PASSWORD
```

To connect to the MQTT broker :
```
MQTT_SERVER
MQTT_PORT
fingerprint  // Broker's certificate fingerprint to verify the connection authenticity
```
