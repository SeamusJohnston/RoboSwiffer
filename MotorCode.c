bool notReceived (int messageNumber1)
{
	int myMessage=0;
	if(myMessage==0)
	{
		myMessage=message;
	}
	if (myMessage==messageNumber1)
	{
		ClearMessage();

		return false;
	}
	else
		return true;
}

float encoderCalcDrive ()
{

	float encoderB=nMotorEncoder[motorB];
	float encoderC=nMotorEncoder[motorC];
	float travelledY= ((((-15.072*encoderB/360)/cos(PI/6))+((15.072*encoderC/360)/cos(PI/6))/2));
	return travelledY;
}

float encoderCalcStrafe ()
{
	float encoderA=nMotorEncoder[motorA];
	float distX= ((15.072*encoderA/360));
	return distX;
}
float convert(float rawValue)		//converts raw PID value into a motor output between 0 & 100;
{
	return rawValue/2000;
}

bool drive(int direction, float distance)//Add float position to parameters
{
	float correctValueA = 0;
	float correctValueB = 0;
	float correctValueC = 0;

	const float kP = 700;
	const float kD = 700;
	const float kI = 700; // PID constants
	const int time = 20; //loop speed

	float errorPrevA = 0;	//used to calculate derivatives of error
	float errorPrevB = 0;
	float errorPrevC = 0;

	float errorSumA = 0;	//integrals of error
	float errorSumB = 0;
	float errorSumC = 0;

	//setInitialSpeed(motor[motorA], motor[motorB], motor[motorC], x, y, xGoal, yGoal);


	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;

	float motorAGoal, motorBGoal, motorCGoal;
	if (direction==1)
	{
		motorAGoal = (0 +  correctValueA);
		motorBGoal = -(0.7 + correctValueB);
		motorCGoal = (0.7  + correctValueC);
	}
	else if(direction==2)
	{
		motorAGoal = (0 + correctValueA);
		motorBGoal = (0.7 + correctValueB);
		motorCGoal = -(0.7 + correctValueC);
	}
	else if (direction==4)
	{
		motorAGoal = (0.8 + correctValueA);
		motorBGoal = -(0.20 + correctValueB);
		motorCGoal = -(0.20 + correctValueC);
	}
	else if (direction==3)
	{
		motorAGoal = -(0.8 + correctValueA);
		motorBGoal = (0.20 + correctValueB);
		motorCGoal = (0.20 + correctValueC);
	}
	float distanceTravelled=0;
	//setMotorGoals(motorAGoal, motorBGoal, motorCGoal, x, y, xGoal, yGoal);
	//	int counter = 0;

	while (notReceived(1)&&notReceived(2)&&notReceived(3) && distanceTravelled < distance) //And while not at position
	{
		float encoderIA = nMotorEncoder[motorA];	//gets first point for calculations
		float encoderIB = nMotorEncoder[motorB];
		float encoderIC = nMotorEncoder[motorC];

		wait1Msec(time); //waits so we can track a change in encoder position and calculate velocity

		float encoderFA = nMotorEncoder[motorA];	//ges the second point for calculations
		float encoderFB = nMotorEncoder[motorB];
		float encoderFC = nMotorEncoder[motorC];

		float errorA = motorAGoal - (encoderFA - encoderIA)/time;	//calculates error (for P control)
		float errorB = motorBGoal - (encoderFB - encoderIB)/time;
		float errorC = motorCGoal - (encoderFC - encoderIC)/time;


		float errorDA = (errorA - errorPrevA)/time;	//calculates derivative of error using current error and past error
		float errorDB = (errorB - errorPrevB)/time;
		float errorDC = (errorC - errorPrevC)/time;


		errorSumA += errorA * time;					//adds to the sum of the error (integral)
		errorSumB += errorB * time;
		errorSumC += errorC * time;

		motor[motorA] = convert(kP * errorA + kD * errorDA + kI * errorSumA);	//convert turns sum of paths into motor output through math or something
		motor[motorB] = convert(kP * errorB + kD * errorDB + kI * errorSumB);
		motor[motorC] = convert(kP * errorC + kD * errorDC + kI * errorSumC);



		errorPrevA = errorA;
		errorPrevB = errorB;
		errorPrevC = errorC;
		/*
		counter++;

		if (counter == 10)
		{
		displayString(0, "%d", motor[motorB]);
		displayString(1, "%d", motor[motorC]);
		displayString(2, "%f", (encoderFB - encoderIB)/time);
		displayString(3, "%f", (encoderFC - encoderIC)/time);
		counter = 0;
		}
		*/
		if(direction==1)
			distanceTravelled=encoderCalcDrive();
		else if (direction==2)
			distanceTravelled=-encoderCalcDrive();
		else if (direction==4)
			distanceTravelled=encoderCalcStrafe();
		else if (direction==3)
			distanceTravelled=-encoderCalcStrafe();
	displayString(0,"%d", distanceTravelled);
	displayString(1,"%d",message);

	}
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	if (distanceTravelled>distance)
	return false;
	else
		return true;

	while(notReceived(1)!=1)
	{
		motor[motorA] = 0;
		motor[motorB] = 5;
		motor[motorC] = -5;

		return true;
	}
	while(notReceived(2)!=1)
	{
		motor[motorA] = -6;
		motor[motorB] = 3;
		motor[motorC] = 3;
		return true;
		displayString(4,"NIGGA GOT HIT FAM");

	}
	while(notReceived(3)!=1)
	{
		motor[motorA] = 0;
		motor[motorB] = -5;
		motor[motorC] = 5;
		return true;
	}
	return false;

}
void driveObstacle(int direction)
{
	float correctValueA = 0;
	float correctValueB = 0;
	float correctValueC = 0;

	const float kP = 700;
	const float kD = 700;
	const float kI = 700; // PID constants
	const int time = 20; //loop speed

	float errorPrevA = 0;	//used to calculate derivatives of error
	float errorPrevB = 0;
	float errorPrevC = 0;

	float errorSumA = 0;	//integrals of error
	float errorSumB = 0;
	float errorSumC = 0;

	//setInitialSpeed(motor[motorA], motor[motorB], motor[motorC], x, y, xGoal, yGoal);
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	float motorAGoal, motorBGoal, motorCGoal;
	if (direction==1)
	{
		motorAGoal = (0 +  correctValueA);
		motorBGoal = -(0.7 + correctValueB);
		motorCGoal = (0.7  + correctValueC);
	}
	else if(direction==2)
	{
		motorAGoal = (0 + correctValueA);
		motorBGoal = (0.7 + correctValueB);
		motorCGoal = -(0.7 + correctValueC);
	}
	else if (direction==4)
	{
		motorAGoal = (0.80 + correctValueA);
		motorBGoal = -(0.20 + correctValueB);
		motorCGoal = -(0.20 + correctValueC);
	}
	else if (direction==3)
	{
		motorAGoal = -(0.80+ correctValueA);
		motorBGoal = (0.20 + correctValueB);
		motorCGoal = (0.20 + correctValueC);
	}
	//setMotorGoals(motorAGoal, motorBGoal, motorCGoal, x, y, xGoal, yGoal);
	//	int counter = 0;

	while (notReceived(4)) //And while not at position
	{
		float encoderIA = nMotorEncoder[motorA];	//gets first point for calculations
		float encoderIB = nMotorEncoder[motorB];
		float encoderIC = nMotorEncoder[motorC];

		wait1Msec(time); //waits so we can track a change in encoder position and calculate velocity

		float encoderFA = nMotorEncoder[motorA];	//ges the second point for calculations
		float encoderFB = nMotorEncoder[motorB];
		float encoderFC = nMotorEncoder[motorC];

		float errorA = motorAGoal - (encoderFA - encoderIA)/time;	//calculates error (for P control)
		float errorB = motorBGoal - (encoderFB - encoderIB)/time;
		float errorC = motorCGoal - (encoderFC - encoderIC)/time;


		float errorDA = (errorA - errorPrevA)/time;	//calculates derivative of error using current error and past error
		float errorDB = (errorB - errorPrevB)/time;
		float errorDC = (errorC - errorPrevC)/time;


		errorSumA += errorA * time;					//adds to the sum of the error (integral)
		errorSumB += errorB * time;
		errorSumC += errorC * time;

		motor[motorA] = convert(kP * errorA + kD * errorDA + kI * errorSumA);	//convert turns sum of paths into motor output through math or something
		motor[motorB] = convert(kP * errorB + kD * errorDB + kI * errorSumB);
		motor[motorC] = convert(kP * errorC + kD * errorDC + kI * errorSumC);



		errorPrevA = errorA;
		errorPrevB = errorB;
		errorPrevC = errorC;

	}
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

float perimeterY()
{
	float encoderBI=nMotorEncoder[motorB];
	float encoderCI=nMotorEncoder[motorC];
	drive(1,1000000);
	float encoderBF=nMotorEncoder[motorB];
	float encoderCF=nMotorEncoder[motorC];
	float encoderB=encoderBF-encoderBI;
	float encoderC=encoderCF-encoderCI;
	float distY= ((((-15.072*encoderB/360)/cos(PI/6))+((15.072*encoderC/360)/cos(PI/6))/2));
	drive(2,distY);
	motor[motorA]=motor[motorB]=motor[motorC]=0;
	return distY;
}

float perimeterX()
{
	float encoderAI=nMotorEncoder[motorA];
	drive(3,1000000);
	float encoderAF=nMotorEncoder[motorA];
	float encoderA=encoderAF-encoderAI;
	float distX=-((15.072*encoderA/360));
	displayString(2,"%f",distX);
	drive(4,distX);
	motor[motorA]=motor[motorB]=motor[motorC]=0;
	return distX;
}
int reps(float widthOfRoom)
{
int lengthOfRobot=32;
return widthOfRoom/(1.2*lengthOfRobot);
}
float distX(float widthOfRoom)
{
	int repitition=reps(widthOfRoom);
	return widthOfRoom/repitition;
}

float obstacle(int direction)
{
	nMotorEncoder[motorA]=0;

	driveObstacle(3);
	float returnDist= -encoderCalcStrafe();
displayString(3, "%f", returnDist);
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;
	if(direction==1)
		driveObstacle(1);
	else if(direction==2)
		driveObstacle(2);
	float lengthOfObstacle=encoderCalcDrive();
	drive(4, returnDist);
	return lengthOfObstacle;
}

task main()
{
	ClearMessage();
	float widthOfRoom=perimeterX();
	float distanceY=perimeterY();
	int repitition=reps(widthOfRoom)/2;
	float distanceX= distX(widthOfRoom);
	bool hit= false;
	int counter=0;

	while((counter<repitition)&&hit!=1)
	{
		hit=drive(1,distanceY-20);
		if(hit)
		{
			float lengthOfObstacle=obstacle(1);
			drive(1, distanceY-lengthOfObstacle-20);
			hit=false;
		}
		drive(3, distanceX);
		hit=drive(2, distanceY-20);
		if(hit)
		{
			wait1Msec(2000);
			float lengthOfObstacle=obstacle(2);
			drive(2, distanceY-lengthOfObstacle-20);
			hit=false;
		}
		hit=drive(3, distanceX);
	}
	motor[motorA]=0;
	motor[motorB]=0;
	motor[motorC]=0;

//ClearMessage();
/*bool hit=drive(1,1000);
if(hit)
		{
				float lengthOfObstacle=obstacle(1);
			drive(1, 1000-lengthOfObstacle);
		}
	*/

/*bool hit=drive(2,10000);
	if(hit)
		{
			motor[motorA]=0;
			motor[motorB]=0;
			motor[motorC]=0;
			wait1Msec(2000);
			float lengthOfObstacle=obstacle(2);
			drive(2, 10000-lengthOfObstacle);
		}

		/*float obsDist=obstacle(1);
		displayString(1,"%f",obsDist);
		wait1Msec(3000);
	*/
}
