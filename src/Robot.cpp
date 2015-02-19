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
	tunnel_roller_motor(t_tun_rol),
	front_roller_left(t_frl_l),
	front_roller_right(t_frl_r),
	ratchet(t_ratchet),
	lift(ct_lift),
	myRobot(leftFrontDrive, leftBackDrive, rightFrontDrive, rightBackDrive),
	lw(NULL),
//	color_sensor(I2C::kOnboard, color),
	min_pos_switch(an_top_switch),
	max_pos_switch(an_bot_switch),
	right_drive(dio_rdu, dio_rdv),
	left_drive(dio_ldu, dio_ldv),
	back_strafe(dio_bu, dio_bv),
	front_strafe(dio_fu, dio_fv),
	lift_turney(dio_lu, dio_lv),
	timer()
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

	stupidTimer = 0;
	stupidRatchet = INITIALIZED;

	SmartDashboard::PutBoolean("Up Twitch", false);
	SmartDashboard::PutBoolean("Up Go", false);
	SmartDashboard::PutBoolean("Down Twitch", false);
	SmartDashboard::PutBoolean("Down Go", false);

}

void Robot::AutonomousInit()
{
	// See the paper "plan"; each code paragraph is supposed to correspond to a step.
	if (THREE_AUTON) {
		// 3 tote in auto zone
		// Working on the first tote.
		tunnel_roller_motor.Set(1.0);
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

		tunnel_roller_motor.Set(-1.0);
		Wait(1.0);

		lift.Set(0.0);
		tunnel_roller_motor.Set(0.0);
	} else if (BORING){
		do {
		front_strafe.Reset();
		back_strafe.Reset();
		strafeFrontDrive.Set(0.8);
		strafeBackDrive.Set(0.8);
		} while ((front_strafe.Get() < 6.4) && (front_strafe.Get() > -6.4));
		do {
			right_drive.Reset();
			left_drive.Reset();
			myRobot.ArcadeDrive(0.8, 0.0);
		} while ((right_drive.Get() < 3.8) && (right_drive.Get() > -3.8));

	}else {
		// 1 tote + 1 bin in auto zone
		tunnel_roller_motor.Set(1.0);
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
	
		tunnel_roller_motor.Set(-1.0);
		Wait(0.5);
		
		strafeFrontDrive.Set(0.5);
		strafeBackDrive.Set(-0.5);
		Wait(0.5);

		strafeFrontDrive.Set(0.0);
		strafeBackDrive.Set(0.0);
		myRobot.ArcadeDrive(0.8, 0.0);
		Wait(0.5);

		tunnel_roller_motor.Set(0.0);
	}
}

void Robot::AutonomousPeriodic()
{
	
}

void Robot::TeleopInit()
{
	timer.Start();
	timer.Stop();
	stupidRatchet = 0;
	ratchet.SetAngle(100);
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



	SmartDashboard::PutBoolean("Down Run", false);
	SmartDashboard::PutBoolean("Up Run", false);
	//std::cout << timer.Get() << std::endl;

	// Lift Block
	if ((control_system_b.GetRawButton(b_lift_up) || stick0.GetRawButton(js_a_lift_up)) /*&& (max_pos_switch.GetVoltage() == 0.0)*/) {
		if(stupidTimer < 10 && timer.Get() < 0.300){
			timer.Reset();
			timer.Start();
			stupidTimer++;
			ratchet.SetAngle(71.5);
			lift.Set(-0.4);
			std::cout << "Up Twitch" << std::endl;
			SmartDashboard::PutBoolean("Up Twitch", true);
		}
		else if(stupidTimer > 10 && timer.Get() > 0.300) {
			timer.Stop();
			stupidTimer = 0;
			stupidRatchet = UP;
			ratchet.SetAngle(103);
			std::cout << "Up Go" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Up Go", true);
		}
		else {
			lift.Set(1.0);
			std::cout << "Up Run" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Up Run", true);
		}
	}else if ((control_system_b.GetRawButton(b_lift_down) || stick0.GetRawButton(js_a_lift_down)) /*&& (min_pos_switch.GetVoltage() == 0.0)*/) {
		if(stupidTimer < 10 && timer.Get() < 0.300){
			timer.Reset();
			timer.Start();
			stupidTimer++;
			ratchet.SetAngle(124);
			lift.Set(0.4);
			std::cout << "Down Twitch" << std::endl;
			SmartDashboard::PutBoolean("Down Twitch", true);
		} else if(stupidTimer > 10 && timer.Get() > 0.300) {
			timer.Stop();
			stupidTimer = 0;
			stupidRatchet = DOWN;
			ratchet.SetAngle(103);
			std::cout << "Down Go" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Down Go", true);
		} else {
			lift.Set(-0.5);
			std::cout << "Down Run" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Down Run", true);
		}
	} else {
		lift.Set(0.0);
		timer.Stop();
		timer.Reset();
		stupidTimer = 0;
		ratchet.SetAngle(103);
	}

	//Tunnel Roller Block
	if (control_system_a.GetRawButton(a_tun_rol_in) || stick0.GetRawButton(js_a_tun_roller_in))
		tunnel_roller_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_tun_rol_out) || stick0.GetRawButton(js_a_tun_roller_out))
		tunnel_roller_motor.Set(-1.0);
	else
		tunnel_roller_motor.Set(0.0);

	//Front Roller Block
	if (((stick1.GetRawButton(js_b_f_rol_in)) && !(stick1.GetRawButton(1))) || control_system_a.GetRawButton(a_f_rol_in)) {
		front_roller_left.Set(1.0);
		front_roller_right.Set(-1.0);
	}
	else if (((stick1.GetRawButton(js_b_f_rol_out)) && !(stick1.GetRawButton(1))) || control_system_a.GetRawButton(a_f_rol_out)) {
		front_roller_left.Set(-1.0);
		front_roller_right.Set(1.0);
	}
	else if (((stick1.GetRawButton(js_b_f_rol_in)) && (stick1.GetRawButton(1))) || control_system_a.GetRawButton(a_f_rol_rotate_right)) {
			front_roller_left.Set(1.0);
			front_roller_right.Set(1.0);
	}
	else if (((stick1.GetRawButton(js_b_f_rol_out)) && (stick1.GetRawButton(1))) || control_system_a.GetRawButton(a_f_rol_rotate_left) ) {
			front_roller_left.Set(-1.0);
			front_roller_right.Set(-1.0);
	}
	else {
		front_roller_left.Set(0.0);
		front_roller_right.Set(0.0);
	}

	UpdateSDB();
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
	SmartDashboard::PutNumber("Lift Encoder", lift_turney.Get());
	SmartDashboard::PutNumber("Ratchet Servo", ratchet.GetAngle());
	SmartDashboard::PutNumber("Stupid Ratchet", stupidRatchet);
}

START_ROBOT_CLASS(Robot);
