int dot_len = 100;
int dash_len = dot_len * 3;
int space_len = dot_len * 7;
int letters=2;
char letter[]={'а','б'};
String morze[2][]={'.-','-...'};

Serial.begin(9600);


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {
    char symbol = Serial.read();
    for (int i=0; i<letters; i++) {
      if (symbol == letter[i]){
        int sym_pos = i;
      }
    }
  for (i=0; i < morse[sym_pos}; i++) {
    if (morse[sym_pos][i] == '.') {
      digitalWrite(led_Pin,HIGH);
      delay(dot_len);
      digitalWrite(led_Pin,LOW);
      delay(dot_len);
    else if (morse[sym_pos][i] == '.') {
      digitalWrite(led_Pin,HIGH);
      delay(dash_len);
      digitalWrite(led_Pin,LOW);
      delay(dash_len);
      
      }
    }
   
    
  }
  
  
  
}
