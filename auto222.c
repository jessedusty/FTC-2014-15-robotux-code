#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     HTIRS2,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rollerMotor,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     flapTrackMotor, tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     leftDriveR,    tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     rightDriveF,   tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S2_C1_1,     rightDriveR,   tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S2_C1_2,     leftDriveF,    tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S2_C2_1,     liftMotor2,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C2_2,     liftMotor1,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C3_1,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S2_C3_2,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S2_C3_3,    servo3,               tServoStandard)
#pragma config(Servo,  srvo_S2_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// THIS IS THE AUTONOMOUS CODE - FOR USE IN AUTONOMOUS, THIS FILE HAS BEEN CREATED ON Oct 18th, 2014



#include "JoystickDriver.c" // ABSOLUTLY CRUTIAL FOR DRIVING WITH A JOYSTICK or WAITFORSTART
#include "drivers/hitechnic-superpro.h"
#include "drivers/hitechnic-irseeker-v2.h"

// VD - varible declarations

int rev = 1; // 1 = not reversed | -1 means reversed
bool normalctl = true; // normal joystick configuration if true, reverse joystick configuration if false
bool leftmode = true; //leftmode is when up/down is on left and left/right is on right
bool rightmode = false; // rightmode the reverse of leftmode
bool isRunning = false;
int power = 1; // value to divide joysticks by if needing to drive slower
int quadrent = 0;

// values set throughout the program

int acS1, acS2, acS3, acS4, acS5 = 0;

float leftstickval = 0;
float rightstickval = 0;
float newrightstickval = 0;

// global speeds of drive motors
float leftmotorval = 0;
float rightmotorval = 0;

// end of "global" varible declaration

// make sure that the values going to the motor arn't less than -100 or greater than 100
float makesureitsinlimits(float input) {
	if (input > 100) input = 100;
	if (input < -100) input = -100;
	return input;
}

// driving joystick (the one that actually moves the robot's position)
void joyval_joystick1st() { // standard mode on left joystick

	leftstickval = pow(((float)joystick.joy1_y1 / (float)64 * (float)5), 2) ;
	if (joystick.joy1_y1 < 0) leftstickval = leftstickval * -1.0;

	rightstickval = pow(((float)joystick.joy1_x1 / (float)64 * (float)5), 2) ;
	if (joystick.joy1_x1 < 0) rightstickval = rightstickval * -1.0;

}

// reverse mode on left joystick
void joyval_joystick1rv() {
	leftstickval = pow(((float)joystick.joy1_y2 / (float)64 * (float)5), 2) ;
	if (joystick.joy1_y2 < 0) leftstickval = leftstickval * -1.0;

	rightstickval = pow(((float)joystick.joy1_x1 / (float)64 * (float)5), 2) ;
	if (joystick.joy1_x1 < 0) rightstickval = rightstickval * -1.0;
}

// standard mode on right joystick
void joyval_joystick2st() {
	leftstickval = pow(((float)joystick.joy2_y1 / (float)64 * (float)5), 2) ;
	if (joystick.joy2_y1 < 0) leftstickval = leftstickval * -1.0;

	rightstickval = pow(((float)joystick.joy2_x2 / (float)64 * (float)5), 2) ;
	if (joystick.joy2_x2 < 0) rightstickval = rightstickval * -1.0;
}

// reverse mode on right joystick
void joyval_joystick2rv() {
	leftstickval = pow(((float)joystick.joy2_y2 / (float)64 * (float)5), 2) ;
	if (joystick.joy2_y2 < 0) leftstickval = leftstickval * -1.0;

	rightstickval = pow(((float)joystick.joy2_x1 / (float)64 * (float)5), 2) ;
	if (joystick.joy2_x1 < 0) rightstickval = rightstickval * -1.0;
}

//discover the values for use in the driving routine (using above methods)

void joyval_correct() {
	if ((normalctl) & (rev == 1)) {
		joyval_joystick1st();
		} else if ((normalctl) & (rev == -1)) {
		joyval_joystick1rv();
		} else if ((!normalctl) & (rev == 1)) {
		joyval_joystick2st();
		} else if ((!normalctl) & (rev == -1)) {
		joyval_joystick2rv();
		} else {
		PlayImmediateTone(500, 1);
	}
	leftstickval = leftstickval * rev;
	//if (abs(leftstickval) < backwardthresh) leftstickval = 0;
	rightstickval *= -1.0;
	//if (abs(rightstickval) < leftrightthresh) rightstickval = 0;
}

