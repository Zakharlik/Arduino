int ledPin=13;



void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int n = random(5);
  Serial.println(n);
  delay(1000);
  for (int i=0; i<n; i++) {
    digitalWrite(ledPin,HIGH);
    delay(200);
    digitalWrite(ledPin,LOW);
    delay(200);
  }
  delay (1000);
}
