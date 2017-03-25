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
#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11

#define USE_TLS

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       ""
#define WLAN_PASS       ""

/************************* broker Setup *********************************/

#define MQTT_SERVER     "iot.eclipse.org"
#ifdef USE_TLS
#define MQTT_PORT       8883            // 8883 for MQTTS
#else
#define MQTT_PORT       1883            // 1883 for MQTT
#endif
#define MQTT_USERNAME    "admin"
#define MQTT_PASSWORD    "admin"

// cert SHA1 fingerprint
const char* fingerprint = "34:CE:0F:D7:E0:71:89:F4:16:04:17:87:EA:1E:E8:45:2A:14:9C:25"; //iot.eclispse.org

/************ Global State (you don't need to change this!) ******************/

// WiFiFlientSecure for SSL/TLS support
#ifdef USE_TLS
WiFiClientSecure client;
#else
WiFiClient client;
#endif

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME); // MQTT_PASSWORD);

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

/****************************** MQTT Topics ***************************************/

// Setup a feed called 'MQTT_USERNAME/temperature/{celcius, farenheit}" for publishing data.
Adafruit_MQTT_Publish cel = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/temperature/celcius");

Adafruit_MQTT_Publish far = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/temperature/farenheit");

Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/humidity");

/*************************** Sketch Code ************************************/

// Bug workaround since mqtt.connected() returns true even a CONNECT was never sent
bool connected;

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void verifyFingerprint();

void setup() {
  connected = false;
  Serial.begin(115200);
  delay(10);

  // initialise DHT sensor
  dht.begin();

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
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // check the fingerprint of io.adafruit.com's SSL cert
  verifyFingerprint();
}

void loop() {
    // wait a couple seconds
  delay(6000);

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();


  // .......... DHT Measures
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Now we can publish stuff!
    Serial.println("Sending temperature");
    if (! cel.publish( t )) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  far.publish( f );

   Serial.println("Sending humidity");
  if (! hum.publish( h )) {
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
    while(1);
  }

#ifdef USE_TLS
  if (client.verify(fingerprint, host)) {
    Serial.println("Connection secure.");
  } else {
    Serial.println("Connection insecure! Halting execution.");
    while(1);
  }
#endif
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (connected && mqtt.connected()) {
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

  connected = true;
  Serial.println("MQTT Connected!");
}
