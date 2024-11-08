
//BOM:
//Fermion: 1.8" 128x160 IPS TFT LCD Display with MicroSD Card Slot (Breakout) https://www.dfrobot.com/product-2580.html 
//Raspberry Pi Pico 2 - RP2350 https://www.adafruit.com/product/6006 
//Adafruit DS3231 Precision RTC Breakout https://www.adafruit.com/product/3013 
//Lithium Ion Battery - 3.7V 10050mAh (10 Ah) https://www.adafruit.com/product/5035 
//USB / DC / Solar Lithium Ion/Polymer charger - Rev C https://www.adafruit.com/product/390 

//Wiring: 

  //LCD:
  //V - 3v3 (36)
  //G - GND (23)
  //CK - GP18 (24)
  //SI - GP19 (25)
  //SO - GP16 (21)
  //CS - GP17 (22)
  //RT - GP14 (19)
  //DC - GP15 (20)
  //BL - GP5 (7)

  //RTC:
  //VIN - 3v3 (36)
  //GND - GND (23)
  //SCL - GP9 (12)
  //SDA - GP8 (11)

  //Battery Charger: 
  //Load+ - VSYS (39) NOTE: Use a diode in between for safety
  //Load- - GND (23)

  //Battery:
  //Positive - BATT+
  //Negative - BATT-

//Libaries to install: 
//Adafruit RTClib Library https://github.com/adafruit/RTClib
//DFRobot GLD Library https://github.com/DFRobot/DFRobot_GDL/tree/master
//Arduino Wire Library https://github.com/esp8266/Arduino/tree/master/libraries/Wire 

#include "RTClib.h"
#include <Wire.h>
#include "DFRobot_GDL.h"

#define TFT_DC  15
#define TFT_CS  17
#define TFT_RST 14
#define TFT_BL  5

int color = 0xFFFF;

RTC_DS3231 rtc;

DFRobot_ST7735_128x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {
  Serial.begin(115200);
  screen.begin();
  screen.fillScreen(COLOR_RGB565_BLACK);

  Wire.setSDA(8);
  Wire.setSCL(9);
  Wire.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  }
}

void loop () {
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print("Temperature: ");
    Serial.print(rtc.getTemperature());
    Serial.println(" C");
    Serial.println();

    screen.setCursor(0, 10);
    screen.setTextColor(color, COLOR_RGB565_BLACK);
    screen.setTextSize(2);

    screen.print(now.year(), DEC);
    screen.print('/');
    screen.print(now.month(), DEC);
    screen.print('/');
    screen.print(now.day(), DEC);
    screen.print("   ");
    screen.println(' ');
    screen.print(now.hour(), DEC);
    screen.print(':');
    screen.print(now.minute(), DEC);
    screen.print(':');
    screen.print(now.second(), DEC);
    screen.println("   ");

    screen.print(rtc.getTemperature());
    screen.println(" C   ");

    delay(10);
}
