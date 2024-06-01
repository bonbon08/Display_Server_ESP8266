#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "Password"
#endif

LiquidCrystal_I2C lcd(0x27,16,2);

const char* ssid     = STASSID;
const char* password = STAPSK;

WiFiUDP Udp;
unsigned int localUdpPort = 80; 
char incomingPacket[255];

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Display on");
  lcd.setCursor(0,1);
  lcd.print("Wlan waiting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(14,1);
    lcd.print("#");
    delay(200);
    lcd.setCursor(14,1);
    lcd.print(" ");
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print("Wlan connected");
  Udp.begin(localUdpPort);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      int len = Udp.read(incomingPacket, 255);
      if (len > 0) {
        incomingPacket[len] = 0;
      }
      String inputString = String(incomingPacket);
      int separatorIndex = inputString.indexOf('§');
      if (separatorIndex > 0) {
        String num1String = inputString.substring(0, separatorIndex);
        String remainingString = inputString.substring(separatorIndex + 1);
        
        int num1 = num1String.toInt();
        int num2 = remainingString[0] - '0';
        String textToDisplay = remainingString.substring(1);
        
        lcd.setCursor(num1, num2);
        lcd.print(textToDisplay);
      }
    }
  }
}
