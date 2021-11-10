#include <LiquidCrystal.h>
const int RS=0,E=1,D4=5,D5=6,D6=7,D7=8;

#define CLK A1
#define DT A2
#define SW A5

LiquidCrystal lcd(RS,E,D4,D5,D6,D7); 





//Pins
const int PWM_pin = 9;


//Variables
int real_temperature_read = 0;
//int set_temperature_read = 0;
int aState;
int aLastState; 
float real_temperature = 0.0;
int set_temperature = 200;

float PID_error = 0;
float previous_error = 0;
unsigned long elapsedTime, Time, timePrev ,lcdTime;
int PID_value = 0;

//PID constants
const float kp = 9.1, ki = 0.3, kd = 1.8;             //      ************************* THE ORIGINAL VALUES ARE : const float kp = 9.1;   int ki = 0.3;   int kd = 1.8;

int PID_p = 0;    int PID_i = 0;    int PID_d = 0;



void setup() {
  pinMode(PWM_pin,OUTPUT);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW,INPUT_PULLUP);
  aLastState = digitalRead(CLK); 
  TCCR1A = 0; //If I don't clear these registers, they seem to retain their value even after re-flashing!
  TCCR1B = 0;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 15.625 kHz
// Mode: Fast PWM top=0x00FF
// OC1A output: Non-Inverted PWM
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 16.384 ms
// Output Pulse(s):
// OC1A Period: 16.384 ms Width: 0 us
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0xD9;
OCR1BH=0x00;
OCR1BL=0x09;


  //TCCR1A = TCCR1A & B01111101 | 0x03;    // pin 9 and 10 PWM frequency of 980.39 Hz 
  lcd.begin(16,2);
  lcd.print("Soldering");
  lcd.setCursor(9,1);
  lcd.print("Station");
  delay(1500);
  lcd.clear();
}


void loop() {
  
  if(digitalRead(SW)==LOW){
    delay(5);
    while(digitalRead(SW)==LOW);

  }
  read_set_temperature();
  Time = millis();
  // First we read the real value of temperature
  real_temperature_read = analogRead(A4);
  //set_temperature_read = analogRead(A5);
  //if(set_temperature_read < 427) set_temperature_read = 427;
  //set_temperature = (float)set_temperature_read / 2.13;
  real_temperature = (float)real_temperature_read / 2.84;  //       This is NOT THE FULLSCALE 480deg Celcius
  //Next we calculate the error between the setpoint and the real value
  PID_error = set_temperature - real_temperature ;
  //Calculate the P value
  PID_p = kp * PID_error;
  //Calculate the I value in a range on +-2                  *******************************************THE ORIGINAL VALUE WAS 3
  if(-2 < PID_error && PID_error < 2)
  {
    PID_i = PID_i + (ki * PID_error);
  }
  
  //For derivative we need real time to calculate speed change rate
  timePrev = Time;                            // the previous time is stored before the actual time read
  Time = millis();                            // actual time read
  elapsedTime = (Time - timePrev) / 1000; 
  //Now we can calculate the D calue
  PID_d = kd*((PID_error - previous_error)/elapsedTime);
  //Final total PID value is the sum of P + I + D
  PID_value = PID_p + PID_i + PID_d;

  //We define PWM range between 0 and 255
  if(PID_value < 0)
  {    PID_value = 0;    }
  if(PID_value > 0xD9)  
  {    PID_value = 0xD9;  }
  //Now we can write the PWM signal to the mosfet on digital pin D3
  OCR1AL = PID_value;
  previous_error = PID_error;     //Remember to store the previous error for next loop.

  //delay(300);
  
  if(millis()- lcdTime > 499){
  lcdTime = millis();
  lcd.setCursor(0,0);
  lcd.print("Set  :");
  lcd.print((int)set_temperature);
  lcd.setCursor(0,1);
  lcd.print("Read :");
  lcd.print((int)real_temperature);
  lcd.setCursor(12,1);
  lcd.print(PID_value);
  if(PID_value < 100)lcd.print(" ");
  }
  //digitalWrite(PWM_pin,HIGH);
}

void read_set_temperature(){
  aState = digitalRead(CLK); // Reads the "current" state of the outputA
  // If the previous and the current state of the CLK are different, that means a Pulse has occured
  if (aState != aLastState){     
     // If the DT state is different to the CLK state, that means the encoder is rotating clockwise
     if (digitalRead(DT) != aState) { 
      if(set_temperature < 480) set_temperature += 5;
     } 
     else if (set_temperature > 200){
       set_temperature -= 5;
     }
  }
  
aLastState = aState; // Updates the previous state of the CLK with the current state
}

