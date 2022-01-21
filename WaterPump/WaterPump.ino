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
#define MAX_MOTOR_TIME 1500000 //Max motor working time in milliseconds
#define MEASURE_PERIOD 2000   //Period betwin measures

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
  if (!digitalRead(BTN) && millis() - btn_t > 600) {
    turnPumpOn();
    btn_t = millis();
  }

  if (!digitalRead(BTN) && millis() - btn_t > 300) {
    was_high = !was_high;
    digitalWrite(LED_WASH, was_high);
    btn_t = millis();
  }
  if (millis() - measure_time > MEASURE_PERIOD) {
    digitalWrite(VCCM_PIN, LOW);
    delay(50);
    low_sensor = 1023 - analogRead(LOW_PIN);
    high_sensor = 1023 - analogRead(HIGH_PIN);
    digitalWrite(VCCM_PIN, HIGH);
    measure_time = millis();
    if (low_sensor > WATER) {
      digitalWrite(LED_LOW, HIGH);
    } else {
      digitalWrite(LED_LOW, LOW);

    }
    if (high_sensor > WATER) {
      digitalWrite(LED_HIGH, HIGH);
    } else {
      digitalWrite(LED_HIGH, LOW);

    }


    if (!pump && was_high && low_sensor < WATER) {
      turnPumpOn();
    }
    if (high_sensor > WATER) {
      pump = false;
      was_high = true;
      digitalWrite(LED_WASH, HIGH);

    }
    if (millis() - motor_timer > MAX_MOTOR_TIME) {
      pump = false;
    }
    if (pump) {
      digitalWrite(RELLAY_PIN, HIGH);
    }
    else {
      digitalWrite(RELLAY_PIN, LOW);
    }

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
  }
  if (pump) {
    i = (byte)((MAX_MOTOR_TIME - (millis() - motor_timer)) / 60000);
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
