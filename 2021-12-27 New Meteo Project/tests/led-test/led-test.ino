//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define HLED_PIN 3
#define LLED_PIN 4
#define L_SENSOR A0

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);


byte myChar[] = {0x14, 0x1f, 0x00, 0x14, 0x00, 0x14, 0x0e}; 

bool hled = 1;
bool lled = 1;
uint32_t htmr = 0;
uint32_t ltmr = 0;
uint32_t stmr = 0;
uint32_t dtmr = 0;

uint32_t hper;
uint32_t lper = 1000;
uint32_t sper = 1600;
uint32_t dper = 1500;

int light;

void setup()
{
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(HLED_PIN, OUTPUT);
  pinMode(LLED_PIN, OUTPUT);
  pinMode(L_SENSOR, INPUT);
  lcd.createChar(0, myChar);
  dht.begin();
}


void loop()
{
  if (millis() - stmr >= sper) {
    stmr = millis();
    light = analogRead(L_SENSOR);
    hper = map(light,0,1023,50,2000);
  }
  if (millis() - ltmr >= lper) {
    ltmr = millis();
    lled = !lled;
    digitalWrite(LLED_PIN, lled);
  }
  if (millis() - htmr >= hper) {
    htmr = millis();
    hled = !hled;
    digitalWrite(HLED_PIN, hled);
  }
  if (millis() - dtmr >= dper) {
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
        lcd.setCursor(0,0);
        lcd.print("Failed to read"); 
        lcd.setCursor(2,1);
        lcd.print("DHT sensor!");
    return;
  }
  lcd.setCursor(1,0);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print("%");
  delay(10);
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.write(223);
  lcd.print("C");
  if (light > 400) {
      lcd.backlight();
    }
    else {
      lcd.noBacklight();
    }
  }
}
