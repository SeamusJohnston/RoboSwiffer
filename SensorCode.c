//Master needs to connect(so this robot)
//The following function checks the values of the sensors and
//sends ONLY the value of the sensor which was triggered (we
//do this to avoid the loop sending all sensor values consecutively).
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

//The following function rotates the turrent either clockwise or
//counter clock wise based on the integer parameter passed to it.
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

//The following function flips the turrent back 180 degrees to
//avoid the cable tangling on the turret. The direction of this
//turn is based on the integer parameter passed to it.
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
		while(nMotorEncoder[motorA]>-180)
	{
		motor[motorA]=-10;
	}
}
nMotorEncoder[motorA]=0;
motor[motorA]=0;
}

task main()
{
    //Sensor initialization
	SensorType[S1]=sensorTouch;
	SensorType[S2]=sensorTouch;
	SensorType[S3]=sensorTouch;
	SensorType[S4]=sensorSONAR;

	float lengthOfRobot=32;

    //Checks sensor values and while perimeterX is running in the motor brick
	while(SensorValue[S1]!=1)
	{
	}
	checkSensors();

    //Checks sensor values while perimeterY is running in the motor brick
	while(SensorValue[S2]!=1)
	{
	}
	checkSensors();
	wait1Msec(1000);
	while((SensorValue[S1]!=1) || (SensorValue[S3]!=1 ))
	{
        //While the robot is driving, sensor brick is checking to see if obstacles are hit
		while(SensorValue[S2]!=1&&SensorValue[S3]!=1)
		{
			checkSensors();

		}
		checkSensors();

        //This if is entered if the front sensor is hit by an obstacle during a vertical path
		if(SensorValue[S2])
		{
            //Waits while robot strafes right around obstacle
			while(SensorValue[S4]<lengthOfRobot)
			{}
			rotateTurret(1);
			sendMessage(4);
			wait1Msec(3000);

            //Waits while robot drives forward along the side of the obstacle
			while(SensorValue[S4]<lengthOfRobot)
			{}
			wait1Msec(1500);
			sendMessage(4);
			rotateTurret(2);
		}

        //This if is entered if the back sensor is hit by an obstacle during a vertical path
		else if(SensorValue[S3])
		{
			flipTurret(1);

            //Waits while robot strafes right around obstacle
			while(SensorValue[S4]<lengthOfRobot)
			{}
			rotateTurret(2);
			sendMessage(4);
			wait1Msec(3000);

            //Waits while robot drives backwards along the side of the obstacle
			while(SensorValue[S4]<lengthOfRobot)
			{}
			wait1Msec(1000);
			sendMessage(4);
			rotateTurret(1);
			flipTurret(2)

		}
	}

}
