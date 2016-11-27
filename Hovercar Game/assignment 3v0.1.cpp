// assignment 3v0.1.cpp: A program using the TL-Engine
#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <sstream>


/*header files*/
#include "hovercar.h"




/*Reading in the coordinates for the map*/
/*******************************************************************************************************************************/

/*Forward Statements for wall placement functions*/
void readWallXPosFile();

void readWallZPosFile();

void readRotatedWallXPosFile();

void readRotatedWallZPosFile();


/*Forward Statements for checkpoint placement functions*/


void readRotatedCheckpointXPosFile();

void readRotatedCheckpointZPosFile();

/*Forward Statements for Isle placement functions*/

void readisleXPosFile();

void readisleZPosFile();

/*Forward Statements for Tank placement functions*/

void readTankXPosFile();
void readTankZPosFile();

/*Forward Statements for Tank placement functions*/
void readWayPointXPosFile();
void readWayPointZPosFile();


/*******************************************************************************************************************************/

/*This creates an empty vector.To have the vector start at a certain size,
this will work as well */

/*Wall Vectors*/
std::vector <float> wallXPositions; // positions for the x axis on the wall, placing it into a vector 
std::vector <IModel*> walls; // placing teh wall models into the a vector 
std::vector  <float> wallZPositions;

/*rotated wall vectors*/
std::vector <float> RotatedWallXPositions; // positions for the x axis on the wall, placing it into a vector 
std::vector <IModel*> rotatedWalls; // placing teh wall models into the a vector 
std::vector  <float> RotatedWallZPositions;


/*WayPoint Vectors*/
std::vector <float> waypointXPositions;
std::vector <IModel*> waypoint;
std::vector <float> waypointZPositions;





/*Rotated Checkpoint Vectors*/
std::vector <float> rotatedCheckpointXPositions;
std::vector <IModel*> rotatedCheckpoint;
std::vector <float> rotatedCheckpointZPositions;

/*Tank Vectors*/
std::vector <float> tankXPositions;
std::vector <IModel*> tank;
std::vector <float> tankZPositions;

/*Isle Vectors*/

std::vector <float> isleXPositions;
std::vector <IModel*> isle;
std::vector <float> isleZPositions;

/*Lap vectors*/

std::vector <string> stageText = { "prePlay", "Play", "Checkpoint One", "Checkpoint Two" , "Checkpoint Three" , "Final Checkpoint" };








/*******************************************************************************************************************************/

/* declaring stages as enums*/



/* declaring gameStates as enums*/
enum gameStates
{
	prePlay,
	play,
	stage1,
	stage2,
	stage3,
	stage4,
	pause,
	restart,
	endGame
};

gameStates gamestate = play;

/*******************************************************************************************************************************/

/*Constants*/

/*Walls and rotated walls min and max */
 float wallsXMin = -0.934082;
 float wallsXMax =  0.934082;
 float wallsYMin = -0.000674248;
 float wallsYMax =  4.52616;
 float wallsZMin = -4.83559;
 float wallsZMax =  4.83559;

 /*Walls and rotated walls min and max */
 float  rotatedCheckpointXMin = -9.86159;
 float  rotatedCheckpointXMax = 9.86159;
 float  rotatedCheckpointYMin = 0.00210619;
 float  rotatedCheckpointYMax = 10.6015;
 float  rotatedCheckpointZMin = -1.28539;
 float  rotatedCheckpointZMax = 1.28539;


const float shipRadius = 1.789;
const float aiRadius = 3.789;
const float wallLength = 1.5f;

bool hovercraftcollision = false;

bool mouseCaptureActive = true; // state of mouse capture



bool chaseCamActive = false;	// state of camera

float steering = 0.0f; // Variable holding the current steering angle
float shipSpeed = 0.0f; // Current driving speed of the ship

int mouseMoveX = 0; // for storing cumulative total of mouse movement on x axis
int mouseMoveY = 0; // for storing cumulative total of mouse movement on y axis

const float kGameSpeed = 1.0f; // ALL movement is multiplied by this. Adjust to taste.

							   // Keyboard controls
const EKeyCode accelerate = Key_W;
const EKeyCode turnLeft = Key_A;
const EKeyCode decelerate = Key_S;
const EKeyCode turnRight = Key_D;

