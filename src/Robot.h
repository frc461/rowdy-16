#ifndef ROBOT_H
#define ROBOT_H

// Comment this out if you are not using the practice robot.
#define PRACTICE 1

#ifdef PRACTICE
	#define SPEEDCONTROLCLASS Jaguar
	#define SPECIALCONTROLCLASS Victor
#else
	#define SPEEDCONTROLCLASS Talon
	#define SPECIALCONTROLCLASS CANTalon
#endif

#define DEADZONE 0.3

enum joysticks {
	js_a = 0, //js = joy stick
	js_b = 1,
	js_cs_a = 2, //cs = control stick it's the word for the button-y fake joysticks
	js_cs_b = 3
};

enum talons {
	t_fl = 5, //t = talon, f = front, l = left, c = center, r = right, b = back
	t_fc = 0,
	t_fr = 2,
	t_bl = 4,
	t_bc = 3,
	t_br = 1,
	t_w = 6,
	t_rol = 7
};

enum can_bus {
	#ifdef PRACTICE
		ct_lift = 9
	#else
		ct_lift = 1
	#endif
};

enum js_a_buttons {
};

enum js_b_buttons {
};

// Please test these buttons!
enum cs_a_buttons {
	a_clamp_in = 9,
	a_clamp_out = 10,
	a_roller_in = 12,
	a_roller_out = 11
};

enum cs_b_buttons {
	b_lift_up = 2,
	b_lift_down = 3
};

class Robot: public IterativeRobot
{
	Joystick stick0; // only joystick
	Joystick stick1; // only joystick
	Joystick control_system_a;
	Joystick control_system_b;
	SPEEDCONTROLCLASS leftFrontDrive;
	SPEEDCONTROLCLASS leftBackDrive;
	SPEEDCONTROLCLASS rightFrontDrive;
	SPEEDCONTROLCLASS rightBackDrive;
	SPEEDCONTROLCLASS strafeFrontDrive;
	SPEEDCONTROLCLASS strafeBackDrive;
	SPEEDCONTROLCLASS window_motor;
	SPEEDCONTROLCLASS roller_motor;
	SPECIALCONTROLCLASS lift;
	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;

	double raw_0_x; //the raw values
	double raw_1_x;
	double raw_0_y;
	double raw_1_y;

	double nt_0_x; //the not raw values
	double nt_1_x;
	double nt_0_y;
	double nt_1_y;

 public:
	Robot();
 private:
	double nullify(double n);
	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestPeriodic();
};

#endif
