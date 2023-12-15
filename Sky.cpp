#include "Sky.h"

Sky::Sky(std::shared_ptr<Mesh> mesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions, 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skySRV, 
	Microsoft::WRL::ComPtr<ID3D11Device> device,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context,
	std::shared_ptr<SimplePixelShader> pixelShader,
	std::shared_ptr<SimpleVertexShader> vertexShader)
{
	this->mesh = mesh;
	this->samplerOptions = samplerOptions;
	this->srv = skySRV;

	D3D11_RASTERIZER_DESC rasterStateDesc = {};
	rasterStateDesc.FillMode = D3D11_FILL_SOLID;
	rasterStateDesc.CullMode = D3D11_CULL_FRONT; 
	device->CreateRasterizerState(&rasterStateDesc, this->rasterizerState.GetAddressOf());

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dsDesc, this->depthState.GetAddressOf());

	this->vertexShader = vertexShader;
	this->pixelShader = pixelShader;
}

Sky::~Sky()
{
}

void Sky::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, std::shared_ptr<Camera> camera)
{
	context->RSSetState(rasterizerState.Get());
	context->OMSetDepthStencilState(depthState.Get(), 0);

	pixelShader->SetShader();
	vertexShader->SetShader();

	pixelShader->SetShaderResourceView("diffuseTexture", srv.Get());
	pixelShader->SetSamplerState("basicSampler", samplerOptions.Get());
	pixelShader->CopyAllBufferData();

	vertexShader->SetMatrix4x4("viewMatrix", camera->GetViewMatrix());
	vertexShader->SetMatrix4x4("projectionMatrix", camera->GetProjectionMatrix());
	vertexShader->CopyAllBufferData();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, (mesh->GetVertexBuffer()).GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer((mesh->GetIndexBuffer()).Get(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);
	context->RSSetState(NULL); 
	context->OMSetDepthStencilState(NULL, 0);
}
