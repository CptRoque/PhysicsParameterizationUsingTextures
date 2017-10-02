#include <stdlib.h>
#include <memory.h>

#include "List.h"
#include "PhysicsSimulationCore.h"

extern "C" int AddObjectToSimulation(Vector3* objectPosition, Quaternion* objectRotation, Vector3* objectScale, Vector3* meshVertices, Vector2* meshUV, int vertexAmount, Pixel* pixels, int textureHeight, int textureWidth, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping)
{
	ObjectData* od = new ObjectData();
	size_t vec3Size = sizeof(Vector3);
	int vertexArraySize = vertexAmount * vec3Size;
	SetupObjectData(od, objectPosition, objectRotation, objectScale, meshVertices, meshUV, vertexAmount, pixels, textureHeight, textureWidth, minMass, maxMass, minStiffness, maxStiffness, minDamping, maxDamping);
	od->originalVertices = (Vector3*)malloc(vertexArraySize);
	memcpy_s(od->originalVertices, vertexArraySize, meshVertices, vertexArraySize);

	return _objects.Add(od);
}

extern "C" void ChangeObjectsPhysicsTexture(int index, Pixel* pixels, int textureHeight, int textureWidth)
{
	if (_objects[index] != nullptr)
	{
		_objects[index]->texturePixels = pixels;
		_objects[index]->textureHeight = textureHeight;
		_objects[index]->textureWidth = textureWidth;
	}
}

extern "C" void ChangeObjectsPhysicsVariableLimits(int index, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping)
{
	if (_objects[index] != nullptr)
	{
		_objects[index]->minMass = minMass;
		_objects[index]->maxMass = maxMass;
		_objects[index]->minStiffness = minStiffness;
		_objects[index]->maxStiffness = maxStiffness;
		_objects[index]->minDamping = minDamping;
		_objects[index]->maxDamping = maxDamping;
	}
}

extern "C" void RemoveObjectFromSimulation(int index)
{
	_objects.Remove(index);
}

extern "C" void Simulate(float deltaTime)
{
	CalculatePhysics(deltaTime);
}

extern "C" void CleanSimulationData()
{
	for (int i = 0; i < _objects.GetLength(); ++i)
	{
		delete _objects[i];
	}

	_objects.Clear();
}

void SetupObjectData(ObjectData * od, Vector3* objectPosition, Quaternion* objectRotation, Vector3* objectScale, Vector3* meshVertices, Vector2* meshUV, int vertexAmount, Pixel* pixels, int textureHeight, int textureWidth, float minMass, float maxMass, float minStiffness, float maxStiffness, float minDamping, float maxDamping)
{
	size_t vec3Size = sizeof(Vector3);

	od->vertexAmount = vertexAmount;
	od->textureHeight = textureHeight;
	od->textureWidth = textureWidth;
	od->uv = meshUV;
	od->vertices = meshVertices;
	od->position = objectPosition;
	od->rotation = objectRotation;
	od->scale = objectScale;
	od->velocity = (Vector3*)calloc(vertexAmount, vec3Size);
	od->texturePixels = pixels;

	od->previousPosition = (Vector3*)malloc(vec3Size);
	*od->previousPosition = *objectPosition;
	od->previousRotation = (Quaternion*)malloc(sizeof(Quaternion));
	*od->previousRotation = *objectRotation;
	od->previousScale = (Vector3*)malloc(vec3Size);
	*od->previousScale = *objectScale;

	od->minMass = minMass;
	od->maxMass = maxMass;
	od->minStiffness = minStiffness;
	od->maxStiffness = maxStiffness;
	od->minDamping = minDamping;
	od->maxDamping = maxDamping;
}

Pixel SampleTexture(Pixel* texture, Vector2* uv, int width, int height)
{
	while (uv->x > 1) uv->x += -1;
	while (uv->x < 0) uv->x += 1;
	while (uv->y > 1) uv->y += -1;
	while (uv->y < 0) uv->y += 1;
	int xPixel = (int)((width - 1) * uv->x);
	int yPixel = (int)((height - 1) * uv->y);

	return texture[xPixel + (yPixel * width)];
}

