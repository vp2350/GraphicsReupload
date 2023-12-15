#include "Material.h"

Material::Material(DirectX::XMFLOAT4 colorTint, std::shared_ptr<SimplePixelShader> pixelShader, std::shared_ptr<SimpleVertexShader> vertexShader, float specExponent, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness)
{
    this->colorTint = colorTint;
    this->pixelShader = pixelShader;
    this->vertexShader = vertexShader;
    this->specExponent = specExponent;
    this->srv = srv;
    this->sampler = sampler;
    this->srvNormal = nullptr;
    this->metalness = metalness;
    this->roughness = roughness;

}

Material::Material(DirectX::XMFLOAT4 colorTint, std::shared_ptr<SimplePixelShader> pixelShader, std::shared_ptr<SimpleVertexShader> vertexShader, float specExponent, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srvNormal, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> metalness,
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> roughness)
{
    this->colorTint = colorTint;
    this->pixelShader = pixelShader;
    this->vertexShader = vertexShader;
    this->specExponent = specExponent;
    this->srv = srv;
    this->sampler = sampler;
    this->srvNormal = srvNormal;
    this->metalness = metalness;
    this->roughness = roughness;
}

Material::~Material()
{
}

DirectX::XMFLOAT4 Material::GetTint()
{
    return colorTint;
}

void Material::SetTint(DirectX::XMFLOAT4 color)
{
    colorTint = color;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
    return pixelShader;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
    return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetSRV()
{
    return srv;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetSRVNormal()
{
    return srvNormal;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetMetal()
{
    return metalness;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetRoughness()
{
    return roughness;
}

Microsoft::WRL::ComPtr<ID3D11SamplerState> Material::GetSampler()
{
    return sampler;
}

float Material::GetSpecExponent()
{
    return specExponent;
}