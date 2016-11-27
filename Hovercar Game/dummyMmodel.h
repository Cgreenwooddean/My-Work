#pragma once
#include<TL-Engine.h>
using namespace tle;
class dummyMmodel
{
private:
	IModel* frontGyroscope;
	IModel* rearGyroscope;
public:
	dummyMmodel(IMesh*theMesh); // constructer


	~dummyMmodel();//destructor
};

