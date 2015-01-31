#include "WPILib.h"
#include "Robot.h"

Robot::Robot() :
	leftFrontDrive(t_fl),
	leftBackDrive(t_bl),
	rightFrontDrive(t_fr),
	rightBackDrive(t_br),
	strafeFrontDrive(t_fc),
	strafeBackDrive(t_bc),
	stick0(js_0),
	stick1(js_1),
	myRobot(leftFrontDrive,leftBackDrive,rightFrontDrive,rightBackDrive),
	lw(NULL)
	/*
	raw_0_x(0),
	raw_1_x(0),
	raw_0_y(0),
	raw_1_y(0),

	nt_0_x(0),
	nt_1_x(0),
	nt_0_y(0),
	nt_1_y(0)
	*/
{
	myRobot.SetExpiration(0.1);
}

double Robot::nullify(double n) {
	if(n < DEADZONE && n > -DEADZONE) {
		return 0.0;
	}
	return n;
}

void Robot::RobotInit()
{
	lw = LiveWindow::GetInstance();
}

void Robot::AutonomousInit()
{

}

void Robot::AutonomousPeriodic()
{

}

void Robot::TeleopInit()
{

}

void Robot::TeleopPeriodic()
{
	//get the raw values
	raw_0_x = stick0.GetX();
	raw_1_x = stick1.GetX();
	raw_0_y = stick0.GetY();
	raw_1_y = stick1.GetY();

	drive_speed_ain_value = ds->GetAnalogIn(2);

	nt_0_x = nullify(raw_0_x) * (drive_speed_ain_value / 5.0);
	nt_1_x = nullify(raw_1_x) * (drive_speed_ain_value / 5.0);
	nt_0_y = nullify(raw_0_y) * (drive_speed_ain_value / 5.0);
	nt_1_y = nullify(raw_1_y) * (drive_speed_ain_value / 5.0);

	myRobot.ArcadeDrive(-nt_0_y,-nt_1_x);
	strafeFrontDrive.Set(nt_0_x);
	strafeBackDrive.Set(-nt_0_x);
}

void Robot::TestPeriodic()
{
	lw->Run();
}


START_ROBOT_CLASS(Robot);
