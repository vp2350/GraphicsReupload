#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
	PointLight light3;
	float3 ambientColor;
	float3 cameraPosition;
	float specExponent;
}

Texture2D diffuseTexture : register(t0);
Texture2D RoughnessMap : register(t1);
Texture2D MetalnessMap : register(t2);
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
float4 main(VertexToPixel input) : SV_TARGET
{
	float roughness = RoughnessMap.Sample(basicSampler, input.uv).r;
	float metalness = MetalnessMap.Sample(basicSampler, input.uv).r;

	float3 textureColor = pow(diffuseTexture.Sample(basicSampler, input.uv).rgb, 2.2f);
	float3 specularColor = lerp(F0_NON_METAL.rrr, textureColor.rgb, metalness);


	float3 surfaceColor = input.color.rgb * textureColor.rgb;
	input.normal = normalize(input.normal);

	float3 light1Normalized = normalize(light.direction);
	float3 light2Normalized = normalize(light2.direction);
	float3 light3Normalized = normalize(input.worldPos - light3.position);

	float3 toCameraVector = normalize(cameraPosition - input.worldPos);
	float spec1 = MicrofacetBRDF(input.normal, light1Normalized, toCameraVector, roughness, metalness, specularColor);
	float spec2 = MicrofacetBRDF(input.normal, light2Normalized, toCameraVector, roughness, metalness, specularColor);
	float spec3 = MicrofacetBRDF(input.normal, light3Normalized, toCameraVector, roughness, metalness, specularColor);

	float diffuseAmount = Diffuse(input.normal, -light1Normalized);
	float diffuse2Amount = Diffuse(input.normal, -light2Normalized);
	float diffuse3Amount = Diffuse(input.normal, -light3Normalized);

	float3 finalColor = 
		surfaceColor * ambientColor + //Ambient
		diffuseAmount * light.color * surfaceColor + spec1.rrr +
		diffuse2Amount * light2.color * surfaceColor + spec2.rrr + 
		diffuse3Amount * light3.color * surfaceColor + spec3.rrr; //Directional Light


    //return float4(input.normal, 1);
	return float4(pow(finalColor, 1.0f/2.2f), 1);
}