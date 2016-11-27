#pragma once
// matchbox.cpp: A program using the TL-Engine
// Features: Linear acceleration of matchbox when W/S held - coasts to a stop when released
//           Realistic steering which naturally returns to straight as car moves
//           Simple chase cam option
//			 FPS-style camera manually implemented, using trick to prevent turning upside-down
//
// NPM December 2015

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;
using namespace std;


I3DEngine* myEngine = New3DEngine(kTLX);

class Setup
{
public:
	Setup()
	{
		// Create a 3D engine (using TLX engine here) and open a window for it
		//myEngine = New3DEngine(kTLX);
		myEngine->StartWindowed();

		// Add default folder for meshes and other media
		myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
		//myEngine ->StartMouseCapture();
	}

	~Setup()
	{
		myEngine->Delete();
	}
};

Setup setup;

// creating a structure so i can call an instance into the main cpp file 
class dummyModel // makes a new type
{
private:
	IModel* frontGyroscope;
	IModel* rearGyroscope;
	IMesh* dummyMesh;
public:
	dummyModel() //constructor 
	{
		dummyMesh = myEngine->LoadMesh("Dummy.x");	
		frontGyroscope = dummyMesh->CreateModel();
		rearGyroscope = dummyMesh->CreateModel();
		frontGyroscope->RotateY(90.0f);
		rearGyroscope->RotateY(90.0f);
	}

	IModel* GetFrontGyro()
	{
		return this->frontGyroscope;
	}

	IModel* GetRearGyro()
	{
		return this->rearGyroscope;
	}

}; 



dummyModel Instance;  // bob would be the name of an instance of the structure e.g middlegyroscope = dummyMesh ->createModel

struct Meshes
{
	IMesh* hoverMesh = myEngine->LoadMesh("race2.x");
	
};


class hoverCraft // makes a new type
{
private:
	IModel* HoverCraft;
public:
	hoverCraft(struct Meshes &meshes) //constructor 
	{
		//hoverCraftMesh = myEngine->LoadMesh("race2.x");
		
		HoverCraft = meshes.hoverMesh->CreateModel(-50.0f, 0.0f, -50.0f);
		HoverCraft->Scale(0.5);
	}

	IModel* GethoverCraft()
	{
		return this->HoverCraft;
	}


};




void mainHoverCar(hoverCraft &Ship)
{

	// Lift up matchbox to make room for wheels
	//Ship.GethoverCraft()->MoveY(14.0f);

	// Attach match 'axles' to the matchbox
	//Instance.GetFrontGyro()->AttachToParent(Ship.GethoverCraft());
	//Instance.GetRearGyro()->AttachToParent(Ship.GethoverCraft());

	Ship.GethoverCraft()->AttachToParent(Instance.GetFrontGyro());



	
}
