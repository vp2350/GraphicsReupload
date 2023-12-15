#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include "SimpleShader.h"
#include <memory>
#include "Mesh.h"
#include "Camera.h"

class Sky
{
public:
	Sky(std::shared_ptr<Mesh> mesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions, 
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV, 
		Microsoft::WRL::ComPtr<ID3D11Device> device, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
		std::shared_ptr<SimplePixelShader> pixelShader,
	    std::shared_ptr<SimpleVertexShader> vertexShader);
	~Sky();
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera);
	

private:
	int m_bufferIndexCount;
	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

};

