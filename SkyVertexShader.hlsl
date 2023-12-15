#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	matrix viewMatrix;
	matrix projectionMatrix;
}

struct VertexToPixel_Sky
{
	float4 position		:SV_POSITION;
	float3 sampleDir	:DIRECTION;
};
// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel_Sky main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel_Sky output;

	// Modify the view matrix and remove the translation portion
	matrix viewNoTranslation = viewMatrix;
	viewNoTranslation._14 = 0;
	viewNoTranslation._24 = 0;
	viewNoTranslation._34 = 0;

	output.position = mul(mul(projectionMatrix, viewNoTranslation) , float4(input.position, 1.0f));
	output.position.z = output.position.w;
	output.sampleDir = input.position;
	
	return output;
}