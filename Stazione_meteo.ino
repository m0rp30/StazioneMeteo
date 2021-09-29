/**** LIBRARIES ****/
#include <DHT.h> // DHT22
#include <BMP180.h>
#include <DS18B20.h> // DS18B20
// wifi and client http
#include <WiFi.h>
#include <HTTPClient.h>

/**** DEFINE ****/
//#define DEBUG

#define uS_TO_S_FACTOR 1000000  // Conversion factgor from microseconds to seconds
#define TIME_TO_SLEEP  1800     // Time ESP32 will stay in deep sleep mode (in seconds)

// DHT22
#define DHTPIN 13
#define DHTTYPE DHT22

// DS18B20
#define DS18B20PIN 4

/**** VARIABLES ****/
// WIFI
const char* ssid = "ssid_name"; // SSID name
const char* password = "ssid_password"; // SSID password
// SERVER
const char* server_name = "http://api.weathercloud.net/set";
// DEVICE
const char* my_id = "devide_id"; // Device id
const char* my_key = "device_key"; // Device key
const char* vers = "/ver/1.2/type/201"; // Site' API version

/**** OBJECT DECLARETIONS ****/
// Declares DHT22
DHT dht22(DHTPIN, DHTTYPE);

// Declares BMP180
BMP180 bmp180(BMP180_HIGHRES);

// Declares DS18B20
DS18B20 ds18b20(DS18B20PIN);

void setup() {
  // Set wake up timer
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  short unsigned int count = 0; // 

  /**** Start wifi connection ****/
  WiFi.begin(ssid, password);
  #ifdef DEBUG
  Serial.println("Connecting to " + String(ssid));
  #endif
  while(WiFi.status() != WL_CONNECTED && count <= 10) {
    delay(500);
    #ifdef DEBUG
    Serial.print("*");
    #endif
    ++count;
  }
  #ifdef DEBUG
  Serial.println();
  Serial.println("Connesso");
  Serial.print("IP ");
  Serial.println(WiFi.localIP());
  #endif

  // If wifi is connected proced
  if(WiFi.status() == WL_CONNECTED) {
    
    /**** Sensors inizialization ****/
    dht22.begin();
    bmp180.begin();
    
    /**** Sensors reading ****/
    // Get all temperature of each sensors and make average
    float temp_ds18b20 = ds18b20.getTempC();
    float temp_bmp180 = bmp180.getTemperature();
    float temp_dht22 = dht22.readTemperature();
    float temp = (temp_ds18b20 + temp_bmp180 + temp_dht22) / 3;
    int hum = dht22.readHumidity(); // Get humidity
    float bar = float(bmp180.getSeaLevelPressure(13))/100; // Get pressure at 13mt above sea level

    #ifdef DEBUG
    Serial.println("Temperatur: " + String(temp));
    Serial.println("Humidity: " + String(hum));
    Serial.println("Pressure: " + String(bar));
    #endif
    
    // Declare http client
    HTTPClient http_client;
  
    // Generate the string to send
    String stringa_dati = server_name;
    stringa_dati += "/wid/" + String(my_id);
    stringa_dati += "/key/" + String(my_key);
    stringa_dati += "/temp/" + String(temp * 10);
    if(!isnan(hum)) {
      stringa_dati += "/hum/" + String(hum);
    }
    if(!isnan(bar)) {
      stringa_dati += "/bar/" + String(bar * 10);
    }
    stringa_dati += vers;
    http_client.begin(stringa_dati.c_str());

    // Send message and get code response
    short unsigned int respons = http_client.GET();
    #ifdef DEBUG
    Serial.print("HTTP Response code is: ");
    Serial.println(respons);
    #endif

    // Close http client-server connection 
    http_client.end();
  }

  /**** Deep sleep mode ****/
  #ifdef DEBUG
  Serial.println("I go in sleep mode");
  #endif
  esp_deep_sleep_start();
}

void loop() {

}
