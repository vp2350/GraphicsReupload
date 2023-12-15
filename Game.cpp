#include "Game.h"
#include "Vertex.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	//-10 z so we can see the origin (negative because of coordinate system)
	//Width/height to get the aspect ratio of the window
	camera = std::shared_ptr<Camera>(new Camera(0, 0, -10, (float)width / (float)height, XM_PIDIV4, 0.01f, 100.0f, 3.0f, 3.0f));
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/bronze_albedo.png").c_str(), 0, srv.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/bronze_normals.png").c_str(), 0, srvNormal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/bronze_metal.png").c_str(), 0, metal1.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/bronze_roughness.png").c_str(), 0, roughness1.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/floor_albedo.png").c_str(), 0, srv2.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/floor_normals.png").c_str(), 0, srv2Normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/floor_metal.png").c_str(), 0, metal2.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/floor_roughness.png").c_str(), 0, roughness2.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/Rocky_Color.png").c_str(), 0, srv3.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/cobblestone_normals.png").c_str(), 0, srv3Normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/wood_metal.png").c_str(), 0, metal3.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/Rocky_Roughness.png").c_str(), 0, roughness3.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/wood_albedo.png").c_str(), 0, srv4.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/wood_normals.png").c_str(), 0, srv4Normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/wood_metal.png").c_str(), 0, metal4.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/wood_roughness.png").c_str(), 0, roughness4.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/scratched_albedo.png").c_str(), 0, srv5.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/scratched_normals.png").c_str(), 0, srv5Normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/scratched_metal.png").c_str(), 0, metal5.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/scratched_roughness.png").c_str(), 0, roughness5.GetAddressOf());

	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/paint_albedo.png").c_str(), 0, srv6.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/paint_normals.png").c_str(), 0, srv6Normal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/paint_metal.png").c_str(), 0, metal6.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/PBR/paint_roughness.png").c_str(), 0, roughness6.GetAddressOf());

	CreateDDSTextureFromFile(device.Get(), GetFullPathTo_Wide(L"../../Assets/Skies/SunnyCubeMap.dds").c_str(), 0, skySRV.GetAddressOf());

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	device->CreateSamplerState(&sampDesc, sampler2.GetAddressOf());
	device->CreateSamplerState(&sampDesc, sampler3.GetAddressOf());
	device->CreateSamplerState(&sampDesc, sampler4.GetAddressOf());
	device->CreateSamplerState(&sampDesc, sampler5.GetAddressOf());
	device->CreateSamplerState(&sampDesc, sampler6.GetAddressOf());

	device->CreateSamplerState(&sampDesc, skySampler.GetAddressOf());

	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMeshes();

	//Load texture

	isFlying = false;
	score = 0;

	light1 = {XMFLOAT3(0.2f, 0.2f, 1.0f), 0.6f, XMFLOAT3(1,-1,0)};
	light2 = { XMFLOAT3(1.0f, 0.2f, 0.2f), 0.6f, XMFLOAT3(1,1,2) };
	light3 = { XMFLOAT3(1.0f, 0.2f, 0.2f), 0.6f, XMFLOAT3(5, 1 ,-2) };

	ambientColor = XMFLOAT3(0.05f, 0.05f, 0.1f);
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str()));
	pixelShader = std::shared_ptr<SimplePixelShader>(new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str()));
	normalVertexShader = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalVertexShader.cso").c_str()));
	normalPixelShader = std::shared_ptr<SimplePixelShader>(new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"NormalPixelShader.cso").c_str()));
	skyVertexShader = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device.Get(), context.Get(), DXCore::GetFullPathTo_Wide(L"SkyVertexShader.cso").c_str()));
	skyPixelShader = std::shared_ptr<SimplePixelShader>(new SimplePixelShader(device.Get(), context.Get(), GetFullPathTo_Wide(L"SkyPixelShader.cso").c_str()));
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateMeshes()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };

	Vertex vertices1[] =
	{
		{ XMFLOAT3(+0.75f, +0.75f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.75f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, +0.75f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }
	};

	unsigned int indices1[] = { 0, 1, 2, 0, 2, 3 };

	Vertex vertices2[] =
	{
		{ XMFLOAT3(-0.25f, +0.5f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.25f, +0.00f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.75f, +0.00f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.75f, +0.50f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.50f, +1.00f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }

	};

	unsigned int indices2[] = { 0, 1, 2, 0, 2, 3, 0, 3, 4 };

	material1 = std::shared_ptr<Material>(new Material(red, pixelShader, vertexShader, 10.0f, sampler, srv, srvNormal, metal1, roughness1));
	material2 = std::shared_ptr<Material>(new Material(green, pixelShader, vertexShader, 100.0f, sampler2, srv2, srv2Normal, metal2, roughness2));
	material3 = std::shared_ptr<Material>(new Material(blue, normalPixelShader, normalVertexShader, 100.0f, sampler3, srv3, srv3Normal, metal3, roughness3));
	material4 = std::shared_ptr<Material>(new Material(blue, normalPixelShader, normalVertexShader, 100.0f, sampler4, srv4, srv4Normal, metal4, roughness4));
	material5 = std::shared_ptr<Material>(new Material(blue, normalPixelShader, normalVertexShader, 100.0f, sampler5, srv5, srv5Normal, metal5, roughness5));
	material6 = std::shared_ptr<Material>(new Material(blue, normalPixelShader, normalVertexShader, 100.0f, sampler5, srv6, srv6Normal, metal6, roughness6));

	newMesh1 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device));
	newMesh2 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));
	newMesh3 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));
	newMesh4 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));
	newMesh5 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));

	targetMesh1 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));
	targetMesh2 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));
	targetMesh3 = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));

	skyMesh = std::shared_ptr<Mesh>(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));

	newEntity1 = std::shared_ptr<Entity>(new Entity(newMesh1, material1));
	entityVector.push_back(newEntity1);
	newEntity2 = std::shared_ptr<Entity>(new Entity(newMesh2, material6));
	entityVector.push_back(newEntity2);
	newEntity3 = std::shared_ptr<Entity>(new Entity(newMesh3, material3));
	entityVector.push_back(newEntity3);
	newEntity4 = std::shared_ptr<Entity>(new Entity(newMesh4, material2));
	entityVector.push_back(newEntity4);
	newEntity5 = std::shared_ptr<Entity>(new Entity(newMesh5, material4));
	entityVector.push_back(newEntity5);

	targetEntity1 = std::shared_ptr<Entity>(new Entity(targetMesh1, material5));
	targetVector.push_back(targetEntity1);
	targetEntity2 = std::shared_ptr<Entity>(new Entity(targetMesh2, material5));
	targetVector.push_back(targetEntity2);
	targetEntity3 = std::shared_ptr<Entity>(new Entity(targetMesh3, material5));
	targetVector.push_back(targetEntity3);

	(entityVector[0]->GetTransformation())->SetPosition(5.0f, 0.0f, 0.0f);
	(entityVector[0]->GetTransformation())->SetScale(0.3f, 0.3f, 0.3f);

	(targetVector[0]->GetTransformation())->SetPosition(-5.0f, 0.0f, 15.0f);
	(targetVector[0]->GetTransformation())->SetScale(1, 1, 1);
	(targetVector[1]->GetTransformation())->SetPosition(0.0f, 5.0f, 15.0f);
	(targetVector[1]->GetTransformation())->SetScale(1, 1, 1);
	(targetVector[2]->GetTransformation())->SetPosition(5.0f, 0.0f, 15.0f);
	(targetVector[2]->GetTransformation())->SetScale(1, 1, 1);
	

	sky = std::shared_ptr<Sky>(new Sky(skyMesh, skySampler, skySRV, device, context, skyPixelShader, skyVertexShader));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	float aspectRatio = (float)width / (float)height;
	//Change the camera aspect ratio on resize
	camera->UpdateProjectionMatrix(aspectRatio, XM_PIDIV4, 0.01f, 100.0f);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE)) {
		Quit();
	}

	if (score >= 3)
	{
		Quit();
	}
	//transform.SetPosition(0.5f, 0, 0);
	transform.MoveAbsolute(deltaTime * 0.1f, 0, 0);
	transform.Rotate(0, 0, deltaTime * 0.1f);
	

	if (!isFlying) {
		(entityVector[0]->GetTransformation())->SetPosition(camera->GetTransform().GetPosition().x, camera->GetTransform().GetPosition().y, camera->GetTransform().GetPosition().z);
		(entityVector[0]->GetTransformation())->SetRotation(camera->GetTransform().GetRotation().x, camera->GetTransform().GetRotation().y, camera->GetTransform().GetRotation().z);
		(entityVector[0]->GetTransformation())->MoveRelative(0, 0, 3.0f);
	}
	else {
		(entityVector[0]->GetTransformation())->MoveRelative(0.0f, 0.0f, 10.0f * deltaTime);

		if (entityVector[0]->GetTransformation()->GetPosition().x > 10 || entityVector[0]->GetTransformation()->GetPosition().x < -10
			|| entityVector[0]->GetTransformation()->GetPosition().y > 10 || entityVector[0]->GetTransformation()->GetPosition().y < -3
			|| entityVector[0]->GetTransformation()->GetPosition().z > 20 || entityVector[0]->GetTransformation()->GetPosition().z < -15) {
			isFlying = false;
		}
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		isFlying = true;
	}

	CheckCollision(entityVector, targetVector);
	camera->Update(deltaTime, this->hWnd);
}

