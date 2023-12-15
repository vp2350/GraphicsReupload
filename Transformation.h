#pragma once

#include <DirectXMath.h>

class Transformation
{
public:
	Transformation();
	~Transformation();

	void MoveAbsolute(float x, float y, float z); 
	void MoveRelative(float x, float y, float z);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 pitchYawRoll;
	DirectX::XMFLOAT3 scale;

	DirectX::XMFLOAT4X4 worldMatrix;
	bool isDirty; //Does the matrix need to be remade
};

