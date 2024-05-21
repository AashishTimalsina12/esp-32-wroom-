#include<WiFi.h>
#include<DHT.h>
#include<ThingSpeak.h>
#include <LiquidCrystal_I2C.h>//for I2c connecter..........16*2
#define Buzzer 18
#define LED 2
#define SENSOR 36
#include<Wire.h>
#define DELAY 500
/////////////////////////////////FOR THE MAIL TRANSFER///////////////////////////////
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <ESP_Mail_Client.h>
//////////////////MAIL DEFIEND LAIBARY END//////////////////////////////////
const char* server = "api.thingspeak.com";

int totalColumns = 16;
int totalRows = 2;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* ssid = "aashish";   // your network SSID 
const char* password = "12345678";   // your network password+++++****



int sensorValue = 0;  // variable to store the value coming from the sensor

unsigned long myChannelNumber =  1800030;
const char* myWriteAPIKey = "2A0RP9UHH04UFYF3";

int dhtPin = 36;
#define dhtType DHT22



// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

// Variables to store temperature and humidity
int temperature;
int humidity;
float h;

//xc////////////////////////////////////////MAIL PART ///////////////////
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "joy9821866953@gmail.com"
#define AUTHOR_PASSWORD "couqvhgoehwfyjgz"

/* Recipient's email*/
//#define RECIPIENT_EMAIL "jakesontimalsina123@gmail.com"

/* The SMTP Session object used for Email sending */
SMTPSession smtp;

SMTP_Message message;

ESP_Mail_Session session;
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

// Create a sensor object
WiFiClient  client;
DHT dht(dhtPin, dhtType);

void setup(){
  pinMode(LED, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(Buzzer, OUTPUT);
  Serial.begin(115200);
  Serial.println("Sensor start");
  //lcd.begin(16,2);
  lcd.init(); 
  lcd.backlight(); // use to turn on and turn off LCD back light
  dht.begin(); 
  WiFi.mode(WIFI_STA); 
  Serial.println("Connecting to WiFi!");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(3000);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
 //////////////////////////MAIL TRNASFER//////////////////////////////////
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();km 

 /** Enable the debug via Serial port
   * none debug or 0
   * basic debug or 1
  */
  smtp.debug(0);

  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = "joy9821866953@gmail.com";
  session.login.password = "couqvhgoehwfyjgz";
  session.login.user_domain = "";

  /* Declare the message class */
//  SMTP_Message message;
     
  /* Set the message headers */
  

  //Send raw text message
 

//  /*Send HTML message*/
//  String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
//  message.html.content = htmlMsg.c_str();
//  message.html.content = htmlMsg.c_str();
//  message.text.charSet = "us-ascii";
//  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  /* Connect to server with the session config */
  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
}
//  wifiConnect();


void loop() {

  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DELAY);              // wait for a second
  
  // read the value from the sensor:
  sensorValue = analogRead(SENSOR);
  Serial.print("Value: "); 
  Serial.println(sensorValue);
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(DELAY);
  if (sensorValue > 800)
  {

    message.sender.name = "ESP";
  message.sender.email = "joy9821866953@gmail.com";
  message.subject = "ESP gas leakeguge test";
  message.addRecipient("Aashish Timalsina", "kabitatimalsina9865@gmail.com");
  String textMsg = "Alert gas level at: " + String(sensorValue);
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;
    if (!MailClient.sendMail(&smtp, &message)){
    Serial.println("Error sending Email, " + smtp.errorReason());
    }
  Serial.println("Gas");
  digitalWrite(Buzzer, HIGH) ; //send tonedelay(5000);
  lcd.setCursor(0, 0);//i2c lcd display system.......
  lcd.print("ALret");
  lcd.setCursor(0,1);
  lcd.print("Gas leakage");
  delay(2000);
  //lcd.clear(); 
  
 
  }
 else {
    digitalWrite(Buzzer, LOW) ;  //no tone
    Serial.println("No Gas");
lcd.setCursor(0, 0);
lcd.print("NO GAS");
lcd.setCursor(0,1);
lcd.print("LEAKAGE");
delay(1500);
//lcd.clear(); 
 }
  //data set into thingspeak datafeild
 
 temperature = analogRead(SENSOR);
      Serial.print("GAS PPM: ");
     Serial.println(temperature);
     Serial.print("Gas Level: ");
     h=(temperature/1023*100);
     Serial.println(h);
     Serial.println("Data Send to Thingspeak");
     ThingSpeak.setField(1, temperature);
     ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  // thingspeak needs minimum 1 sec delay between updates.
  delay(100);
}

///////////////////////out of loop mail//////////////////////////////////////////
/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
