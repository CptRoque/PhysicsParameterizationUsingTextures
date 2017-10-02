#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "ObjectData.h"

#ifdef PHYSICS_SIMULATION_EXPORTS  
#define PHYSICS_SIMULATION_API __declspec(dllexport)   
#else  
#define PHYSICS_SIMULATION_API __declspec(dllimport)   
#endif

extern "C" PHYSICS_SIMULATION_API int AddObjectToSimulation(Vector3* objectPosition, Quaternion* objectRotation, Vector3* objectScale, Vector3* meshVertices, Vector2* meshUV, int vertexAmount, Pixel* pixels, int textureHeight, int textureWidth, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping);
extern "C" PHYSICS_SIMULATION_API void ChangeObjectsPhysicsTexture(int index, Pixel* pixels, int textureHeight, int textureWidth);
extern "C" PHYSICS_SIMULATION_API void ChangeObjectsPhysicsVariableLimits(int index, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping);
extern "C" PHYSICS_SIMULATION_API void RemoveObjectFromSimulation(int index);
extern "C" PHYSICS_SIMULATION_API void Simulate(float deltaTime);
extern "C" PHYSICS_SIMULATION_API void CleanSimulationData();

void SetupObjectData(ObjectData * od, Vector3* objectPosition, Quaternion* objectRotation, Vector3* objectScale, Vector3* meshVertices, Vector2* meshUV, int vertexAmount, Pixel* pixels, int textureHeight, int textureWidth, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping);
Pixel SampleTexture(Pixel* texture, Vector2* uv, int width, int height);
void RotateVector3(Vector3* vector, Quaternion* rotation);
Vector3 CrossProduct(Vector3* a, Vector3* b);
float DotProduct(Vector3* a, Vector3* b);
Quaternion GetRotationFromTo(Vector3* from, Vector3* to);

void CalculatePhysics(float deltaTime);

List<ObjectData*> _objects = List<ObjectData*>();
