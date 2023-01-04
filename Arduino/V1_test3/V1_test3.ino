#include <SoftwareSerial.h>
// define variables
SoftwareSerial HM10(2, 3); //TX,RX
char input_char; //variable to hold the text send to the BT
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
const float s_W = 4.5; // Distance to the side walls //will need to adjust these after trying it out think this should be '4'
const float f_W = 5.0; // Distance to the front wall
const float d_v_s = 0.5; // variations in the distance to the side wall while driving forward
const float l_t_d = 7.0; // left turn distance the distance the before turns left

void setup() {
  // put your setup code here, to run once:
  HM10.begin(9600); // start the BT serial port
  pinMode(rmp1, OUTPUT); //Set pins to OUTPUT mode
  pinMode(rmp2, OUTPUT);
  pinMode(lmp1, OUTPUT);
  pinMode(lmp2, OUTPUT);
  pinMode(lme, OUTPUT);
  pinMode(rme, OUTPUT);
  HM10.println("Waiting. . ."); //wait for the user to connect
  while (!HM10.available()) //Wait for user response
  {
    delay(2500);
    HM10.println("Waiting. . ."); //tell user that the Arduino is waiting
    delay(2500);
  }
  input_char = HM10.read(); //read the user input
}
void loop() {
  // put your main code here, to run repeatedly:
  HM10.println("Ready. . ."); //tell the user that the arduino is ready for the next command
  readbt(); //read the user input
  switch (input_char) // shorter IF else if
  {
    case '8':
      {
        if (sonar(tpin2, epin2) > f_W) {
          long left_sonar;
          left_sonar = sonar(tpin1, epin1);
          HM10.println("Left sonar:");
          HM10.println(left_sonar);
          if (left_sonar > (s_W - d_v_s) && left_sonar < (s_W + d_v_s)) // the left wall is about the right distance away
          {
            drive('F'); //go straight forward

          }
          else if (left_sonar <= (s_W - d_v_s)) // the left wall is too close
          {
            drive('S'); //go slight right

          }
          else if (left_sonar >= (s_W + d_v_s)) // the left wall is too far
          {
            drive('D'); //go slight left

          }

        }
        else {
          HM10.println("Collision Avoidance code stopped your command");
        }
      }
      break;
    case '5':
      {
        if (sonar(tpin2, epin2) > f_W) {
          drive('F'); //go straight forward
        }
        else {
          HM10.println("Collision Avoidance code stopped your command");
        }
      }
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
      long left_sonar;
      long front_sonar;
      long right_sonar;
      delay(1000);
      left_sonar = sonar(tpin1, epin1);
      delay(1000);
      front_sonar = sonar(tpin2, epin2);
      delay(1000);
      right_sonar = sonar(tpin3, epin3);
      HM10.println("Left sonar reads:");
      HM10.println(left_sonar);
      //HM10.println(sonar(tpin1, epin1));
      HM10.println("Front sonar reads:");
      HM10.println(front_sonar);
      //HM10.println(sonar(tpin2, epin2));
      HM10.println("Right sonar reads:");
      HM10.println(right_sonar);
      //HM10.println(sonar(tpin3, epin3));
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
      HM10.println("Drive forward without auto correct: 5");
      //HM10.println("Drive slight left: 7");
      //HM10.println("Drive slight right: 9");
      HM10.println("Turn left: 4");
      HM10.println("Turn right: 6");
      HM10.println("Turn around: 2");
      HM10.println("Autonomous mode: A");
      HM10.println("Help: ?");
      HM10.println("Info: I");
      break;
    case 'I':
      HM10.println("");
      HM10.println("Code made by Nathan Woodburn");
      HM10.println("For more info on this project goto https://nathan.woodburn.au/ANURobot/");
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
  long left_sonar; //create and set variable to store the Sonar readings
  long front_sonar;
  long right_sonar;
  left_sonar = sonar(tpin1, epin1);
  front_sonar = sonar(tpin2, epin2);
  right_sonar = sonar(tpin3, epin3);
  HM10.println("Left sonar reads:"); //tell user the sonar readings
  HM10.println(left_sonar);
  HM10.println("Front sonar reads:");
  HM10.println(front_sonar);
  HM10.println("Right sonar reads:");
  HM10.println(right_sonar);
  //find the best way to go
  if (left_sonar > l_t_d) // if the left wall is far enough to be an exit
  {
    HM10.println("Found left turn");
    drive('L');
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
    HM10.println("This isn't supposed to happen!!!"); //An error that means the code if broken ;)
  }
  delay(100); //delay than loop again
}
void drive(char dir) //forward F; left L; right R; Turn around B; slight right S; slight left D
{
  digitalWrite(lme, HIGH);//turn motors on
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
