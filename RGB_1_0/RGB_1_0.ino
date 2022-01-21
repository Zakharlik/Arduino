int photoPin = 1;
int potPin = 0;
int switchPin = 8;

boolean lastButton = LOW;
boolean currentButton = LOW;
int LEDColor = 0;

int RedLED = 11 ;
int GreenLED = 10;
int BlueLED = 9;

int RedLevel = 11 ;
int GreenLevel = 10;
int BlueLevel = 9;
String outstring;

boolean debounce(boolean last)
{
  boolean current = digitalRead(switchPin);
  if (last != current)
  {
    delay (20);  
    current = digitalRead(switchPin);
  }
  return current;
}

void setup() {
  analogReference(DEFAULT);
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  pinMode(switchPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  outstring = String(analogRead(photoPin))+' '+String(analogRead(potPin))+' '+String(LEDColor)+' '+String(RedLevel);
//  Serial.println(outstring);

  currentButton = debounce(lastButton);
  if (currentButton && !lastButton) 
    {
      LEDColor++;
    }
 
  if (LEDColor == 0) {
    RedLevel=map(analogRead(potPin),0,1023,0,200);
   
  }
  else if (LEDColor == 1) {
    GreenLevel=map(analogRead(potPin),0,1023,0,200);
   
  }
  else if (LEDColor == 2) {
    BlueLevel=map(analogRead(potPin),0,1023,0,200);
   
  }
  else LEDColor = 0;  
  if (analogRead(photoPin) < 890){
    analogWrite(RedLED,RedLevel); 
    analogWrite(GreenLED,GreenLevel);
    analogWrite(BlueLED,BlueLevel); 
  }
  else {
    analogWrite(RedLED,0); 
    analogWrite(GreenLED,0);
    analogWrite(BlueLED,0); 
  }
 
  
/*  for (int r=0; r<=255; r=r+5) {
    analogWrite(RedLED,r);
    for (int g=0; g<=255; g=g+5) {
      analogWrite(GreenLED,g);
      for (int b=0; b<=255; b++) {
        analogWrite(BlueLED,b);
        delay (2);
      }
    }
  }
*/
}
