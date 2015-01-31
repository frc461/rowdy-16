#include "WPILib.h"
#include "Robot.h"


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
	Robot() :
		leftFrontDrive(t_fl),
		leftBackDrive(t_bl),
		rightFrontDrive(t_fr),
		rightBackDrive(t_br),
		strafeFrontDrive(t_fc),
		strafeBackDrive(t_bc),
		stick0(js_0),
		stick1(js_1),
		myRobot(leftFrontDrive,leftBackDrive,rightFrontDrive,rightBackDrive),
		lw(NULL),
		raw_0_x(0),
		raw_1_x(0),
		raw_0_y(0),
		raw_1_y(0),
		nt_0_x(0),
		nt_1_x(0),
		nt_0_y(0),
		nt_1_y(0)
		{
			myRobot.SetExpiration(0.1);
		}

private:
	double nullify(double n) {
		if(n < DEADZONE && n > -DEADZONE) {
			return 0.0;
		}
		return n;
	}

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		//get the raw values
		raw_0_x = stick0.GetX();
		raw_1_x = stick1.GetX();
		raw_0_y = stick0.GetY();
		raw_1_y = stick1.GetY();

		nt_0_x = nullify(raw_0_x);
		nt_1_x = nullify(raw_1_x);
		nt_0_y = nullify(raw_0_y);
		nt_1_y = nullify(raw_1_y);

		myRobot.ArcadeDrive(-nt_0_y,-nt_1_x);
		strafeFrontDrive.Set(nt_0_x);
		strafeBackDrive.Set(-nt_0_x);
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
