#define BTN 3
#define LEDB 2

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LEDB, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(BTN));
  digitalWrite(LEDB, HIGH);
  delay(4);
  digitalWrite(LEDB, LOW);
  delay(10);
}
