#include "pid.h"

Strafe::Strafe() : PIDSubsystem("Strafe", /* pid constants */ 1.0, 0.0, 0.0) {
	GetPIDController()->SetContinuous(false);

	// assign encoders/jags/talons etc
	// strafeFrontDrive = Robot.strafeFrontDrive;
	// strafeBackDrive = Robot.strafeBackDrive;

	SetSetpoint(0.0); // we don't want any difference between the two.
	Enable();
}

double Strafe::ReturnPIDInput() {
	// returns difference between encoders
	return front_strafe.GetRate() - back_strafe.GetRate();
}

void Strafe::UsePIDOutput(output) {
	accumulated_output += output
	if(output > 0) {
		Robot.strafeFrontDrive.Set(Robot.strafe_speed);
		Robot.strafeBackDrive.Set(-Robot.strafe_speed + accumulated_output);
	} else {
		Robot.strafeFrontDrive.Set(Robot.strafe_speed - accumulated_output);
		Robot.strafeBackDrive.Set(-Robot.strafe_speed);
	}
}
