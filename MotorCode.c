bool notReceived (int messageNumber1) //checks  to make sure the sensor block hasn't sent a message
{
	int myMessage=0;
	if(myMessage==0)
	{
		myMessage=message;
	}
	if (myMessage==messageNumber1)
	{
		ClearMessage();

		return false;  //returns false if it finds a message corresponding to the specified integer
	}
	else
		return true; //returns true if none have been found
}

float encoderCalcDrive () //returns the distance travelled forward or backwards
{

	float encoderB=nMotorEncoder[motorB];
	float encoderC=nMotorEncoder[motorC];
	float travelledY= ((((-15.072*encoderB/360)/cos(PI/6))+((15.072*encoderC/360)/cos(PI/6))/2));
	return travelledY;
}

float encoderCalcStrafe () //returns the distance travelled right or left
{
	float encoderA=nMotorEncoder[motorA];
	float distX= ((15.072*encoderA/360));
	return distX;
}


bool drive(int direction, float distance) //drives a certain direction for a specified distance (1=forward, 2=backwards, 3=right, 4=left)
{


    if (direction==1)
    {
        motor[motorA]=0;
        motor[motorB]=-57;
        motor[motorC]=60;
    }
    else if(direction==2)
    {
        motor[motorA]=0;
        motor[motorB]= 57;
        motor[motorC]=-60;

    }
    else if (direction==4)
    {
        motor[motorA]=40;
        motor[motorB]=-20 ;
        motor[motorC]=-20;

    }
    else if (direction==3)
    {
        motor[motorA]=-39;
        motor[motorB]=20;
        motor[motorC]=20;
    }

	float distanceTravelled=0;
	nMotorEncoder[motorA]=0;
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;

	while (notReceived(1)&&notReceived(2)&&notReceived(3) && distanceTravelled < distance) //while no messages received from touch sensors and it hasnt reached it's destination
	{
		if(direction==1) //calculates distance travelled based on direction
			distanceTravelled=encoderCalcDrive();
		else if (direction==2)
			distanceTravelled=-encoderCalcDrive();
		else if (direction==4)
			distanceTravelled=encoderCalcStrafe();
		else if (direction==3)
			distanceTravelled=-encoderCalcStrafe();

	}
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;

	if (distanceTravelled>distance) //returns whether or not it was hit based on whether it stopped because of distance
	return false;
	else
		return true;


}
void driveObstacle(int direction) //Same as drive function with different exit conditions for the PID drive loop
{

 if (direction==1)
    {
        motor[motorA]=0;
        motor[motorB]=-58;
        motor[motorC]=60;
    }
    else if(direction==2)
    {
        motor[motorA]=0;
        motor[motorB]= 90;
        motor[motorC]=-84;

    }
    else if (direction==4)
    {
        motor[motorA]=40;
        motor[motorB]=-20;
        motor[motorC]=-20;

    }
    else if (direction==3)
    {
        motor[motorA]=-40;
        motor[motorB]=20;
        motor[motorC]=20;
    }




	while (notReceived(4)) //while the sensor block hasn't sent a message based on the ultrasonic sensor
	{}
	motor[motorA] = 0;
	motor[motorB] = 0;
	motor[motorC] = 0;
}

float perimeterY() //calculates the length of the room
{
	float encoderBI=nMotorEncoder[motorB]; //set initial motor encoders
	float encoderCI=nMotorEncoder[motorC];
	drive(1,1000000); //drive fowards until it receives a message from the touch sensor (that it hit the front)
	float encoderBF=nMotorEncoder[motorB]; //set final motor encder
	float encoderCF=nMotorEncoder[motorC];
	float encoderB=encoderBF-encoderBI;
	float encoderC=encoderCF- encoderCI;
	float distY= ((((-15.072*encoderB/360)/cos(PI/6))+((15.072*encoderC/360)/cos(PI/6))/2)); //calculates distance travelled
	drive(2,distY); //drive back the length of the room
	motor[motorA]=motor[motorB]=motor[motorC]=0;
	return distY+20; //return length of room
}

float perimeterX() //calculates the width of the room
{
	float encoderAI=nMotorEncoder[motorA]; //set initial motor encoder
	drive(3,1000000); //drive right until a touch sensor hits
	float encoderAF=nMotorEncoder[motorA]; //set final motor encoder
	float encoderA=encoderAF-encoderAI;
	float distX=-((15.072*encoderA/360)); //calculates distance travelled
	drive(4,distX); //drive left the width of the room to return to initial position
	motor[motorA]=motor[motorB]=motor[motorC]=0;
	return distX; //returns width of room
}
int reps(float widthOfRoom) //calculates the repititions needed to clean the room
{
int lengthOfRobot=32;
return widthOfRoom/(1.2*lengthOfRobot);
}
float distX(float widthOfRoom) //calculates the distance to be travelled right for each repitition
{
	int repitition=reps(widthOfRoom);
	return widthOfRoom/repitition;
}

float obstacle(int direction) //avoids obstacles
{
	nMotorEncoder[motorA]=0;
	if(direction==1)
		drive(2,10); //reverse slightly
	if(direction==2)
		drive(1,10); //move forward slightly
	driveObstacle(3); //drive right until the ultrasonic sensor sends a message that it can no longer see an obstacle
	float returnDist= -encoderCalcStrafe(); //calculate distance travelled right to avoid obstacle
	nMotorEncoder[motorB]=0;
	nMotorEncoder[motorC]=0;
	if(direction==1)
		driveObstacle(1); //drive forward until the ultrasonic sensor sends a message that it can no longer see an obstacle
	else if(direction==2)
		driveObstacle(2); //drive backwards until the ultrasonic sensor sends a message that it can no longer see an obstacle
	float lengthOfObstacle=encoderCalcDrive(); //calculates the length of the obstacle
	drive(4, returnDist); //drive left the distance that was initially driven right
	return lengthOfObstacle; // return the length of the obstacle
}

task main()
{
	ClearMessage();
  float widthOfRoom=perimeterX(); //calculate the width of the room
	float distanceY=perimeterY(); //calculate the length of the room
	int repitition=reps(widthOfRoom); //calculate how many repititions of the cleaning cycle it needs to do
	float distanceX= distX(widthOfRoom); //calculate the distance per strafe on the repititions
	bool hit= false;
	int counter=0;

	while((counter<repitition-3)&&hit!=1)// run while it hasn't hit the bottom, or has not completed all of its repititions
	{
		hit=drive(1,distanceY-40); //drive forward -20cm for clearance from wall
		if(hit) //if it hit something go through obstacle functions
		{
			float lengthOfObstacle=obstacle(1); //drive around the obstacle
			drive(1, distanceY-lengthOfObstacle +10); //drive forward the remainder of the distance
			hit=false; //reset hit
		}
		drive(3, distanceX); //drive right
		hit=drive(2, (distanceY+20) ); //drive backwards -20cm for clearance from wall
		if(hit) //if it hits something go through obstacle functions
		{
			wait1Msec(2000); //wait for sensor to rotate
			float lengthOfObstacle=obstacle(2); //drive around the obstacle
			drive(2, distanceY+lengthOfObstacle); //drive the remainder of the distance
			hit=false; //reset hit
		}
		hit=drive(3, distanceX);//drive right
        counter++;
	}
	motor[motorA]=0;
	motor[motorB]=0;
	motor[motorC]=0;

}
