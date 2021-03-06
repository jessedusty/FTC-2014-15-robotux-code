#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     theMotor,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int encoderVal;
task main()
{
	nMotorEncoder[theMotor] = 0;                // reset the Motor Encoder of Motor B
	nMotorEncoderTarget[theMotor] = 1120;        // set the  target for Motor Encoder of Motor B to 360
	motor[theMotor] = 75;                       // motor B is run at a power level of 75

	while(nMotorRunState[theMotor] != runStateIdle)  // while Motor B is still running (hasn't reached target yet):
	{
		// do not continue
	encoderVal = nMotorEncoder[theMotor];
	}
	motor[theMotor] = 0;                       // motor B is stopped at a power level of 0
	motor[theMotor] = 0;                       // motor C is stopped at a power level of 0




}
