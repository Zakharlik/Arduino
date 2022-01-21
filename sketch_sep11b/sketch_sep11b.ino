int R_Pin = 4;
int Resist = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A4,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Resist = analogRead(R_Pin);
  Serial.println(Resist);
  delay(500);
}
