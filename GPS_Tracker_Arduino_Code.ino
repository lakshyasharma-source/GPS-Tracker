#include <SoftwareSerial.h>
SoftwareSerial sim808(7,8);

char phone_no[] = "+919999728860"; 
String a[5];
#define RESULT true
String stateOfGps,timeOfGps,latitude,longitude;

void setup() {
 sim808.begin(9600);
 Serial.begin(9600);
 delay(100);

 sim808.print("AT+CSMP=17,167,0,0");  
 delay(100);
 sim808.print("AT+CMGF=1\r"); 
 delay(400);

 sendData("AT+CGNSPWR=1",1000,RESULT);
 delay(100);
 sendData("AT+CGNSSEQ=RMC",1000,RESULT);
 delay(200);
 
}

void loop() {
  sendData2("AT+CGNSINF",1000,RESULT);
if (stateOfGps !=0) {
    Serial.println("stateOfGps  :"+stateOfGps);
    Serial.println("Time  :"+timeOfGps);
    Serial.println("Latitude  :"+latitude);
    Serial.println("Longitude  :"+longitude);

    sim808.print("AT+CMGS=\"");
    sim808.print(phone_no);
    sim808.println("\"");
    
    delay(300);

    sim808.print("http://maps.google.com/maps?q=loc:");
    sim808.print(latitude);
    sim808.print(",");
    sim808.print (longitude);
    delay(200);
    sim808.println((char)26); 
    delay(200);
    sim808.println();
    delay(20000);
    sim808.flush();
    
    delay(10000);
    
 } else {
   Serial.println("GPS Initialising...");
 }
}

void sendData2(String command , const int timeout , boolean result){

  sim808.println(command);
  long int time = millis();
  int i = 0;

  while((time+timeout) > millis()){
    while(sim808.available()){
      char c = sim808.read();
      if (c != ',') {
         a[i] +=c;
         delay(100);
      } else {
        i++;  
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (RESULT) {
    stateOfGps = a[1];
    timeOfGps = a[2];
    latitude = a[3];
    longitude =a[4];  
  }
}
String sendData (String command , const int timeout ,boolean result){
  String response = "";
  sim808.println(command);
  long int time = millis();
  int i = 0;

  while ( (time+timeout ) > millis()){
    while (sim808.available()){
      char c = sim808.read();
      response +=c;
    }
  }
  if (result) {
     Serial.print(response);
     }
     return response;
}