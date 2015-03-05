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
	min_pos_switch(an_bot_switch),
	max_pos_switch(an_top_switch),
	right_drive(dio_rdu, dio_rdv),
	left_drive(dio_ldu, dio_ldv),
	back_strafe(dio_bu, dio_bv),
	front_strafe(dio_fu, dio_fv),
	lift_turney(dio_lu, dio_lv),
	myStrafe(&front_strafe, &back_strafe, &strafeFrontDrive, &strafeBackDrive),
	timer()
{
	myRobot.SetExpiration(0.1);
	SmartDashboard::init();
	lift_pos = 0;
	//myStrafe.myRobot = this;
//	color_sensor.Write(0x00,0x00);
}

double Robot::nullify(double n) {
	if(n < DEADZONE && n > -DEADZONE) {
		return 0.0;
	}

	return n;
}

void Robot::UpdateSDB() {
	SmartDashboard::PutNumber("Right Drive Encoder", right_drive.Get());
	SmartDashboard::PutNumber("Left Drive Encoder", left_drive.Get());
	SmartDashboard::PutNumber("Back Strafe Encoder", back_strafe.Get());
	SmartDashboard::PutNumber("Front Strafe Encoder", front_strafe.Get());
	SmartDashboard::PutNumber("Lift Encoder", lift_turney.Get());
	SmartDashboard::PutNumber("Ratchet Servo", ratchet.GetAngle());
	SmartDashboard::PutNumber("Stupid Ratchet", stupidRatchet);
	SmartDashboard::PutNumber("Max_Pos_Switch", max_pos_switch.GetVoltage());
	SmartDashboard::PutNumber("Min_Pos_Switch", min_pos_switch.GetVoltage());
}

void Robot::RobotInit()
{
	lw = LiveWindow::GetInstance();

	stupidTimer = 0;
	stupidRatchet = INITIALIZED;

	//SmartDashboard::GetNumber("AUTON (0: Push, 1: FORWARD, 2: STRAFE, 3: ANGLE)");
}

//pre's
void Robot::auton_lift_down_init() {

	lift_turney.Reset();

	//lift
	while((lift_turney.Get() < 500) && timer.Get() < 15.0) {

		lift.Set(-1.0);

	}

}

void Robot::auton_lift_down() {

}

void Robot::auton_lift_up() {
}

// false = left; true = right
void Robot::auton_turn_90( bool opposite = false) {

	right_drive.Reset();
	left_drive.Reset();

	if (opposite) {
		while ((left_drive.Get() > -560 /*|| right_drive.Get() < 560*/) && timer.Get() < 15.0) {
			myRobot.ArcadeDrive(0.0, -0.8);
		}

	} else {
		while ((left_drive.Get() <  560 /*|| right_drive.Get() < 560*/) && timer.Get() < 15.0) {
			myRobot.ArcadeDrive(0.0, 0.8));
	}

	myRobot.ArcadeDrive(0.0,0.0);
}

//strafe
void Robot::auton_strafe(bool direction) {
	SmartDashboard::PutString("Auton Strafe", "Working");
}

//forward
void Robot::auton_forward(bool direction) {
	SmartDashboard::PutString("Auton Forward", "Working");
}

//push both
//place facing east centered relative to the box and directly south of it.
void Robot::auton_push_both() {

	//push stuff!
	front_strafe.Reset();
	back_strafe.Reset();

	while ((front_strafe.Get() < 410 /*|| back_strafe.Get() > -410*/) && timer.Get() < 15.0) {
	//	if (front_strafe.Get() < 410)
			strafeFrontDrive.Set(0.8);
	//	else
	//		strafeFrontDrive.Set(0.0);

	//	if (back_strafe.Get() > -410)
			strafeBackDrive.Set(-0.8);
	//	else
	//		strafeBackDrive.Set(0.0);
	}
	strafeBackDrive.Set(0.0);
	strafeFrontDrive.Set(0.0);

	//drive back and then some
	front_strafe.Reset();
	back_strafe.Reset();

	while ((front_strafe.Get() > -535 /*|| back_strafe.Get() < 535*/) && timer.Get() < 15.0) {
	//	if (front_strafe.Get() > -535)
			strafeFrontDrive.Set(-0.8);
	//	else
	//		strafeFrontDrive.Set(0.0);

	//	if (back_strafe.Get() < 535)
			strafeBackDrive.Set(0.8);
	//	else
	//		strafeBackDrive.Set(0.0);
	}
	strafeBackDrive.Set(0.0);
	strafeFrontDrive.Set(0.0);

	//turn 90° left
	auton_turn_90();

	//move to the container
	//push stuff!
	front_strafe.Reset();
	back_strafe.Reset();

	while ((front_strafe.Get() < 114 /*|| back_strafe.Get() > -114*/) && timer.Get() < 15.0) {
	//	if (front_strafe.Get() < 410)
			strafeFrontDrive.Set(0.8);
	//	else
	//		strafeFrontDrive.Set(0.0);

	//	if (back_strafe.Get() > -410)
			strafeBackDrive.Set(-0.8);
	//	else
	//		strafeBackDrive.Set(0.0);
	}
	strafeBackDrive.Set(0.0);
	strafeFrontDrive.Set(0.0);

	//move forward to the container
	left_drive.Reset();
	right_drive.Reset();

	while((left_drive.Get() < -213)  && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(-0.8, 0.0);
	}

	myRobot.ArcadeDrive(0.0,0.0);

	//lift up - pickup the tote
	auton_lift_up();

	//turn 90° right
	auton_turn_90(true);

	left_drive.Reset();

	//move to the container
	while ((front_strafe.Get() < 535 /*|| back_strafe.Get() > -535*/) && timer.Get() < 15.0) {
	//	if (front_strafe.Get() > -410)
			strafeFrontDrive.Set(0.8);
	//	else
	//		strafeFrontDrive.Set(0.0);

	//	if (back_strafe.Get() < 410)
			strafeBackDrive.Set(-0.8);
	//	else
	//		strafeBackDrive.Set(0.0);
	}
	strafeBackDrive.Set(0.0);
	strafeFrontDrive.Set(0.0);

	//	Drive forward to tote
	/*	left_drive.Reset();
		right_drive.Reset();

		while ((left_drive.Get() > -344)  && timer.Get() < 15.0) {
				myRobot.ArcadeDrive(0.8,0.0);
				}*/

	//	auton_lift_down();

	//	auton_lift_up();


	//	auton_lift_down();

	//	auton_lift_up();

	// auton_turn_90();

	//	Drive Forward a lot code
	//	Needs testing (direction)
		/*	left_drive.Reset();
		right_drive.Reset();

		while ((left_drive.Get() < 2044 || right_drive.Get() > -2044) && timer.Get() < 15.0) {
			myRobot.ArcadeDrive(0.8,0.0);
				}*/
}

