#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xD1, 0x42 }; //Setting MAC Address
char server[] = "api.pushingbox.com"; //pushingbox API server
IPAddress ip(192,168,1,8); //Arduino IP address. Only used when DHCP is turned off.
EthernetClient client; //define 'client' as object
String data;
String humiData;
String soilData;
String tempfData;

float soil = 0;
float temp;
float tempf;
float humi;
boolean connecting = false;

// DHT instances
#define DHTPIN 7 
#define DHTTYPE DHT11
  DHT dht(DHTPIN, DHTTYPE);


// -------------------------------------------------------------------------------------------------------
// put your setup code here, to run once:
void setup() {
     Serial.begin(9600);
     dht.begin();
       if (Ethernet.begin(mac) == 0) {
	  Serial.println("Failed to configure Ethernet using DHCP");
	  Ethernet.begin(mac, ip);
  }
  delay(1000);
}

// -------------------------------------------------------------------------------------------------------
//begin the loops
  void loop() {
  
// read your sensors
   temp = dht.readTemperature();
   tempf = dht.readTemperature(true);
   humi = dht.readHumidity();
   int sensorValue = analogRead(A0);
       sensorValue = constrain(sensorValue, 485, 1023);
       soil = map(sensorValue, 485, 1023, 100, 0);  //map the value to a percentage
  


//
// -------------------------- Timestamp Temperature in Celsius 
//
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
          Serial.print("Temperature Sent : ");
          Serial.print(temp); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          data = ""; //data reset
    }
  }
  delay(1000);
 
//
// -------------------------- Humidity
//
 
  Serial.println(" ");
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
          Serial.println(humi); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          humiData = ""; //data reset
    }
  }
    delay(1000);

//
// -------------------------- Soil
//
    
  Serial.println(" ");
  Serial.println("Getting/Sending Soil Moisture Level");
     getdatasoil();
   Serial.println("connecting...");
   if (client.connect(server, 80)) {
     sendDatasoil();  
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
          Serial.print("Soil Moisture Level Sent :");
          Serial.println(soil); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          soilData = ""; //data reset
    }
  }
    delay(1000);
//
// -------------------------- Temperature in Fahrenheit 
//
  Serial.println(" ");
  Serial.println("Getting/Sending Temperature(F)");
     getdatatempf();
   Serial.println("connecting...");
   if (client.connect(server, 80)) {
     sendDatatempf();  
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
          Serial.print("Temperature(F) Sent :");
          Serial.println(tempf); //print sent value to serial monitor
	  client.stop(); 
          connecting = false; 
          tempfData = ""; //data reset
    }
  }
  delay(60000); // interval
}

// -------------------------------------------------------------------------------------------------------
// Temp(C) Functions
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

// -------------------------------------------------------------------------------------------------------
// Humidity Functions
void getdatahum(){
  humiData+="";
  humiData+="GET /pushingbox?devid=v599FDEDA89D901C&humiData="; //GET request query to pushingbox API
  humiData+=humi;
  humiData+=" HTTP/1.1";
}
void sendDatahum(){
  Serial.println("connected");
  client.println(humiData);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}

// -------------------------------------------------------------------------------------------------------
// Soil Moisture Functions
void getdatasoil(){
  soilData+="";
  soilData+="GET /pushingbox?devid=vCEFEE5BDB7082C4&soilData="; //GET request query to pushingbox API
  soilData+=soil;
  soilData+=" HTTP/1.1";
}
void sendDatasoil(){
  Serial.println("connected");
  client.println(soilData);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}

// -------------------------------------------------------------------------------------------------------
// Temp(F) Functions
void getdatatempf(){
  tempfData+="";
  tempfData+="GET /pushingbox?devid=v2362388FBAF17F6&tempfData="; //GET request query to pushingbox API
  tempfData+=tempf;
  tempfData+=" HTTP/1.1";
}
void sendDatatempf(){
  Serial.println("connected");
  client.println(tempfData);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();
}
