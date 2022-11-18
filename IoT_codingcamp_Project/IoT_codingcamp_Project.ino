#include <Wire.h> 
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include "FirebaseESP8266.h" 

//for LCD
LiquidCrystal_I2C lcd(0x27,16,2);  // 16 x 2 text character lcd

// for wifi
const char* ssid = "iot"; // The name of wifi
const char* password = "15151515"; //The password of wifi

//for firebase
#define FIREBASE_HOST "aicube-codingcamp-default-rtdb.firebaseio.com" // firebase url
#define FIREBASE_AUTH "ov0Du7LTpBoNGwEEUABwvLt5g9YWXNe7eUXJqmk9"  // firebase database password

FirebaseData firebaseData;
FirebaseJson json;
 
void setup(){
  // Initialize text lcd
  lcd.init();   
  lcd.backlight();

  // Initialize Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay (500);
    lcd.print(".");
  }

  displayWIFI_state(); // display wifi status
  firebaseInit(); // Initialize firebase variables

  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" IoT CodingCamp ");
}

void loop() {

  if(Firebase.getString(firebaseData, "sendText"))
  {
    String valStr = firebaseData.stringData();
    //valStr = valStr.substring(2,valStr.length()-2);
    //uncomment when you use Appinventor
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);
    lcd.print(valStr);
  } 
     
  delay(5000);
}

void displayWIFI_state(void)
{
  lcd.setCursor(0,1);
  lcd.print("WiFi connected");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP address");
  
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
}

void firebaseInit(void)
{
// Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
 
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
