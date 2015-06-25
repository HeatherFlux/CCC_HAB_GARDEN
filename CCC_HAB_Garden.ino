#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xD1, 0x42 }; //Setting MAC Address
char server[] = "api.pushingbox.com"; //pushingbox API server
IPAddress ip(192,168,1,8); //Arduino IP address. Only used when DHCP is turned off.
EthernetClient client; //define 'client' as object
String data;
String humdata;
float temp;
float humi;
boolean connecting = false;

// DHT instances
#define DHTPIN 7 
#define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 dht.begin();
 if (Ethernet.begin(mac) == 0) {
	Serial.println("Failed to configure Ethernet using DHCP");
	Ethernet.begin(mac, ip);
  }
  delay(1000);
}

void loop() {
  // read your sensors
  temp = dht.readTemperature();
  humi = dht.readHumidity();
  
  //begin the loops
   getdatatemp();
   Serial.println("connecting...");
   if (client.connect(server, 80)) {
     sendDatatemp();  
     connecting = true; //connected = true
   }
   else{
     Serial.println("connection failed");
   }
    while(connecting){
    if (client.available()) {
	  char c = client.read(); //save http header to c
	  Serial.print(c); //print http header to serial monitor
    }
    if (!client.connected()) {
	  Serial.println();
	  Serial.println("disconnecting.");
          Serial.print("Temperature Sent :");
          Serial.print(temp); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          data = ""; //data reset
    }
  }
  delay(100);
  Serial.println("Getting/Sending Humidity");
     getdatahum();
   Serial.println("connecting...");
   if (client.connect(server, 80)) {
     sendDatahum();  
     connecting = true; //connected = true
   }
   else{
     Serial.println("connection failed");
   }
    while(connecting){
    if (client.available()) {
	  char c = client.read(); //save http header to c
	  Serial.print(c); //print http header to serial monitor
    }
    if (!client.connected()) {
	  Serial.println();
	  Serial.println("disconnecting.");
          Serial.print("Humidity Sent :");
          Serial.print(humi); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          humdata = ""; //data reset
    }
  }
  delay(600000); // interval
}

void getdatatemp(){
  data+="";
  data+="GET /pushingbox?devid=vA9E6B4A30C33313&tempData="; //GET request query to pushingbox API
  data+=temp;
  data+=" HTTP/1.1";
}
void sendDatatemp(){
  Serial.println("connected");
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}

void getdatahum(){
  humdata+="";
  humdata+="GET /pushingbox?devid=v599FDEDA89D901C&humiData="; //GET request query to pushingbox API
  humdata+=humi;
  humdata+=" HTTP/1.1";
}
void sendDatahum(){
  Serial.println("connected");
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}
