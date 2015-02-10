#ifndef ROBOT_H
#define ROBOT_H

#define SPEEDCONTROLCLASS Talon
#define DEADZONE 0.3

enum joysticks {
	js_0 = 0, //js = joy stick
	js_1 = 1,
	cs_a = 2, //cs = control stick it's the word for the button-y fake joysticks
	cs_b = 3
};

enum talons {
	t_fl = 5, //t = talon, f = front, l = left, c = center, r = right, b = back
	t_fc = 0,
	t_fr = 2,
	t_bl = 4,
	t_bc = 3,
	t_br = 1,
	w_m = 6
};

enum can_bus {
	ct_lift = 1
};

enum js_a_buttons {
	a_lyfte_button_down = 5,
	a_lyfte_button_up = 4
};

enum js_b_buttons {
	b_window_button_out = 5,
	b_window_button_in = 4
};

enum cs_a_buttons {
};

enum cs_b_buttons{
};

class Robot: public IterativeRobot
{
	Joystick stick0; // only joystick
	Joystick stick1; // only joystick
	SPEEDCONTROLCLASS leftFrontDrive;
	SPEEDCONTROLCLASS leftBackDrive;
	SPEEDCONTROLCLASS rightFrontDrive;
	SPEEDCONTROLCLASS rightBackDrive;
	SPEEDCONTROLCLASS strafeFrontDrive;
	SPEEDCONTROLCLASS strafeBackDrive;
	SPEEDCONTROLCLASS window_motor;
	CANTalon lyft;
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

	float drive_speed_ain_value;

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
