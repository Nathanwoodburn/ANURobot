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
