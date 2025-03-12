#include <WiFiNINA.h>       
#include <ThingSpeak.h>     
#include <DHT.h>           

// Wi-Fi Credentials 
char ssid[] = "DODO-166E";   //This is my Wifi name   
char pass[] = "EJTZG2WUNN"; //This is my passcode(but needs to be confidential)

WiFiClient client;
unsigned long myChannelNumber = 2870189;  //It is used for telling my thinkgs speaks channel id to which ddta will be sent
const char * myWriteAPIKey = "8XEVD5PFDR9392WD";    //This is the API key of the channel

#define DHTPIN 2            
#define DHTTYPE DHT11       
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);       
  WiFi.begin(ssid, pass);   // Connect to Wi-Fi
  dht.begin();              // We are sending data to the web by connecting our Arduino to wofi

  while (WiFi.status() != WL_CONNECTED) {// THis whilke loop is looping unless the arduino connects to wifi
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi!");//Once connected, we will see this as output

  ThingSpeak.begin(client); 
}

void loop() {
  float temp = dht.readTemperature();  
  float hum = dht.readHumidity();      

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: "); 
  Serial.print(temp); 
  Serial.println(" °C");// Used to print out temperature on serial monotir
  Serial.print("Humidity: "); 
  Serial.print(hum); // Used to print out humidity on serial monitor
  Serial.println(" %");

  
  ThingSpeak.setField(1, temp);//Setting value od first field as temperature as done on thingsspeak
  ThingSpeak.setField(2, hum);//Setting value of second field as Humidity as done on thingsspeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) { //It is used to check whetehr the data has been uploaded successfully or not
    Serial.println("Data sent to ThingSpeak successfully");//This will print out teh line
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }

  delay(60000); //I am sending data after every 1 minute
}