const EKeyCode toggleChaseCam = Key_2;
const EKeyCode toggleMouseCapture = Key_Tab;

const EKeyCode quit = Key_Escape;

const EKeyCode camUp = Key_Up;
const EKeyCode camDown = Key_Down;
const EKeyCode camLeft = Key_Left;
const EKeyCode camRight = Key_Right;

const EKeyCode space = Key_Space;


// Camera control constants
const float kCameraMove = kGameSpeed * 0.1f; // distance for the direction keys x and z axis
const float kMouseWheelMove = kGameSpeed * 1.0f; // distance for wheel movement z axis
const float kMouseRotation = kGameSpeed * 0.1f; // distance (in degrees) for rotation of the camera

												// Gameplay constants
const float kSteerLimit = 100.0f; // Limit on the angle of steering
const float kSteeringFactor = 0.5f; // multiplier to control how fast the matchbox should rotate when steering
float drift = 0.04;

float tankRadius = 2.0f;

const float kCarMaxSpeed = kGameSpeed * 5.5f;	// speed limit
const float kSteerSpeed = kGameSpeed * 0.05f;	// Steering speed of the matchbox
const float kSteerReturnSpeed = kSteerSpeed * 5.0f; // rate at which steering returns to straight when moving - multiply by carSpeed

const float kAccelerationStep = kGameSpeed * 0.01f;
const float kDecelerationStep = kAccelerationStep * 3.0f;


const float kPi = 3.1415926f;

float shipLifePoints = 100;

ISprite*backdrop = myEngine->CreateSprite("ui_backdrop.jpg", 300, 655);


stringstream collisionMessage; //creating a string to hold the text
stringstream spacemsg;
float countdown = 4.0f;

void matrixRotateX(float(&matrix)[4][4], float rotation);
void matrixRotateZ(float(&matrix)[4][4], float rotation);

float tilted = 0.0f;

int checkpointID = -1;

int waypointIterator = 0;
int lapCounter;
const float isleRadius = 4.0;


