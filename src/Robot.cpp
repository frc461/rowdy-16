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
//	front_roller_left(t_frl_l),
//	front_roller_right(t_frl_r),
//	ratchet(t_ratchet),
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
	lift_stopper(ct_pcm_can, pcm_stopper_f, pcm_stopper_r),
	timer(),
	lights(t_lights),
	more_lights(t_more_lights),
	and_more_lights(t_and_more_lights)
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

void Robot::UpdateSDB() {
	SmartDashboard::PutNumber("Right Drive Encoder", right_drive.Get());
	SmartDashboard::PutNumber("Left Drive Encoder", left_drive.Get());
	SmartDashboard::PutNumber("Back Strafe Encoder", back_strafe.Get());
	SmartDashboard::PutNumber("Front Strafe Encoder", front_strafe.Get());
	SmartDashboard::PutNumber("Lift Encoder", lift_turney.Get());
//	SmartDashboard::PutNumber("Ratchet Servo", ratchet.GetAngle());
	SmartDashboard::PutNumber("Stupid Ratchet", stupidRatchet);
	SmartDashboard::PutNumber("Max_Pos_Switch", max_pos_switch.GetVoltage());
	SmartDashboard::PutNumber("Min_Pos_Switch", min_pos_switch.GetVoltage());
	SmartDashboard::PutNumber("Lift Motor", lift.Get());

}

void Robot::RobotInit()
{
	lw = LiveWindow::GetInstance();

	stupidTimer = 0;
	stupidRatchet = INITIALIZED;

	lights.SetRaw(0);
	more_lights.SetRaw(255);
	and_more_lights.SetRaw(100);

	//SmartDashboard::GetNumber("AUTON (0: Push, 1: FORWARD, 2: STRAFE, 3: ANGLE)");
}

//pre's
void Robot::auton_lift_down() {

}

//moves
void Robot::auton_lift_up() {
}

//moves down the lift to the 0 position from the top position
void Robot::auton_lift_down_initial() {

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
			myRobot.ArcadeDrive(0.0, 0.8);
		}
	}
	myRobot.ArcadeDrive(0.0,0.0);
}

//About this auton...
//Placement: on the step facing the container in line with the container.
//Remember: Smart Dashboard doesn't work on the driver user of the driver's station laptop.
//TO DO:
// - We need to write the code for auton_lift_down_initial and auton_lift_up and auton_lift_down.
//   - Test all of these functions in auton.
//     - Figure out which signs correspond to which direction on the encoder (do this in teleop).
//     - Figure out the encoder counts to move the lift all the way down from the top (in teleop). Write auton_lift_down_initial and test(in auton).
//     - Figure out the encoder counts to move the lift down/up one (in teleop). Write auton_lift_up and auton_lift_down (in auton).
//     - Remember to fire/unfire the lift_stopper in that code, take note that the lift_stopper's class has changed.
//   - *Related teleop stuff to write before proceeding*
//     - Talk to Kam about which buttons he wants for auton_lift_up and auton_lift_down.
//     - Write a function like the hat driving button functions that runs auton_lift_up and auton_lift_down SIMILAR CODE. Don't make actual function calls, just write similar code. You can model the code after the HAT driving code. (If you don't know what I'm talking about just ask Garret or Nik. They can explain to you what I'm talking about.)
// - You need to test all of the movement code for this auton not on the ground. If I wrote a sign error it won't stop moving until 15 seconds into the run, and you don't want to do that on the floor. Uncomment the movement code and make sure I wasn't stupid is basically all you have to do.
// - You need to test all of the movement code for this auton on the ground. See if everything works together.
// - Fix my mistakes because y'al are good at doing things and I couldn't test any of this stuff.
// - Delete this todo list! Thanks for doing these things Sam you're the best.

void Robot::auton_container() {

	//Backup so that we can lower the tusks
	/*	left_drive.Reset();
	while((left_drive.Get() < 668)  && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(0.8, 0.0);
	}
	myRobot.ArcadeDrive(0.0,0.0);*/

	//Make the lift go down to it's lowest point from it's highest
	auton_lift_down_initial();

	//Drive forward up to the container
	/*	left_drive.Reset()
	while((left_drive.Get() > -213)  && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(-0.8, 0.0);
	}
	myRobot.ArcadeDrive(0.0,0.0);*/

	//Make the lift go up one tote lenght
	//	auton_lift_up();

	//Move backwards into the auto zone
	/*	left_drive.Reset();
	while((left_drive.Get() < 1337) && timer.Get() < 15.0) {
		myRobot.ArcadeDrive(0.6, 0.0);
	}
	myRobot.ArcadeDrive(0.0,0.0);*/

	//Turn so that we fit.
	//	auton_turn_90();

}

void Robot::AutonomousInit()
{
#ifndef PRACTICE

	int AUTON = BORING; //SmartDashboard::GetNumber("AUTON");
//	const int AUTON = STRAFE;

	timer.Start();

	switch (AUTON) {
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
//	ratchet.SetAngle(100);
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
		lift_stopper.Set(lift_stopper.kReverse);
/*		if(stupidTimer < 10 && timer.Get() < ((control_system_a.GetRawButton(a_twitch)) ? 0.500 : 0.300)){
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
		else {*/
			lift.Set(1.0);
			std::cout << "Up Run" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Up Run", true);
/*		}
		if(max_pos_switch.GetVoltage() < 0.1) {
			ratchet.SetAngle(124);
		}*/
	}else if ((control_system_b.GetRawButton(b_lift_down) /*|| stick0.GetRawButton(js_a_lift_down)*/) /*&& (min_pos_switch.GetVoltage() > .1)*/) {
		lift_stopper.Set(lift_stopper.kReverse);
/*		if(stupidTimer < 10 && timer.Get() < 0.300){
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
		} else {*/
			lift.Set(-0.5);
			std::cout << "Down Run" << timer.Get() << std::endl;
			SmartDashboard::PutBoolean("Down Run", true);
/*		}*/
	} else {
		lift_stopper.Set(lift_stopper.kForward);
		lift.Set(0.0);
		timer.Stop();
		timer.Reset();
		stupidTimer = 0;
//		ratchet.SetAngle(103);
	}

	//Tunnel Roller Block
	if (control_system_a.GetRawButton(a_tun_rol_in) || stick0.GetRawButton(js_a_tun_roller_in))
		tunnel_roller_motor.Set(1.0);
	else if (control_system_a.GetRawButton(a_tun_rol_out) || stick0.GetRawButton(js_a_tun_roller_out))
		tunnel_roller_motor.Set(-1.0);
	else
		tunnel_roller_motor.Set(0.0);

	//Front Roller Block
/*	if (((stick1.GetRawButton(js_b_f_rol_in)) && !(stick1.GetRawButton(1))) || control_system_a.GetRawButton(a_f_rol_in)) {
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
	}*/

	UpdateSDB();
}

void Robot::TestPeriodic()
{
	lw->Run();
}


START_ROBOT_CLASS(Robot);
