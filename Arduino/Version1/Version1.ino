#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo;
SoftwareSerial HM10(2, 3); //TX,RX
char input_char;
bool BLED;
int tpin = 8; //trigger pin
int epin = 9; //echo pin
int servo_pin = 10; //yellow pin is signal, Red pin is pwr, Brown is GND
//Pins for the motor control
int M_pin1 = 4;
int M_pin2 = 5;
int M_pin3 = 6;
int M_pin4 = 7;
int objdist = 7; //distance to detect obstacle
bool autom;
bool driveenabled = false; //disable motor until needed
void setup() {
  BLED = false;
  pinMode(13, OUTPUT);
  pinMode(M_pin1, OUTPUT);
  pinMode(M_pin2, OUTPUT);
  pinMode(M_pin3, OUTPUT);
  pinMode(M_pin4, OUTPUT);
  HM10.begin(9600);
  Serial.begin(9600);
  input_char = "";
  HM10.println("Waiting. . .");
  while (!HM10.available())
  {
    delay(2500);
    HM10.println("Waiting. . .");
    delay(2500);
  }
  input_char = HM10.read();
}
void loop() {
  HM10.println("Ready. . .");
  readbt();
  switch (input_char)
  {
    case 'Q':
      HM10.println("Moving servo to 0 deg");
      servo(0);
      break;
    case 'W':
      HM10.println("Moving servo to 45 deg");
      servo(45);
      break;
    case 'E':
      HM10.println("Moving servo to 90 deg");
      servo(90);
      break;
    case 'R':
      HM10.println("Moving servo to 135 deg");
      servo(135);
      break;
    case 'T':
      HM10.println("Moving servo to 180 deg");
      servo(180);
      break;
    case 'O':
      myservo.detach();
      HM10.println("Detached servo");
      break;
    case 'P':
      myservo.attach(servo_pin);
      HM10.println("Attached servo");
      break;
    case 'I':
      HM10.println("Servo Q-T,O,P");
      HM10.println("Drive 8,4,6");
      HM10.println("Sonar S");
      HM10.println("LED L");
      HM10.println("Auto A");
      HM10.println("");
      break;
    case 'S':
      HM10.print(sonar(tpin, epin)); //Prints to serial the distance detected by the sonar module
      HM10.println(" cm");
      break;
    case 'L':
      HM10.println("LED turned On/Off");
      if (BLED) {
        BLED = false;
        pinMode(13, LOW);
      }
      else {
        BLED = true;
        pinMode(13, HIGH);
      }
      break;
    case '8':
      drive('F');
      break;
    //case '2':
    //drive('B');
    //break;
    case '4':
      drive('L');
      break;
    case '6':
      drive('R');
      break;
    case 'A':
      autom = true;
      HM10.println("Auto mode starting. . .");
      HM10.println("To go to manual mode use the \"X\" Command");
      autom = true;
      for (;;) {
        if (autom) {
          Auto();
        }
        else
        {
          return;
        }
      }
      break;
    case '*':
      HM10.println("Hi");
      HM10.println("Use \"I\" to get a list of commands");
      break;
    default:
      break;
  }
}
void servo(int val1)
{
  val1 = 180 - val1;
  if (myservo.attached())
  {
    myservo.write(val1);
    HM10.println("");
    HM10.print("Servo set to ");
    HM10.print(val1);
    HM10.print(".");
    HM10.println("");
  }
  else
  {
    HM10.println("Servo not attached use the \"P\" command to attach servo");
  }
}
void readbt()
{
  while (!HM10.available()) {
  }
  input_char = HM10.read();
  while (HM10.available()) {
    char junk = HM10.read();
  }
}
long sonar(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  long pulsein1 = pulseIn(echoPin, HIGH);
  return 0.01723 * pulsein1;
}
void drive(char dir) {
  drive(dir, 1000);
}
void drive(char dir, int T)
{
  if (driveenabled)
  {

    HM10.print("Driving: ");
    HM10.println(dir);
    switch (dir) {
      case 'F':
        myservo.attach(servo_pin);
        servo(90);
        delay(50);
        int Sd = sonar(tpin, epin);
        if (Sd > objdist){ //collision avoid
        digitalWrite(M_pin1, HIGH);
        digitalWrite(M_pin3, HIGH);
        }
        else
        {
          HM10.println("Collision avoidance stopped your command.");
        }
        break;
      case 'L':
        digitalWrite(M_pin2, HIGH);
        digitalWrite(M_pin3, HIGH);
        break;
      case 'R':
        digitalWrite(M_pin1, HIGH);
        digitalWrite(M_pin4, HIGH);
        break;
    }
    HM10.print("Waiting: ");
    HM10.print(T);
    HM10.println(" ms");
    delay(T);
    digitalWrite(M_pin1, LOW);
    digitalWrite(M_pin2, LOW);
    digitalWrite(M_pin3, LOW);
    digitalWrite(M_pin4, LOW);
  }
  else
  {
    HM10.println("Drive motor disabled");
  }

}
void Auto()
{
  if (HM10.available())
  {
    if (HM10.read() == 'X')
    {
      autom = false;
    }
  }
  if (!myservo.attached())
  {
    myservo.attach(servo_pin);
  }
  servo(90);
  delay(1000);
  myservo.detach();
  int Sd = sonar(tpin, epin);
  while (Sd > objdist) {
    HM10.print("Distance to drive: ");
    HM10.println(Sd);
    HM10.println("Driving");
    drive('F');
    delay(1000);
    Sd = sonar(tpin, epin);

  }
  HM10.println("Found obstacle");
  if (!myservo.attached())
  {
    myservo.attach(servo_pin);
  }
  servo(0);
  delay(2000);
  int S1 = sonar(tpin, epin);
  HM10.print("Left distance is: ");
  HM10.println(S1);

  servo(180);
  delay(2000);
  int S2 = sonar(tpin, epin);
  HM10.print("Right distance is: ");
  HM10.println(S2);
  if (S1 > S2)
  {
    HM10.println("Turning left");
    drive('L', 2000);
    delay(5000);
  }
  else
  {
    HM10.println("Turning right");
    drive('R', 2000);
    delay(5000);
  }

}
