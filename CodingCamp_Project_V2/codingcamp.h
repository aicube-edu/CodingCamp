#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <LiquidCrystal_I2C.h>
#include "FirebaseESP8266.h" 

// for Firebase
#define FIREBASE_HOST "aicube-codingcamp-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "ov0Du7LTpBoNGwEEUABwvLt5g9YWXNe7eUXJqmk9"

// for wifi
#define STASSID "iot" //와이파이 검색했을때 뜨는 이름
#define STAPSK  "15151515" //패스워드
const char* ssid = STASSID;
const char* password = STAPSK;

// for time (NTP(Network Time Protocol) Client)
WiFiUDP ntpUDP;
const long utcOffsetInSeconds = 32400; // every UTC +1.00, +3600
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
String Week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#define NUM 10
struct avgFilter
{
  float sum = 0.0;
  float data[NUM];
  int index = 0;

  void initData(void)
  {
    for (int i=0 ; i<NUM ; i++)
      data[i] = 0.0;    
  }

  float filteredData(float newData)
  {
    sum = sum - data[index];
    data[index] = newData;
    sum = sum + data[index];
    index++;
  
    if(index >= NUM) index = 0;
  
    return sum/NUM;
  }
};
