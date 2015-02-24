#ifndef PID_H
#define PID_H

class Strafe: public PIDSubsystem {
 public:
	//constants

	// define econders/jags/talons etc
	/* SPEEDCONTROLCLASS strafeFrontDrive; */
	/* SPEEDCONTROLCLASS strafeBackDrive; */

	Strafe();
	double ReturnPIDInput();
	void UsePIDOutput(double output);
	/* void InitDefaultCommand(); */

};

#endif