void Game::CheckCollision(std::vector<std::shared_ptr<Entity>> entityVector, std::vector<std::shared_ptr<Entity>> targetVector) {
	for (int i = 0; i < targetVector.size(); i++)
	{
		float distance = (float)pow((float)(entityVector[0]->GetTransformation()->GetPosition().x - (float)(targetVector[i]->GetTransformation()->GetPosition().x)), 2) +
			pow((float)(entityVector[0]->GetTransformation()->GetPosition().y - (float)(targetVector[i]->GetTransformation()->GetPosition().y)), 2) +
			pow((float)(entityVector[0]->GetTransformation()->GetPosition().z - (float)(targetVector[i]->GetTransformation()->GetPosition().z)), 2);

		if (distance < 1.0f)
		{
			(targetVector[i]->GetTransformation())->SetPosition(-6.0f + (float)i, -0.2f, -7.0f);
			(targetVector[i]->GetTransformation())->SetScale(0.5f, 0.5f, 0.5f);

			isFlying = false;
			score++;
		}

	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	std::shared_ptr<SimpleVertexShader> simpleVS = entityVector[0]->GetMaterial()->GetVertexShader();
	std::shared_ptr<SimplePixelShader> simplePS = entityVector[0]->GetMaterial()->GetPixelShader();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", entityVector[0]->GetMaterial()->GetSpecExponent());
	
	//Not part of buffer data
	simplePS->SetShaderResourceView("diffuseTexture", entityVector[0]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", entityVector[0]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", entityVector[0]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", entityVector[0]->GetMaterial()->GetRoughness().Get());

	if(entityVector[0]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", entityVector[0]->GetMaterial()->GetSRVNormal().Get());

	simpleVS->SetFloat4("colorTint", entityVector[0]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", entityVector[0]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simplePS->CopyAllBufferData();
	simpleVS->CopyAllBufferData();

	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	entityVector[0]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[0]->GetMaterial()->GetPixelShader()->SetShader();
	// Clear the render target and depth buffer (erases what's on the screen)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, ((entityVector[0]->GetMesh())->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((entityVector[0]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		entityVector[0]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);


	//PILLARS 
	(entityVector[1]->GetTransformation())->SetPosition(-5.0f, -3.05f, 15.0f);
	(entityVector[1]->GetTransformation())->SetScale(1.20f, 5.0f, 1.20f);

	simpleVS = entityVector[1]->GetMaterial()->GetVertexShader();
	simplePS = entityVector[1]->GetMaterial()->GetPixelShader();
	
	simpleVS->SetFloat4("colorTint", entityVector[1]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", entityVector[1]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
	
	simpleVS->CopyAllBufferData();
	
	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", entityVector[1]->GetMaterial()->GetSpecExponent());
	
	simplePS->SetShaderResourceView("diffuseTexture", entityVector[1]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", entityVector[1]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", entityVector[1]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", entityVector[1]->GetMaterial()->GetRoughness().Get());
	if (entityVector[1]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", entityVector[1]->GetMaterial()->GetSRVNormal().Get());
	
	simplePS->CopyAllBufferData();
	
	context->IASetVertexBuffers(0, 1, (entityVector[1]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((entityVector[1]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);
	
	entityVector[1]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[1]->GetMaterial()->GetPixelShader()->SetShader();
	
	context->DrawIndexed(
		entityVector[1]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	(entityVector[1]->GetTransformation())->SetPosition(5.0f, -3.05f, 15.0f);
	simpleVS->SetMatrix4x4("world", entityVector[1]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[1]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[1]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[1]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	(entityVector[1]->GetTransformation())->SetPosition(0.0f, 0.0f, 15.0f);
	(entityVector[1]->GetTransformation())->SetScale(1.20f, 9.0f, 1.20f);

	simpleVS->SetMatrix4x4("world", entityVector[1]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[1]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[1]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[1]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	//TARGET

	simpleVS = targetVector[0]->GetMaterial()->GetVertexShader();
	simplePS = targetVector[0]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", targetVector[0]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", targetVector[0]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", targetVector[0]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", targetVector[0]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", targetVector[0]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", targetVector[0]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", targetVector[0]->GetMaterial()->GetRoughness().Get());
	if (targetVector[0]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", targetVector[0]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (targetVector[0]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((targetVector[0]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	targetVector[0]->GetMaterial()->GetVertexShader()->SetShader();
	targetVector[0]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		targetVector[0]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	simpleVS = targetVector[1]->GetMaterial()->GetVertexShader();
	simplePS = targetVector[1]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", targetVector[1]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", targetVector[1]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", targetVector[1]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", targetVector[1]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", targetVector[1]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", targetVector[1]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", targetVector[1]->GetMaterial()->GetRoughness().Get());
	if (targetVector[1]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", targetVector[1]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (targetVector[1]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((targetVector[1]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	targetVector[1]->GetMaterial()->GetVertexShader()->SetShader();
	targetVector[1]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		targetVector[1]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	simpleVS = targetVector[2]->GetMaterial()->GetVertexShader();
	simplePS = targetVector[2]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", targetVector[2]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", targetVector[2]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", targetVector[2]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", targetVector[2]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", targetVector[2]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", targetVector[2]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", targetVector[2]->GetMaterial()->GetRoughness().Get());
	if (targetVector[2]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", targetVector[2]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (targetVector[2]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((targetVector[2]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	targetVector[2]->GetMaterial()->GetVertexShader()->SetShader();
	targetVector[2]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		targetVector[2]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	//LEFT AND RIGHT WALL

	(entityVector[2]->GetTransformation())->SetPosition(8.25f, 2.2f, 2.5f);
	(entityVector[2]->GetTransformation())->SetScale(0.3f, 10.0f, 35.0f);
	(entityVector[2]->GetTransformation())->SetRotation(0.0f, 3.14f, 0.0f);

	simpleVS = entityVector[2]->GetMaterial()->GetVertexShader();
	simplePS = entityVector[2]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", entityVector[2]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", entityVector[2]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", entityVector[2]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", entityVector[2]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", entityVector[2]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", entityVector[2]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", entityVector[2]->GetMaterial()->GetRoughness().Get());
	if (entityVector[2]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", entityVector[2]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (entityVector[2]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((entityVector[2]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	entityVector[2]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[2]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[2]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);
	
	(entityVector[2]->GetTransformation())->SetPosition(-8.25f, 2.2f, 2.5f);
	//(entityVector[2]->GetTransformation())->SetScale(0.3, 15, 25);
	//(entityVector[2]->GetTransformation())->Rotate(0.0f, 3.14f, 0.0f);

	simpleVS->SetMatrix4x4("world", entityVector[2]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[2]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[2]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[2]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);


	//TOP AND BOTTOM

	(entityVector[2]->GetTransformation())->SetPosition(0.0f, -3.0f, 2.5f);
	(entityVector[2]->GetTransformation())->SetScale(0.3f, 17.5f, 35.0f);
	(entityVector[2]->GetTransformation())->SetRotation(0.0f, 3.14f, 1.57f);

	simpleVS->SetMatrix4x4("world", entityVector[2]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[2]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[2]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[2]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	(entityVector[2]->GetTransformation())->SetPosition(0.0f, 7.0f, 2.5f);
	//(entityVector[2]->GetTransformation())->SetScale(0.3, 15, 18);
	//(entityVector[2]->GetTransformation())->Rotate(0.0f, 3.14f, 1.57f);

	simpleVS->SetMatrix4x4("world", entityVector[2]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[2]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[2]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[2]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);
	
	//BACK AND FRONT WALL

	(entityVector[3]->GetTransformation())->SetPosition(0.0f, 2.2f, 17.50f);
	(entityVector[3]->GetTransformation())->SetScale(0.3f, 12.5f, 20.0f);
	(entityVector[3]->GetTransformation())->SetRotation(0.0f, 1.57f, 0.0f);

	simpleVS = entityVector[3]->GetMaterial()->GetVertexShader();
	simplePS = entityVector[3]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", entityVector[3]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", entityVector[3]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", entityVector[3]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", entityVector[3]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", entityVector[3]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", entityVector[3]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", entityVector[3]->GetMaterial()->GetRoughness().Get());
	if (entityVector[3]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", entityVector[3]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (entityVector[3]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((entityVector[3]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	entityVector[3]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[3]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[3]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	(entityVector[3]->GetTransformation())->SetPosition(0.0f, 2.2f, -15.0f);
	//(entityVector[2]->GetTransformation())->SetScale(0.3, 15, 18);
	//(entityVector[2]->GetTransformation())->Rotate(0.0f, 3.14f, 1.57f);

	simpleVS->SetMatrix4x4("world", entityVector[3]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	entityVector[3]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[3]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[3]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);


	//TABLE

	(entityVector[4]->GetTransformation())->SetPosition(0.0f, -1.5f, -7.0f);
	(entityVector[4]->GetTransformation())->SetScale(2, 20, 2);
	(entityVector[4]->GetTransformation())->SetRotation(0.0f, 3.14f, 1.57f);

	simpleVS = entityVector[4]->GetMaterial()->GetVertexShader();
	simplePS = entityVector[4]->GetMaterial()->GetPixelShader();

	simpleVS->SetFloat4("colorTint", entityVector[4]->GetMaterial()->GetTint());
	simpleVS->SetMatrix4x4("world", entityVector[4]->GetTransformation()->GetWorldMatrix());
	simpleVS->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	simpleVS->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());

	simpleVS->CopyAllBufferData();

	simplePS->SetData("light",   // The name of the (eventual) variable in the shader
		&light1,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light2",   // The name of the (eventual) variable in the shader
		&light2,    // The address of the data to set 
		sizeof(DirectionalLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetData("light3",   // The name of the (eventual) variable in the shader
		&light3,    // The address of the data to set 
		sizeof(PointLight)); // The size of the data (the whole struct!) to set   
	simplePS->SetFloat3("ambientColor", ambientColor); // Pass ambient, too
	simplePS->SetFloat3("cameraPosition", (camera->GetTransform()).GetPosition());
	simplePS->SetFloat("specExponent", entityVector[4]->GetMaterial()->GetSpecExponent());

	simplePS->SetShaderResourceView("diffuseTexture", entityVector[4]->GetMaterial()->GetSRV().Get());
	simplePS->SetSamplerState("basicSampler", entityVector[4]->GetMaterial()->GetSampler().Get());
	simplePS->SetShaderResourceView("MetalnssMap", entityVector[4]->GetMaterial()->GetMetal().Get());
	simplePS->SetShaderResourceView("RoughnessMap", entityVector[4]->GetMaterial()->GetRoughness().Get());
	if (entityVector[4]->GetMaterial()->GetSRVNormal().Get() != NULL)
		simplePS->SetShaderResourceView("normalTexture", entityVector[4]->GetMaterial()->GetSRVNormal().Get());

	simplePS->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, (entityVector[4]->GetMesh()->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((entityVector[4]->GetMesh()->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	entityVector[4]->GetMaterial()->GetVertexShader()->SetShader();
	entityVector[4]->GetMaterial()->GetPixelShader()->SetShader();

	context->DrawIndexed(
		entityVector[4]->GetMesh()->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);

	sky->Draw(context, camera);

	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}