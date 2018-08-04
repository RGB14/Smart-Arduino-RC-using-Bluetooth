//Motor Pin to L928N bridge module
int Motor1A = 2; //Left Motor A
int Motor1B = 4; //Left Motor B
int Motor2A = 7; //Right Motor A
int Motor2B = 8; //Right Motor B

//Buzzer
int _freq1[5] = {351, 629, 531, 557, 333};
int _delay[5] = {131, 161, 161, 141, 176};
int _freq2[5] = {2401, 1872, 1414, 872, 919};
int pinBuzzer = 6;
int alarmFreq = 2500;

//Temp_Sensor
int tempSensor = A0;
double sensorInfo = 0;


//Speed Configuration
int vel_max = 255; // Engine speed (0-255)
int zero = 0;
int vel_half = 120;

//Initial Value
char state = 's'; // initial state -> stop

//PWM [speed control for motor]
int Motor1_speedControl = 3;
int Motor2_speedControl = 5;

//LED functions
int LED = 9;
int LED2 = 10;
int brightness = 0;
int fadeAmount = 5;
int fader = 0;

void setup() {
  Serial.begin(9600); // Initialize Serial connection with BLUETOOTH
  pinMode(Motor1A, OUTPUT);
  pinMode(Motor1B, OUTPUT);
  pinMode(Motor2A, OUTPUT);
  pinMode(Motor2B, OUTPUT);
  pinMode(Motor1_speedControl, OUTPUT);
  pinMode(Motor2_speedControl, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(tempSensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void LED_blink_front()
{
  digitalWrite(LED2, HIGH);
  delay(300);
  digitalWrite(LED2, LOW);
  delay(300);
}

void LED_blink_back()
{
  digitalWrite(LED, HIGH);
  delay(300);
  digitalWrite(LED, LOW);
  delay(300);
}

void LED_fade_up()
{
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    analogWrite(LED, fadeValue);
    analogWrite(LED2, fadeValue);
    delay(30);

  }
}

void LED_fade_down()
{
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(LED, fadeValue);
    analogWrite(LED2, fadeValue);
    delay(30);
  }
}

void buzz()
{
  for (int i = 0; i < 5; i++)
  {
    tone(pinBuzzer, _freq1[i]);
    delay(_delay[i]);
    tone(pinBuzzer, _freq2[i]);
    noTone(pinBuzzer);
    delay(1000);
  }
}
void goForward()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, vel_max);
  digitalWrite(Motor1A, vel_max);

}

void goReverse()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, vel_max);
  digitalWrite(Motor1B, vel_max);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, zero);
}

void goRight()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, vel_max);
  delay(300);
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, zero);
}

void goStop()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, zero);

}

void goLeft()
{
  for (int i = 0; i < 256; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, vel_max);
  digitalWrite(Motor1A, zero);
  delay(300);
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, zero);
}


void goLeft360()
{
  for (int i = 0; i < 120; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, zero);
  digitalWrite(Motor1B, vel_half);
  digitalWrite(Motor2A, vel_half);
  digitalWrite(Motor1A, zero);

}
void goRight360()
{
  for (int i = 0; i < 120; i++)
  {
    analogWrite(Motor1_speedControl, i);
    analogWrite(Motor2_speedControl, i);
  }
  digitalWrite(Motor2B, vel_half);
  digitalWrite(Motor1B, zero);
  digitalWrite(Motor2A, zero);
  digitalWrite(Motor1A, vel_half);

}

void sensorTemp()
{
  double convertedInfo = (sensorInfo * 5000) / 1024;
  if (convertedInfo >= 600)
  {
    tone(pinBuzzer, alarmFreq);
    goStop();
    delay(100);
    goReverse();
    delay(600);
    goStop();

  }
  else
  {
    noTone(pinBuzzer);
  }
}

void demo()
{
  goStop();
  for (int i = 0; i <= 1000; i++)
  {
    LED_fade_up();
    if (i = 1000)
    {
      delay(300);
      goto BLINK_UP;
    }
  }
BLINK_UP:
  LED_blink_back();
  delay(100);
  LED_blink_front();
  delay(100);
  for (int i = 0; i <= 1000; i++)
  {
    LED_fade_down();
    if (i = 1000)
    {
      delay(300);
      goto BLINK_DOWN;
    }
  }
BLINK_DOWN:
  LED_blink_back();
  delay(100);
  LED_blink_front();
  delay(500);
}
void loop() {
  sensorInfo = analogRead(tempSensor); //Receive information from TEMPARETURE SENSOR
  sensorTemp();

  if (Serial.available() > 0) { // Receive information from BLUETOOTH
    state = Serial.read();
    switch (state) {
      case 'a': // goForward
        Serial.println(state);
        goForward();
        LED_blink_front();

        break;

      case 'd': // goRight
        Serial.println(state);
        goRight();
        LED_blink_front();
        LED_blink_back();

        break;

      case 'c': //Stop
        Serial.println(state);
        goStop();
        //LED_fade_max();
        //LED_fade_zero();
        break;

      case 'b': // goLeft
        Serial.println(state);
        goLeft();
        LED_blink_back();
        LED_blink_front();
        break;

      case 'e': // Reverse
        Serial.println(state);
        goReverse();
        LED_blink_back();
        break;

      case 'p':
        Serial.println(state); //right360
        goRight360();
        //LED_fade_max();
        delay(300);
        break;

      case 'q':
        Serial.println(state); //Left360
        goLeft360();
        //LED_fade_zero();
        delay(300);
        break;

      case 'n': //Stop
        Serial.println(state);
        goStop();
        LED_blink_front();
        LED_blink_back();
        buzz();
        break;

      case 's': //INITIAL STATE after ON or RESET
        Serial.println(state);
        goStop();
        break;

      case 'h':
        Serial.println(state);
        goStop();
        LED_blink_front();
        goRight360();
        delay(300);
        goLeft360();
        LED_blink_back();

      default:
        Serial.println(state);
        goStop();
        buzz();
    }
  }
  else
  {
    demo();
  }
}