void main()
{

	Meshes meshes;
	hoverCraft Ship(meshes);

	/**** Set up your scene here ****/

	IMesh*skyMesh = myEngine->LoadMesh("skybox 07.x");

	//IMesh* groundMesh = myEngine->LoadMesh("ground.x");

	IModel* skybox = skyMesh->CreateModel(0.0f, -960.0f, 0.0f);
	//IModel* ground =  groundMesh->CreateModel(0.0f, 0.0f, 0.0f);

	IMesh*groundMesh = myEngine->LoadMesh("ground.x");
	//IMesh* groundMesh = myEngine->LoadMesh("ground.x");


	IModel* ground = groundMesh->CreateModel(0.0f, 0.0f, 0.0f);
	//IModel* ground =  groundMesh->CreateModel(0.0f, 0.0f, 0.0f);

	IFont*myFont = myEngine->LoadFont("Comic Sans MS", 36);

	//camera set up
	ICamera* camera = myEngine->CreateCamera(kManual); // place in constructor
	camera->AttachToParent(Ship.GethoverCraft());
	/*calling in the function in the main*/

	// ai Craft set up 
	IModel* aiCraft;
	IMesh* aiCraftMesh = myEngine->LoadMesh("race2.x");
	aiCraft = meshes.hoverMesh->CreateModel(-20.0f, 0.0f, 40.0f);
	aiCraft->Scale(0.5);

	
	
	bool waypointcollision;

	/*calling wall function*/
	readWallXPosFile();
	readWallZPosFile();

	/*calling rotated wall*/
	readRotatedWallZPosFile();
	readRotatedWallXPosFile();



	/*calling rotated checkpoint function*/
	readRotatedCheckpointXPosFile();
	readRotatedCheckpointZPosFile();

	/*calling isle function*/
	readisleXPosFile();
	readisleZPosFile();

	/*calling Tank function*/
	readTankXPosFile();
	readTankZPosFile();

	/*calling Waypoint function*/
	readWayPointXPosFile();
	readWayPointZPosFile();

	/***************************************************************************************************************************/

		/*Looping through the wall vectors*/
	IMesh* wallMesh = myEngine->LoadMesh("wall.x");
	for (int i = 0; i < wallXPositions.size(); i++)
	{
		walls.push_back(wallMesh->CreateModel(wallXPositions[i], 0.0f, wallZPositions[i]));
	}



	for (int i = 0; i < RotatedWallXPositions.size(); i++)
	{
		rotatedWalls.push_back(wallMesh->CreateModel(RotatedWallXPositions[i], 0.0f, RotatedWallZPositions[i]));
		rotatedWalls[i]->RotateY(90.0f);
	}

	/******************************************************************************************************************/

	/*Looping through Waypoint*/
	IMesh* waypointMesh = myEngine->LoadMesh("dummy.x");
	for (int i = 0; i < waypointXPositions.size(); i++)
	{
		waypoint.push_back(waypointMesh->CreateModel(waypointXPositions[i], 0.0f, waypointZPositions[i]));
	}



	/*Looping through the checkpoint vectors*/
	IMesh* checkpointMesh = myEngine->LoadMesh("Checkpoint.x");

	IMesh* rotatedCheckpointMesh = myEngine->LoadMesh("Checkpoint.x");
	for (int i = 0; i < rotatedCheckpointXPositions.size(); i++)
	{
		rotatedCheckpoint.push_back(checkpointMesh->CreateModel(rotatedCheckpointXPositions[i], 0.0f, rotatedCheckpointZPositions[i]));
		rotatedCheckpoint[i]->RotateY(90.0f);
	}

	IMesh*isleMesh = myEngine->LoadMesh("IsleStraight.x");
	for (int i = 0; i < isleXPositions.size(); i++)
	{
		isle.push_back(isleMesh->CreateModel(isleXPositions[i], 0.0f, isleZPositions[i]));

	}

	IMesh*tankMesh = myEngine->LoadMesh("TankSmall1.x");
	for (int i = 0; i < tankXPositions.size(); i++)
	{
		tank.push_back(tankMesh->CreateModel(tankXPositions[i], 0.0f, tankZPositions[i]));

	}

	// The main game loop, repeat until engine is stopped
	gamestate = prePlay;
	bool keystartcoundown = false;
	myEngine->Timer(); // calling the timer function here to start the timer
	while (myEngine->IsRunning())
	{
		// Draw the scene

		myEngine->DrawScene();
		float frameTime = myEngine->Timer(); // when it hits the second timer function here it gets the difference between how long it took to complete the game loop
		if (gamestate == prePlay)
		{
			if (keystartcoundown)
			{
				countdown -= frameTime;
				shipSpeed = 0;

				if (countdown < 1.0f && countdown > 0.0f)
				{
					spacemsg << "Go!!";
					myFont->Draw(spacemsg.str(), 450, 300);
					spacemsg.str("");
				}
				else if (countdown < 0.0f)
				{
					gamestate = play;
				}
				else
				{
					spacemsg << int(countdown);
					myFont->Draw(spacemsg.str(), 200, 200);
					spacemsg.str("");
				}
			}
			else
			{
				spacemsg << "press space to start";
				myFont->Draw(spacemsg.str(), 450, 300);
				spacemsg.str("");
				if (myEngine->KeyHit(space))
				{
					keystartcoundown = true;
				}
				else
				{
					// start race
					myFont->Draw("Moo", 200, 200);
				}
			}
		}

		
		aiCraft->MoveLocalZ(1.0f);

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHit(Key_P))									   // if the P Key is hit the program state is paused 
		{
			if (gamestate == pause)
			{
				gamestate = play;
			}
			else if (gamestate == play)
			{
				gamestate = pause;
			}
		}

		/**** Update your scene each frame here ****/
	
	
		// quit the game
		if (myEngine->KeyHit(quit))
		{
			myEngine->Stop();
		}

		if (gamestate != pause)
	{
		/*collision*/
		for (int i = 0; i < wallXPositions.size(); i++)
		{

					wallsXMin = walls[i]->GetX() - wallLength -shipRadius;
					wallsXMax = walls[i]->GetX() + wallLength + shipRadius;

					wallsZMin = walls[i]->GetZ() - wallLength - shipRadius;
					wallsZMax = walls[i]->GetZ() + wallLength + shipRadius;

					//dummy is in the header file, it just isnt attached 

					if (Ship.GethoverCraft()->GetX() > wallsXMin && Ship.GethoverCraft()->GetX() < wallsXMax && Ship.GethoverCraft()->GetZ() > wallsZMin && Ship.GethoverCraft()->GetZ() < wallsZMax)
					{
						//hovercraftcollision = true;
						myFont->Draw("ship taken damage", 450, 300);
						shipSpeed *= -1.0f;
						//for (int shipLifePoints = 100; i < wallXPositions.size(); shipLifePoints --) // decremeting health by 1 everytime it hits a wall
						//{
						//	myFont->Draw("health", 450, 700); shipLifePoints;
						//}

					}
				}

				for (int i = 0; i < RotatedWallXPositions.size(); i++)
				{

					wallsXMin = rotatedWalls[i]->GetX() - wallLength  -shipRadius;
					wallsXMax = rotatedWalls[i]->GetX() + wallLength + shipRadius;

					wallsZMin = rotatedWalls[i]->GetZ() - wallLength - shipRadius;
					wallsZMax = rotatedWalls[i]->GetZ() + wallLength + shipRadius;

					//dummy is in the header file, it just isnt attached 

					if (Ship.GethoverCraft()->GetX() > wallsXMin && Ship.GethoverCraft()->GetX() < wallsXMax && Ship.GethoverCraft()->GetZ() > wallsZMin && Ship.GethoverCraft()->GetZ() < wallsZMax)
					{
						//hovercraftcollision = true;
						myFont->Draw("ship taken damage", 450, 300);
						shipSpeed *= -1.0f;
					}
				}

				/*************************************************************************************************************************************************/
				/*collision with checkpoints */

				float checkpointwidth = 3.0f;
				float structradius = 0.0f;
				int checkpointid = 0;

				for (int j = 0; j < rotatedCheckpoint.size(); j++)
				{
					float structMinX = rotatedCheckpoint[j]->GetX() - checkpointwidth - structradius;
					float structMaxX = rotatedCheckpoint[j]->GetX() + checkpointwidth + structradius;
					float structMinDistX = structMinX - Ship.GethoverCraft()->GetX();	// Using the large model’s
					float structMinDistZ = rotatedCheckpoint[j]->GetZ() - Ship.GethoverCraft()->GetZ();
					float structMincollisionDist = sqrt(structMinDistX*structMinDistX + structMinDistZ*structMinDistZ);
					float structMaxDistX = structMaxX - Ship.GethoverCraft()->GetX();
					float structMaxDistZ = rotatedCheckpoint[j]->GetZ() - Ship.GethoverCraft()->GetZ();
					float structMaxcollisionDist = sqrt(structMaxDistX*structMaxDistX + structMaxDistZ*structMaxDistZ);

					float rotatedCheckpointXMin = rotatedCheckpoint[j]->GetX() - checkpointwidth;
					float rotatedCheckpointXMax = rotatedCheckpoint[j]->GetX() + checkpointwidth;
					float rotatedCheckpointZMin = rotatedCheckpoint[j]->GetZ() - checkpointwidth;
					float rotatedCheckpointZMax = rotatedCheckpoint[j]->GetZ() + checkpointwidth;

					if (Ship.GethoverCraft()->GetX() > rotatedCheckpointXMin && Ship.GethoverCraft()->GetX() < rotatedCheckpointXMax && Ship.GethoverCraft()->GetZ() > rotatedCheckpointZMin && Ship.GethoverCraft()->GetZ() < rotatedCheckpointZMax)
					{
						myFont->Draw("Halleluyaaaaa", 200, 400);

						if (structMaxcollisionDist < shipRadius || structMincollisionDist < shipRadius)
						{
							Ship.GethoverCraft()->MoveX(shipSpeed *= -0.5f);
							myFont->Draw("value of J:"+ to_string(j), 300, 400);
						}
						else if (j == checkpointID + 1)
						{
							checkpointID = j;
							lapCounter++;
							gamestate = static_cast<gameStates>(lapCounter);
							stageText[lapCounter];

						}
					}
						
				}




			

				/*********************************************************************************************************************/
				mainHoverCar(Ship);

				float matrix[4][4];
				Ship.GethoverCraft()->GetMatrix(&matrix[0][0]);

				// Adjust car speed according to controls
				/////////////////////////////////////////
				if (myEngine->KeyHeld(accelerate))
				{
					if (shipSpeed < kCarMaxSpeed) shipSpeed += kAccelerationStep/*frameTime*/; // accelerating forwards
				}
				else
				{
					if (shipSpeed > 0.0f) shipSpeed -= kDecelerationStep /*frameTime*/; // drifting
				}

				if (myEngine->KeyHeld(decelerate))
				{
					if (shipSpeed > -kCarMaxSpeed) shipSpeed -= kAccelerationStep/*frameTime*/; // accelerating backwards
				}
				else
				{
					if (shipSpeed < 0.0f) shipSpeed += kDecelerationStep/*frameTime*/; // drifting
				}

				// Adjust car steering according to controls
				////////////////////////////////////////////

				// calculate how much to move ship back towards centre when player not actively steering
				//float returnSpeed = shipSpeed > 0.0f ? kSteerReturnSpeed * shipSpeed : -kSteerReturnSpeed * shipSpeed;
				//if (myEngine->KeyHeld(turnLeft)) matrixRotateZ(matrix, 0.5f);

				const float kTiltSpeed = 1.0f;
				const float kMaxTilt = 10.0f;



				if (myEngine->KeyHeld(turnRight) && steering <= kSteerLimit) // steering right
				{
					myFont->Draw("is this working", 200, 200);
					//matrixRotateZ(matrix, -0.5f);    // This doesnt work
					Instance.GetFrontGyro()->RotateY(shipSpeed * kSteeringFactor);
					Instance.GetRearGyro()->RotateY(shipSpeed * kSteeringFactor);
					steering += kSteerSpeed;
					//nstance.GetFrontGyro()->MoveX(-6.5); // this does not work 
					if (tilted <= kMaxTilt)
					{
						Ship.GethoverCraft()->RotateLocalZ(-kTiltSpeed/*frameTime*/);
						tilted += kTiltSpeed;
					}

				}
				else if (myEngine->KeyHeld(turnLeft) && steering >= -kSteerLimit/*frameTime*/) // steering left
				{
					//matrixRotateZ(matrix, 0.5f);    
					Instance.GetFrontGyro()->RotateY(shipSpeed * -kSteeringFactor);
					Instance.GetRearGyro()->RotateY(shipSpeed *-kSteeringFactor);
					steering -= kSteerSpeed;
					//Instance.GetFrontGyro()->MoveX(6.5); // this does not work
					if (tilted >= -kMaxTilt/*frameTime*/)
					{
						Ship.GethoverCraft()->RotateLocalZ(kTiltSpeed/*frameTime*/);
						tilted -= kTiltSpeed;
					}
				}
				else // let go of steering control
				{

					if (tilted > 0.0f)
					{
						Ship.GethoverCraft()->RotateZ(kTiltSpeed);
						tilted -= kTiltSpeed;
					}
					else if (tilted < 0.0f)
					{
						Ship.GethoverCraft()->RotateZ(-kTiltSpeed);
						tilted += kTiltSpeed;
					}
					/*if (steering > 0.0f)
					{
						Instance.GetFrontGyro()->RotateY(-kSteerSpeed);
						steering -= kSteerSpeed;
					}

					else if (steering < 0.0f)
					{
						Instance.GetFrontGyro()->RotateY(kSteerSpeed);
						steering += kSteerSpeed;
					}
					//Instance.GetFrontGyro()->RotateY(steering < 0.0f ? kSteerSpeed : -kSteerSpeed);
					//steering = steering < 0.0f ? steering + kSteerSpeed : steering - kSteerSpeed;
				*/
				}

				// Movement of car based on forward speed and ship position
				/////////////////////////////////////////////////////////////////////
				Instance.GetFrontGyro()->MoveLocalZ(shipSpeed);
				Instance.GetRearGyro()->MoveLocalZ(shipSpeed);
				//Ship.GethoverCraft()->RotateLocalY(shipSpeed * steering * kSteeringFactor);
				// hover

				// Camera control
				/////////////////

				// keyboard controlled camera movement
				if (myEngine->KeyHeld(camUp)) camera->MoveLocalZ(kCameraMove);
				if (myEngine->KeyHeld(camDown)) camera->MoveLocalZ(-kCameraMove);
				if (myEngine->KeyHeld(camRight)) camera->MoveLocalX(kCameraMove);
				if (myEngine->KeyHeld(camLeft)) camera->MoveLocalX(-kCameraMove);

				// mouse-controlled camera movement when captured
				if (mouseCaptureActive && !chaseCamActive)
				{
					mouseMoveX += myEngine->GetMouseMovementX(); // maintain cumulative total of movement
					mouseMoveY += myEngine->GetMouseMovementY();

					camera->ResetOrientation(); // reset the camera before moving... avoids nasty turning upside-down-ness
					camera->RotateLocalY(mouseMoveX * kMouseRotation); // the MouseRotation reduces the rotation speed
					camera->RotateLocalX(mouseMoveY * kMouseRotation);


				}

				// toggle mouse capture mode
				if (myEngine->KeyHit(toggleMouseCapture))
				{
					mouseCaptureActive = !mouseCaptureActive;
					if (mouseCaptureActive)
					{
						myEngine->StartMouseCapture();
					}
					else
					{
						myEngine->StopMouseCapture();
					}
				}

				// toggle chase cam mode
				if (myEngine->KeyHit(toggleChaseCam))
				{
					chaseCamActive = !chaseCamActive;
					if (chaseCamActive)
					{
						camera->AttachToParent(Instance.GetRearGyro());
						camera->ResetOrientation();
					}
					else
					{
						camera->DetachFromParent();
					}
				}

				/* Game info output */
				stringstream shipOutText;
				shipOutText << "ship speed" << int(shipSpeed);
				myFont->Draw(shipOutText.str(), 400, 650);
				shipOutText.str("");

				shipOutText << "State" << gamestate;
				myFont->Draw(shipOutText.str(), 580, 650);
				shipOutText.str("");

				shipOutText << "Health" << shipLifePoints;
				myFont->Draw(shipOutText.str(), 780, 650);
				shipOutText.str("");

				/* AI car */

				for (int i = 0; i < waypoint.size(); i++)
					
				{

					float waypointXpos =  waypoint[i]->GetLocalX() - aiCraft -> GetLocalX();	// Using the large model’s
					float waypointZpos = waypoint[i]->GetLocalZ() - aiCraft->GetLocalZ();	// position as it
					float collisionDist = sqrt(waypointXpos*waypointXpos + waypointZpos*waypointZpos);	// need #include <math.h>

					if (collisionDist < aiRadius)
					{
						float aiLocationIterater = 0.0f;
						float aiLocationBegin = 0.0f;
						waypointIterator++;
						// making the ai continuosly loop round the track
						if (aiLocationIterater == waypoint.size())
						{
							aiLocationIterater = aiLocationBegin;
						}
					}
					aiCraft->LookAt(waypoint[waypointIterator]);
				}
				
					// creating the isle collision distance, if it radius is more collision occures which sets back the car
				for (int l = 0; l < isle.size(); l++)
				{
					bool collision;

					float isleXPosition = isle[l]->GetX() - Ship.GethoverCraft()->GetX();
					float isleZPosition = isle[l]->GetZ() - Ship.GethoverCraft()->GetZ();
					float islecollisionDist = sqrt(isleXPosition*isleXPosition + isleZPosition*isleZPosition);
					
					if (islecollisionDist < isleRadius)
					{
						collision = true;
					}
					if (collision == true)
					{
						shipLifePoints--;
						shipSpeed *= -1.0f;
						collision = false;
					}

				}

				for (int k = 0; k < tank.size(); k++)
				{
					bool collision;
					
					float tankXPositions = tank[k]->GetX() - Ship.GethoverCraft()->GetX();
					float tankZPositions = tank[k]->GetZ() - Ship.GethoverCraft()->GetZ();
					float tankcollisionDist = sqrt(tankXPositions*tankXPositions + tankZPositions*tankZPositions);
					if (tankcollisionDist < tankRadius)
					{
						collision = true;

					}
					if (collision == true)
					{
						shipLifePoints--;
						shipSpeed *= -1.0f;
						collision = false;
					}
				}
			}

	} // end of game loop
}




	// reading in the coordinates 

	void readWallXPosFile()
	{
	ifstream wallXPosFile("wallXpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!wallXPosFile.eof())
	{
		char ch;
		wallXPosFile.get(ch);
		if (!wallXPosFile.eof())
		{
			if (ch == '\n')
			{
				wallXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	wallXPosFile.close();
}


void readWallZPosFile()
{
	ifstream wallZPosFile("wallZpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!wallZPosFile.eof())
	{
		char ch;
		wallZPosFile.get(ch);
		if (!wallZPosFile.eof())
		{
			if (ch == '\n')
			{
				wallZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	wallZPosFile.close();
}



void readRotatedWallXPosFile()
{
	ifstream RotatedWallXPosFile("wallRotatedXpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!RotatedWallXPosFile.eof())
	{
		char ch;
		RotatedWallXPosFile.get(ch);
		if (!RotatedWallXPosFile.eof())
		{
			if (ch == '\n')
			{
				RotatedWallXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	RotatedWallXPosFile.close();
}


void readRotatedWallZPosFile()
{
	ifstream RotatedWallZPosFile("wallRotatedZpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!RotatedWallZPosFile.eof())
	{
		char ch;
		RotatedWallZPosFile.get(ch);
		if (!RotatedWallZPosFile.eof())
		{
			if (ch == '\n')
			{
				RotatedWallZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	RotatedWallZPosFile.close();
}
/***************************************************************************************************************************/

/*reading checkpoint positions in from the file*/



void readRotatedCheckpointXPosFile()
{
	ifstream rotatedCheckpointXPosFile("rotatedXcheckpoints.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!rotatedCheckpointXPosFile.eof())
	{
		char ch;
		rotatedCheckpointXPosFile.get(ch);
		if (!rotatedCheckpointXPosFile.eof())
		{
			if (ch == '\n')
			{
				rotatedCheckpointXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
				
			}
			else
			{
				str += ch;
			}
		}
	}

	rotatedCheckpointXPosFile.close();

}

void readRotatedCheckpointZPosFile()
{
	ifstream rotatedCheckpointZPosFile("rotatedZcheckpoints.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!rotatedCheckpointZPosFile.eof())
	{
		char ch;
		rotatedCheckpointZPosFile.get(ch);
		if (!rotatedCheckpointZPosFile.eof())
		{
			if (ch == '\n')
			{
				rotatedCheckpointZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	rotatedCheckpointZPosFile.close();

}

	
void readisleXPosFile()
{
	ifstream isleXPosFile("isleXPosition.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!isleXPosFile.eof())
	{
		char ch;
		isleXPosFile.get(ch);
		if (!isleXPosFile.eof())
		{
			if (ch == '\n')
			{
				isleXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	isleXPosFile.close();

}

void readisleZPosFile()
{
	ifstream isleZPosFile("isleZPosition.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!isleZPosFile.eof())
	{
		char ch;
		isleZPosFile.get(ch);
		if (!isleZPosFile.eof())
		{
			if (ch == '\n')
			{
				isleZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	isleZPosFile.close();

}

void readTankXPosFile()
{
	ifstream tankXPosFile("tankxpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!tankXPosFile.eof())
	{
		char ch;
		tankXPosFile.get(ch);
		if (!tankXPosFile.eof())
		{
			if (ch == '\n')
			{
				tankXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	tankXPosFile.close();

}

void readTankZPosFile()
{
	ifstream tankZPosFile("tankzpositions.txt");

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!tankZPosFile.eof())
	{
		char ch;
		tankZPosFile.get(ch);
		if (!tankZPosFile.eof())
		{
			if (ch == '\n')
			{
				tankZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	tankZPosFile.close();

}


void readWayPointXPosFile()
{
	ifstream waypointXPosFile("waypointsXpositions.txt");

	if (!waypointXPosFile) cout << "uh oh..";

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!waypointXPosFile.eof())
	{
		char ch;
		waypointXPosFile.get(ch);
		if (!waypointXPosFile.eof())
		{
			if (ch == '\n')
			{
				waypointXPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	waypointXPosFile.close();
}


void readWayPointZPosFile()
{
	ifstream waypointZPosFile("waypointsZpositions.txt");

	if (!waypointZPosFile) cout << "uh oh..";

	// This is a formatting command. It says do not skip // white space when reading from the input stream
	string str;
	while (!waypointZPosFile.eof())
	{
		char ch;
		waypointZPosFile.get(ch);
		if (!waypointZPosFile.eof())
		{
			if (ch == '\n')
			{
				waypointZPositions.push_back(stof(str));// stof = string to float conversion 
				str.erase();
			}
			else
			{
				str += ch;
			}
		}
	}

	waypointZPosFile.close();
}
// Rotate around the World x-axis and the local origin.
//  float rotation : angle to rotate in degrees
//  matrix[4][4] : model to be rotated
void matrixRotateX(float(&matrix)[4][4], float rotation)
{
	// convert the rotation from degrees to radians
	float alpha = rotation * kPi / 180.0f;
	// find the sine of the angle
	float sinAlpha = sin(alpha);
	// find the cosine of the angle
	float cosAlpha = cos(alpha);

	// Need to use temporary values because x and y get overwritten during the
	// course of the calculations 
	float tempY;
	float tempZ;

	// Perform the minimum of calculations rather than use full matrix
	// multiplication. A rotation around the world x-axis only causes the y
	// and z components of each of the locals axes to be changed. 

	// Firstly rotate the local x-axis (first row)
	tempY = matrix[0][1] * cosAlpha - matrix[0][2] * sinAlpha;
	tempZ = matrix[0][1] * sinAlpha + matrix[0][2] * cosAlpha;
	matrix[0][1] = tempY;
	matrix[0][1] = tempZ;

	// Rotate the local y-axis (second row)
	tempY = matrix[1][1] * cosAlpha - matrix[1][2] * sinAlpha;
	tempZ = matrix[1][1] * sinAlpha + matrix[1][2] * cosAlpha;
	matrix[1][1] = tempY;
	matrix[1][2] = tempZ;

	// Rotate the local z-axis (third row)
	tempY = matrix[2][1] * cosAlpha - matrix[2][2] * sinAlpha;
	tempZ = matrix[2][1] * sinAlpha + matrix[2][2] * cosAlpha;
	matrix[2][1] = tempY;
	matrix[2][2] = tempZ;
}

void matrixRotateZ(float(&matrix)[4][4], float rotation)
{
	// convert the rotation from degrees to radians
	float alpha = rotation * kPi / 180.0f;
	// find the sine of the angle
	float sinAlpha = sin(alpha);
	// find the cosine of the angle
	float cosAlpha = cos(alpha);

	// Need to use temporary values because x and y get overwritten during the
	// course of the calculations 
	float tempY;
	float tempX;

	// Perform the minimum of calculations rather than use full matrix
	// multiplication. A rotation around the world x-axis only causes the y
	// and z components of each of the locals axes to be changed. 

	// Firstly rotate the local x-axis (first row)
	tempX = matrix[0][0] * cosAlpha - matrix[0][1] * sinAlpha;
	tempY = matrix[0][0] * sinAlpha + matrix[0][1] * cosAlpha;
	matrix[0][0] = tempX;
	matrix[0][1] = tempY;

	// Rotate the local y-axis (second row)
	tempX = matrix[1][0] * cosAlpha - matrix[1][1] * sinAlpha;
	tempY = matrix[1][0] * sinAlpha + matrix[1][1] * cosAlpha;
	matrix[1][0] = tempX;
	matrix[1][1] = tempY;

	// Rotate the local z-axis (third row)
	tempX = matrix[2][0] * cosAlpha - matrix[2][1] * sinAlpha;
	tempY = matrix[2][0] * sinAlpha + matrix[2][1] * cosAlpha;
	matrix[2][0] = tempX;
	matrix[2][1] = tempY;
}


	/*
	1.finish off reading in and reading out 
	2.create ai 
	3.collision detection sphere to shpere/ sphere to box
	4. do framespeed eg kGamespeed * Carmovementspeed* framespeed
	5.place stages into vector array
	6.output time / damage take e.g if hovercar collides with something health = 100 ;--1
	7.if(stage 

	*/