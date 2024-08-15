#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "ThingSpeak.h" 
int au=0;
#include <Adafruit_Fingerprint.h>
#include <ESP8266WiFi.h>
SoftwareSerial mySerial(D5, D6);
WiFiClient  client;
int flag=0;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int x=0;
 


void setup()  
{
     lcd.begin();
  lcd.backlight();
   lcd.setCursor(0,0); 
  lcd.print("Sri Hari ");
  delay(5000);
  lcd.clear();
  Serial.begin(9600);
 // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin("Project", "12345678");  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
 WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  finger.begin(57600);
   
  
    pinMode(D4,OUTPUT);
    
      digitalWrite(D4,HIGH);
 
 
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  delay(2000);
  int x = ThingSpeak.writeField(991316, 8, 0,"C6TYSSCZS42F7QCI");

}

void loop()                     // run over and over again
{
  
  if(Serial.available()>0)
  {
    char s=Serial.read();
    Serial.println(s);
    if(s=='a')
    {
      flag=1;
        x=getFingerprintIDez();
      while(x==-1)
       {
        x=getFingerprintIDez();
       delay(300);
       }
    
      if(flag==x)
      {
         digitalWrite(D4,LOW);

      lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("Person 1");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();
   
       
      
        }
        else {
                    // wait for one second before reading the sensors again
lcd.clear();
          }
      }
      else if(s=='b')
      {
        flag=2;
      x=getFingerprintIDez();
      while(x==-1)
       {
        x=getFingerprintIDez();
       delay(300);
       }
      
        if(flag==x)
      {
         digitalWrite(D4,LOW);
    
              lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("Person 2");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();
        
  
     
        }
        else{
          flag=0;
           digitalWrite(D4,HIGH);
          lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("Unknown");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();            // wait for one second before reading the sensors again
                 int x = ThingSpeak.writeField(991316, 7, "UNKNOWN","C6TYSSCZS42F7QCI");
while(au==0)
{
    int count = ThingSpeak.readIntField(991316, 8, "YUT4K6JD9D3WGB0Q");  
if(count==3)
{
  au=1;
   digitalWrite(D4,LOW);
          lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("AUTHENTICATED");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();
  }
  }  
                   
        }
      }
        else if(s=='u'){
          flag=0;
           digitalWrite(D4,HIGH);
          lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("Unknown");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();
          int x = ThingSpeak.writeField(991316, 7, "UNKNOWN","C6TYSSCZS42F7QCI");
while(au==0)
{
    int count = ThingSpeak.readIntField(991316, 8, "YUT4K6JD9D3WGB0Q");  
if(count==3)
{
  au=1;
   digitalWrite(D4,LOW);
          lcd.setCursor(0,1);              // set the cursor to the position after "Current: "              // print the current value
  lcd.print("AUTHENTICATED");
  delay(5000);                     // wait for one second before reading the sensors again
lcd.clear();
  }
  } 
          
          }
    
  delay(50);            //don't ned to run this at full speed.
}
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
