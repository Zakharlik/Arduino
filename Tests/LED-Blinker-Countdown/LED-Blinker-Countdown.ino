//Water Pump Controller
//v 0.1
//Controls water pump to fill in water barell by measureng voltage betwen wire on bottom and top of the barrel.
//Writen by Zlong

#define VCCM_PIN 4    //Common bottom pin
#define RELLAY_PIN 2  //Relay pin
#define HIGH_PIN A0   //Pin in top of the barell
#define LOW_PIN A1    //Pin in bottom of barell
#define BTN 3         //Button pin
#define LED_RELAY 13     //Led to display rellay status
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
  i = (byte)((MAX_MOTOR_TIME - (millis() - motor_timer)) / 60000);
  if (k == 0) {
    if (blink_flag) {
      blink_flag = false;
      k = 5;

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
    Serial.print(i);
    Serial.print(',');
    Serial.print(blink_flag);
    Serial.print(',');
    Serial.println(k);
    rl_t = millis();
  }

}