void RotateVector3(Vector3* vector, Quaternion* rotation)
{
	float doubleX = rotation->x * 2;
	float doubleY = rotation->y * 2;
	float doubleZ = rotation->z * 2;
	float xDoubleX = rotation->x * doubleX;
	float yDoubleY = rotation->y * doubleY;
	float zDoubleZ = rotation->z * doubleZ;
	float xDoubleY = rotation->x * doubleY;
	float xDoubleZ = rotation->x * doubleZ;
	float yDoubleZ = rotation->y * doubleZ;
	float wDoubleX = rotation->w * doubleX;
	float wDoubleY = rotation->w * doubleY;
	float wDoubleZ = rotation->w * doubleZ;
	float finalX = (1 - (yDoubleY + zDoubleZ)) * vector->x + (xDoubleY - wDoubleZ) * vector->y + (xDoubleZ + wDoubleY) * vector->z;
	float finalY = (xDoubleY + wDoubleZ) * vector->x + (1 - (xDoubleX + zDoubleZ)) * vector->y + (yDoubleZ - wDoubleX) * vector->z;
	float finalZ = (xDoubleZ - wDoubleY) * vector->x + (yDoubleZ + wDoubleX) * vector->y + (1 - (xDoubleX + yDoubleY)) * vector->z;
	vector->x = finalX;
	vector->y = finalY;
	vector->z = finalZ;
}

Vector3 CrossProduct(Vector3 * a, Vector3 * b)
{
	Vector3 result;
	result.x = a->y * b->z - a->z * b->y;
	result.y = a->z * b->x - a->x * b->z;
	result.z = a->x * b->y - a->y * b->x;
	return result;
}

float DotProduct(Vector3 * a, Vector3 * b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

Quaternion GetRotationFromTo(Vector3 * from, Vector3 * to)
{
	Quaternion result;
	Vector3 nFrom = from->GetNormalized();
	Vector3 nTo = to->GetNormalized();
	Vector3 cross = CrossProduct(&nFrom, &nTo);
	result.x = cross.x;
	result.y = cross.y;
	result.z = cross.z;
	result.w = sqrtf(nFrom.SquaredLength() * nTo.SquaredLength()) + DotProduct(&nFrom, &nTo);
	result.Normalize();
	return result;
}

void CalculatePhysics(float deltaTime)
{
	ObjectData* od;
	Quaternion externalNormalRotation;
	Quaternion externalNormalRotationInversed;
	Quaternion externalTangentRotation;
	Quaternion externalTangentRotationInversed;
	Vector3 pDelta;
	Vector3 sDelta;
	Vector3 vDelta;
	Vector3 accel;
	Quaternion rDelta;
	Pixel sample;
	float sqrDeltaTime = deltaTime * deltaTime;

	for (int i = 0; i < _objects.GetLength(); ++i)
	{
		od = _objects[i];
		pDelta = *od->position - *od->previousPosition;
		pDelta /= *od->scale;
		rDelta = od->rotation->Inversed() * *od->previousRotation;
		RotateVector3(&pDelta, &od->rotation->Inversed());
		sDelta = *od->scale - *od->previousScale;

		for (int j = 0; j < od->vertexAmount; ++j)
		{
			sample = SampleTexture(od->texturePixels, &od->uv[j], od->textureWidth, od->textureHeight);

			sample.r = ((1.0f - sample.r) * od->minMass) + (sample.r * od->maxMass);
			sample.g = ((1.0f - sample.g) * od->minStiffness) + (sample.g * od->maxStiffness);
			sample.b = ((1.0f - sample.b) * od->minDamping) + (sample.b * od->maxDamping);
			
			if (sample.r == 0 || sample.a == 0) continue;

			RotateVector3(&od->vertices[j], &rDelta);
						
			od->vertices[j] /= ((sDelta * sample.a) + *od->previousScale) / *od->previousScale;
			od->vertices[j] -= pDelta * sample.a;

			if (sample.g == 0) continue;

			od->vertices[j] += od->velocity[j] * deltaTime;

			vDelta = od->originalVertices[j] - od->vertices[j];

			accel = (vDelta * sample.g - od->velocity[j] * sample.b) / sample.r;
			od->vertices[j] += accel * 0.5f * sqrDeltaTime;
			od->velocity[j] += accel * deltaTime;
		}

		od->previousPosition->x = od->position->x;
		od->previousPosition->y = od->position->y;
		od->previousPosition->z = od->position->z;

		od->previousRotation->x = od->rotation->x;
		od->previousRotation->y = od->rotation->y;
		od->previousRotation->z = od->rotation->z;
		od->previousRotation->w = od->rotation->w;

		od->previousScale->x = od->scale->x != 0 ? od->scale->x : 1;
		od->previousScale->y = od->scale->y != 0 ? od->scale->y : 1;
		od->previousScale->z = od->scale->z != 0 ? od->scale->z : 1;
	}
}