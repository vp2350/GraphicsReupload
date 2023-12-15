#include "Transformation.h"

using namespace DirectX;

Transformation::Transformation()
{
	position = XMFLOAT3(0, 0, 0);
	pitchYawRoll = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	isDirty = false;
}

Transformation::~Transformation()
{
}

void Transformation::MoveAbsolute(float x, float y, float z)
{
	//XMVECTOR newPos = XMVectorAdd(XMLoadFloat3(&position), XMVectorSet(x, y, z, 0));
	position.x += x;
	position.y += y;
	position.z += z;
	isDirty = true;
}

void Transformation::MoveRelative(float x, float y, float z)
{
	XMVECTOR desired = XMVectorSet(x, y, z, 0);

	//Find the rotation quaternion
	XMVECTOR rotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));

	//Movement adjusted according to current object rotation
	XMVECTOR adjusted = XMVector3Rotate(desired, rotation);

	//Adjust movement added to current position
	XMStoreFloat3(&position, XMLoadFloat3(&position) + adjusted);

	isDirty = true;
}

void Transformation::Rotate(float pitch, float yaw, float roll)
{
	//XMVECTOR newRotation = XMVectorAdd(XMLoadFloat3(&pitchYawRoll), XMVectorSet(pitch, yaw, roll, 0));
	pitchYawRoll.x += pitch;
	pitchYawRoll.y += yaw;
	pitchYawRoll.z += roll;
	isDirty = true;
}

void Transformation::Scale(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
	isDirty = true;
	//Not sure if this would work depending on how vector multiplication works
	//XMVECTOR newScale = XMVectorMultiply(XMLoadFloat3(&scale), XMVectorSet(x, y, z, 0));
}

void Transformation::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	isDirty = true;
}

void Transformation::SetRotation(float pitch, float yaw, float roll)
{
	pitchYawRoll.x = pitch;
	pitchYawRoll.y = yaw;
	pitchYawRoll.z = roll;
	isDirty = true;
}

void Transformation::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	isDirty = true;
}

DirectX::XMFLOAT3 Transformation::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transformation::GetRotation()
{
	return pitchYawRoll;
}

DirectX::XMFLOAT3 Transformation::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transformation::GetWorldMatrix()
{
	if (isDirty) {
		XMMATRIX translation = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
		XMMATRIX scaling = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
		XMMATRIX world = scaling * rotation * translation;

		XMStoreFloat4x4(&worldMatrix, world);
	}
	isDirty = false;
	return worldMatrix;
}