//push tote
void Robot::auton_push_tote() {
	SmartDashboard::PutString("Auton Push", "Working");

	front_strafe.Reset();
	back_strafe.Reset();

	//push stuff!
	while ((front_strafe.Get() > -410 /*|| back_strafe.Get() < 410*/) && timer.Get() < 15.0) {
//		if (front_strafe.Get() > -410)
			strafeFrontDrive.Set(0.8);
//		else
//			strafeFrontDrive.Set(0.0);

//		if (back_strafe.Get() < 410)
			strafeBackDrive.Set(-0.8);
//		else
//			strafeBackDrive.Set(0.0);
		UpdateSDB();
	}
	strafeBackDrive.Set(0.0);
	strafeFrontDrive.Set(0.0);

	//Pick the stuffz up...
	//	auton_lift_down_init();

	//	auton_lift_up();

	// Drive forward a bit...
	// This needs more testing (Direction)
	/*	left_drive.Reset();
	right_drive.Reset();

	while ((left_drive.Get() < 410 || right_drive.Get() > -410) && timer.Get() < 15.0) {
			myRobot.ArcadeDrive(0.8,0.0);
			}*/


	//	auton_turn_90();

	//Drive forward a lot of bits...
	//This needs more testing (Direction)
	//Same as the other 2044 one.
	/*	left_drive.Reset();
	right_drive.Reset();

	while ((left_drive.Get() < 2044 || right_drive.Get() > -2044) && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(0.8,0.0);
			}*/

}

void Robot::auton_container() {

	left_drive.Reset();

	while((left_drive.Get() < -213)  && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(-0.8, 0.0);
	}

	myRobot.ArcadeDrive(0.0,0.0);

	auton_lift_up();

	auton_turn_90(true);

	left_drive.Reset();

	while((left_drive.Get < -2044) && timer.Get < 15.0) {
		myRobot.ArcadeDrive(-0.6, 0.0);
	}

	myRobot.ArcadeDrive(0.0,0.0);

	auton_turn_90();

}

void Robot::AutonomousInit()
{
#ifndef PRACTICE

	int AUTON = BORING; //SmartDashboard::GetNumber("AUTON");
//	const int AUTON = STRAFE;

	timer.Start();

	switch (AUTON) {
	case PUSH_TOTE:
		auton_push_tote();
		break;
	case FORWARD:
		auton_forward(false);
		break;
	case STRAFE:
		auton_strafe(false);
		break;
	case PUSH_FULL:
		break;
	case CONTAINER:
		auton_container();
		break;
	case BORING:
		while(timer.Get() < 1.1) {
			myRobot.ArcadeDrive(-1.0, 0.0);
		}
		auton_turn_90();
		break;
	}

#endif
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
	b_half = false;
	f_half = false;
}

