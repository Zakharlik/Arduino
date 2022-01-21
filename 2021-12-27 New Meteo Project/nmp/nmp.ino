#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define OUTDHTPIN 2     // Digital pin connected to the Outside DHT sensor
#define INDHTPIN 3      // Digital pin connected to the Inside DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define L_SENSOR A0

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dhto(OUTDHTPIN, DHTTYPE);
DHT dhti(INDHTPIN, DHTTYPE);

bool flag;
uint32_t stmr = 0;
uint32_t dtmr = 0;

uint32_t hper;
uint32_t lper = 1000;
uint32_t sper = 1600;
uint32_t dper = 3500;

int light;

void setup()
{
  lcd.init();                      // initialize the lcd
  lcd.backlight();
  pinMode(L_SENSOR, INPUT);
  dhto.begin();
  dhti.begin();
  //  Serial.begin(9600);
}


void loop()
{
  float h;
  float t;
  if (millis() - stmr >= sper) {
    stmr = millis();
    light = analogRead(L_SENSOR);

    //    Serial.println(light);

  }
  if (millis() - dtmr >= dper) {
    dtmr = millis();
    if (flag) {
      h = dhto.readHumidity();
      // Read temperature as Celsius (the default)
      t = dhto.readTemperature();
      flag = !flag;
      // Check if any reads failed and exit early (to try again).
      if (isnan(h) || isnan(t)) {
        lcd.setCursor(0, 0);
        lcd.print("Failed to read");
        lcd.setCursor(2, 1);
        lcd.print("Out sensor!");
        return;
      }
    }
    else {
      h = dhti.readHumidity();
      // Read temperature as Celsius (the default)
      t = dhti.readTemperature();
      // Check if any reads failed and exit early (to try again).
      flag = !flag;
      if (isnan(h) || isnan(t)) {
        lcd.setCursor(0, 0);
        lcd.print("Failed to read");
        lcd.setCursor(2, 1);
        lcd.print("In sensor!");
        return;
      }

    }
    lcd.setCursor(1, 0);
    lcd.print("Hum: ");
    lcd.print(h);
    lcd.print("%");
    delay(10);
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.write(223);
    lcd.print("C  ");
    if (light > 200) {
      lcd.backlight();
    }
    else {
      lcd.noBacklight();
    }
  }
}
