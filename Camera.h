#pragma once

#include "Transformation.h"

#include <DirectXMath.h>
#include <Windows.h>

class Camera
{
public:
	Camera(float x, float y, float z, float aspectRatio, float fov, float nearPlane, float farPlane, float speed, float mouseSpeed);
	~Camera();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	void Update(float deltaTime, HWND windowHandle);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane);
	Transformation GetTransform();
private:
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix; 
	float speed; 
	float mouseSpeed;

	Transformation transform; 
	POINT previousMousePosition;
};