// returns larger of two numbers
float whichismax(float left, float right) {
	float returnval;
	if (left > right) returnval = left;
	if (right > left) returnval = right;
	if (right == left) returnval = (left + right) / 2;
	return returnval;
}

//MTR - move the motors, where the main drive motors are driven
void movethemotors() {
	motor[leftDriveF] = leftmotorval / power;
	motor[leftDriveR] = leftmotorval / power;

	motor[rightDriveF] = rightmotorval / power;
	motor[rightDriveR] = rightmotorval / power;
}

//DJ - where the main driving happens (it calls the other stuff above)
void driving_joystick() {
	if (joy1Btn(9)) { while (joy1Btn(9)) {PlayTone(4000, 1);} rev *= -1;}
	joyval_correct(); //put the correct joystick values in their varibles
	if (leftstickval > 0.0) {
		if (rightstickval > 0.0) {
			quadrent = 1;
			leftmotorval = leftstickval - rightstickval;
			rightmotorval = whichismax(leftstickval, rightstickval);
			} else {
			quadrent = 2;
			leftmotorval = whichismax(leftstickval, (rightstickval * -1.0));
			rightmotorval = leftstickval + rightstickval;
		}
		} else {
		if (rightstickval > 0.0) {
			quadrent = 3;
			leftmotorval = (whichismax((leftstickval * -1.0), rightstickval)) * -1.0;
			rightmotorval = leftstickval + rightstickval;
			} else {
			quadrent = 4;
			leftmotorval = leftstickval - rightstickval;
			rightmotorval = (whichismax((leftstickval * -1.0), (rightstickval * -1.0))) * -1.0;
		}
	}

	movethemotors();
}

// power control (used for slowing down driving motors to get greater accuracy)

void powercontrol () {
	if (normalctl) {
		if (joy1Btn(1)) power = 1;
		if (joy1Btn(2)) power = 2;
		if (joy1Btn(3)) power = 3;
		if (joy1Btn(4)) power = 4;
		} else {
		if (joy2Btn(1)) power = 1;
		if (joy2Btn(2)) power = 2;
		if (joy2Btn(3)) power = 3;
		if (joy2Btn(4)) power = 4;
	}
}

float withinval(float low, float high, float value) {
	if (value < low) value = low;
	if (value > high) value = high;
	return value;
}

int motorPowerCalc(float target, float current, int stopZone, int holdSpeed, int slowSpeed, int fastSpeed)
{
	int slowZone = 1000;
	int holdingZone = stopZone * 4;
	if (target == current) return 0;
	if (abs(target-current) < stopZone) return 0;
	int retval = fastSpeed; //maximum power
	//if (abs(target - current) < slowZone) retval = 1/90 * (target - current);
	if (abs(target - current) < slowZone) retval = slowSpeed;
	if (abs(target - current) < holdingZone) retval = holdSpeed;
	if (target > current) retval = retval * -1;

	return retval;
}

#define flapDistance -295;
bool flapRunning = false;

void encoderPositionDebugging();

void moveFlapUpOne() {
	if (flapRunning == false) {
		flapRunning = true;
		nMotorEncoder[flapTrackMotor] = 0;
		nMotorEncoderTarget[flapTrackMotor] = 295;
		motor[flapTrackMotor] = 100;

		while (nMotorRunState[flapTrackMotor] != runStateIdle) {
			encoderPositionDebugging();
		}
		motor[flapTrackMotor] = 0;
		flapRunning = false;

		//StartTask(waitForFlapMove);
	}
}

int motorLiftPower = 30;

void accessoryControl() {
	if (abs(joystick.joy1_y2) > 5) {
		motor[flapTrackMotor] = joystick.joy1_y2 * -1;
		} else if (joy1Btn(5)) {
		motor[flapTrackMotor] = 100;
		} else if (joy1Btn(7)) {
		motor[flapTrackMotor] = -100;
		} else if (joy1Btn(1)) {
		motor[flapTrackMotor] = motorLiftPower;
		} else {
		motor[flapTrackMotor] = 0;

	}
	if (joy1Btn(6)) {
		motor[rollerMotor] = 100;
		} else if (joy1Btn(8)) {
		motor[rollerMotor] = -100;
		} else {
		motor[rollerMotor] = 0;
	}

	if (joy1Btn(2)) {
		motor[liftMotor1] = 100;
		motor[liftMotor2] = 100;
		} else if (joy1Btn(4)) {
		motor[liftMotor1] = -100;
		motor[liftMotor2] = -100;
		} else {
		motor[liftMotor1] = 0;
		motor[liftMotor2] = 0;
	}

	if (joy1Btn(10)) {
		moveFlapUpOne();
	}
}

