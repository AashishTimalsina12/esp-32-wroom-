#include<WiFi.h>
#include<DHT.h>
#include<ThingSpeak.h>

const char* ssid = "bijaya";   // your network SSID 
const char* password = "shrestha";   // your network password

int dhtPin = A0;
#define dhtType DHT22

unsigned long myChannelNumber = 1790599;
const char* myWriteAPIKey = "Y13ES3PAQ04X46M6";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variables to store temperature and humidity
int temperature;
int humidity;

// Create a sensor object
WiFiClient  client;
DHT dht(dhtPin, dhtType);

void setup(){
  Serial.begin(9600);
  dht.begin();  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  wifiConnect();
}

void loop() {

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    Serial.print("Temperature (ºC): ");
    Serial.println(temperature);
    Serial.print("Humidity (%): ");
    Serial.println(humidity);
    
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 
    delay(5000);
}
// Function to connect WiFi
void wifiConnect(){
  Serial.println("Connecting to WiFi!");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

  
