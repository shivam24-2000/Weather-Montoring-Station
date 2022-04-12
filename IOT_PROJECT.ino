#include <ESP8266WiFi.h>                                                    // NodeMCU Library
#include <DHT.h>                                                          // DHT Sensor Library
#include <ThingSpeak.h>                                                     // ThingSpeak Library 
DHT dht(D3, DHT11);                                                         // Declaring D3 as dht pin 
WiFiClient client;                                                      // WiFi client object for ThingSpeak 
long myChannelNumber = 1700965 ;                                        // ThingSpeak channel
const char myWriteAPIKey[] = "W0CFRCUTCJ0TGQ76";                        // ThingSpeak write key 
int sensorPin = A0;                                                         // Raindrop sensor input pin
int enable2 = 13;                                                           // Raindrop sensor output led pin
int sensorValue2 = 0;                                                  // Variable to store the value coming from Raindrop sensor
bool flag = false;     // Boolean to save rain status

 void setup() {
    pinMode(enable2, OUTPUT);                                                 // Setting led pin as output
    Serial.begin(115200);                                                       // Initializing baud rate 
    WiFi.begin("OnePlus7Pro", "12341234");                                  // Connect to WiFi using given id and password 
    while (WiFi.status() != WL_CONNECTED)                                // WiFi getting connected
    {
      delay(500);
      Serial.print("..");
    }
    Serial.println();
    Serial.println("NodeMCU is connected!");
    Serial.println(WiFi.localIP());                                       // Print IP connected
    dht.begin();                                                              // Connect to dht sensor
    ThingSpeak.begin(client);                                              // Connect to ThingSpeak 
 } 
 
void loop() {
  sensorValue2 = analogRead(sensorPin);                                 // Reading Raindrop sensor value
  sensorValue2 = constrain(sensorValue2, 0, 1023);                      // Limiting the range of values
  sensorValue2 = map(sensorValue2, 0 , 1023, 100, 0);                  // Invert the range
  if (sensorValue2 >= 20)                                                   // If rain sensor value > threshold
  {
    Serial.println("rain is detected ");                                    // Print on serial monitor that rain detected
//    Serial.println(sensorValue2);
    digitalWrite(enable2, HIGH);                                        // Light the LED on board
    flag = true;                                                            // Rain status is true
  }
  else                                                                    // If rain sensor value < threshold
  {
    Serial.println("rain not detected ");                                   // Print on serial monitor that rain detected
    digitalWrite(enable2, LOW);                                         // Switch off the LED on board
    flag = false;                                                           // Rain status is false
  }
  float h = dht.readHumidity();                                         // Reading and saving Humidity value
  float t = dht.readTemperature();                                      // Reading and saving Temperature value
  Serial.println("Temperature: " + (String) t);                           // Printing these values on serial monitor
  Serial.println("Humidity: " + (String) h);
  Serial.println();                                                                        
  // SENDING DATA TO THINGSPEAK                                                                        
  if (flag == true) {                                                       // If it is raining
    ThingSpeak.writeField(myChannelNumber, 3, sensorValue2, myWriteAPIKey);        // Send 1 to ThingSpeak in channel 3
    //delay(100);
    ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);        // Send humidity value to ThingSpeak in channel 2
    //delay(100);
    ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);        // Send temperature value to ThingSpeak in channel 1
  }
  else {                                                                    // If it is not raining
      ThingSpeak.writeField(myChannelNumber, 3, 0, myWriteAPIKey);          // Send 0 to ThingSpeak in channel 3
      //delay(100);
      ThingSpeak.writeField(myChannelNumber, 2, h, myWriteAPIKey);        // Send humidity value to ThingSpeak in channel 2
      //delay(100);
      ThingSpeak.writeField(myChannelNumber, 1, t, myWriteAPIKey);       // Send temperature value to ThingSpeak in channel 1
   }
 delay(1000);
 }
