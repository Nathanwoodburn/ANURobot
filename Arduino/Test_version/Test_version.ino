#include <SoftwareSerial.h>
// define variables
// currently using pins 2-9 and A0-A5
SoftwareSerial HM10(2, 3); //TX,RX
char input_char;
const int rmp1 = 4; //motor pins Left motor pin 1&2
const int rmp2 = 5;
const int lmp1 = 6;//motor pins right motor pin 1&2
const int lmp2 = 7;
const int lme = 8; // motor enable pins left motor
const int rme = 9; // right motor
//trigger and echo pins for three sonar modules
const int tpin1 = A0; //left sonar
const int epin1 = A1;
const int tpin2 = A2; //front sonar
const int epin2 = A3;
const int tpin3 = A4; //right sonar
const int epin3 = A5;
const int s_W = 4; // Distance to the side walls //will need to adjust these after trying it out
const int f_W = 8; // Distance to the front wall
const int d_v_s = 2; // variations in the distance to the side wall while driving forward
const int l_t_d = 6; // left turn distance the distance the before turns left

void setup() {
  // put your setup code here, to run once:
  HM10.begin(9600);
  pinMode(rmp1, OUTPUT);
  pinMode(rmp2, OUTPUT);
  pinMode(lmp1, OUTPUT);
  pinMode(lmp2, OUTPUT);
  pinMode(lme, OUTPUT);
  pinMode(rme, OUTPUT);
  HM10.println("Waiting. . ."); //wait for the user to connect
  while (!HM10.available())
  {
    delay(2500);
    HM10.println("Waiting. . .");
    delay(2500);
  }
  input_char = HM10.read();
}
void loop() {
  // put your main code here, to run repeatedly:
  HM10.println("Ready. . .");
  readbt();
  switch (input_char)
  {
    case '8':
      /*      if (sonar(tpin2, epin2) > f_W) {
              long left_sonar;
              left_sonar = sonar(tpin1, epin1);
              //long left_sonar;
              //left_sonar = sonar(tpin1, epin1);
              if (left_sonar > (s_W - d_v_s) && left_sonar < (s_W + d_v_s)) // the right wall is about the right distance away
              {
                drive('F'); //go forward
                //HM10.println("Straight forward");
              }
              else if (left_sonar <= (s_W - d_v_s)) // the right wall is too close
              {
                drive('D'); //go slight left
                //HM10.println("Slight left");
              }
              else if (left_sonar >= (s_W + d_v_s)) // the right wall is too far
              {
                drive('S'); //go slight right
                //HM10.println("Slight right");
              }
              //drive('F');
            }
            else {
              HM10.println("Collision Avoidance code stopped your command");
            }*/
      drive('F');

      break;
    case '2':
      drive('B');
      break;
    case '4':
      drive('L');
      break;
    case '6':
      drive('R');
      break;
    case 'S': //get sonar readings
    {
      long left_sonar;
      //long front_sonar;
      //long right_sonar;
      left_sonar = sonar(tpin1, epin1);
      delay(1000);
      //HM10.println(sonar(tpin1,epin1));
      //HM10.println(tpin1);
      //front_sonar = sonar(tpin2, epin2);
      //right_sonar = sonar(tpin3, epin3);
      HM10.println("Left sonar reads:");
      HM10.println(sonar(tpin1,epin1));
      HM10.println(left_sonar);
      //HM10.println("Front sonar reads:");
      //HM10.println(front_sonar);
      //HM10.println("Right sonar reads:");
    } //HM10.println(right_sonar);
      break;
    case 'A':
      HM10.println("Auto mode starting. . .");
      HM10.println("To go to manual mode press the reset button");
      for (;;) {
        Auto();
      }
      break;
    case '?':
      HM10.println("Sonar: S");
      HM10.println("Drive forward: 8");
      //HM10.println("Drive slight left: 7");
      //HM10.println("Drive slight right: 9");
      HM10.println("Turn left: 4");
      HM10.println("Turn right: 6");
      HM10.println("Turn around: 2");
      HM10.println("Autonomous mode: A");
      HM10.println("Help: ?");
      HM10.println("");
      break;
    default:
      break;
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
void Auto()
{
  long left_sonar;
  long front_sonar;
  long right_sonar;
  left_sonar = sonar(tpin1, epin1);
  front_sonar = sonar(tpin2, epin2);
  right_sonar = sonar(tpin3, epin3);
  HM10.println("Left sonar reads:");
  HM10.println(left_sonar);
  HM10.println("Front sonar reads:");
  HM10.println(front_sonar);
  //HM10.println("Right sonar reads:");
  //HM10.println(right_sonar);
  //find the best way to go
  if (left_sonar > l_t_d)
  {
    HM10.println("Found left turn");
    drive('F');
    drive('F');
    drive('L');
    drive('F');
    drive('F');
  }
  else if (front_sonar > f_W) // clear path ahead
  {
    HM10.println("Driving forward. . .");
    HM10.println("Detecting wall distances. . .");
    if (left_sonar > (s_W - d_v_s) && left_sonar < (s_W + d_v_s)) // the left wall is about the right distance away
    {
      drive('F'); //go forward
      //HM10.println("Straight forward");
    }
    else if (left_sonar <= (s_W - d_v_s)) // the left wall is too close
    {
      drive('D'); //go slight left
      //HM10.println("Slight left");
    }
    else if (left_sonar >= (s_W + d_v_s)) // the left wall is too far
    {
      drive('S'); //go slight right
      //HM10.println("Slight right");
    }
  }
  else if (left_sonar <= (s_W + 10) && right_sonar > (s_W + 10)) //turn right
  {
    drive('R');
    HM10.println("Turn right");
  }
  else if (left_sonar > (s_W + 10) && right_sonar > (s_W + 10)) //turn right
  {
    drive('L');
    HM10.println("Turn left");
  }
  else if (left_sonar > (s_W + 10) && right_sonar <= (s_W + 10)) //turn left
  {
    drive('L');
    HM10.println("Turn left");
  }
  else if (left_sonar <= (s_W + 10) && right_sonar <= (s_W + 10)) //turn around
  {
    drive('B');
    HM10.println("Turn around");
  }
  else
  {
    HM10.println("This isn't supposed to happen!!!");
  }
  delay(100);
}
void drive(char dir) //forward F; left L; right R; Turn around B; slight right S; slight left D
{
  digitalWrite(lme, HIGH);
  digitalWrite(rme, HIGH);
  switch (dir)
  {
    //driving
    case 'F':
      HM10.println("Driving forward. . .");
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(1000);
      break;
    case 'L':
      HM10.println("Turning left. . .");
      digitalWrite(lmp2, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(4000);
      break;
    case 'R':
      HM10.println("Turning right. . .");
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp2, HIGH);
      delay(4000);
      break;
    case 'B':
      HM10.println("Turning around. . .");
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp2, HIGH);
      delay(8000);
      break;
    case 'S':
      HM10.println("Drive slight right. . .");
      digitalWrite(lmp1, HIGH);
      //digitalWrite(rmp1, HIGH);
      delay(1000);
      break;
    case 'D':
      HM10.println("Drive slight left. . .");
      //digitalWrite(lmp1, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(4000);
      break;
    default:
      break;
  }
  digitalWrite(lmp1, LOW);
  digitalWrite(lmp2, LOW);
  digitalWrite(rmp1, LOW);
  digitalWrite(rmp2, LOW);
  digitalWrite(lme, LOW);
  digitalWrite(rme, LOW);
}
float sonar(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
//  HM10.println("Turn on");
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  //HM10.println("Pulse in");
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  long pulsein1 = pulseIn(echoPin, HIGH);
//  HM10.println(pulsein1);
  return 0.01723 * pulsein1;
}
