//Water Pump Controller
//v 0.1
//Controls water pump to fill in water barell by measureng voltage betwen wire on bottom and top of the barrel.
//Writen by Zlong

#define VCCM_PIN 4    //Common bottom pin
#define RELLAY_PIN 2  //Relay pin
#define HIGH_PIN A0   //Pin in top of the barell
#define LOW_PIN A1    //Pin in bottom of barell
#define BTN 3         //Button pin
#define LED_RELAY 9     //Led to display rellay status
#define LED_LOW   10    //Led displays if water is higher than Low
#define LED_HIGH  11    //LED displays if water is higher than High
#define LED_WASH  12    //Led displays if water was in high 


#define WATER 200             //Measure to determine if water
#define MAX_MOTOR_TIME 40 //Max motor working time in minutes
#define MEASURE_PERIOD 2000   //Period betwin measures

int max_motor_time = 40;

bool pump = false;
bool was_high = false;
int low_sensor, high_sensor;
uint32_t motor_timer = 0;
uint32_t measure_time = 0;
uint32_t rl_t = 0;
uint32_t btn_t = 5000;


byte  k, i = 0;
bool ledr = true;
bool blink_flag = true;
void setup()
{
  pinMode(RELLAY_PIN, OUTPUT);
  pinMode(VCCM_PIN, OUTPUT);
  pinMode(HIGH_PIN, INPUT_PULLUP);
  pinMode(LOW_PIN, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED_RELAY, OUTPUT);
  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_HIGH, OUTPUT);
  pinMode(LED_WASH, OUTPUT);
  Serial.begin(9600);
  measure_time = millis();
}


void loop()
{
  if (!digitalRead(BTN) && millis() - btn_t > 600) { //Turning pump on if button pressed long
    turnPumpOn();
    btn_t = millis();
  }
  else {
    if (!digitalRead(BTN) && millis() - btn_t > 300) {  //Swich was_high status if button pressed short
      was_high = !was_high;
      digitalWrite(LED_WASH, was_high);
      btn_t = millis();
    }
    
  }
  
  if (millis() - measure_time > MEASURE_PERIOD) { //Measure water level
    digitalWrite(VCCM_PIN, LOW);
    delay(50);
    low_sensor = 1023 - analogRead(LOW_PIN);
    high_sensor = 1023 - analogRead(HIGH_PIN);
    digitalWrite(VCCM_PIN, HIGH);
    measure_time = millis();
    if (low_sensor > WATER) { //If water higher than low sensor - turn on low LED
      digitalWrite(LED_LOW, HIGH);
    } else {
      digitalWrite(LED_LOW, LOW);

    }
    if (high_sensor > WATER) { //If water higher than high sensor - turn on high LED
      digitalWrite(LED_HIGH, HIGH);
    } else {
      digitalWrite(LED_HIGH, LOW);

    }


    if (!pump && was_high && low_sensor < WATER) { //If pump off and water was high and now water is veri low then turn on the pump
      turnPumpOn();
    }

    if (high_sensor > WATER) { //If water upper than high sensor - turn pump off
      pump = false;
      was_high = true;
      digitalWrite(LED_WASH, HIGH);
      max_motor_time = max_motor_time * 0.95 + (millis() - motor_timer) * 0.11;

    }
    if (millis() - motor_timer > max_motor_time * 60 * 1000) { //If timer expired - turn pump off
      pump = false;
    }
    if (pump) {                           //turn pump ON and OFF
      digitalWrite(RELLAY_PIN, HIGH); 
    }
    else {
      digitalWrite(RELLAY_PIN, LOW);
    }

/*  // Debugging
    Serial.print(high_sensor);
    Serial.print(',');
    Serial.print(low_sensor);
    Serial.print(',');
    Serial.print((int)((MAX_MOTOR_TIME - (millis() - motor_timer)) / 60000));
    Serial.print(',');
    Serial.print(millis() - motor_timer);
    Serial.print(',');
    Serial.print(was_high);
    Serial.print(',');
    Serial.println(pump);
*/
  }
  if (pump) { //Blink Motor LED
    i = (byte)((max_motor_time * 60 * 1000 - (millis() - motor_timer)) / 60000);
    if (k == 0) {
      if (blink_flag) {
        blink_flag = false;
        k = 8;

      } else {
        blink_flag = true;
        k = i * 2;
      }
    }
    if (millis() - rl_t > 300) {
      k--;
      if (blink_flag) {
        ledr = !ledr;
      }
      digitalWrite(LED_RELAY, ledr);
      rl_t = millis();
    }

  }
  else {
    digitalWrite(LED_RELAY, LOW);
  }

}

void turnPumpOn() {
  pump = true;
  motor_timer = millis();
  rl_t = millis();
  was_high = false;
  digitalWrite(LED_WASH, LOW);
  k = 0;
}