int flapTrackMotorEncoder, heightEncoder.;
int leftMotor, rightMotor;

void encoderPositionDebugging() {
	flapTrackMotorEncoder = nMotorEncoder[flapTrackMotor];
	leftMotor = nMotorEncoder[leftDriveF];
	rightMotor = nMotorEncoder[rightDriveF];
	heightEncoder = nMotorEncoder[liftMotor1];
}

int batteryVoltage = 0;
// BC - battery check
void batterycheck () {
	if (externalBattery == -1) PlayImmediateTone(4000, 1);

	batteryVoltage = externalBatteryAvg;
}

void runLoopPause() {
	while (time1[T1] < 50) {
		wait1Msec(1);
	}
	time1[T1] = 0;
}

void resetMotorEncoders() {
	nMotorEncoder[leftDriveF] = 0;
	nMotorEncoder[rightDriveR] = 0;
	nMotorEncoder[leftDriveR] = 0;
	nMotorEncoder[rightDriveF] = 0;
}

#define startVal -10
#define waitTimePerLoop 10

int leftEncoder, rightEncoder;

void rampForward(int distance, int upAcc, int downAcc) {

	int ticks = 1120.0 * distance / 4.0 * 3.14159 * 1.2244897959183673469387755102041;

	//leftmotorval = -100;
	//rightmotorval = -100;
	//movethemotors();

	//for (int i = startVal; i > -100; i -= upAcc) {
	//	leftmotorval = i;
	//	rightmotorval = i;
	//	movethemotors();
	//	wait1Msec(waitTimePerLoop);
	//}

	nMotorEncoder[rightDriveR] = 0;
	nMotorEncoder[leftDriveF] = 0;
	nMotorEncoderTarget[rightDriveR] = -1 * ticks;
	nMotorEncoderTarget[leftDriveF] = -1 * ticks;

	leftmotorval = -100;
	rightmotorval = -100;
	movethemotors();


	while (nMotorRunState[rightDriveR] != runStateIdle && nMotorRunState[leftDriveF] != runStateIdle) {
		leftEncoder = nMotorEncoder[leftDriveF];
		rightEncoder = nMotorEncoder[rightDriveR];

		// Wait for distance moved
	}

	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();

	//for (int i = -100; i < 0; i+=downAcc) {
	//	leftmotorval = i;
	//	rightmotorval = i;
	//	movethemotors();
	//	wait1Msec(waitTimePerLoop);
	//}
}



void moveForward(int time) {
	leftmotorval = -100;
	rightmotorval = -100;
	movethemotors();
	wait1Msec(time);
	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();
}


void turnLeft(float angle) {
	resetMotorEncoders();
	float ticks = angle * 1200.0 / 90.0; // Working but has the variable bug
	nMotorEncoderTarget[leftDriveF] = ticks;        // set the  target for Motor Encoder of Motor B to 360
	nMotorEncoderTarget[rightDriveF] =  -ticks;        // set the  target for Motor Encoder of Motffffffffffffffffffffffff
	rightmotorval = -50;
	movethemotors();

	while(nMotorRunState[leftDriveF] != runStateIdle && nMotorRunState[rightDriveF] != runStateIdle)


	// while Motor B AND Motor C are still running (haven't yet reached their target):
	{
		// do not continue
	}
	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();

}

void turnRight(float angle) {
	resetMotorEncoders();
	float ticks = angle * 1200.0 / 90.0; // Working but has the variable bug
	nMotorEncoderTarget[leftDriveF] = -ticks;        // set the  target for Motor Encoder of Motor B to 360
	nMotorEncoderTarget[rightDriveF] =  ticks;        // set the  target for Motor Encoder of Motor C to 360
	leftmotorval = -50;
	rightmotorval = 50;
	movethemotors();

	while(nMotorRunState[leftDriveF] != runStateIdle && nMotorRunState[rightDriveF] != runStateIdle)
		// while Motor B AND Motor C are still running (haven't yet reached their target):
	{
		// do not continue
	}
	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();


}