void Robot::TeleopPeriodic()
{
	//driving block
	//get the raw values
	raw_0_x = stick0.GetX();
	raw_1_x = stick1.GetX();
	raw_0_y = stick0.GetY();
	raw_1_y = stick1.GetY();

	//drive_speed_ain_value = ds->GetAnalogIn(2);

	//create modified values
	nt_0_x = nullify(raw_0_x) * ((stick0.GetRawButton(js_a_slow_down)) ? 0.75 : 1.0);
	nt_1_x = nullify(raw_1_x) * ((stick0.GetRawButton(js_a_slow_down)) ? 0.75 : 1.0);
	nt_0_y = -nullify(raw_0_y) * ((stick0.GetRawButton(js_a_slow_down)) ? 0.75 : 1.0);
	nt_1_y = nullify(raw_1_y) * ((stick0.GetRawButton(js_a_slow_down)) ? 0.75 : 1.0);

	//smoothing factor sub-block
	if (stick1.GetRawButton(js_b_smooth)) {
		nt_0_x *= smoothing_factor;
		nt_1_x *= smoothing_factor;
		nt_0_y *= smoothing_factor;
		nt_1_y *= smoothing_factor;
		if (smoothing_factor < 1.0) {
			smoothing_factor += .001;
		}
	} else {
		smoothing_factor = 0.65;
	}

	//functions sub-block
#ifndef PRACTICE
	if (f_half) {
		if (left_drive.Get() < -114) {
			f_half = false;
			SmartDashboard::PutBoolean("Hat Done", true);
			myRobot.ArcadeDrive(0.0,0.0);
		} else
			myRobot.ArcadeDrive(-0.6,0.0);
	} else if (b_half)  {
		if (left_drive.Get() > 114) {
			b_half = false;
			SmartDashboard::PutBoolean("Hat Done", true);
			myRobot.ArcadeDrive(0.0,0.0);
		} else
			myRobot.ArcadeDrive(0.6,0.0);
	} else if(stick0.GetRawButton(js_a_f_half)) {
		f_half = 1;
		left_drive.Reset();
		myRobot.ArcadeDrive(-0.6,0.0);
		SmartDashboard::PutBoolean("Hat", true);
	} else if(stick0.GetRawButton(js_a_b_half)) {
		b_half = 1;
		left_drive.Reset();
		myRobot.ArcadeDrive(0.6,0.0);
		SmartDashboard::PutBoolean("Hat", true);
	} else {
#endif
		SmartDashboard::PutBoolean("Hat", false);
		myRobot.ArcadeDrive(-nt_0_y, -nt_1_x);
		strafeFrontDrive.Set(nt_0_x);
		strafeBackDrive.Set(-nt_0_x);
		myStrafe.strafe_speed = nt_0_x;
#ifndef PRACTICE
	}
#endif

	SmartDashboard::PutBoolean("b_half", b_half);
	SmartDashboard::PutBoolean("f_half", f_half);


	SmartDashboard::PutBoolean("Down Run", false);
	SmartDashboard::PutBoolean("Up Run", false);
	//std::cout << timer.Get() << std::endl;

	// Lift Block
	if ((control_system_b.GetRawButton(b_lift_up) /*|| stick0.GetRawButton(js_a_lift_up)*/) && (max_pos_switch.GetVoltage() > 0.1)) {
		if(stupidTimer < 10 && timer.Get() < ((control_system_a.GetRawButton(a_twitch)) ? 0.500 : 0.300)){
			timer.Reset();
			timer.Start();
			stupidTimer++;
			ratchet.SetAngle(71.5);
			lift.Set(((control_system_a.GetRawButton(a_twitch)) ? -0.600 : -0.400));//-0.4);
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
		if(max_pos_switch.GetVoltage() < 0.1) {
			ratchet.SetAngle(124);
		}
	}else if ((control_system_b.GetRawButton(b_lift_down) /*|| stick0.GetRawButton(js_a_lift_down)*/) /*&& (min_pos_switch.GetVoltage() > .1)*/) {
		if(stupidTimer < 10 && timer.Get() < 0.300){
			timer.Reset();
			timer.Start();
			stupidTimer++;
			ratchet.SetAngle(124);
			lift.Set(((control_system_a.GetRawButton(a_twitch)) ? 0.600 : 0.400));//0.4);
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


START_ROBOT_CLASS(Robot);

Strafe::Strafe(Encoder *passedfront_strafe, Encoder *passedback_strafe, SPEEDCONTROLCLASS *passedstrafeFrontDrive, SPEEDCONTROLCLASS *passedstrafeBackDrive) : PIDSubsystem("Strafe", /* pid constants */ 1.0, 0.0, 0.0) {
	GetPIDController()->SetContinuous(false);

	// assign encoders/jags/talons etc
	// strafeFrontDrive = Robot.strafeFrontDrive;
	// strafeBackDrive = Robot.strafeBackDrive;
	front_strafe = passedfront_strafe;
	back_strafe = passedback_strafe;
	strafeFrontDrive = passedstrafeFrontDrive;
	strafeBackDrive = passedstrafeBackDrive;

	SetSetpoint(0.0); // don't move at start
	//Enable();
}

double Strafe::ReturnPIDInput() {
	// returns difference between encoders
	return front_strafe->GetRate() - back_strafe->GetRate();
}

void Strafe::UsePIDOutput(double output) {
	accumulated_error += output;
	if(output > 0) {
		strafeFrontDrive->Set(strafe_speed);
		strafeBackDrive->Set(strafe_speed - accumulated_error);
	} else {
		strafeFrontDrive->Set(strafe_speed - accumulated_error);
		strafeBackDrive->Set(strafe_speed);
	}
}
