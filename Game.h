#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include <memory>
#include "Entity.h"
#include "Material.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "Transformation.h"
#include "Camera.h"
#include "SimpleShader.h"
#include "Lights.h"
#include "Sky.h"
#include <cmath>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void CheckCollision(std::vector<std::shared_ptr<Entity>> entityVector, std::vector<std::shared_ptr<Entity>> targetVector);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMeshes();

	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	std::shared_ptr<Mesh> newMesh1;
	std::shared_ptr<Mesh> newMesh2;
	std::shared_ptr<Mesh> newMesh3;
	std::shared_ptr<Mesh> newMesh4;
	std::shared_ptr<Mesh> newMesh5;

	std::shared_ptr<Mesh> targetMesh1;
	std::shared_ptr<Mesh> targetMesh2;
	std::shared_ptr<Mesh> targetMesh3;

	std::shared_ptr<Entity> newEntity1;
	std::shared_ptr<Entity> newEntity2;
	std::shared_ptr<Entity> newEntity3;
	std::shared_ptr<Entity> newEntity4;
	std::shared_ptr<Entity> newEntity5;

	std::shared_ptr<Entity> targetEntity1;
	std::shared_ptr<Entity> targetEntity2;
	std::shared_ptr<Entity> targetEntity3;

	std::vector<std::shared_ptr<Entity>> entityVector;
	std::vector<std::shared_ptr<Entity>> targetVector;

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBufferVS;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv2Normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv3Normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv4Normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv5Normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv6;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv6Normal;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metal6;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness3;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness4;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness5;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness6;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler2;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler3;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler4;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler5;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler6;

	// Shaders and shader-related constructs
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> normalPixelShader;
	std::shared_ptr<SimpleVertexShader> normalVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	std::shared_ptr<Material> material1;
	std::shared_ptr<Material> material2;
	std::shared_ptr<Material> material3;
	std::shared_ptr<Material> material4;
	std::shared_ptr<Material> material5;
	std::shared_ptr<Material> material6;

	Transformation transform;
	DirectionalLight light1;
	DirectionalLight light2;
	PointLight light3;

	DirectX::XMFLOAT3 ambientColor;

	bool isFlying;
	int score;
	//A pointer so it is easy to pass into other stuff
	std::shared_ptr<Camera> camera;

	std::shared_ptr<Sky> sky;
	std::shared_ptr<Mesh> skyMesh;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> skySampler;
	std::shared_ptr<SimplePixelShader> skyPixelShader;
	std::shared_ptr<SimpleVertexShader> skyVertexShader;
};