void moveDistanceForward(float inches, int power) {
	//		resetMotorEncoders();
	/////    float ticks = 1120 * inch/ 4 * 3.14159;
	//		nMotorEncoderTarget[leftDriveF] = nMotorEncoderTarget[rightDriveF] = ticks;
	//		leftmotorval = rightmotorval = power;
	//		movethemotors();
	//		while(nMotorRunState[leftDriveF] != runStateIdle && nMotorRunState[rightDriveF] != runStateIdle) {
	//			// Wait for move to be done
	//		}

	//		leftmotorval = rightmotorval = 0;


}

void setupIR() {
	tHTIRS2DSPMode _mode = DSP_1200;
	HTIRS2setDSPMode(HTIRS2, _mode);
}

void readIR() {
	HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 );
}


bool position1() {
	if (acS3 > 20) return true;
	return false;
}
bool position2() {
	if (acS3 <= 20 && acS4 > 10) return true;
	return false;
}
bool position3() {
	if (acS3 <=10) return true;
	return false;
}

void moveDistanceBackward(float inches, int power, int time) {

			resetMotorEncoders();

	    float ticks = 1120 * inches / 12.0 / 4 * 3.14159;
			nMotorEncoderTarget[leftDriveF] = -ticks;
			nMotorEncoderTarget[rightDriveF] = -ticks;
			leftmotorval = -power;
			rightmotorval = -power;
			movethemotors();
			ClearTimer(T1);

			while(nMotorRunState[rightDriveF] != runStateIdle && nMotorRunState[leftDriveF] != runStateIdle && time1[T1] < time) {
				// Wait for move to be done
			rightEncoder = nMotorEncoder[rightDriveF];
			leftEncoder = nMotorEncoder[leftDriveF];
			}
			leftmotorval = 0;
			rightmotorval = 0;
			movethemotors();
}

void followIR() {
	while (true) {
		readIR();
		if (acS1 + acS2 + acS3 + acS4 + acS5 < 15){
			leftmotorval = 0;
			rightmotorval = 0;
			movethemotors();
			return;
			} else if (acS1 + acS2 > acS4 + acS5) {
			leftmotorval = -10;
			rightmotorval = -30;
			} else if (acS1 + acS2 < acS4 + acS5) {
			leftmotorval = -30;
			rightmotorval = -10;
			} else  {
			leftmotorval = -30;
			rightmotorval = -30;
		}
		movethemotors();
	}
}

void pushDownPole() {
	readIR();

	//	if (position1()) {
	rampForward(5,10,10); // Inches, acceleration up, acceleration down
	//}


	//turnLeft(82.0); // angle
	//	moveDistanceForward(12 * 5, 100); // Inches

}

task moveDownDrawBridge() {
	servo[drawBridgeServo] = 0;
	wait1Msec(9500);
	servo[drawBridgeServo] = 132;
}

void moveUpDrawBridge() {
	servo[drawBridgeServo] = 180;
	wait1Msec(9500);
	servo[drawBridgeServo] = 132;
}

void flapsDown() {

	servo[goalServo] = 10;
}

void flapsUp() {

	servo[goalServo] = 230;
}
void ballFlapUp(){
	servo[flipperServo] = 180;
}
void ballFlapDown(){
	servo[FlipperServo] = 90;
}

void moveBackOutOfParkingZone(){
	moveDistanceBackward(32,35,5000);
}
void auto() {


	pushDownPole();
	flapsUp();

	leftmotorval = -30;
	rightmotorval = -100;
	movethemotors();
	wait1Msec(2000);

	turnLeft(45);

	leftmotorval = -50;
	rightmotorval = -50;
	movethemotors();
	wait1Msec(3000);


	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();
	wait1Msec(1000);

	flapsDown();

	return;
wait1Msec(3000);

	leftmotorval = 100;
	rightmotorval = 0;
	movethemotors();
	wait1Msec(875);

	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();
	wait1Msec(1000);


	leftmotorval = 100;
	rightmotorval = 100;
	movethemotors();
	wait1Msec(4500);

	leftmotorval = 0;
	rightmotorval = 0;
	movethemotors();

//	StartTask(moveDownDrawBridge);





	//moveDownRamp();


	//	readIR();
	//	if (position2()) turnLeft(30);
	//if (position3()) turnLeft(50);


	//	readIR();

	//	followIR();

	/*leftmotorval = -50;
	rightstickval = -50;
	movethemotors();
	wait1Msec(3000);
	leftmotorval = 0;
	rightstickval = 0;
	movethemotors();
	*/

	//moveForward(4000);



}


// TMT this is the main thread code
task main() {
	setupIR();

	isRunning = true; // sets isRunning to true, just in case it gets set to false

	waitForStart();

	moveBackOutOfParkingZone();

}
