#include "WPILib.h"
#include "Robot.h"

Robot::Robot():
	stick0(js_a),
	stick1(js_b),
	control_system_a(js_cs_a),
	control_system_b(js_cs_b),
	leftFrontDrive(t_fl),
	leftBackDrive(t_bl),
	rightFrontDrive(t_fr),
	rightBackDrive(t_br),
	strafeFrontDrive(t_fc),
	strafeBackDrive(t_bc),
	window_motor(t_w),
	roller_motor(t_rol),
	lift(ct_lift),
	myRobot(leftFrontDrive, leftBackDrive, rightFrontDrive, rightBackDrive),
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

	//drive_speed_ain_value = ds->GetAnalogIn(2);

	nt_0_x = nullify(raw_0_x) /** (drive_speed_ain_value / 5.0)*/;
	nt_1_x = nullify(raw_1_x) /** (drive_speed_ain_value / 5.0)*/;
	nt_0_y = -nullify(raw_0_y) /** (drive_speed_ain_value / 5.0)*/;
	nt_1_y = nullify(raw_1_y) /** (drive_speed_ain_value / 5.0)*/;

	myRobot.ArcadeDrive(-nt_0_y,-nt_1_x);
	strafeFrontDrive.Set(nt_0_x);
	strafeBackDrive.Set(-nt_0_x);

	if (control_system_a.GetRawButton(a_clamp_in))
		window_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_clamp_out))
		window_motor.Set(-1.0);
	else
		window_motor.Set(0.0);

	// Consider tweaking this stuff.
	if (control_system_b.GetRawButton(b_lift_up))
		lift.Set(0.6);
	else if (control_system_b.GetRawButton(b_lift_down))
		lift.Set(-0.2);
	else
		lift.Set(0.0);

	if (control_system_a.GetRawButton(a_roller_in))
		roller_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_roller_out))
		roller_motor.Set(-1.0);
	else
		roller_motor.Set(0.0);

}

void Robot::TestPeriodic()
{
	lw->Run();
}


START_ROBOT_CLASS(Robot);
