/*
   Title: ANU Autonomous Rover
   Purpose: To deliver a load (of jelly beans)
            to a patient trapped in a collapsed
            building by traveling through an air
            duct autonomously without hitting the
            walls.
   Code By: Nathan Woodburn (nathan@anurobot.tech)
   For more info goto https://anurobot.tech
   For a Controller goto http://bt.anurobot.tech
   Date: 27/10/2020
*/

#include <SoftwareSerial.h> //add library for the bluetooth serial connection
SoftwareSerial HM10(2, 3); //Create a bluetooth serial connection in ports 2 (TX) & 3 (RX)
// define pins
const int rmp1 = 4; //motor pins Left motor pin 1
const int rmp2 = 5; // & 2
const int lmp1 = 6; //motor pins right motor pin 1
const int lmp2 = 7; // & 2
const int lme = 8; // motor enable pins for left motor
const int rme = 9; // & right motor
//trigger and echo pins for the three sonar modules
const int tpin1 = A0; //left sonar trig
const int epin1 = A1; // & echo
const int tpin2 = A2; //front sonar trig
const int epin2 = A3; // & echo
const int tpin3 = A4; //right sonar trig
const int epin3 = A5; // & echo
//define variable to hold input text
char input_char; //variable to hold the input text send through the BT
// define constants
const int s_W = 4; // Distance to the side walls, to make the robot in the centre of the path
const int f_W = 8; // Distance to the front wall, to make sure that the robot doesn't hit the front wall
const int t_d = 5; // Distance required to u-turn, between the front wall and the robot
const int r_t_d = 4; // Distance required to turn, between the side walls and the robot
const int d_v_s = 1; // Variations in the distance to the side wall while driving forward
const int l_t_d = 8; // Left turn distance, the distance the before the robot turns left around a corner
const int t_a_c = 7; // min distance to turn around corner
const int a_f_w = 2;

