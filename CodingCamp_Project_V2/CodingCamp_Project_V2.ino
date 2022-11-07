#include "codingcamp.h"

// for time (NTP(Network Time Protocol) Client)
unsigned long cTime=0;
unsigned long lTime=0;

// for textLCD
LiquidCrystal_I2C lcd(0x27,16,2);  

// for Switch and Temperature Sensor

int swL = D5;
int swR = D6;
int temp = A0;
bool cStateL = false;
bool lStateL = false;
bool cStateR = false;
bool lStateR = false;

avgFilter Temp;

// for Firebase
FirebaseData firebaseData;
FirebaseJson json;
 
int dispMode = 0;// 0: default, 1: NTP Clock; 2:IoT Display

void setup(){
  // Initialize switches 
  pinMode(swL, INPUT);
  pinMode(swR, INPUT);
  Temp.initData();

  // Initialize the lcd   
  lcd.init();   
  lcd.backlight();

  // Initialize Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    lcd.print(".");
  }
  lcd.setCursor(0,1);
  lcd.print("WiFi connected");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP address");
  
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  // Initialize NTP  
  timeClient.begin();

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  delay(2000);
}

void loop() {
  lStateL = cStateL;
  lStateR = cStateR;
  cStateL = digitalRead(swL);
  cStateR = digitalRead(swR);
  delay(10);

  if(cStateL && (cStateL != lStateL))
  {
    dispMode--;
    if(dispMode < 0) dispMode = 0;
  }
  else if(cStateR && (cStateR != lStateR))
  {
    dispMode++;
    if(dispMode > 2) dispMode = 2;
  }
  else {}

  cTime = millis();
  if(cTime - lTime > 1000) displayLCD(dispMode);
}

void displayLCD(int MODE)
{
  if(MODE == 0)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*** 2022 IoT ***");
    lcd.setCursor(0,1);
    lcd.print("** CodingCamp **");
  }
  else if (MODE == 1)
  {
    timeClient.update();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("*** NTP Clock **");
    lcd.setCursor(2,1);
    lcd.print(Week[timeClient.getDay()]);
    lcd.print(" ");
    lcd.print(timeClient.getHours());
    lcd.print(":");
    lcd.print(timeClient.getMinutes());
    lcd.print(":");
    lcd.print(timeClient.getSeconds());
  }
  else if (MODE == 2)
  {
    String valStr = "";
    float filteredData = 0.0;
    for (int i=0 ; i<NUM ; i++)
    {
      float valADC = analogRead(temp);
      float tempDegree = 320*valADC / 1024 - 50;
      filteredData = Temp.filteredData(tempDegree);
    }
    Firebase.setFloat(firebaseData, "Temp", filteredData);
    
    
    if(Firebase.getString(firebaseData, "sendText"))
    {
      valStr = firebaseData.stringData();
      valStr = valStr.substring(2,valStr.length()-2);
      lcd.print(valStr);
    }    

    lcd.clear();
    String strDegree = String(filteredData);
    lcd.setCursor(0,0);
    lcd.print("Temp. ");  
    lcd.print(strDegree);   
    lcd.print(" C");  
    lcd.setCursor(0,1);
    lcd.print(valStr);
  }
  else {}
  lTime = millis();
}
