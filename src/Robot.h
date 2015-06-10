#ifndef ROBOT_H
#define ROBOT_H


// Comment this out if you are not using the practice robot.
//#define PRACTICE 0

// Set this to false to use the other auton.
enum  auton {
	CONTAINER = 0,
	BORING = 1
};

//#define PRACTICE 1

#ifdef PRACTICE
	#define SPEEDCONTROLCLASS Jaguar
	#define SPECIALCONTROLCLASS CANTalon
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

enum p_w_m {
	t_fl = 5, //t = talon, f = front, l = left, c = center, r = right, b = back, rl = roller, tun = tunnel
	t_fc = 0,
	t_fr = 2,
	t_bl = 4,
	t_bc = 3,
	t_br = 1,
//	t_frl_r = 6,
//	t_frl_l = 8,
	t_tun_rol = 7,
	t_lights = 10,
	t_more_lights = 11,
	t_and_more_lights = 12

};

enum can_bus {
	ct_lift = 1,
	ct_pcm_can = 2
};

enum pcm {
	pcm_stopper_f = 0, //f = forward, r = reverse
	pcm_stopper_r = 1,
	pcm_grabber_f = 2,
	pcm_grabber_r = 3
};

enum js_a_buttons {
	js_a_slow_down = 1,
	js_a_b_half = 2, //f = forward, b = backward
	js_a_f_half = 3,
	js_a_tun_roller_out = 4,
	js_a_tun_roller_in = 5
};

enum js_b_buttons {
	js_b_smooth = 1,
	js_b_f_rol_in = 4,
	js_b_f_rol_out = 5
};

/*enum sensors {
	color = 0x29
};*/

// Please test these buttons!
enum cs_a_buttons {
	a_tun_rol_in = 8,
	a_tun_rol_out = 7,
	a_f_rol_rotate_left = 9,
	a_f_rol_rotate_right = 10,
	a_lift_up = 1,
	a_lift_down = 2,
	a_totes_close = 12,
	a_totes_open = 11
};

enum cs_b_buttons {
	b_lift_up = 3,
	b_lift_down = 2
};

enum digital_io {
	dio_rdu = 0, //r = right l = left d = drive f = front b = back u = a v = b
	dio_rdv = 1,
	dio_ldu = 2,
	dio_ldv = 3,
	dio_bu = 4,
	dio_bv = 5,
	dio_fu = 6,
	dio_fv = 7,
	dio_lu = 8,
	dio_lv = 9
};

enum analog_in {
	an_top_switch = 1, //an = analog
	an_bot_switch = 0
};

enum positions {
	INITIALIZED,
	UP,
	DOWN
};

// Define the constants
#define	LEFT_DRIVE 114
#define	LIFT_ONE_TOTE 2700
#define	LIFT_FULL 7300
#define	AUTON_TURN 320


class Robot: public IterativeRobot
{
public:

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
	SPEEDCONTROLCLASS tunnel_roller_motor;
//	SPEEDCONTROLCLASS front_roller_left;
//	SPEEDCONTROLCLASS front_roller_right;
//	Servo ratchet;
	SPECIALCONTROLCLASS lift;

	Encoder right_drive;
	Encoder left_drive;
	Encoder back_strafe;
	Encoder front_strafe;
	Encoder lift_turney;

	Compressor compress;
	DoubleSolenoid lift_stopper;
	DoubleSolenoid totes_grabber;

	RobotDrive myRobot; // robot drive system
	LiveWindow *lw;
	SendableChooser *chooser;
	AnalogInput min_pos_switch;
	AnalogInput max_pos_switch;
	Timer timer;

	PWM lights;
	PWM more_lights;
	PWM and_more_lights;

	SendableChooser *auton_chooser;

	double lift_pos;

	double raw_0_x; //the raw values
	double raw_1_x;
	double raw_0_y;
	double raw_1_y;

	double nt_0_x; //the not raw values
	double nt_1_x;
	double nt_0_y;
	double nt_1_y;

	double smoothing_factor;

	int stupidRatchet;
	int stupidTimer;

	bool b_half;
	bool f_half;
	bool lift_working;
	bool gripping;

	bool auton_done;

	int AUTON;

	Robot();
private:
	double nullify(double n);

	void auton_container();

	void auton_lift_down_initial();
	void auton_lift_down();
	void auton_lift_up();
	void auton_turn_90(bool opposite);

	void RobotInit();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestPeriodic();
	void UpdateSDB();

	void lift_brake();
	void lift_unbrake();

};



#endif
