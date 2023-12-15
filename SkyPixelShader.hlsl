#include "ShaderIncludes.hlsli"

struct VertexToPixel_Sky
{
	float4 position		:SV_POSITION;
	float3 sampleDir	:DIRECTION;
};

TextureCube diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel_Sky input) : SV_TARGET
{
	return diffuseTexture.Sample(basicSampler, input.sampleDir);
}