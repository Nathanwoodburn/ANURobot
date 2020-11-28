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
const int s_W = 5; // Distance to the side walls, to make the robot in the centre of the path
const int f_W = 8; // Distance to the front wall, to make sure that the robot doesn't hit the front wall
const int t_d = 5; // Distance required to u-turn, between the front wall and the robot
const int r_t_d = 4; // Distance required to turn, between the side walls and the robot
const int d_v_s = 1; // Variations in the distance to the side wall while driving forward
const int l_t_d = 9; // Left turn distance, the distance the before the robot turns left around a corner
const int t_a_c = 7; // min distance to turn around corner
const int a_f_w = 5; //distance to decide there is a wall in front to allow turning without collisitons
const int t_t = 3000; //time set to turn around a corner while driving autonomously (this is in hundredths of a second


void setup() { // To run once when arduino powered on
  HM10.begin(9600); // start the BT serial port
  //Set motor pins to OUTPUT mode
  pinMode(rmp1, OUTPUT);
  pinMode(rmp2, OUTPUT);
  pinMode(lmp1, OUTPUT);
  pinMode(lmp2, OUTPUT);
  pinMode(lme, OUTPUT);
  pinMode(rme, OUTPUT);
  //Give info about the robot at startup
  HM10.println("Hello, my name is ANUROBOT.TECH");
  HM10.println("To find out more about me goto https://anurobot.tech");
  HM10.println("Thanks");
  HM10.println("");
}
void loop() {  // main code loop, to run repeatedly
  HM10.println("Ready. . ."); //tell the user that the arduino is ready for commands
  readbt(); //read the user input from bluetooth (see below for function code)
  HM10.println(""); //Write a blank line to bluetooth
  switch (input_char) // shorter IF/else if
  {
    case '8': // if the user sent 8
      {
        if (sonar(tpin2, epin2) > f_W) { // check whether there is enough room to drive forward
          long left_sonar; // create a variable to store the left sonar reading
          left_sonar = sonar(tpin1, epin1); // read and store the left sonar reading
          HM10.println("Left sonar:"); // tell the user the value of the left sonar
          HM10.println(left_sonar);
          driveautocorrect();
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
        HM10.println("To exit Autonomous mode press the reset button");

        for (;;) { // infinite loop
          Auto(); // Run the autonomous driving code
        }
        break;
      case '?': // if the user sent ?
        // give the user a list of avaliable commands
        HM10.println("Sonar: S");
        HM10.println("Drive forward: 8");
        HM10.println("Drive forward without auto correct: 5");
        HM10.println("Drive far forward: F");
        HM10.println("Drive slight left: 7");
        HM10.println("Drive slight right: 9");
        HM10.println("Turn left: 4");
        HM10.println("Turn right: 6");
        HM10.println("Turn left around corner: l");
        HM10.println("Turn right around corner: r");
        HM10.println("Turn around: 2");
        HM10.println("Reverse: 0 (Warning not safe in duct)");
        HM10.println("Autonomous mode: A");
        HM10.println("Help: ?");
        HM10.println("Info: I");
        break;
      case 'a':
        for (;;)
        {
          Auto2();
        }
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
  void Auto() // function to control the rover autonomously
  { //Note: Most bluetooth communication has been disabled to stop the BT from crashing
    long left_sonar; //create variables to store the Sonar readings
    long front_sonar;
    long right_sonar;
    left_sonar = sonar(tpin1, epin1); //set sonar variables to sonar reading
    front_sonar = sonar(tpin2, epin2);
    right_sonar = sonar(tpin3, epin3);

    if (left_sonar > l_t_d) //left turn found
    {
      // this code is forces the robot to center in the turn
      for (int i = 3; i > 0; i = i - 1) // repeat code three times
      {
        if (sonar(tpin2, epin2) > 3) // if there is room ahead
        {
          drive('O'); // drive forwards
        }
      }

      HM10.println("Turn left");  // give feedback to user
      if (sonar(tpin1, epin1) > l_t_d) { // double check that the left wall if far enough to turn left
        // reverse to set the distance to the front wall
        while (sonar(tpin2, epin2) < 9) // while front wall is closer than 9 units
        {
          drive('P'); // reverse
        }
        drive('4'); //turn left
        // this code stops the robot from detecting the corner again
        for (int i = 8; i > 0; i = i - 1) // loop 8 times
        {
          if (sonar(tpin2, epin2) > a_f_w) // if there is room in front
          {
            drive('O'); // drive forward twice
            drive('O');
          }
        }
      }
      else // there is a wall to the left
      {
        HM10.println("Abort left turn"); // Alert User
      }
    }


    else if (front_sonar > a_f_w) //clear path ahead so drive forward
    {
      // code to center the robot in the middle of the vent
      driveautocorrect();
    }


    //  wall ahead

    else if (right_sonar > l_t_d) //right turn found
    {

      HM10.println("Turn Right"); // give feedback to user
      if (sonar(tpin3, epin3) > l_t_d) { // double check that there is enough room to turn right

        // reverse to set the distance to the front wall
        while (sonar(tpin2, epin2) < 9) // while front wall is closer than 9 units
        {
          drive('P'); // reverse
        }

        if (sonar(tpin1, epin1) > l_t_d) // if there is enough room to turn left
        {
          HM10.println("Was turning right but found Left turn"); // tell user there was an error in the measurements
        }
        else
        {
          drive('6'); //turn right
        }

        // make sure the robot doesn't find the turn again
        for (int i = 8; i > 0; i = i - 1) // loop 8 times
        {
          if (sonar(tpin2, epin2) > a_f_w) // if there is room in front
          {
            drive('O'); // drive forward twice
            drive('O');
          }
        }
        drive('O'); // drive forwards once more just to be sure
      }
      else // right wall too close
      {
        HM10.println("Abort right turn"); // Alert user
      }
    }

    // its a dead end
    else if (front_sonar > t_d) //if enough room turn around
    {

      HM10.println("U-turn"); // Tell user whats happening
      drive('B'); // u-turn
    }

    else // there isn't enough room to perform a u-turn
    {
      while (sonar(tpin2, epin2) > 6) // while the front wall in closer than 6 units
      {
        drive('O'); // drive forwards
      }

      if (sonar(tpin1, epin1) <= l_t_d) // if the left wall is close enough
      {
        if (sonar(tpin3, epin3) <= l_t_d) // if the right wall is close enough
        {
          if (sonar(tpin2, epin2) <= 6) // if the left wall is closer than 6 units
          {
            drive('B'); // u-turn
          }
          else
          {
            HM10.println("Break 1"); // tell user where it is in the code
            while (sonar(tpin2, epin2) > 6) // while the front wall is closer than 6 units
            {
              drive('O'); // drive forwards
            }
            if (sonar(tpin1, epin1) > l_t_d) // there is a left turn
            {
              drive('Q'); // reverse
              drive('4'); //turn left

              // make sure the robot doesn't detect that turn again
              for (int i = 3; i > 0; i = i - 1) // repeat code three times
              {
                if (sonar(tpin2, epin2) > a_f_w) // if front wall if far enough ahead
                {
                  drive('O'); // drive forwards
                }
              }
              drive('O'); // drive forward twice to be extra sure
              drive('O');
            }
            else if (sonar(tpin3, epin3) > l_t_d) // there is a right turn
            {
              drive('Q'); // Reverse
              drive('6'); //turn right

              // make sure the robot doesn't detect that turn again
              for (int i = 3; i > 0; i = i - 1) // repeat code three times
              {
                if (sonar(tpin2, epin2) > a_f_w) // if front wall if far enough ahead
                {
                  drive('O'); // drive forwards
                }
              }
              drive('O'); // drive forward twice to be extra sure
              drive('O');
            }
            else // if there is a dead end
            {
              while (sonar(tpin1, epin1) < t_d) // while the front distance is too small to u-turn
              {
                drive('Q'); // reverse
              }
              drive('B'); // u-turn
            }
          }
        }
      }

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
  
  void driveautocorrect() {
    if (sonar(tpin1, epin1) < s_W) // left wall too close
    {
      drive('S'); //drive slight right
    }
    else if (sonar(tpin1, epin1) > s_W) // right wall too close
    {
      drive('D'); //drive slight left
    }
    else // the robot is centered
    {
      drive('O'); // drive straight ahead
    }
  }
  void driveautocorrectl() {
    long left_sonar; //create variables to store the Sonar readings
    long front_sonar;
    long right_sonar;
    left_sonar = sonar(tpin1, epin1); //set sonar variables to sonar reading
    front_sonar = sonar(tpin2, epin2);
    right_sonar = sonar(tpin3, epin3);
    HM10.println(right_sonar);
    Check:
    
    if (right_sonar > s_W) // left wall too close
    {
      drive('S'); //drive slight right
    }
    else if (right_sonar < s_W) // right wall too close
    {
      drive('D'); //drive slight left
    }
    else // the robot is centered
    {
      drive('O'); // drive straight ahead
    }
//    if (sonar(tpin3, epin3) > 5) // left wall too close
//    {
//      if (sonar(tpin3,epin3) > 5)
//      {
//      drive('S'); //drive slight right
//      }
//      else
//      {
//        goto Check;
//      }
//      
//    }
//    else if (sonar(tpin3, epin3) < 5) // right wall too close
//    {
//      if (sonar(tpin3,epin3) < 5)
//      {
//      drive('D'); //drive slight left
//      }
//      else
//      {
//        goto Check;
//      }
//    } 
//    else // the robot is centered
//    {
//      drive('O'); // drive straight ahead
//    }
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
      case 'O': // if the command sent was 'O'
        // Turn both motors on forwards
        digitalWrite(lmp1, HIGH);
        digitalWrite(rmp1, HIGH);
        delay(100); //wait 0.1 sec
        break;
      case 'F': // if the command sent was 'F'
        // Turn both motors on forwards
        digitalWrite(lmp1, HIGH);
        digitalWrite(rmp1, HIGH);
        delay(1000); //wait 1 sec
        break;
      case 'P': // if the command sent was 'P'
        // Turn both motors on backwards
        digitalWrite(lmp2, HIGH);
        digitalWrite(rmp2, HIGH);
        delay(500); // wait 0.5 sec
        break;
      case 'H': // if the command sent was 'H'
        // Turn both motors on forwards
        digitalWrite(lmp1, HIGH);
        digitalWrite(rmp1, HIGH);
        delay(2000); // wait 2 sec
        break;
      case 'L': // if the command sent was 'L'
        digitalWrite(lmp2, HIGH); // Turn left motor backwards
        digitalWrite(rmp1, HIGH); // Turn right motor forwards
        delay(1500); // wait 1.5 sec
        break;
      case 'R':
        digitalWrite(lmp1, HIGH); // Turn left motor forwards
        digitalWrite(rmp2, HIGH); // Turn right motor backwards
        delay(1500); // Wiat 1.5 sec
        break;
      case 'B': // if the command sent was 'B'
        digitalWrite(lmp1, HIGH); // Turn left motor forwards
        digitalWrite(rmp2, HIGH); // Turn right motor backwards
        delay(3200); // wait 3.2 sec
        break;
      case 'S': // if the command sent was 'S'
        digitalWrite(lmp1, HIGH); // turn left motor forwards
        delay(200); // wait 0.2 sec
        digitalWrite(rmp1, HIGH); // turn right motor forwards
        delay(100); // wait 0.1 sec
        break;
      case '3': // if the command sent was '3'
        digitalWrite(lmp2, HIGH); // turn left motor backwards
        delay(400); // wait 0.4 sec
        digitalWrite(rmp2, HIGH); // turn right motor backwards
        delay(100); // wait 0.1 sec
        break;
      case 'D': // if the command sent was 'D'
        digitalWrite(rmp1, HIGH); // turn right motor forwards
        delay(200); // wait 0.2 sec
        digitalWrite(lmp1, HIGH); // turn left motor forwards
        delay(100); // wait 0.1 sec
        break;
      case 'Q': // if the command sent was 'Q'
        // turn both motors on backwards
        digitalWrite(lmp2, HIGH);
        digitalWrite(rmp2, HIGH);
        delay(1000); // wait 1 sec
        break;
      case '4': // if the command sent was '4'
        digitalWrite(rmp1, HIGH); // turn right motor forwards
        delay(t_t); // wait for the prechosen time
        break;
      case '6': // if the command sent was '6'
        digitalWrite(lmp1, HIGH); // turn left motor forwards
        delay(t_t); // wait for the prechosen time
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
    pinMode(triggerPin, OUTPUT);  // set pinmode of the triggerPin to output
    digitalWrite(triggerPin, LOW); // Clear the trigger
    delayMicroseconds(2); // for 2 microseconds
    // Sets the trigger pin to HIGH state for 10 microseconds
    digitalWrite(triggerPin, HIGH); // turn pin on
    delayMicroseconds(10); // wait 10 microseconds
    digitalWrite(triggerPin, LOW); // then turn it off
    pinMode(echoPin, INPUT); // set pinmode of echopin to input
    // Reads the echo pin, and returns the sound wave travel time in microseconds
    long pulsein1 = pulseIn(echoPin, HIGH);
    // calculate the distance in centimeters
    return 0.01723 * pulsein1;
  }
  void Auto2() // function to control the rover autonomously by following the right wall
  { //Note: Most bluetooth communication has been disabled to stop the BT from crashing
    long left_sonar; //create variables to store the Sonar readings
    long front_sonar;
    long right_sonar;
    left_sonar = sonar(tpin1, epin1); //set sonar variables to sonar reading
    front_sonar = sonar(tpin2, epin2);
    right_sonar = sonar(tpin3, epin3);

    if (right_sonar > l_t_d) // right turn found
    {
      // this code is forces the robot to center in the turn
      for (int i = 3; i > 0; i = i - 1) // repeat code three times
      {
        if (sonar(tpin2, epin2) > 3) // if there is room ahead
        {
          drive('O'); // drive forwards
        }
      }

      HM10.println("Turn right");  // give feedback to user
      if (sonar(tpin3, epin3) > l_t_d) { // double check that the left wall if far enough to turn left
        // reverse to set the distance to the front wall
        while (sonar(tpin2, epin2) < 9) // while front wall is closer than 9 units
        {
          drive('P'); // reverse
        }
        drive('6'); //turn right
        // this code stops the robot from detecting the corner again
        for (int i = 8; i > 0; i = i - 1) // loop 8 times
        {
          if (sonar(tpin2, epin2) > a_f_w) // if there is room in front
          {
            drive('O'); // drive forward twice
            drive('O');
          }
        }
      }
      else // there is a wall to the left
      {
        HM10.println("Abort left turn"); // Alert User
      }
    }


    else if (front_sonar > a_f_w) //clear path ahead so drive forward
    {
      // code to center the robot in the middle of the vent
      driveautocorrectl();
      
    }


    //  wall ahead

    else if (left_sonar > l_t_d) // left turn found
    {

      HM10.println("Turn Left"); // give feedback to user
      if (sonar(tpin1, epin1) > l_t_d) { // double check that there is enough room to turn right

        // reverse to set the distance to the front wall
        while (sonar(tpin2, epin2) < 9) // while front wall is closer than 9 units
        {
          drive('P'); // reverse
        }

        if (sonar(tpin3, epin3) > l_t_d) // if there is enough room to turn left
        {
          HM10.println("Was turning left but found right turn"); // tell user there was an error in the measurements
           drive('6'); //turn right
        }
        else
        {
          drive('4'); //turn left
        }

        // make sure the robot doesn't find the turn again
        for (int i = 8; i > 0; i = i - 1) // loop 8 times
        {
          if (sonar(tpin2, epin2) > a_f_w) // if there is room in front
          {
            drive('O'); // drive forward twice
            drive('O');
          }
        }
        drive('O'); // drive forwards once more just to be sure
      }
      else // right wall too close
      {
        HM10.println("Abort right turn"); // Alert user
      }
    }

    // its a dead end
    else if (front_sonar > t_d) //if enough room turn around
    {

      HM10.println("U-turn"); // Tell user whats happening
      drive('B'); // u-turn
    }

    else // there isn't enough room to perform a u-turn
    {
      while (sonar(tpin2, epin2) > 6) // while the front wall in closer than 6 units
      {
        drive('O'); // drive forwards
      }

      if (sonar(tpin3, epin3) <= l_t_d) // if the right wall is close enough
      {
        if (sonar(tpin1, epin1) <= l_t_d) // if the left wall is close enough
        {
          if (sonar(tpin2, epin2) <= 6) // if the front wall is closer than 6 units
          {
            drive('B'); // u-turn
          }
          else
          {
            while (sonar(tpin2, epin2) > 6) // while the front wall is closer than 6 units
            {
              drive('O'); // drive forwards
            }
            if (sonar(tpin3, epin3) > l_t_d) // there is a right turn
            {
              drive('Q'); // reverse
              drive('6'); //turn right

              // make sure the robot doesn't detect that turn again
              for (int i = 3; i > 0; i = i - 1) // repeat code three times
              {
                if (sonar(tpin2, epin2) > a_f_w) // if front wall if far enough ahead
                {
                  drive('O'); // drive forwards
                }
              }
              drive('O'); // drive forward twice to be extra sure
              drive('O');
            }
            else if (sonar(tpin1, epin1) > l_t_d) // there is a left turn
            {
              drive('Q'); // Reverse
              drive('4'); //turn left

              // make sure the robot doesn't detect that turn again
              for (int i = 3; i > 0; i = i - 1) // repeat code three times
              {
                if (sonar(tpin2, epin2) > a_f_w) // if front wall if far enough ahead
                {
                  drive('O'); // drive forwards
                }
              }
              drive('O'); // drive forward twice to be extra sure
              drive('O');
            }
            else // if there is a dead end
            {
              while (sonar(tpin2, epin2) < t_d) // while the front distance is too small to u-turn
              {
                drive('Q'); // reverse
              }
              drive('B'); // u-turn
            }
          }
        }
      }
    }
  }
