#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
#define photoPin 0
#define LightPin 6
#define TempPin 5

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

float last_temp; 
float max_temp;
float min_temp;
int last_light; 
int max_light;
int min_light;

String outstring;

//Setup Date/Time
byte day=13;
byte month=9;
byte hour=19;
byte minute=10;
byte sec = 0;
byte year = 17; 
unsigned long worktime = 0;
unsigned long workdelay = 0;


// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void PrintDate()
{
  if (day < 10) Serial.print(0);
  Serial.print(day);Serial.print('.');
  if (month < 10) Serial.print(0);
  Serial.print(month);Serial.print(".20");Serial.print(year);Serial.print(' ');
  if (hour < 10) Serial.print(0);
  Serial.print(hour);Serial.print(':');
  if (minute < 10) Serial.print(0);
  Serial.print(minute);Serial.print(":");
  if (sec < 10) Serial.print(0);
  Serial.print(sec);
} 

void CalcDate()
{
   unsigned long nowtime = millis();
   unsigned long mdelay = nowtime - worktime;
   worktime = nowtime;
   mdelay =int(mdelay/1000);
   mdelay = mdelay + 3;
   sec = sec + mdelay%60;
   if (sec > 59) {
    mdelay = mdelay + 60;
    sec = sec - 60;
   }
   mdelay = int(mdelay/60);
   minute = minute + mdelay%60;
   if (minute > 59) {
    mdelay = mdelay + 60;
    minute = minute - 60;
   }
   mdelay = int(mdelay/60);
   hour = hour + mdelay%24;
   if (hour > 23) {
    mdelay = mdelay + 24;
    hour = hour - 24;
   }
   mdelay = int(mdelay/24);
   day = day + mdelay;
   while (((day > 31) && ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)))) {
     day = day - 31;
     month++;
   }
   while ((day > 30) && ((month == 4) || (month == 6) || (month == 9) || (month == 11))) {
     day = day - 30;
     month++;
   }
   while ((day > 29) && (month == 2) && ((year%4) == 0)) {
     day = day - 29;
     month++;
   }
   while ((day > 28) && (month == 2) && ((year%4) != 0)) {
     day = day - 28;
     month++;
   } 
   while (month > 12) {
     month = month - 12;
     year++;
   }
}
 
void setup(void)
{
  analogReference(DEFAULT);
  pinMode(LightPin,OUTPUT);
  pinMode(TempPin,OUTPUT);
  Serial.begin(9600);
// Раскомментировать следующий блок для ввода времени.
 
  Serial.println("Enter current Date in folowing format (DDMM)");
  while(Serial.available() < 1)
  {
    delay(2);
  }
  int Curdate=Serial.parseInt();
  day = byte(Curdate/100);
  month = Curdate%100;
  Serial.println("Enter current Time in folowing format (hhmm)");
  while(Serial.available() < 1)
  {
    delay(2);
  }
  Curdate=Serial.parseInt();
  hour = byte(Curdate/100);
  minute = Curdate%100;
  PrintDate();
  Serial.println();
  Serial.println("Date               ; Temperatures     ;;;      Light          ;ms");
  Serial.println("                   ;Last ; Min  ; Max  ; Last ;  Min  ;  Max  ;");
  sensors.begin();

    
/*  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();


//  sensors.getAddress(insideThermometer, 0);
  Serial.print("Detected sensor: ");
//  printAddress(insideThermometer);
  Serial.println(); */
  sensors.setResolution(insideThermometer, 12);
  sensors.requestTemperatures();
  max_temp = sensors.getTempCByIndex(0);
  min_temp = max_temp;

  max_light = analogRead(photoPin);
  min_light = max_light;
}
 
 
void loop(void)
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  last_temp = sensors.getTempCByIndex(0);
  if (max_temp < last_temp) max_temp = last_temp;
  if (min_temp > last_temp) min_temp = last_temp;
  last_light = analogRead(photoPin);
  if (last_light > max_light) max_light = last_light;
  if (last_light < min_light) min_light = last_light;
//  Serial.print(last_light);Serial.print(' ');Serial.print(max_light);Serial.print(' ');Serial.println(min_light);
  CalcDate();
  PrintDate();
  outstring = "; "+String(last_temp)+"; "+String(min_temp)+"; "+String(max_temp)+";   "
  +String(last_light)+";    "+String(min_light)+";    "+String(max_light)+"; "+String(int(millis()/1000));
  outstring.replace('.',',');
  Serial.println(outstring);
  if (last_light < ((max_light + min_light) / 2)) digitalWrite(LightPin,HIGH);
  else digitalWrite(LightPin,LOW);
  for (int i =0; i < int (last_temp/10); i++) {
    digitalWrite(TempPin,HIGH);
    delay(300); 
    digitalWrite(TempPin,LOW);
    delay(300); 
  }
  for (int i =0; i < (int(last_temp)%10); i++) {
    if ((i%5) == 0) delay(300);  
    digitalWrite(TempPin,HIGH);
    delay(150); 
    digitalWrite(TempPin,LOW);
    delay(150); 
  }

  delay(60000);
}

