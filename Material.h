#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> 
#include "SimpleShader.h"
#include "Material.h"
#include <memory>

class Material
{
public:
	Material(DirectX::XMFLOAT4 colorTint,
		std::shared_ptr<SimplePixelShader> pixelShader,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		float specExponent,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness);
	Material(DirectX::XMFLOAT4 colorTint,
		std::shared_ptr<SimplePixelShader> pixelShader,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		float specExponent,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvNormal,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness
		);
	~Material();
	DirectX::XMFLOAT4 GetTint();
	void SetTint(DirectX::XMFLOAT4 color);
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV();
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampler();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRVNormal();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetMetal();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetRoughness();


	float GetSpecExponent();
private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvNormal;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness;
	float specExponent;
};

