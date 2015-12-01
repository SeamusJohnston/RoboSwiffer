//Master needs to connect(so this robot)
void checkSensors()
{
	int count=0;
	if(SensorValue[S2])
	{
		sendMessage(2);
		count++;
	}
	else if(SensorValue[S1]&&count!=1)
	{
		sendMessage(1);
			count++;
	}
	else if(SensorValue[S3]&&count!=1)
	{
		sendMessage(3);
			count++;
	}

}

void rotateTurret (int turretDirection)
{
	if(turretDirection==1)
	{
		while(nMotorEncoder[motorA]>-90)
			motor[motorA]=-10;
	}

	else if(turretDirection==2)
	{
		while(nMotorEncoder[motorA]<90)
			motor[motorA]= 10;
	}
	motor[motorA]=0;
	nMotorEncoder[motorA]=0;
}

void flipTurret(int direction)
{
	if(direction==1)
	{
	while(nMotorEncoder[motorA]<180)
	{
		motor[motorA]=10;
	}
}
	else if (direction==2)
	{
		while(nMotorEncoder[motorA]>0)
	{
		motor[motorA]=-10;
	}
}
nMotorEncoder[motorA]=0;
motor[motorA]=0;
}
task main()
{
	SensorType[S1]=sensorTouch;
	SensorType[S2]=sensorTouch;
	SensorType[S3]=sensorTouch;
	SensorType[S4]=sensorSONAR;

	float lengthOfRobot=32;

	while(SensorValue[S1]!=1)
	{
	}
	checkSensors();
	while(SensorValue[S2]!=1)
	{
	}
	checkSensors();
	wait1Msec(1000);
	while((SensorValue[S1]!=1) || (SensorValue[S3]!=1 ))
	{
		while(SensorValue[S2]!=1&&SensorValue[S3]!=1)
		{
			checkSensors();
			displayString(0,"IN CHECKING");

		}
		checkSensors();
		if(SensorValue[S2])
		{
			eraseDisplay();
			displayString(0, "I SEE U BITCH");
			displayString(1, "%f", SensorValue[S4]);

			while(SensorValue[S4]<lengthOfRobot)
			{}
			eraseDisplay();
			displayString(0, "I DUN SEE U BITCH");
			displayString(2, "%f", SensorValue[S4]);
			rotateTurret(1);
			sendMessage(4);
			wait1Msec(1500);

			while(SensorValue[S4]<lengthOfRobot)
			{}
			wait1Msec(1500);
			sendMessage(4);
			rotateTurret(2);
		}

		else if(SensorValue[S3])
		{
			flipTurret(1);
			while(SensorValue[S4]<lengthOfRobot)
			{}
			displayString(0, "%d", SensorValue[S4]);
			rotateTurret(2);
			sendMessage(4);
			wait1Msec(1500);
			while(SensorValue[S4]<lengthOfRobot)
			{}
			wait1Msec(500);
			sendMessage(4);
			rotateTurret(2);
		}
	}

}
