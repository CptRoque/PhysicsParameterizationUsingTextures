#pragma once

#include <stdlib.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Pixel.h"

struct ObjectData
{
	int vertexAmount;
	int textureHeight;
	int textureWidth;

	float minMass;
	float maxMass;
	float minStiffness;
	float maxStiffness;
	float minDamping;
	float maxDamping;

	Vector2* uv;						//array	//external
	Vector3* vertices;					//array	//external
	Vector3* originalVertices;			//array
	Vector3* position;							//external
	Vector3* previousPosition;
	Quaternion* rotation;						//external
	Quaternion* previousRotation;
	Vector3* scale;								//external
	Vector3* previousScale;
	Vector3* velocity;					//array
	Pixel* texturePixels;				//array	//external

	~ObjectData()
	{
		free(originalVertices);
		free(velocity);
		free(previousPosition);
		free(previousRotation);
		free(previousScale);
	}
};