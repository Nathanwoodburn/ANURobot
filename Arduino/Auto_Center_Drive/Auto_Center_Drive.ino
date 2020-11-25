void driveautocorrect(){ // code to center the robot in the vent
if (sonar(tpin1,epin1) < s_W) // left wall too close
    {
      drive('S'); //drive slight right
    }
    else if (sonar(tpin1,epin1) > s_W) // right wall too close
    {
      drive('D'); //drive slight left
    }
    else // the robot is centered
    {
      drive('O'); // drive straight ahead
    }
}
