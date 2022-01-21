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

 
void setup(void)
{
  analogReference(DEFAULT);
  pinMode(LightPin,OUTPUT);
  pinMode(TempPin,OUTPUT);
  Serial.begin(9600);
  Serial.println("   Temperatures   ;;;        Light");
  Serial.println("Last ; Min  ; Max  ; Last ;  Min  ;  Max  ;");
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
  outstring = String(last_temp)+"; "+String(min_temp)+"; "+String(max_temp)+";   "
  +String(last_light)+";    "+String(min_light)+";    "+String(max_light);
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

