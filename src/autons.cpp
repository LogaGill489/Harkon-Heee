#include "main.h"

/////
// For instalattion, upgrading, documentations and tutorials, check out website!
// https://ez-robotics.github.io/EZ-Template/
/////

const int DRIVE_SPEED = 110; // This is 110/127 (around 87% of max speed).  We don't suggest making this 127.
                             // If this is 127 and the robot tries to heading correct, it's only correcting by
                             // making one side slower.  When this is 87%, it's correcting by making one side
                             // faster and one side slower, giving better heading correction.
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///

// It's best practice to tune constants when the robot is empty and with heavier game objects, or with lifts up vs down.
// If the objects are light or the cog doesn't change much, then there isn't a concern here.

void default_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.35, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.35, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void one_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void two_mogo_constants()
{
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);
}

void exit_condition_defaults()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

void modified_exit_condition()
{
  chassis.set_exit_condition(chassis.turn_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.swing_exit, 100, 3, 500, 7, 500, 500);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);
}

///
// Drive Example
///
void drive_example()
{
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater then the slew distance + a few inches

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(-12, DRIVE_SPEED);
  chassis.wait_drive();
}

///
// Turn Example
///
void turn_example()
{
  // The first parameter is target degrees
  // The second parameter is max speed the robot will drive at

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed()
{
  // wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();

  chassis.set_turn_pid(45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(-45, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  // When the robot gets to -6 inches, the robot will travel the remaining distance at a max speed of 40
  chassis.set_drive_pid(-24, DRIVE_SPEED, true);
  chassis.wait_until(-6);
  chassis.set_max_speed(40); // After driving 6 inches at DRIVE_SPEED, the robot will go the remaining distance at 40 speed
  chassis.wait_drive();
}

///
// Swing Example
///
void swing_example()
{
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is target degrees
  // The third parameter is speed of the moving side of the drive

  chassis.set_swing_pid(ez::LEFT_SWING, 45, SWING_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_until(12);

  chassis.set_swing_pid(ez::RIGHT_SWING, 0, SWING_SPEED);
  chassis.wait_drive();
}

///
// Interference example
///
void tug(int attempts)
{
  for (int i = 0; i < attempts - 1; i++)
  {
    // Attempt to drive backwards
    printf("i - %i", i);
    chassis.set_drive_pid(-12, 127);
    chassis.wait_drive();

    // If failsafed...
    if (chassis.interfered)
    {
      chassis.reset_drive_sensor();
      chassis.set_drive_pid(-2, 20);
      pros::delay(1000);
    }
    // If robot successfully drove back, return
    else
    {
      return;
    }
  }
}

// If there is no interference, robot will drive forward and turn 90 degrees.
// If interfered, robot will drive forward and then attempt to drive backwards.
void interfered_example()
{
  chassis.set_drive_pid(24, DRIVE_SPEED, true);
  chassis.wait_drive();

  if (chassis.interfered)
  {
    tug(3);
    return;
  }

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();
}


//gap from pre-generated code
//code below is made by me, not EZ-Template
//variables are declared under include, auton.hpp

// distance sensor
void distanceChallenge()
{
  // driving forward, where we use a distance sensor to detect when to stop

  // declaring distance sensor
  pros::Distance disSens(9);

  int wallDist = 10000;

  while (1 == 1)
  {
    //move forward
    for (int i = 0; i < 2; i++)
    {
      chassis.left_motors[i].move(50);
      chassis.right_motors[i].move(50);
    }

    wallDist = disSens.get(); // distance in mm

    if (wallDist <= 300)
    {
      // turningDistR(90);
      int degree = 90;

      // turning, will figure out what is approx 90 degrees
      long timerT = pros::millis();
      double timeDegree = degree * 3.4; // degress to time

      for (int i = 0; i < 2; i++)
      {
        chassis.left_motors[i].move(-110);
        chassis.right_motors[i].move(110);
      }

      // spin motors for desired time
      while (timeDegree + timerT > pros::millis())
      {
        pros::delay(10);
      }
    }
  }
}

// optic sensor
void opticChallenge()
{
  //declare optical and wacker
  pros::Optical opticSens(9);
  pros::Motor wacker(14);

  //get optical's proximity and hue
  int proxim = opticSens.get_proximity(); // detects proximity (0-255 range)
  double hue = opticSens.get_hue();       // range of hue (0 - 359.99 range)

  //string blockDetect;

  //bool range1 = false;
  //bool range2 = false;

  // if green go right
  // if orange go left

  // drive forward
  for (int i = 0; i < 2; i++)
  {
    chassis.left_motors[i].move(20); // sets motors to move forward
    chassis.right_motors[i].move(20);
  }

  while (1)
  {
    proxim = opticSens.get_proximity(); // detects proximity (0-255 range)
    hue = opticSens.get_hue();          // range of hue (0 - 359.99 range)
    // pros::screen::set_pen(COLOR_BLUE);
    pros::screen::print(pros::E_TEXT_MEDIUM, 3, "Read Hue, %lf", hue); //print hue to screen
    // print_to_screen("hue: %lf \n", hue);

    /*
    if (proxim < 200 && range1 == false) { //check if proixmity is within 60
      if (hue >= 120 && hue <= 135) { //orange hue range
        pros::screen::print(pros::E_TEXT_MEDIUM, 4, "Gween");
        blockDetect = "green";
        range1 = true;
      }
      else if (hue >= 18 && hue <= 25) { //green hue range
        pros::screen::print(pros::E_TEXT_MEDIUM, 4, "Ornage");
        blockDetect = "orange";
        range1 = true;
        range2 = true;
      }
    }

    //lines up to push the green block left
    if (blockDetect == "green" && proxim <= 100) {
      wacker = 70;
      pros::delay(300);
      wacker = 0;
      range2 == true;
    }

    //hits the green block left
    if (blockDetect == "green" && proxim <= 20 && range2 == true) {
      wacker = -110;
      pros::delay(300);
      wacker = 0;

      range1 = false;
      range2 = false;
      blockDetect = "";
    }

    //pushes orange block and resets position
    if (blockDetect == "orange" && proxim <= 20 && range2 == true) {
      wacker = 110;
      pros::delay(600);
      wacker = -70;
      pros::delay(600);
      wacker = 0;

      range1 = false;
      range2 = false;
      blockDetect = "";
    }
    */
    if (hue >= 120 && hue <= 135)
    { // orange hue range
      pros::screen::print(pros::E_TEXT_MEDIUM, 4, "Gween");
      wacker = -110;
    }
    else if (hue >= 18 && hue <= 25)
    { // green hue range
      pros::screen::print(pros::E_TEXT_MEDIUM, 4, "Ornage");
      wacker = 110;
    }
    else
    {
      //dont move
      wacker = 0;
    }
  }
}

// standard pid actions under EZ
void interialChallenge()
{
  //declaring motors
  pros::Motor H1(3);
  pros::Motor H2(20);
  pros::Motor_Group H({H1, H2});

  chassis.set_drive_pid(26, DRIVE_SPEED);
  chassis.wait_until(25);

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(27, DRIVE_SPEED);
  chassis.wait_until(25);

  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(28, DRIVE_SPEED);
  chassis.wait_drive();

  pros::delay(600); // pick #1

  chassis.set_drive_pid(-19, DRIVE_SPEED);
  chassis.wait_until(-17);

  chassis.set_turn_pid(90, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(22, DRIVE_SPEED);
  chassis.wait_drive();

  pros::delay(600); // delivery #1

  chassis.set_drive_pid(-20, DRIVE_SPEED);
  chassis.wait_until(-18);

  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(27, DRIVE_SPEED);
  chassis.wait_drive();

  pros::delay(600); // pick #2

  chassis.set_drive_pid(-21, DRIVE_SPEED);
  chassis.wait_until(-19);

  chassis.set_turn_pid(264, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(78, DRIVE_SPEED);
  chassis.wait_drive();

  pros::delay(600); // delivery #2

  chassis.set_drive_pid(-73, DRIVE_SPEED);
  chassis.wait_until(-71);

  chassis.set_turn_pid(180, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(24, DRIVE_SPEED);
  chassis.wait_drive();

  pros::delay(600); // pick #3

  chassis.set_drive_pid(-20, DRIVE_SPEED);
  chassis.wait_until(-18);

  chassis.set_turn_pid(264, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(78, DRIVE_SPEED);
  chassis.wait_drive();

  chassis.set_turn_pid(0, TURN_SPEED);
  chassis.wait_drive();

  chassis.set_drive_pid(46, DRIVE_SPEED);
  chassis.wait_drive();

  while (1)
  {
    pros::delay(100);
  }
}

void finalChallenge()
{
  pros::Motor_Group flyWheel({10, 20}); // flywheel ports 10 (left) & 20 (right), keep both positive
  // intake motor
  pros::Motor intake(1); // togglable motor for intake

  // intake.move(-127);
  flyWheel.move(127); //start flywheel to get it to speed

  chassis.set_drive_pid(15, DRIVE_SPEED); //drive up to ball
  chassis.wait_drive();

  intake.move(-127); //partially intake into robot
  pros::delay(300);

  intake.move(0);

  chassis.set_drive_pid(-22, DRIVE_SPEED); //drive to position closer to middle and launch
  chassis.wait_drive();
  intake.move(-127);

  pros::delay(3000); //3 second delay to ensure bot launches

  intake.move(0); //turn of robot
  flyWheel.move(0);

  //unused code
  /*
  chassis.set_turn_pid(225, 110);
  chassis.wait_drive();
  flyWheel.move(0);

  chassis.set_drive_pid(60, DRIVE_SPEED);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(300);

  intake.move(0);
  flyWheel.move(127);

  chassis.set_turn_pid(45, 110);
  chassis.wait_drive();

  intake.move(-127);
  pros::delay(2000);

  intake.move(0);
  flyWheel.move(0);
*/
}