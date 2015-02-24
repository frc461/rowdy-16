#include "pid.h"

Strafe::Strafe() : PIDSubsystem("Strafe", /* pid constants */ 1.0, 0.0, 0.0) {
	GetPIDController()->SetContinuous(false);

	// assign encoders/jags/talons etc
	// strafeFrontDrive = Robot.strafeFrontDrive;
	// strafeBackDrive = Robot.strafeBackDrive;

	SetSetpoint(0.0); // don't move at start
	Enable();
}

double Strafe::ReturnPIDInput() {
	// returns difference between encoders
	return front_strafe.GetRate() - back_strafe.GetRate();
}

void Strafe::UsePIDOutput(output) {
	if(output > 0) {
		Robot.strafeFrontDrive.Set(Robot.strafe_speed);
		Robot.strafeBackDrive.Set(Robot.strafe_speed - output);
	} else {
		Robot.strafeFrontDrive.Set(Robot.strafe_speed - output);
		Robot.strafeBackDrive.Set(Robot.strafe_speed);
	}
}