void setup() { // To run once when arduino powered on
  HM10.begin(9600); // start the BT serial port
  //Set motor pins to OUTPUT mode
  pinMode(rmp1, OUTPUT);
  pinMode(rmp2, OUTPUT);
  pinMode(lmp1, OUTPUT);
  pinMode(lmp2, OUTPUT);
  pinMode(lme, OUTPUT);
  pinMode(rme, OUTPUT);
  HM10.println("Waiting. . ."); // tell user that the ardunio in waiting to connect
  while (!HM10.available()) //Wait for user to respond
  {
    delay(2500); //wait
    HM10.println("Waiting. . ."); //remind the user that the Arduino is waiting
    delay(2500); //wait to stop the BT serial from crashing
  }
  input_char = HM10.read(); //read the user input to clear the input terminal
}
void loop() {
  // put your main code here, to run repeatedly:
  HM10.println("Ready. . ."); //tell the user that the arduino is ready to control
  readbt(); //read the user input
  switch (input_char) // shorter IF/else if
  {
    case '8': // if the user sent 8
      {
        if (sonar(tpin2, epin2) > f_W) { // check whether there is enough room to drive forward
          long left_sonar; // create a variable to store the left sonar reading
          left_sonar = sonar(tpin1, epin1); // read and store the left sonar reading
          HM10.println("Left sonar:"); // tell the user the value of the left sonar
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
        else { // the front wall is too close to allow for driving forward
          HM10.println("Collision Avoidance code stopped your command"); //tell user that the front wall is too close
          // this makes the user know that the arduino is working properly and not just broken
        }
      }
      break;
    case '5':  // if the user sent 5
      {
        long f_sonar = sonar(tpin2, epin2); // create variable to store the front sonar reading and set the variable
        HM10.print("Front sonar reads "); //tell the user the front sonar reading
        HM10.println(f_sonar);
        if (f_sonar > f_W) { // if there is room in front to drive

          drive('F'); //go straight forward
        }
        else { // there isn't room in front
          HM10.println("Collision Avoidance code stopped your command"); //tell user that the front wall is too close
          // this makes the user know that the arduino is working properly and not just broken
        }
      }
      break;
    case '2': // if the user sent 2
      {
        long f_sonar = sonar(tpin2, epin2); // create and set a variable to store the front sonar reading
        HM10.print("Front sonar reads "); //tell the user the front sonar reading
        HM10.println(f_sonar);
        long l_sonar = sonar(tpin1, epin1); // create and set a variable to store the left sonar reading
        HM10.print("Left sonar reads "); //tell the user the left sonar reading
        HM10.println(l_sonar);
        if (f_sonar >= t_d && l_sonar >= r_t_d) //check the wall distances to allow room to turn around
        {
          drive('B'); // turn around
        }
        else // not enough room to turn around
        {
          HM10.println("Collision Avoidance code stopped your command"); //tell user that the walls are too close
          // this makes the user know that the arduino is working properly and not just broken
        }
      }
      break;
    case '4': // if the user sent 4
      {
        long r_sonar = sonar(tpin3, epin3); // create and set a variable to store the right sonar reading
        HM10.print("Right sonar reads "); //tell the user the sonar reading
        HM10.println(r_sonar);
        if (r_sonar < r_t_d) // check if there's enough room to turn left
        { // there's not enough room
          HM10.println("Collision Avoidance code stopped your command"); //tell user that the right wall is too close
          // this makes the user know that the arduino is working properly and not just broken
        }
        else // there's enough room
        {
          drive('L'); // turn left
        }
      }
      break;
    case '6': // if the user sent 6
      {
        long l_sonar = sonar(tpin1, epin1); // create and set a variable to store the front sonar reading
        HM10.print("Left sonar reads "); //tell the user the sonar reading
        HM10.println(l_sonar);
        if (l_sonar < r_t_d) // check if there's enough room to turn right
        { // there's not enough room
          HM10.println("Collision Avoidance code stopped your command");//tell user that the left wall is too close
          // this makes the user know that the arduino is working properly and not just broken
        }
        else // there's enough room
        {
          drive('R'); // turn right
        }
      }
      break;
    case '7': // if the user sent 7
      drive('D'); // drive slight left
      break;
    case '9': // if the user sent 9
      drive('S'); // drive slight right
      break;
    case 'F':  // if the user sent F
      {
        long f_sonar = sonar(tpin2, epin2);  // create and set a variable to store the front sonar reading
        HM10.print("Front sonar reads "); //tell the user the sonar reading
        HM10.println(f_sonar);
        if (f_sonar > f_W * 2) { // check if there's enough room to drive forward
          drive('H'); // drive far forward
        }
        else { //there's not enough room
          HM10.println("Collision Avoidance code stopped your command"); //tell user that the front wall is too close
          // this makes the user know that the arduino is working properly and not just broken
        }
      }
      break;
    case '0': // if the user sent 0
      drive('Q'); // reverse
      break;
    case 'l':
      drive('4');
      break;
    case 'r':
      drive('6');
      break;
    case 'S': // if the user sent S
      // create variables to store the sonar readings
      long left_sonar;
      long front_sonar;
      long right_sonar;
      // set variables with the current sonar reading
      left_sonar = sonar(tpin1, epin1);
      front_sonar = sonar(tpin2, epin2);
      right_sonar = sonar(tpin3, epin3);
      //tell the user the sonar readings
      HM10.print("Left sonar reads: ");
      HM10.println(left_sonar);
      HM10.print("Front sonar reads: ");
      HM10.println(front_sonar);
      HM10.print("Right sonar reads: ");
      HM10.println(right_sonar);
      break;
    case 'A': // if the user sent A
      HM10.println("Auto mode starting. . ."); // tell the user that Auto mode is starting
      // and that the only way to go back to manual mode if by pressing the reset button
      HM10.println("To go to manual mode press the reset button");
      for (;;) { // infinite loop
        Auto(); // Run the autunomous driving code
      }
      break;
    case '?': // if the user sent ?
      // tell the user all the commands to control the robot
      HM10.println("Sonar: S");
      HM10.println("Drive forward: 8");
      HM10.println("Drive forward without auto correct: 5");
      HM10.println("Drive far forward: F");
      HM10.println("Drive slight left: 7");
      HM10.println("Drive slight right: 9");
      HM10.println("Turn left: 4");
      HM10.println("Turn right: 6");
      HM10.println("Turn around: 2");
      HM10.println("Reverse: 0 (Warning not safe in duct)");
      HM10.println("Autonomous mode: A");
      HM10.println("Help: ?");
      HM10.println("Info: I");
      break;
    case 'I': // if the user sent I
      // tell the user info about the code and project
      HM10.println("");
      HM10.println("Title: ANU Autonomous Rover");
      HM10.println("Purpose: To deliver a load (of jelly beans)");
      HM10.println("                to a patient trapped in a collapsed");
      HM10.println("                building by traveling through an air");
      HM10.println("                duct autonomously without hitting the");
      HM10.println("                walls.");
      HM10.println("Code By: Nathan Woodburn (nathan@anurobot.tech)");
      HM10.println("For more info goto https://anurobot.tech");
      HM10.println("For a Controller goto http://bt.anurobot.tech");
      HM10.println("Date: 27/10/2020");
      break;
    default: // if the user sent any other text
      HM10.println("For a list of commands use [?]"); // tell the user to use the ? command to get a list of all commands
      break;
  }
}
void readbt() // function to wait for the user to send text then set input_char as the first char sent
{
  while (!HM10.available()) { // wait until text is avaliable
  }
  input_char = HM10.read(); // set input_char as the first char sent
  while (HM10.available()) { // while text is avaliable
    char junk = HM10.read(); // create a temporary char and get the text from the BT serial port to clear the termainal
  }
}
void Auto() // function to control the rover autonomously
{ //Note: All outputs have been commented to stop the BT from crashing
  long left_sonar; //create variables to store the Sonar readings
  long front_sonar;
  long right_sonar;
  left_sonar = sonar(tpin1, epin1); //set sonar variables to sonar reading
  front_sonar = sonar(tpin2, epin2);
  right_sonar = sonar(tpin3, epin3);
  //tell the user the sonar reading
  //  HM10.println("Left:");
  //  HM10.println(left_sonar);
    HM10.println("Front:");
    HM10.println(front_sonar);
  //  HM10.println("Right:");
  //  HM10.println(right_sonar);


  if (left_sonar > l_t_d) //left turn found
  {

    for (int i = 4; i > 0; i = i - 1)
    {
      if (sonar(tpin2, epin2) > 3)
      {
        drive('O');
      }
    }
    HM10.println("Turn left");
    if (sonar(tpin1, epin1) > l_t_d) {
      drive('4'); //turn left
      for (int i = 3; i > 0; i = i - 1)
      {
        if (sonar(tpin2, epin2) > a_f_w)
        {
          drive('O');
        }
      }
      delay(2000); // delay 2 secs
    }
    else// wall to the left
    {
      HM10.println("Abort left turn");
    }
  }


  else if (front_sonar > a_f_w) //clear path ahead so drive forward
  {
    HM10.println("Forward");
    if (left_sonar < s_W) //too close to the left wall
    {
      //HM10.println("Slight right");
      drive('S'); //drive slight right
    }
    else if (left_sonar > s_W + 1) //too close to right wall
    {
      // HM10.println("Slight left");
      drive('D'); //drive slight left
    }
    else // right distance from the walls
    {
      //HM10.println("Straight forward");
      drive('O'); // drive straight ahead
    }
  }


  //all else after here happen when wall ahead
  else if (right_sonar > l_t_d) //right turn found
  {

    for (int i = 4; i > 0; i = i - 1)
    {
      if (sonar(tpin2, epin2) > 3)
      {
        drive('O');
      }
    }
    HM10.println("Turn Right");
    if (sonar(tpin3, epin3) > l_t_d) {
      drive('6'); //turn right
      for (int i = 3; i > 0; i = i - 1)
      {
        if (sonar(tpin2, epin2) > a_f_w)
        {
          drive('O');
        }
      }
    }
    else// wall to the left
    {
      HM10.println("Abort left turn");
    }
  }


  else if (front_sonar > t_d) //if enough room turn around
  {

    HM10.println("U-turn");
    drive('B'); //u-turn
  }


  else // else reverse than u-turn
  {
    // HM10.println("Reverse");
    drive('Q'); //reverse
    HM10.println("U-turn");
    drive('B'); //then u-turn
  }
  // temporary code to debug problems
  // HM10.println("Press enter to continue. . ."); // tell user that robot is waiting
  //readbt(); // wait until user sends text
  delay(500); //delay than loop again //will take out when finished testing

}
void drive(char dir) // Function to control the driving of the rover
{
  // Commands are as follows:
  // forward F; left L; right R; Turn around B; slight right S; slight left D;
  // reverse Q; far forwards H; turn around a corner left 4; right 6; small reverse P; small forwards O
  digitalWrite(lme, HIGH);//turn motors on
  digitalWrite(rme, HIGH);
  switch (dir) // shorter if/else if
  {
    case 'O':
      //HM10.println("Driving forward. . .");
      // Turn both motors on forwards
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(100); //wait 1 sec
      break;
    case 'F':
      //    HM10.println("Driving forward. . .");
      // Turn both motors on forwards
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(1000); //wait 1 sec
      break;
    case 'P':
      digitalWrite(lmp2, HIGH);
      digitalWrite(rmp2, HIGH);
      delay(500); // wait 1 sec
      break;
    case 'H':
      //  HM10.println("Driving forward. . .");
      // Turn both motors on forwards
      digitalWrite(lmp1, HIGH);
      digitalWrite(rmp1, HIGH);
      delay(2000); // wait 2 sec
      break;
    case 'L':
      //   HM10.println("Turning left. . .");
      digitalWrite(lmp2, HIGH); // Turn left motor backwards
      digitalWrite(rmp1, HIGH); // Turn right motor forwards
      delay(1500); // wait 1.5 sec
      break;
    case 'R':
      //   HM10.println("Turning right. . .");
      digitalWrite(lmp1, HIGH); // Turn left motor forwards
      digitalWrite(rmp2, HIGH); // Turn right motor backwards
      delay(1500); // Wiat 1.5 sec
      break;
    case 'B':
      //  HM10.println("Turning around. . .");
      digitalWrite(lmp1, HIGH); // Turn left motor forwards
      digitalWrite(rmp2, HIGH); // Turn right motor backwards
      delay(3200); // wait 3 sec
      break;
    case 'S':
      //  HM10.println("Drive slight right. . .");
      digitalWrite(lmp1, HIGH); // turn left motor forwards
      delay(200); // wait 0.5 sec
      digitalWrite(rmp1, HIGH); // turn right motor forwards
      delay(100); // wait 0.4 sec
      break;
    case 'D':
      //   HM10.println("Drive slight left. . .");
      digitalWrite(rmp1, HIGH); // turn right motor forwards
      delay(200); // wait 0.5 sec
      digitalWrite(lmp1, HIGH); // turn left motor forwards
      delay(100); // wait 0.4 sec
      break;
    case 'Q':
      //  HM10.println("Reversing. . .");
      // turn both motors on backwards
      digitalWrite(lmp2, HIGH);
      digitalWrite(rmp2, HIGH);
      delay(1000); // wait 1 sec
      break;
    case '4':
      //  HM10.println("Turning left. . .");
      digitalWrite(rmp1, HIGH); // turn right motor forwards
      delay(3000); // wait 3 sec
      break;
    case '6':
      //   HM10.println("Turning right. . .");
      digitalWrite(lmp1, HIGH); // turn left motor forwards
      delay(3000); // wait 3 sec
      break;
    default:
      break;
  }
  // turn all motor pins off
  digitalWrite(lmp1, LOW);
  digitalWrite(lmp2, LOW);
  digitalWrite(rmp1, LOW);
  digitalWrite(rmp2, LOW);
  digitalWrite(lme, LOW);
  digitalWrite(rme, LOW);
}
long sonar(int triggerPin, int echoPin) // function calculate and return sonar readings
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
  // calculate the distance in centimeters
  return 0.01723 * pulsein1;
}
