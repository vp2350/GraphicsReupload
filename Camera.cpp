#include "Camera.h"

using namespace DirectX;

Camera::Camera(float x, float y, float z, float aspectRatio, float fov, float nearPlane, float farPlane, float speed, float mouseSpeed)
{
	transform.SetPosition(x, y, z);
	this->speed = speed;
	this->mouseSpeed = mouseSpeed;
	//Called once so we have a valid value to start
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio, fov, nearPlane, farPlane);
}

Camera::~Camera()
{
}

//Returns the View Matrix
XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

//Returns the projection matrix
XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::Update(float deltaTime, HWND windowHandle)
{
	float moveSpeed = deltaTime * speed;
	float mouseSpeed = deltaTime * speed;
	POINT mousePos = {}; 
	GetCursorPos(&mousePos); 
	ScreenToClient(windowHandle, &mousePos);

	// Movement (checking keyboard key states)
	if (GetAsyncKeyState('W') & 0x8000)  transform.MoveRelative(0, 0, moveSpeed); 
	if (GetAsyncKeyState('S') & 0x8000)  transform.MoveRelative(0, 0, -moveSpeed); 
	if (GetAsyncKeyState('A') & 0x8000)  transform.MoveRelative(-moveSpeed, 0, 0); 
	if (GetAsyncKeyState('D') & 0x8000)  transform.MoveRelative(moveSpeed, 0, 0); 
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)  transform.MoveAbsolute(0, moveSpeed, 0);
	if (GetAsyncKeyState('X') & 0x8000)  transform.MoveAbsolute(0, -moveSpeed, 0);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		float x = mouseSpeed * (mousePos.x - previousMousePosition.x);
		float y = mouseSpeed * (mousePos.y - previousMousePosition.y);

		//Reverse because moving mouse left and right actually rotates around y axis (yaw)
		transform.Rotate(y, x, 0); 
	}
	previousMousePosition = mousePos;

	UpdateViewMatrix();
}

//Update when the view changes
void Camera::UpdateViewMatrix()
{
	XMFLOAT3 rotation = transform.GetRotation();
	XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
	//positive z because of left handed system
	XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), rotationQuat); 
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMFLOAT3 position = transform.GetPosition();

	//Calculate view matrix
	XMMATRIX view = XMMatrixLookToLH(XMLoadFloat3(&position), forward, XMVectorSet(0, 1, 0, 0));	

	// Store result into the view matrix
	XMStoreFloat4x4(&viewMatrix, view);
}

//Update when the window size or field of view changes
void Camera::UpdateProjectionMatrix(float aspectRatio, float fov, float nearPlane, float farPlane)
{
	//XMMatrixPerspective for left handed coordinates
	//Calculates projection matrix 
	//FOV angle
	//Aspect Ratio
	//Near clip plane (how close till it is behind)
	//Far clip plane (how far till we cannot see it anymore)"
	DirectX::XMMATRIX projection = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);

	//Store projection in the projection matrix
	XMStoreFloat4x4(&projectionMatrix, projection);
}

Transformation Camera::GetTransform()
{
	return transform;
}