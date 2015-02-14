#include "WPILib.h"
#include "Robot.h"
//#include "I2C.h"

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
	roller_motor(t_rol),
	window_motor(t_w),
	ratchet(t_ratchet),
	lift(ct_lift),
	myRobot(leftFrontDrive, leftBackDrive, rightFrontDrive, rightBackDrive),
	lw(NULL),
//	color_sensor(I2C::kOnboard, color),
//	min_pos_switch(di_min),
//	max_pos_switch(di_max),
	right_drive(dio_rdu, dio_rdv),
	left_drive(dio_ldu, dio_ldv),
	back_strafe(dio_bu, dio_bv),
	front_strafe(dio_fu, dio_fv)
{
	myRobot.SetExpiration(0.1);
	SmartDashboard::init();
	lift_pos = 0;
//	color_sensor.Write(0x00,0x00);
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
	// See the paper "plan"; each code paragraph is supposed to correspond to a step.
	if (THREE_AUTON) {
		// 3 tote in auto zone
		// Working on the first tote.
		roller_motor.Set(1.0);
		Wait(0.2);
		
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		myRobot.ArcadeDrive(0.0, 0.0);
		lift.Set(0.6);
		Wait(0.5);

		strafeFrontDrive.Set(-0.5);
		strafeBackDrive.Set(0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		// Working on the second tote.
		strafeFrontDrive.Set(0.5);
		strafeBackDrive.Set(-0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		myRobot.ArcadeDrive(0.0, 0.0);
		lift.Set(-0.2);
		Wait(0.2);

		lift.Set(0.6);
		Wait(0.2);

		strafeFrontDrive.Set(-0.5);
		strafeBackDrive.Set(0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		// Working on the third tote.
		myRobot.ArcadeDrive(0.0, 0.0);
		strafeFrontDrive.Set(0.5);
		strafeBackDrive.Set(-0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		lift.Set(-0.2);
		Wait(0.2);

		lift.Set(0.6);
		Wait(0.2);

		myRobot.ArcadeDrive(0.0, -0.5);
		Wait(0.5);

		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		myRobot.ArcadeDrive(0.0, 0.0);
		lift.Set(-0.2);
		Wait(0.2);

		roller_motor.Set(-1.0);
		Wait(1.0);

		lift.Set(0.0);
		roller_motor.Set(0.0);
	} else {
		// 1 tote + 1 bin in auto zone
		roller_motor.Set(1.0);
		Wait(0.2);

		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		myRobot.ArcadeDrive(0.0, 0.0);
		lift.Set(0.6);
		Wait(0.5);

		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		myRobot.ArcadeDrive(0.0, 0.0);
		lift.Set(-0.2);
		Wait(0.2);

		lift.Set(0.0);
		myRobot.ArcadeDrive(0.0, 0.5);
		Wait(0.3);

		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);
	
		roller_motor.Set(-1.0);
		Wait(0.5);
		
		strafeFrontDrive.Set(0.5);
		strafeBackDrive.Set(-0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		roller_motor.Set(0.0);
	}
}

void Robot::AutonomousPeriodic()
{
	
}

void Robot::TeleopInit()
{
	// timer.Start();
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

	myRobot.ArcadeDrive(-nt_0_y, -nt_1_x);
	strafeFrontDrive.Set(nt_0_x);
	strafeBackDrive.Set(-nt_0_x);

	if (control_system_a.GetRawButton(a_clamp_in) || stick1.GetRawButton(js_b_clamp_in))
		window_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_clamp_out) || stick1.GetRawButton(js_b_clamp_out))
		window_motor.Set(-1.0);
	else
		window_motor.Set(0.0);

	// Consider tweaking this stuff.
	if ((control_system_b.GetRawButton(b_lift_up) || stick0.GetRawButton(js_a_lift_up)) /*&& !max_pos_switch.Get()*/)
		lift.Set(0.6);
	else if ((control_system_b.GetRawButton(b_lift_down) || stick0.GetRawButton(js_a_lift_down))/* && !min_pos_switch.Get()*/)
		lift.Set(-0.2);
	else
		lift.Set(0.0);

	if (control_system_a.GetRawButton(a_roller_in) || stick0.GetRawButton(js_a_roller_in))
		roller_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_roller_out) || stick0.GetRawButton(js_a_roller_out))
		roller_motor.Set(-1.0);
	else
		roller_motor.Set(0.0);

	// Changes every second.
/*	if (((int) (timer.Get())) % 2 == 0) {
		ratchet.Set(0.5);
	}
	else {
		ratchet.Set(-0.5);
	}*/
}

void Robot::TestPeriodic()
{
	lw->Run();
}

void Robot::UpdateSDB() {
	SmartDashboard::PutNumber("Right Drive Encoder", right_drive.Get());
	SmartDashboard::PutNumber("Left Drive Encoder", left_drive.Get());
	SmartDashboard::PutNumber("Back Strafe Encoder", back_strafe.Get());
	SmartDashboard::PutNumber("Front Strafe Encoder", front_strafe.Get());
}

START_ROBOT_CLASS(Robot);
