/***************************************************
  Adafruit MQTT Library ESP8266 Adafruit IO SSL/TLS example

  Must use the latest version of ESP8266 Arduino from:
   https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  SSL/TLS additions by Todd Treece for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <OneWire.h>

#include <DallasTemperature.h>


/** DALLAS Temperature sensor */
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       ""
#define WLAN_PASS       ""

/************************* broker Setup *********************************/

#define MQTT_SERVER     "https://mqtt.awesome.iot-playground.org"
#define MQTT_PORT       443           // 8883 for MQTTS
#define MQTT_USERNAME    ""
#define MQTT_PASSWORD    ""

// cert SHA1 fingerprint
const char* fingerprint = "29:21:C6:E9:B1:E6:48:D0:10:FC:FA:5F:E0:40:60:7D:D7:7B:66:76"; //sky.lulf.no

/************ Global State (you don't need to change this!) ******************/

// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;
//WiFiClient client;


// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME); // MQTT_PASSWORD);

// Initialize DHT sensor.
//DHT dht(DHTPIN, DHTTYPE);

/****************************** MQTT Topics ***************************************/

// Setup a feed called 'temperature" for publishing data.
Adafruit_MQTT_Publish cel = Adafruit_MQTT_Publish(&mqtt, "temperature");

//Adafruit_MQTT_Publish far = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/temperature/farenheit");

//Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/humidity");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void verifyFingerprint();

void setup() {
  Serial.begin(115200);
  delay(10);

  // initialise DHT sensor
  // dht.begin();
  sensors.begin();
  sensors.setResolution(12);
  Serial.println(F("eclipse IO MQTTS (SSL/TLS) Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  delay(1000);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  delay(2000);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  // check the fingerprint of io.adafruit.com's SSL cert
  verifyFingerprint();

}

void loop() {
  // wait a couple seconds
  delay(2000);

  Serial.print("We have ");
  Serial.print(sensors.getDeviceCount());
  Serial.println(" devices");

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();


  // .......... DHT Measures
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  Serial.print("Requesting temperatures");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float temp = sensors.getTempCByIndex(0);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(temp)) {
     Serial.println("Failed to read from DHT sensor!");
     return;
    }

  // Now we can publish stuff!
  Serial.print("Sending temperature ");
  Serial.println(temp);
  if (! cel.publish( temp )) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
}


void verifyFingerprint() {

  const char* host = MQTT_SERVER;

  Serial.print("Connecting to ");
  Serial.println(host);

  if (! client.connect(host, MQTT_PORT)) {
    Serial.println("Connection failed. Halting execution.");
    while (1);
  }

    if (client.verify(fingerprint, host)) {
      Serial.println("Connection secure.");
    } else {
      Serial.println("Connection insecure! Halting execution.");
      while(1);
    }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}
